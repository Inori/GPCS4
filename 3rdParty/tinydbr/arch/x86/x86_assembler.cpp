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

#include "x86_assembler.h"

// int3
// unsigned char BREAKPOINT[] = {0xCC};

// hlt
// we'll debug inter process, but visual studio can not
// leave int3 breaks to application's exception handler,
// hence we use hlt instead, this will generate a 
// EXCEPTION_PRIV_INSTRUCTION exception, and we treat it
// as a debug break point exception
unsigned char BREAKPOINT[] = {0xF4};

// nop
unsigned char NOP[] = {0x90};

// jmp offset
unsigned char JMP[] = {0xe9, 0x00, 0x00, 0x00, 0x00};

// call offset
unsigned char CALL[] = {0xe8, 0x00, 0x00, 0x00, 0x00};

// warning, this is rip-relative on x64 but absolute on 32-bit
// jmp [offset]
unsigned char JMP_MEM[] = {0xFF, 0x25, 0x00, 0x00, 0x00, 0x00};

// lea rsp, [rsp + disp]
unsigned char LEARSP[] = {0x48, 0x8d, 0xa4, 0x24, 0x00, 0x00, 0x00, 0x00};
// lea esp, [esp + disp]
unsigned char LEAESP[] = {0x8D, 0xA4, 0x24, 0x00, 0x00, 0x00, 0x00};

// push flags
// push rax
// push rbx
unsigned char PUSH_FAB[] = {0x9c, 0x50, 0x53};

// push flags
// push rax
unsigned char PUSH_FA[] = {0x9c, 0x50};

// push flags
unsigned char PUSH_F[] = {0x9c};

// push rax
unsigned char PUSH_A[] = {0x50};

// push rbx
unsigned char PUSH_B[] = {0x53};

// pop rbx
// pop rax
// pop flags
unsigned char POP_BAF[] = {0x5B, 0x58, 0x9d};

// pop rax
// pop flags
unsigned char POP_AF[] = {0x58, 0x9d};

// pop rax
unsigned char POP_A[] = {0x58};

// and rbx, constant
unsigned char AND_RBX[] = {0x48, 0x81, 0xe3, 0x00, 0x00, 0x00, 0x00};
// and ebx, constant
unsigned char AND_EBX[] = {0x81, 0xe3, 0x00, 0x00, 0x00, 0x00};

// mov rbx, rax
unsigned char MOV_RBXRAX[] = {0x48, 0x89, 0xC3};
// mov ebx, eax
unsigned char MOV_EBXEAX[] = {0x89, 0xC3};

// add rbx, [offset]
unsigned char ADD_RBXRIPRELATIVE[] = {0x48, 0x03, 0x1D, 0x00, 0x00, 0x00, 0x00};
// add ebx, [offset]
unsigned char ADD_EBXRIPRELATIVE[] = {0x03, 0x1D, 0x00, 0x00, 0x00, 0x00};

// jmp [rbx]
unsigned char JMP_B[] = {0xFF, 0x23};

// cmp rax, [offset]
unsigned char CMP_RAX[] = {0x48, 0x3B, 0x05, 0x00, 0x00, 0x00, 0x00};
// cmp eax, [offset]
unsigned char CMP_EAX[] = {0x3B, 0x05, 0x00, 0x00, 0x00, 0x00};

// je offset
unsigned char JE[] = {0x0F, 0x84, 0x00, 0x00, 0x00, 0x00};

// mov [rsp], imm
unsigned char WRITE_SP_IMM[] = {0xC7, 0x04, 0x24, 0xAA, 0xAA, 0xAA, 0xAA};
// mov [rsp+4], imm
unsigned char WRITE_SP_4_IMM[] = {0xC7, 0x44, 0x24, 0x04,
                                  0xAA, 0xAA, 0xAA, 0xAA};

// mov rax, [rsp + offset]
unsigned char MOV_RAX_RSPMEM[] = {0x48, 0x8B, 0x84, 0x24,
                                  0xAA, 0xAA, 0xAA, 0x0A};
// mov eax, [esp + offset]
unsigned char MOV_EAX_ESPMEM[] = {0x8B, 0x84, 0x24, 0xAA, 0xAA, 0xAA, 0x0A};

// mov [rsp + offset], rax
unsigned char MOV_RSPMEM_RAX[] = {0x48, 0x89, 0x84, 0x24,
                                  0xAA, 0xAA, 0xAA, 0x0A};
// mov [esp + offset], eax
unsigned char MOV_ESPMEM_EAX[] = {0x89, 0x84, 0x24, 0xAA, 0xAA, 0xAA, 0x0A};

// mov byte ptr [0], 0
unsigned char CRASH_64[] = {0xC6, 0x04, 0x25, 0x00, 0x00, 0x00, 0x00, 0x00};
unsigned char CRASH_32[] = {0xC6, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00};

// fixes the memory displacement of the current instruction
// (assumes it is in the 4 last bytes)
inline void X86Assembler::FixDisp4(ModuleInfo *module, int32_t disp) {
  *(int32_t *)(module->instrumented_code_local +
               module->instrumented_code_allocated - 4) = disp;
}

void X86Assembler::Crash(ModuleInfo *module) {
  if (tinyinst_.child_ptr_size == 8) {
    tinyinst_.WriteCode(module, CRASH_64, sizeof(CRASH_64));
  } else {
    tinyinst_.WriteCode(module, CRASH_32, sizeof(CRASH_32));
  }
}

void X86Assembler::Breakpoint(ModuleInfo *module) {
  tinyinst_.WriteCode(module, &BREAKPOINT, sizeof(BREAKPOINT));
}

