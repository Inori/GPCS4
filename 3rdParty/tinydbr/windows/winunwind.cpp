/*
Copyright 2021 Google LLC

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

https ://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifdef _WIN64

#include "winunwind.h"
#include "executor.h"
#include <Windows.h>

 WinUnwindData::WinUnwindData() :
	table_registered(false),
	register_breakpoint(0), register_continue_IP(0),
	last_lookup_translate(NULL), last_lookup_other(NULL),
	last_translated_entry(NULL)
{
}

WinUnwindData::~WinUnwindData() {
  for (auto pair : unwind_infos)
    delete pair.second;
  for (auto info : original_function_infos)
    delete info;
}

FunctionInfo* WinUnwindData::LookupFunctionInfoForTranslate(size_t IP) {
  if (original_function_infos.empty()) return NULL;

  if (last_lookup_translate
    && (IP >= last_lookup_translate->function_start)
    && (IP < last_lookup_translate->function_end))
  {
    return last_lookup_translate;
  }

  last_lookup_translate = LookupFunctionInfoSlow(IP);
  return last_lookup_translate;
}

FunctionInfo* WinUnwindData::LookupFunctionInfoForOther(size_t IP) {
  if (original_function_infos.empty()) return NULL;

  if (last_lookup_other
    && (IP >= last_lookup_other->function_start)
    && (IP < last_lookup_other->function_end))
  {
    return last_lookup_other;
  }

  last_lookup_other = LookupFunctionInfoSlow(IP);
  return last_lookup_other;
}

FunctionInfo* WinUnwindData::LookupFunctionInfoSlow(size_t IP) {
  // binary search yay
  int m, L = 0, R = (int)(original_function_infos.size() - 1);
  while (L <= R) {
    m = (L + R) / 2;
    FunctionInfo* cur = original_function_infos[m];
    if (IP >= cur->function_end) {
      L = m + 1;
    } else if (IP < cur->function_start) {
      R = m - 1;
    } else if ((IP >= cur->function_start) && (IP < cur->function_end)) {
      return cur;
    } else {
      return NULL;
    }
  }
  return NULL;
}


void WinUnwindData::DoTranslate(size_t original_address, size_t translated_address) {
  if (last_translated_entry && last_lookup_translate && 
      (original_address >= last_lookup_translate->function_start) &&
      (original_address < last_lookup_translate->function_end))
  {
    last_translated_entry->function_end = translated_address;
    return;
  }

  CommitLastTranslated();

  LookupFunctionInfoForTranslate(original_address);
  if (!last_lookup_translate) {
    return;
  }

  last_translated_entry = new FunctionInfo;
  *last_translated_entry = *last_lookup_translate;
  last_translated_entry->function_start = translated_address;
  last_translated_entry->function_end = translated_address;
}

void WinUnwindData::CommitLastTranslated() {
  if (last_translated_entry) {
    translated_infos.push_back(*last_translated_entry);
    delete last_translated_entry;
    last_translated_entry = NULL;
  }
}

void WinUnwindGenerator::Init()
{
	HMODULE hNtdll = GetModuleHandleA("ntdll.dll");
	RtlAddFunctionTable_addr = (size_t)GetProcAddress(hNtdll, "RtlAddFunctionTable");
}

void WinUnwindGenerator::OnModuleInstrumented(ModuleInfo* module) {
  if (tinydbr_.child_ptr_size == 4) {
    FATAL("-generate_unwind is unneeded (and thus incompatible) with 32-bit targets");
  }

  WinUnwindData* unwind_data = new WinUnwindData;
  module->unwind_data = unwind_data;

  // read the entire image
  // TODO (ifratric) can this fail for some modules?
  size_t image_size = module->max_address - module->min_address;
  BYTE* modulebuf = (BYTE*)malloc(image_size);
  tinydbr_.RemoteRead((void *)module->min_address, modulebuf, image_size);

  DWORD exception_table_offset;
  DWORD exception_table_size;
  if (!GetExceptionTableOffsetAndSize((char*)modulebuf,
									  (uint32_t*)&exception_table_offset,
									  (uint32_t*)&exception_table_size))
  {
	  FATAL("Error getting exception table");
  }
  if ((size_t)exception_table_offset + exception_table_size > image_size) {
    FATAL("Exception table out of bounds");
  }
  if (exception_table_size % (3 * sizeof(DWORD))) {
    FATAL("Incorrect exception table format?");
  }

  DWORD* exception_table = (DWORD*)(modulebuf + exception_table_offset);
  DWORD* exception_table_end = (DWORD*)(modulebuf + exception_table_offset + exception_table_size);

  DWORD function_start;
  DWORD function_end;
  DWORD unwind_info_offset;

  while (exception_table < exception_table_end) {
    size_t function_info_addr = module->min_address + ((BYTE *)exception_table - modulebuf);

    function_start = *exception_table; exception_table++;
    function_end = *exception_table; exception_table++;
    unwind_info_offset = *exception_table; exception_table++;

    if (function_start >= function_end) continue;

    UnwindInfo* unwind_info = ReadUnwindInfo(module, modulebuf, image_size, unwind_info_offset);

    FunctionInfo *new_info = new FunctionInfo;

    new_info->function_start = module->min_address + function_start;
    new_info->function_end = module->min_address + function_end;

    new_info->function_info_addr = function_info_addr;

    new_info->unwind_info = unwind_info;

    unwind_data->original_function_infos.push_back(new_info);
  }

  size_t code_size_before = module->instrumented_code_allocated;

  for (size_t handler : unwind_data->handlers) {
    WriteHandler(module, handler);
  }

  for (auto pair : unwind_data->unwind_infos) {
    WriteUnwindInfo(module, pair.second);
  }

  size_t code_size_after = module->instrumented_code_allocated;
  tinydbr_.CommitCode(module, code_size_before, (code_size_after - code_size_before));

  free(modulebuf);
}

UnwindInfo* WinUnwindGenerator::ReadUnwindInfo(ModuleInfo* module, unsigned char* modulebuf, size_t image_size, uint32_t unwind_info_offset) {
  WinUnwindData* unwind_data = (WinUnwindData*)module->unwind_data;

  auto iter = unwind_data->unwind_infos.find(unwind_info_offset);
  if (iter != unwind_data->unwind_infos.end()) {
    return iter->second;
  }

  UnwindInfo* new_info = new UnwindInfo();
  unwind_data->unwind_infos[unwind_info_offset] = new_info;

  // need to loop here because of chained unwind infos
  while (1) {
    BYTE* unwind_info = modulebuf + unwind_info_offset;

    size_t handler_address = 0;
    size_t handler_data = 0;

    BYTE version_flags = *unwind_info;
    BYTE version = version_flags & 7;
    BYTE flags = (version_flags & 0xF8) >> 3;
    if (version > 2) {
      WARN("Unknown UNWIND_INFO version\n");
      break;
    }
    unwind_info += 1;

    BYTE prolog_size = *unwind_info;
    unwind_info += 1;

    BYTE num_unwind_codes = *unwind_info;
    unwind_info += 1;

    BYTE frame_register = *unwind_info;
    unwind_info += 1;

    USHORT* codes = (USHORT*)unwind_info;
    for (int i = 0; i < num_unwind_codes; i++) {
      new_info->unwind_codes.push_back(codes[i]);
    }

    if (num_unwind_codes % 2) num_unwind_codes++; // alignment
    unwind_info += sizeof(USHORT) * num_unwind_codes;

    if (flags & (UNW_FLAG_EHANDLER | UNW_FLAG_UHANDLER)) {
      DWORD handler_offset = *((DWORD*)unwind_info);
      handler_address = module->min_address + handler_offset;
      handler_data = module->min_address + (unwind_info - modulebuf) + sizeof(DWORD);
      unwind_data->handlers.insert(handler_address);
    } else if (flags & UNW_FLAG_CHAININFO) {
      // we don't care about the chained function start/end (?)
      unwind_info += 2 * sizeof(DWORD);
      unwind_info_offset = *((DWORD*)unwind_info);
      continue;
    }

    // printf("version: %x, frame_register: %x\n", version_flags, frame_register);

    new_info->version_flags = version_flags;
    new_info->frame_register = frame_register;

    new_info->handler = handler_address;
    new_info->handler_data = handler_data;

    FixUnwindCodes(new_info);

    return new_info;
  }

  return NULL;
}

int WinUnwindGenerator::GetExceptionTableOffsetAndSize(char* data, uint32_t* offset, uint32_t* size)
{
  DWORD pe_offset;
  pe_offset = *((DWORD*)(data + 0x3C));
  char* pe = data + pe_offset;
  DWORD signature = *((DWORD*)pe);
  if (signature != 0x00004550) {
    return 0;
  }
  pe = pe + 0x18;
  WORD magic = *((WORD*)pe);
  if (magic == 0x10b) {
    *offset = *(DWORD*)(pe + 120);
    *size = *(DWORD*)(pe + 124);
  } else if (magic == 0x20b) {
    *offset = *(DWORD*)(pe + 136);
    *size = *(DWORD*)(pe + 140);
  } else {
    return 0;
  }

  return 1;
}

void WinUnwindGenerator::OnModuleUninstrumented(ModuleInfo* module) {
  delete module->unwind_data;
  module->unwind_data = NULL;
}

void WinUnwindGenerator::OnModuleLoaded(void* module, char* module_name) {
  if (!strcmp(module_name, "ntdll.dll") && !RtlAddFunctionTable_addr) {
    DWORD offset = tinydbr_.GetProcOffset((HMODULE)module, "RtlAddFunctionTable");
    if (!offset) {
      FATAL("Error locating RtlAddFunctionTable");
    }
    RtlAddFunctionTable_addr = (size_t)module + offset;
  }
}

void WinUnwindGenerator::WriteHandler(ModuleInfo* module, size_t original_handler) {
  WinUnwindData* data = (WinUnwindData*)module->unwind_data;

  auto iter = data->translated_handler_map.find(original_handler);
  if (iter != data->translated_handler_map.end()) {
    return;
  }

  size_t handler_address = tinydbr_.GetCurrentInstrumentedAddress(module);

  data->handler_start_breakpoints[handler_address] = original_handler;
  tinydbr_.assembler_->Breakpoint(module);

  unsigned char ret = 0xC3;
  tinydbr_.WriteCode(module, &ret, sizeof(ret));

  data->translated_handler_map[original_handler] = handler_address;
}

void WinUnwindGenerator::FixUnwindCodes(UnwindInfo* info) {
  // bump the version to at least 2
  if ((info->version_flags & 0x7) == 1) info->version_flags++;

  // remove the chain info flag
  info->version_flags &= 0xDF;

  info->prolog_size = 0;

  std::vector<USHORT> new_unwind_codes;

  // set the epilog size to 0
  new_unwind_codes.push_back(0x0600);
  new_unwind_codes.push_back(0x0600);

  size_t i = 0;
  while (i < info->unwind_codes.size()) {
    USHORT code = info->unwind_codes[i];

    // claar offset
    code &= 0xFF00;

    USHORT opcode = ((code & 0x0F00) >> 8);
    USHORT opinfo = (code >> 12);

    size_t extra = 0;
    bool skip = false;

    switch (opcode) {
    case 0: /* UWOP_PUSH_NONVOL */
      break;
    case 1: /* UWOP_ALLOC_LARGE */
      if (!opinfo) extra = 1;
      else extra = 2;
      break;
    case 2: /* UWOP_ALLOC_SMALL */
      break;
    case 3: /* UWOP_SET_FPREG */
      break;
    case 4: /* UWOP_SAVE_NONVOL */
      extra = 1;
      break;
    case 5: /* UWOP_SAVE_NONVOL_FAR */
      extra = 2;
      break;
    case 6: /* UWOP_EPILOG */
      skip = true; // we already added epilog
      extra = 1;
      break;
    case 7: /* UWOP_SPARE_CODE */
      extra = 2;
      break;
    case 8: /* UWOP_SAVE_XMM128 */
      extra = 1;
      break;
    case 9: /* UWOP_SAVE_XMM128_FAR */
      extra = 2;
      break;
    case 10: /* UWOP_PUSH_MACHFRAME */
      break;
    default:
      WARN("Unknown unwind opcode");
      break;
    }

    if(!skip) new_unwind_codes.push_back(code);
    i++;

    for (size_t j = 0; j < extra; j++) {
      if (i > info->unwind_codes.size()) {
        WARN("Unknown or malformed unwind code format");
        break;
      }
      code = info->unwind_codes[i];
      if (!skip) new_unwind_codes.push_back(code);
      i++;
    }
  }

  info->unwind_codes = new_unwind_codes;
}

