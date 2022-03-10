#include "api_helper.h"
#include <Windows.h>
#include "executor.h"

ApiHelper::ApiHelper(Executor& executor):
	m_executor(executor)
{
}

ApiHelper::~ApiHelper()
{
}

void ApiHelper::ExtractAndProtectCodeRanges(
	void* module_base, size_t min_address, size_t max_address,
	std::list<AddressRange>* executable_ranges, size_t* code_size)
{
	LPCVOID                  end_address = (char*)max_address;
	LPCVOID                  cur_address = module_base;
	MEMORY_BASIC_INFORMATION meminfobuf;

	AddressRange newRange;

	for (auto iter = executable_ranges->begin();
		 iter != executable_ranges->end(); iter++)
	{
		free(iter->data);
	}
	executable_ranges->clear();
	*code_size = 0;

	while (cur_address < end_address)
	{
		size_t ret = VirtualQuery(cur_address,
								  &meminfobuf,
								  sizeof(MEMORY_BASIC_INFORMATION));

		if (!ret)
			break;

		if (meminfobuf.Protect & 0xF0)
		{
			// printf("%p, %llx, %lx\n", meminfobuf.BaseAddress, meminfobuf.RegionSize, meminfobuf.Protect);
			newRange.data = (char*)malloc(meminfobuf.RegionSize);

			// TODO:
			// the byte at entry point will be set to 0xCC by visual studio if run in debugger.
			// thus the copied data is wrong at this address.
			std::memcpy(newRange.data, meminfobuf.BaseAddress, meminfobuf.RegionSize);

			uint8_t low      = meminfobuf.Protect & 0xFF;
			low              = low >> 4;
			DWORD newProtect = (meminfobuf.Protect & 0xFFFFFF00) + low;
			DWORD oldProtect;
			if (!VirtualProtect(meminfobuf.BaseAddress,
								meminfobuf.RegionSize,
								newProtect,
								&oldProtect))
			{
				FATAL("Error in VirtualProtectEx");
			}

			newRange.from = (size_t)meminfobuf.BaseAddress;
			newRange.to   = (size_t)meminfobuf.BaseAddress + meminfobuf.RegionSize;
			executable_ranges->push_back(newRange);

			*code_size += newRange.to - newRange.from;
		}

		cur_address = (char*)meminfobuf.BaseAddress + meminfobuf.RegionSize;
	}
}

void ApiHelper::ProtectCodeRanges(std::list<AddressRange>* executable_ranges)
{
	MEMORY_BASIC_INFORMATION meminfobuf;

	for (auto iter = executable_ranges->begin();
		 iter != executable_ranges->end(); iter++)
	{
		size_t ret = VirtualQuery((void*)iter->from,
								  &meminfobuf,
								  sizeof(MEMORY_BASIC_INFORMATION));

		// if the module was already instrumented, everything must be the same as before
		if (!ret)
		{
			FATAL("Error in ProtectCodeRanges."
				  "Target incompatible with persist_instrumentation_data");
		}
		if (iter->from != (size_t)meminfobuf.BaseAddress)
		{
			FATAL("Error in ProtectCodeRanges."
				  "Target incompatible with persist_instrumentation_data");
		}
		if (iter->to != (size_t)meminfobuf.BaseAddress + meminfobuf.RegionSize)
		{
			FATAL("Error in ProtectCodeRanges."
				  "Target incompatible with persist_instrumentation_data");
		}
		if (!(meminfobuf.Protect & 0xF0))
		{
			FATAL("Error in ProtectCodeRanges."
				  "Target incompatible with persist_instrumentation_data");
		}

		uint8_t low      = meminfobuf.Protect & 0xFF;
		low              = low >> 4;
		DWORD newProtect = (meminfobuf.Protect & 0xFFFFFF00) + low;
		DWORD oldProtect;
		if (!VirtualProtect(meminfobuf.BaseAddress,
							meminfobuf.RegionSize,
							newProtect,
							&oldProtect))
		{
			FATAL("Error in VirtualProtectEx");
		}
	}
}

///////////////////////////////////////////////////////////////////

ModuleHelper::ModuleHelper(Executor& executor):
	ApiHelper(executor)
{
}

ModuleHelper::~ModuleHelper()
{
}