void X86Assembler::Nop(ModuleInfo *module) {
  tinyinst_.WriteCode(module, &NOP, sizeof(NOP));
}

void X86Assembler::JmpAddress(ModuleInfo *module, size_t address) {
  // just insert a jump to address
  tinyinst_.WriteCode(module, JMP_MEM, sizeof(JMP_MEM));
  if (tinyinst_.child_ptr_size == 4) {
    FixDisp4(module, (int32_t)tinyinst_.GetCurrentInstrumentedAddress(module));
  }
  tinyinst_.WritePointer(module, (size_t)address);
}

// checks if the instruction uses RIP-relative addressing,
// e.g. mov rax, [rip+displacement]; call [rip+displacement]
// and, if so, returns the memory address being referenced
bool X86Assembler::IsRipRelative(ModuleInfo*  module,
								 Instruction& inst,
								 size_t       instruction_address,
								 size_t*      mem_address)
{
	bool        rip_relative = false;
	const auto& zinst        = inst.zinst;
	bool        is_relative = zinst.instruction.attributes & ZYDIS_ATTRIB_IS_RELATIVE;

	if (!is_relative)
	{
		return false;
	}
	
    auto operand = FindExplicitMemoryOperand(inst);
    if (!operand)
    {
		return false;
    }

    rip_relative = operand->mem.base == ZYDIS_REGISTER_EIP ||
				   operand->mem.base == ZYDIS_REGISTER_RIP;

    if (!rip_relative)
    {
		return false;
    }

	if (mem_address)
	{
		int64_t disp             = zinst.instruction.raw.disp.value;
		size_t  instruction_size = zinst.instruction.length;
		*mem_address             = (size_t)(instruction_address + instruction_size + disp);
	}

	return rip_relative;
}

// checks if the instruction uses RSP-relative addressing,
// e.g. mov rax, [rsp+displacement];
// and, if so, returns the displacement
bool X86Assembler::IsRspRelative(Instruction& inst,
								 size_t*      displacement)
{
	bool rsp_relative = false;
	auto operand      = FindExplicitMemoryOperand(inst);
	if (!operand)
	{
		return rsp_relative;
	}

	if (operand->mem.base == ZYDIS_REGISTER_SP ||
		operand->mem.base == ZYDIS_REGISTER_ESP ||
		operand->mem.base == ZYDIS_REGISTER_RSP)
	{
		rsp_relative = true;
		if (displacement && operand->mem.disp.has_displacement)
		{
			*displacement = operand->mem.disp.value;
		}
	}
	return rsp_relative;
}

// adds/subtracts a given offset to the stack pointer
// this is done using LEA instruction rather than ADD/SUB
// to avoid clobbering the flags
void X86Assembler::OffsetStack(ModuleInfo *module, int32_t offset) {
  // lea rsp, [rsp + offset]
  if (tinyinst_.child_ptr_size == 8) {
    tinyinst_.WriteCode(module, LEARSP, sizeof(LEARSP));
  } else {
    tinyinst_.WriteCode(module, LEAESP, sizeof(LEAESP));
  }

  FixDisp4(module, offset);
}

// mov rax, [rsp + offset]
void X86Assembler::ReadStack(ModuleInfo *module, int32_t offset) {
  if (tinyinst_.child_ptr_size == 8) {
    tinyinst_.WriteCode(module, MOV_RAX_RSPMEM, sizeof(MOV_RAX_RSPMEM));
  } else {
    tinyinst_.WriteCode(module, MOV_EAX_ESPMEM, sizeof(MOV_EAX_ESPMEM));
  }
  FixDisp4(module, offset);
}

// mov [rsp + offset], rax
void X86Assembler::WriteStack(ModuleInfo *module, int32_t offset) {
  if (tinyinst_.child_ptr_size == 8) {
    tinyinst_.WriteCode(module, MOV_RSPMEM_RAX, sizeof(MOV_RSPMEM_RAX));
  } else {
    tinyinst_.WriteCode(module, MOV_ESPMEM_EAX, sizeof(MOV_ESPMEM_EAX));
  }
  FixDisp4(module, offset);
}

// adds another observed original_target -> actual_target pair
// to the golbal jumptable at the appropriate location
void X86Assembler::TranslateJmp(ModuleInfo *module,
                                ModuleInfo *target_module,
                                size_t original_target,
                                IndirectBreakpoinInfo& breakpoint_info,
                                bool global_indirect,
                                size_t previous_offset) {

  // cmp RAX, [original_target]
  if (tinyinst_.child_ptr_size == 8) {
    tinyinst_.WriteCode(module, CMP_RAX, sizeof(CMP_RAX));
  } else {
    tinyinst_.WriteCode(module, CMP_EAX, sizeof(CMP_EAX));
  }
  size_t cmp_offset = module->instrumented_code_allocated;

  // je label
  tinyinst_.WriteCode(module, JE, sizeof(JE));
  FixDisp4(module, sizeof(JMP));

  // jmp previous_list_head
  tinyinst_.WriteCode(module, JMP, sizeof(JMP));
  FixDisp4(module, (int32_t)((int64_t)previous_offset -
                             (int64_t)module->instrumented_code_allocated));

  // (maybe) pop RBX
  // pop RAX
  // pop flags
  if (global_indirect) {
    tinyinst_.WriteCode(module, POP_BAF, sizeof(POP_BAF));
  } else {
    tinyinst_.WriteCode(module, POP_AF, sizeof(POP_AF));
  }

  if (tinyinst_.sp_offset) {
    OffsetStack(module, tinyinst_.sp_offset);
  }

  // consider indirect call/jump an edge and insert appropriate instrumentation
  tinyinst_.InstrumentEdge(module, target_module, breakpoint_info.source_bb,
                           original_target);

  // jmp [actual_target]
  tinyinst_.WriteCode(module, JMP_MEM, sizeof(JMP_MEM));

  if (tinyinst_.child_ptr_size == 8) {
    FixDisp4(module, (int32_t)tinyinst_.child_ptr_size);
    *(int32_t *)(module->instrumented_code_local + cmp_offset - 4) =
    (int32_t)((int64_t)module->instrumented_code_allocated -
              (int64_t)cmp_offset);
  } else {
    FixDisp4(module,
             (int32_t)(tinyinst_.GetCurrentInstrumentedAddress(module) +
             tinyinst_.child_ptr_size));
    *(int32_t *)(module->instrumented_code_local + cmp_offset - 4) =
        (int32_t)tinyinst_.GetCurrentInstrumentedAddress(module);
  }
}