uint32_t WinUnwindGenerator::WriteUnwindInfo(ModuleInfo* module, UnwindInfo* info)
{
  WinUnwindData* data = (WinUnwindData*)module->unwind_data;

  DWORD offset = (DWORD)module->instrumented_code_allocated;

  // must be DWORD aligned
  if (offset % sizeof(DWORD)) {
    int padding_bytes = sizeof(DWORD) - (offset % sizeof(DWORD));
    char padding[] = { 0, 0, 0, 0 };
    tinydbr_.WriteCode(module, padding, padding_bytes);
    offset += padding_bytes;
  }

  info->translated_offset = offset;

  if (info->unwind_codes.size() > 255) {
    WARN("Unwind code array too large");
    return 0;
  }

  BYTE flags = info->version_flags;
  tinydbr_.WriteCode(module, &flags, sizeof(flags));

  BYTE prolog = info->prolog_size;
  tinydbr_.WriteCode(module, &prolog, sizeof(prolog));

  BYTE count = (BYTE)info->unwind_codes.size();
  tinydbr_.WriteCode(module, &count, sizeof(count));

  tinydbr_.WriteCode(module, &info->frame_register, sizeof(info->frame_register));

  // unwind code array
  if (!info->unwind_codes.empty()) {
    tinydbr_.WriteCode(module, &(info->unwind_codes[0]), info->unwind_codes.size() * sizeof(info->unwind_codes[0]));
  }

  // padding
  if (info->unwind_codes.size() % 2) {
    USHORT padding = 0;
    tinydbr_.WriteCode(module, &padding, sizeof(padding));
  }

  if (info->handler) {
    size_t translated_handler = data->translated_handler_map[info->handler];
    DWORD handler_offset = (DWORD)(translated_handler - (size_t)module->instrumented_code_remote);
    tinydbr_.WriteCode(module, &handler_offset, sizeof(handler_offset));

    // in our case, the handler data is going to be a pointer to the original handler data
    tinydbr_.WritePointer(module, info->handler_data);
  }

  return offset;
}

