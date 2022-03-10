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

#ifndef WINUNWIND_H
#define WINUNWIND_H

#ifdef _WIN64

#include "unwind.h"
#include "tinydbr.h"

#include <vector>
#include <set>
#include <unordered_map>


struct UnwindInfo
{
	uint8_t version_flags;
	uint8_t prolog_size;
	uint8_t frame_register;

	std::vector<uint16_t> unwind_codes;

	size_t handler;
	size_t handler_data;

	uint32_t translated_offset;
};

struct FunctionInfo {
	size_t function_start;
	size_t function_end;

	UnwindInfo* unwind_info;

	// address of this function info in the original code
	size_t function_info_addr;
};

struct FunctionTable {
	FunctionTable() :
		addr(0), max_entries(0)
	{
	}

	size_t addr;
	size_t offset;
	size_t num_entries;
	size_t max_entries;
};

class WinUnwindData : public UnwindData {
public:
	WinUnwindData();

	~WinUnwindData();

	// maps from offset (in the original code) to UnwindInfo
	// presumably a single UnwindInfo can be referenced from
	// multtiple FunctionInfo structures
	std::unordered_map<uint32_t, UnwindInfo *> unwind_infos;
	std::vector<FunctionInfo *> original_function_infos;
	std::set<size_t> handlers;

	bool           table_registered;
	size_t         register_breakpoint;
	size_t         register_continue_IP;
	SavedRegisters register_saved_registers;

	struct ReturnAddressInfo {
		size_t original_return_address;
		FunctionInfo* original_function_info;
	};

	// Maps the return addresses in the instrumented code (the keys)
	// to the return addresses in the original code (the values).
	std::unordered_map<size_t, ReturnAddressInfo> return_addresses;

	FunctionInfo* LookupFunctionInfoForTranslate(size_t IP);
	FunctionInfo* LookupFunctionInfoForOther(size_t IP);
	FunctionInfo* LookupFunctionInfoSlow(size_t IP);

	FunctionInfo* last_lookup_translate;
	FunctionInfo* last_lookup_other;
	FunctionInfo* last_translated_entry;

	// FunctionInfo structures for the instrumented code
	// that haven't been written to the remote process yet
	std::vector<FunctionInfo> translated_infos;

	void DoTranslate(size_t original_address, size_t translated_address);
	
	// maps from original handler to translated handler
	std::unordered_map<size_t, size_t> translated_handler_map;
	// maps from translated handler to original handler
	std::unordered_map<size_t, size_t> handler_start_breakpoints;

	// info about the function table for the instrumented code
	// (in the remote process)
	FunctionTable function_table;

	void CommitLastTranslated();
};

class WinUnwindGenerator : public UnwindGenerator {
public:
	WinUnwindGenerator(TinyDBR& tinyinst) : UnwindGenerator(tinyinst), RtlAddFunctionTable_addr(0) { }

	virtual void Init() override;

	void OnModuleInstrumented(ModuleInfo* module) override;
	void OnModuleUninstrumented(ModuleInfo* module) override;

	void OnModuleLoaded(void* module, char* module_name) override;

	size_t MaybeRedirectExecution(ModuleInfo* module, size_t IP, void* context) override;

	bool HandleBreakpoint(ModuleInfo* module, void* address, void* context) override;

	void OnReturnAddress(ModuleInfo* module,
		size_t original_address,
		size_t translated_address) override;

	void OnBasicBlockStart(ModuleInfo* module,
		size_t original_address,
		size_t translated_address) override;

	void OnInstruction(ModuleInfo* module,
		size_t original_address,
		size_t translated_address) override;

	void OnBasicBlockEnd(ModuleInfo* module,
		size_t original_address,
		size_t translated_address) override;

	bool Is64BitOnly() override { return true; }

protected:
	UnwindInfo* ReadUnwindInfo(ModuleInfo* module, unsigned char* modulebuf, size_t image_size, uint32_t unwind_info_offset);

	int GetExceptionTableOffsetAndSize(char* data, uint32_t* offset, uint32_t* size);

	void WriteFunctionInfo(ModuleInfo* module, FunctionInfo *info, FunctionTable* functionTable);

	uint32_t WriteUnwindInfo(ModuleInfo* module, UnwindInfo* info);

	void WriteHandler(ModuleInfo* module, size_t original_handler);

	size_t WriteFunctionTable(ModuleInfo* module, FunctionTable &functionTable, size_t max_entries);

	void FixUnwindCodes(UnwindInfo* info);

	size_t RtlAddFunctionTable_addr;
};

#endif // _WIN64

#endif // WINUNWIND_H