void X86Assembler::InstrumentRet(ModuleInfo*                      module,
								 Instruction&                     inst,
								 size_t                           instruction_address,
								 TinyDBR::IndirectInstrumentation mode,
								 size_t                           bb_address)
{
	// lots of moving around, but the problem is
	// we need to store context in the same place
	// where the return address is

	// at the end, the stack must be
	// saved RAX
	// saved EFLAGS
	// <sp_offset>
	// and RAX must contain return address

	const auto& zinst = inst.zinst;
	// store rax to a safe offset
	int32_t ax_offset = -tinyinst_.sp_offset - 2 * tinyinst_.child_ptr_size;
	WriteStack(module, ax_offset);
	// copy return address to a safe offset
	int32_t ret_offset = ax_offset - tinyinst_.child_ptr_size;
	ReadStack(module, 0);
	WriteStack(module, ret_offset);
	// get ret immediate
	int32_t imm = zinst.instruction.raw.imm[0].value.u;
	// align the stack
	int32_t ret_pop =
		(int32_t)tinyinst_.child_ptr_size + imm - tinyinst_.sp_offset;
	OffsetStack(module, ret_pop);  // pop
	ax_offset -= ret_pop;
	ret_offset -= ret_pop;
	// write data to stack
	tinyinst_.WriteCode(module, PUSH_F, sizeof(PUSH_F));
	ax_offset += tinyinst_.child_ptr_size;
	ret_offset += tinyinst_.child_ptr_size;
	ReadStack(module, ax_offset);
	tinyinst_.WriteCode(module, PUSH_A, sizeof(PUSH_A));
	ax_offset += tinyinst_.child_ptr_size;
	ret_offset += tinyinst_.child_ptr_size;
	ReadStack(module, ret_offset);
	tinyinst_.InstrumentIndirect(module,
								 inst,
								 instruction_address,
								 mode,
								 bb_address);
}

// converts an indirect jump/call into a MOV instruction
// which moves the target of the indirect call into the RAX/EAX register
// and writes this instruction into the code buffer
void X86Assembler::MovIndirectTarget(ModuleInfo*  module,
									 Instruction& inst,
									 size_t       original_address,
									 int32_t      stack_offset)
{
	size_t mem_address = 0;
	bool   rip_relative =
		IsRipRelative(module, inst, original_address, &mem_address);

	ZydisRegister dest_reg;
	if (tinyinst_.child_ptr_size == 4)
	{
		dest_reg = ZYDIS_REGISTER_EAX;
	}
	else
	{
		dest_reg = ZYDIS_REGISTER_RAX;
	}

	ZydisEncoderRequest mov;
	memset(&mov, 0, sizeof(mov));
	mov.mnemonic = ZYDIS_MNEMONIC_MOV;

	mov.machine_mode      = tinyinst_.child_ptr_size == 8
								? ZYDIS_MACHINE_MODE_LONG_64
								: ZYDIS_MACHINE_MODE_LEGACY_32;
	mov.address_size_hint = tinyinst_.child_ptr_size == 8
								? ZYDIS_ADDRESS_SIZE_HINT_64
								: ZYDIS_ADDRESS_SIZE_HINT_32;
	mov.operand_size_hint = tinyinst_.child_ptr_size == 8
								? ZYDIS_OPERAND_SIZE_HINT_64
								: ZYDIS_OPERAND_SIZE_HINT_32;

	mov.operand_count         = 2;
	mov.operands[0].type      = ZYDIS_OPERAND_TYPE_REGISTER;
	mov.operands[0].reg.value = dest_reg;

	const auto& operand = inst.zinst.operands[0];

	if (operand.type == ZYDIS_OPERAND_TYPE_MEMORY)
	{
		mov.operands[1].type      = ZYDIS_OPERAND_TYPE_MEMORY;
		mov.operands[1].mem.base  = operand.mem.base;
		mov.operands[1].mem.index = operand.mem.index;
		mov.operands[1].mem.scale = operand.mem.scale;
		mov.operands[1].mem.size  = operand.size / 8;

		// in an unlikely case where base is rsp, disp needs fixing
		// this is because we pushed stuff on the stack
		const auto& base = operand.mem.base;
		if ((base == ZYDIS_REGISTER_SP) ||
			(base == ZYDIS_REGISTER_ESP) ||
			(base == ZYDIS_REGISTER_RSP))
		{
			// printf("base = sp\n");
			int64_t disp = operand.mem.disp.value + stack_offset;
			mov.operands[1].mem.displacement = disp;
		}
		else
		{
			mov.operands[1].mem.displacement = operand.mem.disp.value;
		}
	}
	else if (operand.type == ZYDIS_OPERAND_TYPE_REGISTER)
	{
		mov.operands[1].type      = ZYDIS_OPERAND_TYPE_REGISTER;
		mov.operands[1].reg.value = operand.reg.value;
	}
	else
	{
		FATAL("Unexpected operand in indirect jump/call");
	}

	ZyanU8     encoded_instruction[ZYDIS_MAX_INSTRUCTION_LENGTH] = { 0 };
	ZyanUSize  encoded_length                                    = sizeof(encoded_instruction);
	ZyanStatus zstatus = ZydisEncoderEncodeInstruction(&mov, encoded_instruction, &encoded_length);
	if (ZYAN_FAILED(zstatus))
	{
		FATAL("Error encoding instruction");
	}

	if (rip_relative)
	{
		// fix displacement
		size_t  out_instruction_size = encoded_length;
		int64_t fixed_disp =
			(int64_t)mem_address -
			(int64_t)((size_t)module->instrumented_code_remote +
					  module->instrumented_code_allocated +
					  out_instruction_size);
		mov.operands[1].mem.displacement = fixed_disp;
		encoded_length                   = sizeof(encoded_instruction);
		zstatus                          = ZydisEncoderEncodeInstruction(&mov, encoded_instruction, &encoded_length);
		if (ZYAN_FAILED(zstatus))
		{
			FATAL("Error encoding instruction");
		}

		if (encoded_length != out_instruction_size)
		{
			FATAL("Unexpected instruction size");
		}
	}

	tinyinst_.WriteCode(module, encoded_instruction, encoded_length);
}