void WinUnwindGenerator::WriteFunctionInfo(ModuleInfo* module, FunctionInfo* info, FunctionTable *functionTable) {
  // write this entry to the function table
  if (functionTable->num_entries >= functionTable->max_entries) {
    FATAL("Insufficient size allocated for function table");
  }
  functionTable->num_entries++;
  DWORD start_offset = (DWORD)(info->function_start - (size_t)module->instrumented_code_remote);
  DWORD end_offset = (DWORD)(info->function_end - (size_t)module->instrumented_code_remote);
  tinydbr_.WriteCodeAtOffset(module, functionTable->offset, &start_offset, sizeof(DWORD));
  functionTable->offset += sizeof(DWORD);
  tinydbr_.WriteCodeAtOffset(module, functionTable->offset, &end_offset, sizeof(DWORD));
  functionTable->offset += sizeof(DWORD);
  tinydbr_.WriteCodeAtOffset(module, functionTable->offset, &info->unwind_info->translated_offset, sizeof(DWORD));
  functionTable->offset += sizeof(DWORD);
}

size_t WinUnwindGenerator::WriteFunctionTable(ModuleInfo* module, FunctionTable& functionTable, size_t max_entries) {
  if (functionTable.addr) return functionTable.addr;

  size_t function_table_addr = tinydbr_.GetCurrentInstrumentedAddress(module);

  // must be DWORD aligned
  if (function_table_addr % sizeof(DWORD)) {
    int padding_bytes = sizeof(DWORD) - (function_table_addr % sizeof(DWORD));
    char padding[] = { 0, 0, 0, 0 };
    tinydbr_.WriteCode(module, padding, padding_bytes);
    function_table_addr += padding_bytes;
  }

  function_table_addr = tinydbr_.GetCurrentInstrumentedAddress(module);
  size_t function_table_offset = module->instrumented_code_allocated;

  size_t buf_size = max_entries * sizeof(DWORD) * 3;
  DWORD* buf = (DWORD*)malloc(buf_size);

  DWORD max_offset = (DWORD)module->instrumented_code_size;

  DWORD* cur = buf;
  for (size_t i = 0; i < max_entries; i++) {
    *cur = max_offset; cur++;
    *cur = max_offset; cur++;
    *cur = 0xFFFFFFFF; cur++;
  }

  tinydbr_.WriteCode(module, buf, buf_size);

  free(buf);

  functionTable.addr = function_table_addr;
  functionTable.num_entries = 0;
  functionTable.max_entries = max_entries;
  functionTable.offset = function_table_offset;

  return function_table_addr;
}

