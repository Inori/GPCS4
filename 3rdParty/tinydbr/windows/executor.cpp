#include "executor.h"
#include "dllnotify.h"
#include "tinydbr.h"

#include <Psapi.h>


Executor::Executor()
{
}

Executor::~Executor()
{
}

Executor* Executor::instance = nullptr;

void Executor::Init(const std::vector<TargetModule>& target_modules,
					const Options&                   options)
{
	instance           = this;
	instrument_modules = target_modules;

	shellcode_mode     = options.shellcode_mode;
	trace_debug_events = options.trace_debug_events;

	veh_handle        = InstallVEHHandler();
	if (!shellcode_mode)
	{
		dll_notify_cookie = InstallDllNotification(&Executor::DllNotificationHandler, this);
	}

	DWORD dwPid = GetCurrentProcessId();
	self_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	if (!self_handle)
	{
		FATAL("open self process failed.");
	}

	// get allocation granularity
	SYSTEM_INFO system_info;
	GetSystemInfo(&system_info);
	allocation_granularity = system_info.dwAllocationGranularity;

	if (shellcode_mode)
	{
		api_helper = std::make_unique<ShellcodeHelper>(*this);
	}
	else
	{
		api_helper = std::make_unique<ModuleHelper>(*this);
	}

#ifdef _DEBUG	
	AllocConsole();
	FILE* fDummy;
	freopen_s(&fDummy, "CONIN$", "r", stdin);
	freopen_s(&fDummy, "CONOUT$", "w", stderr);
	freopen_s(&fDummy, "CONOUT$", "w", stdout);
#endif

}

void Executor::Unit()
{
	if (veh_handle)
	{
		UninstallVEHHandler(veh_handle);
		veh_handle = nullptr;
	}

	if (dll_notify_cookie)
	{
		UninstallDllNotification(dll_notify_cookie);
		dll_notify_cookie = nullptr;
	}
}


long Executor::OnVEHException(EXCEPTION_POINTERS* ExceptionInfo)
{
	LONG action = EXCEPTION_CONTINUE_SEARCH;
	std::lock_guard<std::mutex> lock(exception_mutex);
	do
	{
		Exception exception = {};
		ConvertException(ExceptionInfo->ExceptionRecord, &exception);

		bool handled = OnException(&exception, ExceptionInfo->ContextRecord);

		if (handled)
		{
			action = EXCEPTION_CONTINUE_EXECUTION;
		}

	} while (false);

	return action;
}

void Executor::OnEntrypoint()
{
	HMODULE* module_handles = NULL;
	DWORD    num_modules    = GetLoadedModules((void**)&module_handles);
	for (DWORD i = 0; i < num_modules; i++)
	{
		char base_name[MAX_PATH];
		GetModuleBaseNameA(self_handle, module_handles[i], (LPSTR)(&base_name), sizeof(base_name));
		if (trace_debug_events)
			printf("Debugger: Loaded module %s at %p\n", base_name, (void*)module_handles[i]);
		OnModuleLoaded((void*)module_handles[i], base_name);
	}
	if (module_handles)
		free(module_handles);

	child_entrypoint_reached = true;

	if (trace_debug_events)
		printf("Debugger: Process entrypoint reached\n");
}

void Executor::OnProcessCreated()
{
}

void Executor::OnProcessExit()
{
}

void Executor::OnModuleLoaded(void* module, char* module_name)
{
}

void Executor::OnModuleUnloaded(void* module)
{
}

// should return true if the exception has been handled
bool Executor::OnException(Exception* exception_record, Context* context_record)
{
	if (!child_entrypoint_reached && !shellcode_mode)
	{
		void* main_module_base = GetModuleHandleA(NULL);
		void* entry_point      = GetModuleEntrypoint(main_module_base);
		if (entry_point == exception_record->ip)
		{
			OnEntrypoint();
		}
	}
	return false;
}

void Executor::OnCrashed(Exception* exception_record)
{
}

size_t Executor::GetTranslatedAddress(size_t address)
{
	return 0;
}

// detects executable memory regions in the module
// makes them non-executable
// and copies code out
void Executor::ExtractAndProtectCodeRanges(
	void*                    module_base,
	size_t                   min_address,
	size_t                   max_address,
	std::list<AddressRange>* executable_ranges,
	size_t*                  code_size)
{
	api_helper->ExtractAndProtectCodeRanges(
		module_base, min_address, max_address, 
		executable_ranges, code_size);
}