// translates indirect jump or call
// using global jumptable
void X86Assembler::InstrumentGlobalIndirect(ModuleInfo*  module,
											Instruction& inst,
											size_t       instruction_address)
{

	auto category = inst.zinst.instruction.meta.category;
	if (category != ZYDIS_CATEGORY_RET)
	{
		if (tinyinst_.sp_offset)
		{
			OffsetStack(module, -tinyinst_.sp_offset);
		}

		// push eflags
		// push RAX
		// push RBX
		tinyinst_.WriteCode(module, PUSH_FAB, sizeof(PUSH_FAB));

		int32_t stack_offset = tinyinst_.sp_offset + 3 * tinyinst_.child_ptr_size;

		if (category == ZYDIS_CATEGORY_CALL)
		{
			stack_offset += tinyinst_.child_ptr_size;
		}

		MovIndirectTarget(module, inst, instruction_address, stack_offset);
	}
	else
	{
		// stack already set up, just push RBX
		tinyinst_.WriteCode(module, PUSH_B, sizeof(PUSH_B));
	}

	// mov rbx, rax
	// and rbx, (JUMPTABLE_SIZE - 1) * child_ptr_size
	if (tinyinst_.child_ptr_size == 8)
	{
		tinyinst_.WriteCode(module, MOV_RBXRAX, sizeof(MOV_RBXRAX));
		tinyinst_.WriteCode(module, AND_RBX, sizeof(AND_RBX));
	}
	else
	{
		tinyinst_.WriteCode(module, MOV_EBXEAX, sizeof(MOV_EBXEAX));
		tinyinst_.WriteCode(module, AND_EBX, sizeof(AND_EBX));
	}
	FixDisp4(module, (int32_t)((JUMPTABLE_SIZE - 1) * tinyinst_.child_ptr_size));

	// add rbx, [jumptable_address]
	if (tinyinst_.child_ptr_size == 8)
	{
		tinyinst_.WriteCode(module, ADD_RBXRIPRELATIVE, sizeof(ADD_RBXRIPRELATIVE));
		FixDisp4(module, (int32_t)((int64_t)module->jumptable_address_offset -
								   (int64_t)module->instrumented_code_allocated));
	}
	else
	{
		tinyinst_.WriteCode(module, ADD_EBXRIPRELATIVE, sizeof(ADD_EBXRIPRELATIVE));
		FixDisp4(module, (int32_t)((size_t)module->instrumented_code_remote +
								   module->jumptable_address_offset));
	}

	// jmp [RBX]
	tinyinst_.WriteCode(module, JMP_B, sizeof(JMP_B));
}

// translates indirect jump or call
// using local jumptable
void X86Assembler::InstrumentLocalIndirect(ModuleInfo*  module,
										   Instruction& inst,
										   size_t       instruction_address,
										   size_t       bb_address)
{
	auto category = inst.zinst.instruction.meta.category;
	if (category != ZYDIS_CATEGORY_RET)
	{
		if (tinyinst_.sp_offset)
		{
			OffsetStack(module, -tinyinst_.sp_offset);
		}

		// push eflags
		// push RAX
		tinyinst_.WriteCode(module, PUSH_FA, sizeof(PUSH_FA));

		int32_t stack_offset = tinyinst_.sp_offset + 2 * tinyinst_.child_ptr_size;

		if (category == ZYDIS_CATEGORY_CALL)
		{
			stack_offset += tinyinst_.child_ptr_size;
		}

		MovIndirectTarget(module, inst, instruction_address, stack_offset);
	}
	else
	{
		// stack already set up
	}

	// jmp [breakpoint]
	tinyinst_.WriteCode(module, JMP_MEM, sizeof(JMP_MEM));

	size_t breakpoint_address = tinyinst_.GetCurrentInstrumentedAddress(module);

	if (tinyinst_.child_ptr_size == 8)
	{
		FixDisp4(module, 1);
	}
	else
	{
		FixDisp4(module, (int32_t)(breakpoint_address + 1));
	}

	// int3
	Breakpoint(module);
	module->br_indirect_newtarget_list[breakpoint_address] = {
		module->instrumented_code_allocated, bb_address
	};

	// breakpoint_address
	if (tinyinst_.child_ptr_size == 8)
	{
		uint64_t address = (uint64_t)breakpoint_address;
		tinyinst_.WriteCode(module, &address, sizeof(address));
	}
	else
	{
		uint32_t address = (uint32_t)breakpoint_address;
		tinyinst_.WriteCode(module, &address, sizeof(address));
	}
}