size_t WinUnwindGenerator::MaybeRedirectExecution(ModuleInfo* module, size_t IP, void* context) {
  WinUnwindData* unwind_data = (WinUnwindData*)module->unwind_data;

  size_t code_size_before = module->instrumented_code_allocated;

  // TODO(ifratric) is this sufficient?
  size_t max_function_table_size = unwind_data->original_function_infos.size() * 3;
  size_t function_table_addr = WriteFunctionTable(module, unwind_data->function_table, max_function_table_size);

  unwind_data->CommitLastTranslated();

  size_t function_table_old_offset = unwind_data->function_table.offset;
  for (auto iter = unwind_data->translated_infos.begin(); iter != unwind_data->translated_infos.end(); iter++) {
    WriteFunctionInfo(module, &(*iter), &unwind_data->function_table);
  }
  size_t function_table_new_offset = unwind_data->function_table.offset;
  if (function_table_old_offset != function_table_new_offset) {
    tinydbr_.CommitCode(module, function_table_old_offset, function_table_new_offset - function_table_old_offset);
  }

  unwind_data->translated_infos.clear();

  if (unwind_data->table_registered) {
    // compute how much data we wrote and commit it all to the target process
    size_t code_size_after = module->instrumented_code_allocated;
    if (code_size_after != code_size_before) {
      tinydbr_.CommitCode(module, code_size_before, (code_size_after - code_size_before));
    }

    return IP;
  }

  if (!RtlAddFunctionTable_addr) {
    FATAL("Need to register unwind info but the address of RtlAddFunctionTable is unknown");
  }

  size_t continue_address = tinydbr_.GetCurrentInstrumentedAddress(module);

  tinydbr_.assembler_->OffsetStack(module, -tinydbr_.sp_offset);

  unsigned char register_assembly_part1[] = {
    // push flags and volatile registers
    0x9C, 0x50, 0x51, 0x52, 0x41, 0x50, 0x41, 0x51, 0x41, 0x52, 0x41, 0x53,
    // save rbp, rsp
    0x55, // push rbp
    0x48, 0x89, 0xE5, // mov rbp, rsp
    // fix stack alignment
    0x48, 0xF7, 0xC4, 0x0F, 0x00, 0x00, 0x00, // test   rsp,0xf
    0x0F, 0x84, 0x02, 0x00, 0x00, 0x00, // je skip_alignment
    //0x0F, 0x85, 0x02, 0x00, 0x00, 0x00, // jne skip_alignment
    0x6A, 0x00, // push 0
    // load parameters
    0x48, 0x8B, 0x05, 0x1A, 0x00, 0x00, 0x00, // mov rax, [rip+offset]
    0x48, 0x8B, 0x0D, 0x1B, 0x00, 0x00, 0x00, // mov rcx, [rip+offset]
    0x4C, 0x8B, 0x05, 0x1C, 0x00, 0x00, 0x00, // mov r8, [rip+offset]
    0x48, 0x8B, 0x15, 0x1D, 0x00, 0x00, 0x00, // mov rdx, [rip+offset]
    0xE9, 0x20, 0x00, 0x00, 0x00, // jmp 0x18
  };

  tinydbr_.WriteCode(module, register_assembly_part1, sizeof(register_assembly_part1));
  tinydbr_.WritePointer(module, RtlAddFunctionTable_addr);
  tinydbr_.WritePointer(module, function_table_addr);
  tinydbr_.WritePointer(module, (size_t)module->instrumented_code_remote);
  tinydbr_.WritePointer(module, max_function_table_size);

  unsigned char register_assembly_part2[] = {
    // allocate shadow space
    0x48, 0x8D, 0x64, 0x24, 0xE0, // lea rsp,[rsp-0x20]
    // RtlAddFunctionTable(function_table_addr, 1, instrumented_code_remote)
    0xFF, 0xD0, // call rax
    // free shadow space
    0x48, 0x8D, 0x64, 0x24, 0x20, // lea rsp,[rsp+0x20]
    //restore rsp, rbp
    0x48, 0x89, 0xEC, // mov    rsp,rbp
    0x5D, // pop rbp
    // pop volatile registers and flags
    0x41, 0x5B, 0x41, 0x5A, 0x41, 0x59, 0x41, 0x58, 0x5A, 0x59, 0x58, 0x9D
  };

  tinydbr_.WriteCode(module, register_assembly_part2, sizeof(register_assembly_part2));

  tinydbr_.assembler_->OffsetStack(module, tinydbr_.sp_offset);

  unwind_data->register_breakpoint = tinydbr_.GetCurrentInstrumentedAddress(module);
  tinydbr_.assembler_->Breakpoint(module);
  unwind_data->register_continue_IP = IP;
  tinydbr_.SaveRegisters((Executor::Context*)context, &unwind_data->register_saved_registers);

  // compute how much data we wrote and commit it all to the target process
  size_t code_size_after = module->instrumented_code_allocated;
  tinydbr_.CommitCode(module, code_size_before, (code_size_after - code_size_before));

  unwind_data->table_registered = true;

  return continue_address;
}