// sets all pages containing (previously detected)
// code to non-executable
void Executor::ProtectCodeRanges(std::list<AddressRange>* executable_ranges)
{
	api_helper->ProtectCodeRanges(executable_ranges);
}

uint32_t Executor::GetImageSize(void* base_address)
{
	return api_helper->GetImageSize(base_address);
}

void Executor::GetImageSize(void* base_address, size_t* min_address, size_t* max_address)
{
	*min_address      = (size_t)base_address;
	DWORD SizeOfImage = GetImageSize(base_address);
	*max_address      = *min_address + SizeOfImage;
}

// allocates memory in process as close as possible
// to max_address, but at address larger than min_address
void* Executor::RemoteAllocateBefore(
	uint64_t         min_address,
	uint64_t         max_address,
	size_t           size,
	MemoryProtection protection)
{
	DWORD protection_flags = WindowsProtectionFlags(protection);

	MEMORY_BASIC_INFORMATION meminfobuf;
	void*                    ret_address = NULL;

	uint64_t cur_code = max_address;
	while (cur_code > min_address)
	{
		// Don't attempt allocating on the null page
		if (cur_code < 0x1000)
			break;

		size_t step = size;

		size_t query_ret = VirtualQuery((LPCVOID)cur_code,
										&meminfobuf,
										sizeof(MEMORY_BASIC_INFORMATION));
		if (!query_ret)
			break;

		if (meminfobuf.State == MEM_FREE)
		{
			if (meminfobuf.RegionSize >= size)
			{
				size_t address = (size_t)meminfobuf.BaseAddress +
								 (meminfobuf.RegionSize - size);
				ret_address = VirtualAlloc((LPVOID)address,
										   size,
										   MEM_COMMIT | MEM_RESERVE,
										   protection_flags);
				if (ret_address)
				{
					if (((size_t)ret_address >= min_address) &&
						((size_t)ret_address <= max_address))
					{
						return ret_address;
					}
					else
					{
						return NULL;
					}
				}
			}
			else
			{
				step = size - meminfobuf.RegionSize;
			}
		}

		cur_code = (size_t)meminfobuf.BaseAddress;
		if (cur_code < step)
			break;
		else
			cur_code -= step;
	}

	return ret_address;
}

// allocates memory in target process as close as possible
// to min_address, but not higher than max_address
void* Executor::RemoteAllocateAfter(
	uint64_t         min_address,
	uint64_t         max_address,
	size_t           size,
	MemoryProtection protection)
{
	DWORD protection_flags = WindowsProtectionFlags(protection);

	MEMORY_BASIC_INFORMATION meminfobuf;
	void*                    ret_address = NULL;

	uint64_t cur_code = min_address;
	while (cur_code < max_address)
	{
		size_t query_ret = VirtualQuery((LPCVOID)cur_code,
										&meminfobuf,
										sizeof(MEMORY_BASIC_INFORMATION));
		if (!query_ret)
			break;

		if (meminfobuf.State == MEM_FREE)
		{
			size_t region_address = (size_t)meminfobuf.BaseAddress;
			size_t region_size    = meminfobuf.RegionSize;
			// make sure we are allocating on an address that
			// is aligned according to allocation_granularity
			size_t alignment = region_address & (allocation_granularity - 1);
			if (alignment)
			{
				size_t offset = (allocation_granularity - alignment);
				region_address += offset;
				if (region_size > offset)
				{
					region_size -= offset;
				}
				else
				{
					region_size = 0;
				}
			}
			if (region_size >= size)
			{
				ret_address = VirtualAlloc((LPVOID)region_address,
										   size,
										   MEM_COMMIT | MEM_RESERVE,
										   protection_flags);
				if (ret_address)
				{
					if (((size_t)ret_address >= min_address) &&
						((size_t)ret_address <= max_address))
					{
						return ret_address;
					}
					else
					{
						return NULL;
					}
				}
			}
		}

		cur_code = (size_t)meminfobuf.BaseAddress + meminfobuf.RegionSize;
	}

	return ret_address;
}

uint32_t Executor::WindowsProtectionFlags(MemoryProtection protection)
{
	switch (protection)
	{
	case READONLY:
		return PAGE_READONLY;
	case READWRITE:
		return PAGE_READWRITE;
	case READEXECUTE:
		return PAGE_EXECUTE_READ;
	case READWRITEEXECUTE:
		return PAGE_EXECUTE_READWRITE;
	default:
		FATAL("Unumplemented memory protection");
	}
}