// pushes return address on the target stack
void X86Assembler::PushReturnAddress(ModuleInfo *module,
                                     uint64_t return_address) {
  // printf("retun address: %llx\n", return_address);
  // write the original return address
  OffsetStack(module, -(int)tinyinst_.child_ptr_size);
  uint32_t return_lo = (uint32_t)(((uint64_t)return_address) & 0xFFFFFFFF);
  uint32_t return_hi = (uint32_t)(((uint64_t)return_address) >> 32);

  // mov dword ptr [sp], return_lo
  tinyinst_.WriteCode(module, WRITE_SP_IMM, sizeof(WRITE_SP_IMM));
  *(uint32_t *)(module->instrumented_code_local +
                module->instrumented_code_allocated - 4) = return_lo;

  if (tinyinst_.child_ptr_size == 8) {
    // mov dword ptr [sp+4], return_hi
    tinyinst_.WriteCode(module, WRITE_SP_4_IMM, sizeof(WRITE_SP_4_IMM));
    *(uint32_t *)(module->instrumented_code_local +
                  module->instrumented_code_allocated - 4) = return_hi;
  }
}

const ZydisDecodedOperand* X86Assembler::FindExplicitMemoryOperand(
	const Instruction& inst,
	size_t*            index)
{
	const auto& zinst          = inst.zinst;
	size_t      explicit_count = zinst.instruction.operand_count_visible;
	for (size_t i = 0; i != explicit_count; ++i)
	{
		if (zinst.operands[i].type == ZYDIS_OPERAND_TYPE_MEMORY &&
			zinst.operands[i].visibility == ZYDIS_OPERAND_VISIBILITY_EXPLICIT)
		{
			if (index)
			{
				*index = i;
			}
			return &zinst.operands[i];
		}
	}
	return nullptr;
}

// detect if instruction is indirect jmp or call
bool X86Assembler::IsIndirectBranch(Instruction& inst)
{
	auto brance_type  = inst.zinst.instruction.meta.branch_type;
	if (brance_type == ZYDIS_BRANCH_TYPE_NONE)
	{
		return false;
	}

	// must have a operand
	auto operand_type = inst.zinst.operands[0].type;
	return operand_type == ZYDIS_OPERAND_TYPE_REGISTER || operand_type == ZYDIS_OPERAND_TYPE_MEMORY;
}

// outputs instruction into the translated code buffer
// fixes stuff like rip-relative addressing
void X86Assembler::FixInstructionAndOutput(
	ModuleInfo*          module,
	Instruction&         inst,
	const unsigned char* input,
	const unsigned char* input_address_remote,
	bool                 convert_call_to_jmp)
{
	size_t mem_address = 0;
	bool   rip_relative =
		IsRipRelative(module, inst, (size_t)input_address_remote, &mem_address);
	const auto& zinst = inst.zinst;

	size_t original_instruction_size = zinst.instruction.length;

	bool needs_fixing = rip_relative || convert_call_to_jmp;

	// fast path
	// just copy instruction bytes without encoding
	if (!needs_fixing)
	{
		tinyinst_.WriteCode(module, (void*)input, original_instruction_size);
		return;
	}

	ZydisEncoderRequest req     = {};
	ZyanStatus          zstatus = ZydisEncoderDecodedInstructionToEncoderRequest(
        &zinst.instruction,
        zinst.operands,
        zinst.instruction.operand_count_visible,
        &req);

	ZyanU8    encoded_instruction[ZYDIS_MAX_INSTRUCTION_LENGTH] = { 0 };
	ZyanUSize encoded_length                                    = sizeof(encoded_instruction);

	if (convert_call_to_jmp)
	{
		req.mnemonic = ZYDIS_MNEMONIC_JMP;
	}

	if (!rip_relative)
	{
		zstatus = ZydisEncoderEncodeInstruction(&req, encoded_instruction, &encoded_length);
		if (ZYAN_FAILED(zstatus))
		{
			FATAL("Error encoding instruction");
		}
		tinyinst_.WriteCode(module, encoded_instruction, encoded_length);
		return;
	}

	size_t  instruction_end_addr;
	int64_t fixed_disp;

	instruction_end_addr = (size_t)module->instrumented_code_remote +
						   module->instrumented_code_allocated +
						   original_instruction_size;

	// encode an instruction once just to get the instruction size
	// as it needs not be the original size
	fixed_disp = (int64_t)(mem_address) - (int64_t)(instruction_end_addr);

	if (llabs(fixed_disp) > 0x7FFFFFFF)
		FATAL("Offset larger than 2G");

	size_t mem_idx = 0;
	auto   operand = FindExplicitMemoryOperand(inst, &mem_idx);
	if (!operand)
	{
		FATAL("No memory operand for a rip-relative instruction.");
	}

	encoded_length                         = sizeof(encoded_instruction);
	req.operands[mem_idx].mem.displacement = fixed_disp;

	zstatus = ZydisEncoderEncodeInstruction(&req, encoded_instruction, &encoded_length);
	if (ZYAN_FAILED(zstatus))
	{
		FATAL("Error encoding instruction");
	}

	size_t out_instruction_size = encoded_length;
	if ((module->instrumented_code_allocated + out_instruction_size) >
		module->instrumented_code_size)
	{
		FATAL("Insufficient memory allocated for instrumented code");
	}

	instruction_end_addr = (size_t)module->instrumented_code_remote +
						   module->instrumented_code_allocated +
						   out_instruction_size;

	fixed_disp = (int64_t)(mem_address) - (int64_t)(instruction_end_addr);

	if (llabs(fixed_disp) > 0x7FFFFFFF)
		FATAL("Offset larger than 2G");

	encoded_length                         = sizeof(encoded_instruction);
	req.operands[mem_idx].mem.displacement = fixed_disp;
	zstatus                                = ZydisEncoderEncodeInstruction(&req, encoded_instruction, &encoded_length);
	if (ZYAN_FAILED(zstatus))
	{
		FATAL("Error encoding instruction");
	}

	void* code_buffer = module->instrumented_code_local +
						module->instrumented_code_allocated;
	memcpy(code_buffer, encoded_instruction, encoded_length);

	if (encoded_length != out_instruction_size)
	{
		FATAL("Unexpected instruction size");
	}

	module->instrumented_code_allocated += encoded_length;
}