bool WinUnwindGenerator::HandleBreakpoint(ModuleInfo* module, void* address, void* context)
{
  WinUnwindData* unwind_data = (WinUnwindData*)module->unwind_data;
  if (!unwind_data) return false;
  Executor::Context* ctx = (Executor::Context*)context;

  if (size_t(address) == unwind_data->register_breakpoint)
  {
	  // size_t rax = tinyinst_.GetRegister(RAX);
	  // printf("Registration complete, rax: %zx\n", rax);
	  tinydbr_.RestoreRegisters(ctx, &unwind_data->register_saved_registers);
	  tinydbr_.SetRegister(ctx, RIP, unwind_data->register_continue_IP);
	  return true;
  }

  auto handler_iter = unwind_data->handler_start_breakpoints.find((size_t)address);
  if (handler_iter != unwind_data->handler_start_breakpoints.end()) {
    // printf("handler start breakpoint\n");

    PDISPATCHER_CONTEXT pdispatcher_context = 
        (PDISPATCHER_CONTEXT)tinydbr_.GetRegister((Executor::Context*)context, R9);
    DISPATCHER_CONTEXT dispatcher_context;
 
    tinydbr_.RemoteRead((void*)pdispatcher_context, &dispatcher_context, sizeof(dispatcher_context));

    auto iter = unwind_data->return_addresses.find(dispatcher_context.ControlPc);
    if (iter == unwind_data->return_addresses.end()) {
      WARN("Return address not found");
    } else {
      // lookup original return address from map
      dispatcher_context.ControlPc = iter->second.original_return_address;

      FunctionInfo* original_function_info = iter->second.original_function_info;

      // handler data passed to the instrumented handler is a pointer
      // to the original handler data. Thus we could to read it from memory
      // to restore it.
      // However, it's faster for now to just set it from original_function_info
      // (avoids a RemoteRead)
      // size_t fixed_handler_data;
      // tinyinst_.RemoteRead(dispatcher_context.HandlerData, &fixed_handler_data, sizeof(fixed_handler_data));
      // dispatcher_context.HandlerData = (PVOID)fixed_handler_data;
      dispatcher_context.HandlerData = (PVOID)original_function_info->unwind_info->handler_data;

      // seems unecessary for now
      // dispatcher_context.LanguageHandler = (PEXCEPTION_ROUTINE)handler_iter->second;

      // TODO(ifratric) do we need to restore this
      dispatcher_context.ImageBase = module->min_address;
      dispatcher_context.FunctionEntry = (PRUNTIME_FUNCTION)original_function_info->function_info_addr;
    }

    tinydbr_.RemoteWrite((void*)pdispatcher_context, &dispatcher_context, sizeof(dispatcher_context));

    // redirect execution to the corresponding original handler
	tinydbr_.SetRegister(ctx, RIP, handler_iter->second);
    return true;
  }

  return false;
}