uint32_t Executor::GetLoadedModules(void** modules)
{
	DWORD    module_handle_storage_size = 1024 * sizeof(HMODULE);
	HMODULE* module_handles             = (HMODULE*)malloc(module_handle_storage_size);
	DWORD    hmodules_size;
	while (true)
	{
		if (!EnumProcessModulesEx(self_handle,
								  module_handles,
								  module_handle_storage_size,
								  &hmodules_size,
								  LIST_MODULES_ALL))
		{
			FATAL("EnumProcessModules failed, %x\n", (uint32_t)GetLastError());
		}
		if (hmodules_size <= module_handle_storage_size)
			break;
		module_handle_storage_size *= 2;
		module_handles = (HMODULE*)realloc(module_handles, module_handle_storage_size);
	}
	*modules = module_handles;
	return hmodules_size / sizeof(HMODULE);
}

void Executor::ConvertException(
	EXCEPTION_RECORD* win_exception_record,
	Exception*        exception)
{
	switch (win_exception_record->ExceptionCode)
	{
	case EXCEPTION_BREAKPOINT:
	case 0x4000001f:
		exception->type = BREAKPOINT;
		break;
	case EXCEPTION_ACCESS_VIOLATION:
		exception->type = ACCESS_VIOLATION;
		break;
	case EXCEPTION_ILLEGAL_INSTRUCTION:
		exception->type = ILLEGAL_INSTRUCTION;
		break;
	case EXCEPTION_STACK_OVERFLOW:
		exception->type = STACK_OVERFLOW;
		break;
	default:
		exception->type = OTHER;
		break;
	}

	exception->ip = win_exception_record->ExceptionAddress;

	exception->maybe_execute_violation = false;
	exception->maybe_write_violation   = false;
	exception->access_address          = 0;
	if (win_exception_record->ExceptionCode == EXCEPTION_ACCESS_VIOLATION)
	{
		if (win_exception_record->ExceptionInformation[0] == 8)
		{
			exception->maybe_execute_violation = true;
		}
		if (win_exception_record->ExceptionInformation[0] == 1)
		{
			exception->maybe_write_violation = true;
		}

		exception->access_address = (void*)(win_exception_record->ExceptionInformation[1]);
	}

	if (win_exception_record->ExceptionCode == EXCEPTION_PRIV_INSTRUCTION)
	{
		uint8_t* ip = reinterpret_cast<uint8_t*>(win_exception_record->ExceptionAddress);
		// treat hlt as breakpoint instruction.
		// see comments of BREAKPOINT.
		const uint8_t hlt_code = 0xF4;
		if (ip[0] == hlt_code)
		{
			exception->type = BREAKPOINT;
		}
	}
}

void* Executor::RemoteAllocateNear(
	uint64_t         region_min,
	uint64_t         region_max,
	size_t           size,
	MemoryProtection protection,
	bool             use_shared_memory /*= false*/)
{
	void* ret = NULL;

	// try before first
	uint64_t min_address = region_max;
	if (min_address < 0x80000000)
		min_address = 0;
	else
		min_address -= 0x80000000;
	uint64_t max_address = region_min;
	if (max_address < size)
		max_address = 0;
	else
		max_address -= size;

	ret = RemoteAllocateBefore(min_address,
							   max_address,
							   size,
							   protection);

	if (ret)
		return ret;

	min_address                = region_max;
	uint64_t address_range_max = 0xFFFFFFFFFFFFFFFFULL;
	if (child_ptr_size == 4)
	{
		address_range_max = 0xFFFFFFFFULL;
	}
	if ((address_range_max - 0x80000000) < region_min)
	{
		max_address = address_range_max - size;
	}
	else
	{
		max_address = region_min + 0x80000000 - size;
	}

	ret = RemoteAllocateAfter(min_address,
							  max_address,
							  size,
							  protection);

	return ret;
}

void Executor::RemoteFree(void* address, size_t size)
{
	VirtualFree(address, 0, MEM_RELEASE);
}