bool X86Assembler::DecodeInstruction(Instruction&         inst,
									 const unsigned char* buffer,
									 unsigned int         buffer_size)
{

	ZyanStatus zstatus = ZydisDecoderDecodeFull(&decoder, buffer, buffer_size,
												&inst.zinst.instruction, inst.zinst.operands, ZYDIS_MAX_OPERAND_COUNT,
												0);
	if (ZYAN_FAILED(zstatus))
		return false;

	inst.address                 = (size_t)buffer;
	inst.length                  = inst.zinst.instruction.length;
	inst.bbend                   = false;

	ZydisInstructionCategory category    = inst.zinst.instruction.meta.category;
	ZydisMnemonic            mnemonic    = inst.zinst.instruction.mnemonic;
	ZydisBranchType          branch_type = inst.zinst.instruction.meta.branch_type;

	switch (category)
	{
	case ZYDIS_CATEGORY_CALL:
	case ZYDIS_CATEGORY_RET:
	case ZYDIS_CATEGORY_UNCOND_BR:
	case ZYDIS_CATEGORY_COND_BR:
		inst.bbend = true;
		break;
	default:
		break;
	}

	inst.iclass = InstructionClass::OTHER;
	if (branch_type == ZYDIS_BRANCH_TYPE_NEAR || branch_type == ZYDIS_BRANCH_TYPE_SHORT)
	{
		if (category == ZYDIS_CATEGORY_RET &&
			mnemonic == ZYDIS_MNEMONIC_RET)
		{
			inst.iclass = InstructionClass::RET;
		}
		else if (mnemonic == ZYDIS_MNEMONIC_JMP)
		{
			inst.iclass = InstructionClass::IJUMP;
		}
		else if (mnemonic == ZYDIS_MNEMONIC_CALL)
		{
			inst.iclass = InstructionClass::ICALL;
		}
	}

	return true;
}

// fixes an offset in the jump instruction (at offset jmp_offset in the
// instrumented code) to jump to the given basic block (at offset bb in the
// original code)
void X86Assembler::FixOffset(ModuleInfo *module,
                             uint32_t jmp_offset,
                             uint32_t target_offset) {
  int32_t jmp_relative_offset =
    (int32_t)target_offset - (int32_t)(jmp_offset + 4);
  *(int32_t *)(module->instrumented_code_local + jmp_offset) =
    jmp_relative_offset;
}

void X86Assembler::PrintInstruction(const Instruction& inst)
{
#ifdef _DEBUG
	const auto& zinst = inst.zinst;
	char buffer[256];
	ZydisFormatterFormatInstruction(&formatter,
									&zinst.instruction, zinst.operands,
									zinst.instruction.operand_count_visible,
									buffer, sizeof(buffer), inst.address);

	printf("%016" PRIX64 "  ", inst.address);
	for (size_t i = 0; i != zinst.instruction.length; ++i)
	{
		uint8_t byte = ((uint8_t*)inst.address)[i];
		printf("%02X ", byte);
	}
	printf(" %s\n", buffer);

#endif  // _DEBUG
}