uint32_t ModuleHelper::GetImageSize(void* base_address)
{
	unsigned char headers[4096];
	std::memcpy(headers, base_address, 4096);

	DWORD pe_offset;
	pe_offset                = *((DWORD*)(headers + 0x3C));
	unsigned char* pe        = headers + pe_offset;
	DWORD          signature = *((DWORD*)pe);
	if (signature != 0x00004550)
	{
		FATAL("PE signature error\n");
	}
	pe         = pe + 0x18;
	WORD magic = *((WORD*)pe);
	if ((magic != 0x10b) && (magic != 0x20b))
	{
		FATAL("Unknown PE magic value\n");
	}
	DWORD SizeOfImage = *((DWORD*)(pe + 56));
	return SizeOfImage;
}

uint32_t ModuleHelper::GetProcOffset(void* module, const char* name)
{
	void*    proc_address = (void*)GetProcAddress((HMODULE)module, name);
	uint32_t offset =
		static_cast<uint32_t>(
			reinterpret_cast<uintptr_t>(proc_address) - reinterpret_cast<uintptr_t>(module));
	return offset;
}

void ModuleHelper::GetExceptionHandlers(size_t module_header, std::unordered_set<size_t>& handlers)
{
	// only present on x86
	USHORT ProcessMachine = 0;
	USHORT NativeMachine  = 0;
	IsWow64Process2(GetCurrentProcess(), &ProcessMachine, &NativeMachine);
	if (ProcessMachine == IMAGE_FILE_MACHINE_UNKNOWN)
		return;

	DWORD size_of_image = GetImageSize((void*)module_header);

	char* modulebuf = (char*)malloc(size_of_image);
	std::memcpy(modulebuf, (void*)module_header, size_of_image);

	DWORD pe_offset;
	pe_offset       = *((DWORD*)(modulebuf + 0x3C));
	char* pe        = modulebuf + pe_offset;
	DWORD signature = *((DWORD*)pe);
	if (signature != 0x00004550)
	{
		free(modulebuf);
		return;
	}
	pe          = pe + 0x18;
	WORD  magic = *((WORD*)pe);
	DWORD lc_offset;
	DWORD lc_size;
	if (magic == 0x10b)
	{
		lc_offset = *(DWORD*)(pe + 176);
		lc_size   = *(DWORD*)(pe + 180);
	}
	else if (magic == 0x20b)
	{
		lc_offset = *(DWORD*)(pe + 192);
		lc_size   = *(DWORD*)(pe + 196);
	}
	else
	{
		free(modulebuf);
		return;
	}

	if (!lc_offset || (lc_size != 64))
	{
		free(modulebuf);
		return;
	}

	char* lc = modulebuf + lc_offset;

	size_t seh_table_address;
	DWORD  seh_count;
	if (magic == 0x10b)
	{
		seh_table_address = *(DWORD*)(lc + 64);
		seh_count         = *(DWORD*)(lc + 68);
	}
	else if (magic == 0x20b)
	{
		seh_table_address = *(uint64_t*)(lc + 96);
		seh_count         = *(DWORD*)(lc + 104);
	}
	else
	{
		free(modulebuf);
		return;
	}

	size_t seh_table_offset = seh_table_address - module_header;

	DWORD* seh_table = (DWORD*)(modulebuf + seh_table_offset);
	for (DWORD i = 0; i < seh_count; i++)
	{
		handlers.insert(module_header + seh_table[i]);
	}

	free(modulebuf);
}

///////////////////////////////////////////////////////////////////

ShellcodeHelper::ShellcodeHelper(Executor& executor):
	ApiHelper(executor)
{
}

ShellcodeHelper::~ShellcodeHelper()
{
}

uint32_t ShellcodeHelper::GetImageSize(void* base_address)
{
	const TargetModule* mod = GetModule(base_address);
	if (!mod)
	{
		return 0;
	}

	uint32_t code_size = 0;
	for (const auto& sec : mod->code_sections)
	{
		code_size += sec.size;
	}
	return code_size;
}

uint32_t ShellcodeHelper::GetProcOffset(void* module, const char* name)
{
	return 0;
}

void ShellcodeHelper::GetExceptionHandlers(size_t module_header, std::unordered_set<size_t>& handlers)
{
}

const TargetModule* ShellcodeHelper::GetModule(void* base_address)
{
	for (const auto& mod : m_executor.instrument_modules)
	{
		const auto& sections = mod.code_sections;
		for (const auto& sec : sections)
		{
			if (sec.code == base_address)
			{
				return &mod;
			}
		}
	}

	return nullptr;
}