void Executor::RemoteWrite(void* address, void* buffer, size_t size)
{
	SIZE_T size_written;
	if (WriteProcessMemory(
			self_handle,
			address,
			buffer,
			size,
			&size_written))
	{
		return;
	}

	// we need to
	// (a) read page permissions
	// (b) make it writable, and
	// (c) restore permissions
	DWORD oldProtect;
	if (!VirtualProtectEx(self_handle,
						  address,
						  size,
						  PAGE_READWRITE,
						  &oldProtect))
	{
		FATAL("Error in VirtualProtectEx");
	}

	if (!WriteProcessMemory(
			self_handle,
			address,
			buffer,
			size,
			&size_written))
	{
		FATAL("Error writing target memory\n");
	}

	DWORD ignore;
	if (!VirtualProtectEx(self_handle,
						  address,
						  size,
						  oldProtect,
						  &ignore))
	{
		FATAL("Error in VirtualProtectEx");
	}
}

void Executor::RemoteRead(void* address, void* buffer, size_t size)
{
	SIZE_T size_read;
	if (!ReadProcessMemory(
			self_handle,
			address,
			buffer,
			size,
			&size_read))
	{
		FATAL("Error reading target memory\n");
	}
}

void Executor::RemoteProtect(void* address, size_t size, MemoryProtection protect)
{
	DWORD protection_flags = WindowsProtectionFlags(protect);
	DWORD old_protect;

	if (!VirtualProtect(address,
						size,
						protection_flags,
						&old_protect))
	{
		FATAL("Could not apply memory protection");
	}
}

size_t Executor::GetRegister(Context* context, Register r)
{
	CONTEXT& lcContext = *context;

#ifdef _WIN64

	switch (r)
	{
	case RAX:
		return lcContext.Rax;
	case RCX:
		return lcContext.Rcx;
	case RDX:
		return lcContext.Rdx;
	case RBX:
		return lcContext.Rbx;
	case RSP:
		return lcContext.Rsp;
	case RBP:
		return lcContext.Rbp;
	case RSI:
		return lcContext.Rsi;
	case RDI:
		return lcContext.Rdi;
	case R8:
		return lcContext.R8;
	case R9:
		return lcContext.R9;
	case R10:
		return lcContext.R10;
	case R11:
		return lcContext.R11;
	case R12:
		return lcContext.R12;
	case R13:
		return lcContext.R13;
	case R14:
		return lcContext.R14;
	case R15:
		return lcContext.R15;
	case RIP:
		return lcContext.Rip;
	default:
		FATAL("Unimplemented");
	}

#else

	switch (r)
	{
	case RAX:
		return lcContext.Eax;
	case RCX:
		return lcContext.Ecx;
	case RDX:
		return lcContext.Edx;
	case RBX:
		return lcContext.Ebx;
	case RSP:
		return lcContext.Esp;
	case RBP:
		return lcContext.Ebp;
	case RSI:
		return lcContext.Esi;
	case RDI:
		return lcContext.Edi;
	case RIP:
		return lcContext.Eip;
	default:
		FATAL("Unimplemented");
	}

#endif
}

void Executor::SetRegister(Context* context, Register r, size_t value)
{
	CONTEXT& lcContext = *context;

#ifdef _WIN64

	switch (r)
	{
	case RAX:
		lcContext.Rax = value;
		break;
	case RCX:
		lcContext.Rcx = value;
		break;
	case RDX:
		lcContext.Rdx = value;
		break;
	case RBX:
		lcContext.Rbx = value;
		break;
	case RSP:
		lcContext.Rsp = value;
		break;
	case RBP:
		lcContext.Rbp = value;
		break;
	case RSI:
		lcContext.Rsi = value;
		break;
	case RDI:
		lcContext.Rdi = value;
		break;
	case R8:
		lcContext.R8 = value;
		break;
	case R9:
		lcContext.R9 = value;
		break;
	case R10:
		lcContext.R10 = value;
		break;
	case R11:
		lcContext.R11 = value;
		break;
	case R12:
		lcContext.R12 = value;
		break;
	case R13:
		lcContext.R13 = value;
		break;
	case R14:
		lcContext.R14 = value;
		break;
	case R15:
		lcContext.R15 = value;
		break;
	case RIP:
		lcContext.Rip = value;
		break;
	default:
		FATAL("Unimplemented");
	}

#else

	switch (r)
	{
	case RAX:
		lcContext.Eax = value;
		break;
	case RCX:
		lcContext.Ecx = value;
		break;
	case RDX:
		lcContext.Edx = value;
		break;
	case RBX:
		lcContext.Ebx = value;
		break;
	case RSP:
		lcContext.Esp = value;
		break;
	case RBP:
		lcContext.Ebp = value;
		break;
	case RSI:
		lcContext.Esi = value;
		break;
	case RDI:
		lcContext.Edi = value;
		break;
	case RIP:
		lcContext.Eip = value;
		break;
	default:
		FATAL("Unimplemented");
	}

#endif
}

