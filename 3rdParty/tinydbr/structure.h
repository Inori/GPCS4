#pragma once

#include <cstdint>
#include <vector>
#include <string>


struct AddressRange
{
	size_t from = 0;
	size_t to   = 0;
	char*  data = nullptr;
};

struct CodeSection
{
	void*  code;
	size_t size;
};

struct Options
{
	bool    shellcode_mode                = false;
	bool    trace_debug_events            = false;
	bool    instrument_modules_on_load    = false;
	bool    patch_return_addresses        = false;
	bool    instrument_cross_module_calls = true;
	bool    persist_instrumentation_data  = true;
	bool    trace_basic_blocks            = false;
	bool    trace_module_entries          = false;
	int32_t sp_offset                     = 0;
	bool    generate_unwind               = true;
};

struct TargetModule
{
	// module name
	std::string name;
	// is main executable module.
	bool is_main = false;
	// is shellcode without module.
	bool is_shellcode = false;
	// only valid if is_shellcode is true.
	std::vector<CodeSection> code_sections;
};