void X86Assembler::HandleBasicBlockEnd(
	const char*                               address,
	ModuleInfo*                               module,
	std::set<char*>*                          queue,
	std::list<std::pair<uint32_t, uint32_t>>* offset_fixes,
	Instruction&                              inst,
	const char*                               code_ptr,
	size_t                                    offset,
	size_t                                    last_offset)
{
	const auto&                zinst      = inst.zinst;
	ZydisInstructionCategory   category   = zinst.instruction.meta.category;

	if (category == ZYDIS_CATEGORY_RET)
	{
		TinyDBR::IndirectInstrumentation ii_mode =
			tinyinst_.ShouldInstrumentIndirect(module,
											   inst,
											   (size_t)address + last_offset);

		if (ii_mode != TinyDBR::IndirectInstrumentation::II_NONE)
		{
			InstrumentRet(module, inst, (size_t)address + last_offset, ii_mode,
						  (size_t)address);
		}
		else
		{
			FixInstructionAndOutput(
				module,
				inst,
				(unsigned char*)(code_ptr + last_offset),
				(unsigned char*)(address + last_offset));
		}
	}
	else if (category == ZYDIS_CATEGORY_COND_BR)
	{
		// j* target_address
		// gets instrumented as:
		//   j* label
		//   <edge instrumentation>
		//   jmp continue_address
		// label:
		//   <edge instrumentation>
		//   jmp target_address

		if (IsIndirectBranch(inst))
		{
			FATAL("Error getting branch target");
		}

		ZyanI64 imm = zinst.instruction.raw.imm[0].value.s;

		const char* target_address1 = address + offset;
		const char* target_address2 = address + offset + imm;

		if (tinyinst_.GetModule((size_t)target_address2) != module)
		{
			WARN("Relative jump to a differen module in bb at %p\n", address);
			tinyinst_.InvalidInstruction(module);
			return;
		}

		// preliminary encode jump instruction
		// displacement might be changed later as we don't know
		// the size of edge instrumentation yet
		// assuming 0 for now
		int32_t             fixed_disp                                        = sizeof(JMP);
		ZyanU8              encoded_instruction[ZYDIS_MAX_INSTRUCTION_LENGTH] = { 0 };
		ZyanUSize           encoded_length                                    = sizeof(encoded_instruction);
		uint32_t            jump_size                                         = 0;
		ZydisEncoderRequest req                                               = {};
		ZyanStatus          zstatus = ZydisEncoderDecodedInstructionToEncoderRequest(
            &zinst.instruction,
            zinst.operands,
            zinst.instruction.operand_count_visible,
            &req);
		if (ZYAN_FAILED(zstatus))
		{
			FATAL("Error converting request.");
		}

		req.operands[0].imm.s = fixed_disp;
		zstatus               = ZydisEncoderEncodeInstruction(&req, encoded_instruction, &encoded_length);
		if (ZYAN_FAILED(zstatus))
		{
			FATAL("Error encoding instruction");
		}

		jump_size                = encoded_length;
		size_t jump_start_offset = module->instrumented_code_allocated;
		tinyinst_.WriteCode(module, encoded_instruction, jump_size);
		size_t jump_end_offset = module->instrumented_code_allocated;

		// instrument the 1st edge
		tinyinst_.InstrumentEdge(module, module, (size_t)address,
								 (size_t)target_address1);

		// jmp target_address1
		tinyinst_.WriteCode(module, JMP, sizeof(JMP));

		tinyinst_.FixOffsetOrEnqueue(
			module,
			(uint32_t)((size_t)target_address1 - (size_t)(module->min_address)),
			(uint32_t)(module->instrumented_code_allocated - 4), queue,
			offset_fixes);

		// time to fix that conditional jump offset
		if ((module->instrumented_code_allocated - jump_end_offset) != fixed_disp)
		{
			fixed_disp =
				(int32_t)(module->instrumented_code_allocated - jump_end_offset);

			ZyanUSize encoded_length = sizeof(encoded_instruction);
			req.operands[0].imm.s    = fixed_disp;
			zstatus                  = ZydisEncoderEncodeInstruction(&req, encoded_instruction, &encoded_length);
			if (ZYAN_FAILED(zstatus))
			{
				FATAL("Error encoding instruction");
			}

			if (jump_size != encoded_length)
			{
				FATAL("Instruction size changed?");
			}
			tinyinst_.WriteCodeAtOffset(module, jump_start_offset, encoded_instruction,
										jump_size);
		}

		// instrument the 2nd edge
		tinyinst_.InstrumentEdge(module, module, (size_t)address,
								 (size_t)target_address2);

		// jmp target_address2
		tinyinst_.WriteCode(module, JMP, sizeof(JMP));

		tinyinst_.FixOffsetOrEnqueue(
			module,
			(uint32_t)((size_t)target_address2 - (size_t)(module->min_address)),
			(uint32_t)(module->instrumented_code_allocated - 4),
			queue,
			offset_fixes);
	}
	else if (category == ZYDIS_CATEGORY_UNCOND_BR)
	{
		if (!IsIndirectBranch(inst))
		{
			// jmp address
			// gets instrumented as:
			// jmp fixed_address

			// must have an operand
			int32_t     imm            = zinst.operands[0].imm.value.s;
			const char* target_address = address + offset + imm;

			if (tinyinst_.GetModule((size_t)target_address) != module)
			{
				WARN("Relative jump to a differen module in bb at %p\n", address);
				tinyinst_.InvalidInstruction(module);
				return;
			}

			// jmp target_address
			tinyinst_.WriteCode(module, JMP, sizeof(JMP));

			tinyinst_.FixOffsetOrEnqueue(
				module,
				(uint32_t)((size_t)target_address - (size_t)(module->min_address)),
				(uint32_t)(module->instrumented_code_allocated - 4), queue,
				offset_fixes);
		}
		else
		{
			TinyDBR::IndirectInstrumentation ii_mode =
				tinyinst_.ShouldInstrumentIndirect(module,
												   inst,
												   (size_t)address + last_offset);

			if (ii_mode != TinyDBR::IndirectInstrumentation::II_NONE)
			{
				tinyinst_.InstrumentIndirect(module, inst,
											 (size_t)address + last_offset, ii_mode,
											 (size_t)address);
			}
			else
			{
				FixInstructionAndOutput(
					module,
					inst,
					(unsigned char*)(code_ptr + last_offset),
					(unsigned char*)(address + last_offset));
			}
		}
	}
	else if (category == ZYDIS_CATEGORY_CALL)
	{
		if (!IsIndirectBranch(inst))
		{
			// call target_address
			// gets instrumented as:
			//   call label
			//   jmp return_address
			// label:
			//   jmp target_address

			// must have an operand
			int32_t     imm            = zinst.operands[0].imm.value.s;
			const char* return_address = address + offset;
			const char* call_address   = address + offset + imm;

			if (tinyinst_.GetModule((size_t)call_address) != module)
			{
				WARN("Relative jump to a differen module in bb at %p\n", address);
				tinyinst_.InvalidInstruction(module);
				return;
			}

			// fix the displacement and emit the call
			if (!tinyinst_.patch_return_addresses)
			{
				ZyanU8              encoded_instruction[ZYDIS_MAX_INSTRUCTION_LENGTH] = { 0 };
				ZyanUSize           encoded_length                                    = sizeof(encoded_instruction);
				ZydisEncoderRequest req                                               = {};
				ZyanStatus          zstatus = ZydisEncoderDecodedInstructionToEncoderRequest(
                    &zinst.instruction,
                    zinst.operands,
                    zinst.instruction.operand_count_visible,
                    &req);
				if (ZYAN_FAILED(zstatus))
				{
					FATAL("Error converting request.");
				}

				req.operands[0].imm.s = sizeof(JMP);
				zstatus               = ZydisEncoderEncodeInstruction(&req, encoded_instruction, &encoded_length);

				if (ZYAN_FAILED(zstatus))
				{
					FATAL("Error encoding instruction");
				}
				tinyinst_.WriteCode(module, encoded_instruction, encoded_length);

				size_t translated_return_address = tinyinst_.GetCurrentInstrumentedAddress(module);
				tinyinst_.OnReturnAddress(module, (size_t)return_address, translated_return_address);

				// jmp return_address
				tinyinst_.WriteCode(module, JMP, sizeof(JMP));

				tinyinst_.FixOffsetOrEnqueue(
					module,
					(uint32_t)((size_t)return_address - (size_t)(module->min_address)),
					(uint32_t)(module->instrumented_code_allocated - 4), queue,
					offset_fixes);

				// jmp call_address
				tinyinst_.WriteCode(module, JMP, sizeof(JMP));

				tinyinst_.FixOffsetOrEnqueue(
					module,
					(uint32_t)((size_t)call_address - (size_t)(module->min_address)),
					(uint32_t)(module->instrumented_code_allocated - 4), queue,
					offset_fixes);
			}
			else
			{
				PushReturnAddress(module, (uint64_t)return_address);

				// jmp call_address
				tinyinst_.WriteCode(module, JMP, sizeof(JMP));

				tinyinst_.FixOffsetOrEnqueue(
					module,
					(uint32_t)((size_t)call_address - (size_t)(module->min_address)),
					(uint32_t)(module->instrumented_code_allocated - 4), queue,
					offset_fixes);

				// done, we don't need to do anything else as return gets redirected
				// later
			}
		}
		else /* CALL, operand_name != XED_OPERAND_RELBR */
		{
			const char* return_address = address + offset;

			TinyDBR::IndirectInstrumentation ii_mode =
				tinyinst_.ShouldInstrumentIndirect(module,
												   inst,
												   (size_t)address + last_offset);

			if (ii_mode != TinyDBR::IndirectInstrumentation::II_NONE)
			{
				if (tinyinst_.patch_return_addresses)
				{
					PushReturnAddress(module, (uint64_t)return_address);

					tinyinst_.InstrumentIndirect(module, inst,
												 (size_t)address + last_offset,
												 ii_mode,
												 (size_t)address);
				}
				else
				{
					//   call label
					//   jmp return_address
					//  label:
					//    <indirect instrumentation>

					tinyinst_.WriteCode(module, CALL, sizeof(CALL));
					FixDisp4(module, sizeof(JMP));

					size_t translated_return_address = tinyinst_.GetCurrentInstrumentedAddress(module);
					tinyinst_.OnReturnAddress(module, (size_t)return_address, translated_return_address);

					tinyinst_.WriteCode(module, JMP, sizeof(JMP));

					tinyinst_.FixOffsetOrEnqueue(
						module,
						(uint32_t)((size_t)return_address -
								   (size_t)(module->min_address)),
						(uint32_t)(module->instrumented_code_allocated - 4), queue,
						offset_fixes);

					tinyinst_.InstrumentIndirect(module, inst,
												 (size_t)address + last_offset, ii_mode,
												 (size_t)address);
				}
			}
			else
			{
				if (tinyinst_.patch_return_addresses)
				{
					PushReturnAddress(module, (uint64_t)return_address);
					// xed_decoded_inst_t jmp;
					// CallToJmp(&xedd, &jmp);
					FixInstructionAndOutput(
						module,
						inst,
						(unsigned char*)(code_ptr + last_offset),
						(unsigned char*)(address + last_offset), true);
				}
				else
				{
					FixInstructionAndOutput(
						module,
						inst,
						(unsigned char*)(code_ptr + last_offset),
						(unsigned char*)(address + last_offset));

					size_t translated_return_address = tinyinst_.GetCurrentInstrumentedAddress(module);
					tinyinst_.OnReturnAddress(module, (size_t)return_address, translated_return_address);

					tinyinst_.WriteCode(module, JMP, sizeof(JMP));

					tinyinst_.FixOffsetOrEnqueue(
						module,
						(uint32_t)((size_t)return_address -
								   (size_t)(module->min_address)),
						(uint32_t)(module->instrumented_code_allocated - 4), queue,
						offset_fixes);
				}
			}
		}
	}
}

X86Assembler::~X86Assembler()
{
}

void X86Assembler::Init()
{
	ZydisMachineMode machine_mode = tinyinst_.child_ptr_size == 8
										? ZYDIS_MACHINE_MODE_LONG_64
										: ZYDIS_MACHINE_MODE_LEGACY_32;
	ZydisStackWidth  stack_width  = tinyinst_.child_ptr_size == 8
										? ZYDIS_STACK_WIDTH_64
										: ZYDIS_STACK_WIDTH_32;
	ZydisDecoderInit(&decoder, machine_mode, stack_width);

#ifdef _DEBUG
	ZydisFormatterInit(&formatter, ZYDIS_FORMATTER_STYLE_INTEL);
#endif  // _DEBUG
}