void Executor::SaveRegisters(Context* context, SavedRegisters* registers)
{
	memcpy(&registers->saved_context, context, sizeof(registers->saved_context));
}

void Executor::RestoreRegisters(Context* context, SavedRegisters* registers)
{
	memcpy(context, &registers->saved_context, sizeof(registers->saved_context));
}

uint32_t Executor::GetProcOffset(void* module, const char* name)
{
	return api_helper->GetProcOffset(module, name);
}

// Gets the registered safe exception handlers for the module
void Executor::GetExceptionHandlers(size_t module_header, std::unordered_set<size_t>& handlers)
{
	api_helper->GetExceptionHandlers(module_header, handlers);
}

template <typename T>
void Executor::PatchPointersRemoteT(size_t min_address, size_t max_address, std::unordered_map<size_t, size_t>& search_replace)
{
	size_t module_size = max_address - min_address;
	char*  buf         = (char*)malloc(module_size);
	RemoteRead((void*)min_address, buf, module_size);

	size_t remote_address = min_address;
	for (size_t i = 0; i < (module_size - child_ptr_size + 1); i++)
	{
		T    ptr  = *(T*)(buf + i);
		auto iter = search_replace.find(ptr);
		if (iter != search_replace.end())
		{
			// printf("patching entry %zx at address %zx\n", (size_t)ptr, remote_address);
			T fixed_ptr = (T)iter->second;
			RemoteWrite((void*)remote_address, &fixed_ptr, child_ptr_size);
		}
		remote_address += 1;
	}

	free(buf);
}

void Executor::PatchPointersRemote(size_t min_address, size_t max_address, std::unordered_map<size_t, size_t>& search_replace)
{
	if (child_ptr_size == 4)
	{
		PatchPointersRemoteT<uint32_t>(min_address, max_address, search_replace);
	}
	else
	{
		PatchPointersRemoteT<uint64_t>(min_address, max_address, search_replace);
	}
}

void* Executor::InstallVEHHandler()
{
	// TODO:
	// There left a problem:
	// If the target process then register a VEH handler at the front of handler list after
	// we do, the exception will pass to that handler first, and since the target's code is
	// protected to be non-executable by us, then there will be an infinite loop calling
	// the target's VEH handler and finally cause stack overflow.
	void* handle = AddVectoredExceptionHandler(TRUE, &Executor::VectoredExceptionHandler);
	return handle;
}

void Executor::UninstallVEHHandler(void* handle)
{
	do
	{
		if (!handle)
		{
			break;
		}

		RemoveVectoredExceptionHandler(handle);

	} while (false);
}


LONG WINAPI Executor::VectoredExceptionHandler(EXCEPTION_POINTERS* ExceptionInfo)
{
	LONG action = EXCEPTION_CONTINUE_SEARCH;
	do
	{
		Executor* executor = reinterpret_cast<Executor*>(instance);
		if (!executor)
		{
			break;
		}

		action = executor->OnVEHException(ExceptionInfo);

	} while (false);

	return action;
}

void __stdcall Executor::DllNotificationHandler(
	unsigned long                    notification_reason,
	const LDR_DLL_NOTIFICATION_DATA* notification_data,
	void*                            context)
{
	Executor* executor = reinterpret_cast<Executor*>(context);
	if (notification_reason == LDR_DLL_NOTIFICATION_REASON_LOADED)
	{
		auto& loaded = notification_data->Loaded;
		std::string dll_name = UnicodeToAnsi(loaded.BaseDllName->Buffer, CP_ACP);
		executor->OnModuleLoaded(loaded.DllBase, const_cast<char*>(dll_name.c_str()));
	}
	else if (notification_reason == LDR_DLL_NOTIFICATION_REASON_UNLOADED)
	{
		auto& unloaded = notification_data->Unloaded;
		executor->OnModuleUnloaded(unloaded.DllBase);
	}
}