void WinUnwindGenerator::OnReturnAddress(ModuleInfo* module,
  size_t original_address,
  size_t translated_address)
{
  WinUnwindData* unwind_data = (WinUnwindData*)module->unwind_data;

  FunctionInfo *info = unwind_data->LookupFunctionInfoForOther(original_address - 1);
  if (!info) return;

  WinUnwindData::ReturnAddressInfo ret_info;
  ret_info.original_return_address = original_address;
  ret_info.original_function_info = info;

  // printf("Return address %zx %zx\n", translated_address, original_address);

  unwind_data->return_addresses[translated_address] = ret_info;
}

void WinUnwindGenerator::OnBasicBlockStart(ModuleInfo* module,
  size_t original_address,
  size_t translated_address)
{
  WinUnwindData* unwind_data = (WinUnwindData*)module->unwind_data;
  unwind_data->DoTranslate(original_address, translated_address);
}

void WinUnwindGenerator::OnInstruction(ModuleInfo* module,
  size_t original_address,
  size_t translated_address)
{
  WinUnwindData* unwind_data = (WinUnwindData*)module->unwind_data;
  unwind_data->DoTranslate(original_address, translated_address);
}

void WinUnwindGenerator::OnBasicBlockEnd(ModuleInfo* module,
  size_t original_address,
  size_t translated_address)
{
  WinUnwindData* unwind_data = (WinUnwindData*)module->unwind_data;
  if (unwind_data->last_translated_entry) 
    unwind_data->last_translated_entry->function_end = translated_address;
}

#endif // _WIN64
