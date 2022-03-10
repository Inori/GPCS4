/*
Copyright 2020 Google LLC

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

#ifndef TINYINST_H
#define TINYINST_H

#include <list>
#include <set>
#include <unordered_map>
#include <unordered_set>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
#include "windows/executor.h"
#elif __APPLE__
#include "macOS/debugger.h"
#endif

#include "assembler.h"
#include "common.h"
#include "instruction.h"
#include "unwind.h"

#if defined(_WIN64)

#include "windows/winunwind.h"

#elif __APPLE__

#include "macOS/unwindmacos.h"
class UnwindGeneratorMacOS;

#endif

// must be a power of two
#define JUMPTABLE_SIZE 0x2000

// we will allocate
// original_code_size * CODE_SIZE_MULTIPLIER +
// JUMPTABLE_SIZE * child_ptr_size
// for instrumented code
#define CODE_SIZE_MULTIPLIER 4


class ModuleInfo;

enum InstructionResult
{
	INST_HANDLED,
	INST_NOTHANDLED,
	INST_STOPBB
};

class TinyDBR : public Executor
{
public:
	virtual void Init(const std::vector<TargetModule>& target_modules,
					  const Options&                   options) override;


	virtual void Unit() override;

	void EnableInstrumentation();

	void DisableInstrumentation();

protected:
	enum IndirectInstrumentation
	{
		II_NONE,
		II_GLOBAL,
		II_LOCAL,
		II_AUTO
	};

	enum PatchModuleEntriesValue
	{
		OFF  = 0,
		DATA = 1,
		CODE = 2,
		ALL  = DATA | CODE
	};

	std::list<ModuleInfo*> instrumented_modules;

	struct CrossModuleLink
	{
		ModuleInfo* module1;
		ModuleInfo* module2;
		size_t      offset1;
		size_t      offset2;
	};

	virtual void OnEntrypoint() override;
	virtual void OnProcessCreated() override;
	virtual void OnProcessExit() override;
	virtual void OnModuleLoaded(void* module, char* module_name) override;
	virtual void OnModuleUnloaded(void* module) override;
	virtual bool OnException(Exception* exception_record, Context* context_record) override;
	virtual void OnCrashed(Exception* exception_record) override;

	virtual size_t GetTranslatedAddress(size_t address) override;

	void   WriteCode(ModuleInfo* module, void* data, size_t size);
	void   WriteCodeAtOffset(ModuleInfo* module, size_t offset, void* data, size_t size);
	void   WritePointer(ModuleInfo* module, size_t value);
	void   WritePointerAtOffset(ModuleInfo* module, size_t value, size_t offset);
	size_t ReadPointer(ModuleInfo* module, size_t offset);

	size_t GetCurrentInstrumentedAddress(ModuleInfo* module);
	void   CommitCode(ModuleInfo* module, size_t start_offset, size_t size);
	void   CommitValueAtomic(ModuleInfo* module, size_t start_offset, size_t size);

	ModuleInfo*   GetModuleByName(const char* name);
	ModuleInfo*   GetModule(size_t address);
	ModuleInfo*   GetModuleFromInstrumented(size_t address);
	AddressRange* GetRegion(ModuleInfo* module, size_t address);

	// instrumentation API
	virtual void OnModuleEntered(ModuleInfo* module, size_t entry_address)
	{
	}
	virtual void InstrumentBasicBlock(ModuleInfo* module, size_t bb_address)
	{
	}
	virtual void InstrumentEdge(ModuleInfo* previous_module,
								ModuleInfo* next_module,
								size_t      previous_address,
								size_t      next_address)
	{
	}

	virtual InstructionResult InstrumentInstruction(ModuleInfo*  module,
													Instruction& inst,
													size_t       bb_address,
													size_t       instruction_address)
	{
		return INST_NOTHANDLED;
	}

	virtual void OnModuleInstrumented(ModuleInfo* module);
	virtual void OnModuleUninstrumented(ModuleInfo* module);

	int32_t    sp_offset;
	Assembler* assembler_;

	UnwindGenerator* unwind_generator;
	virtual void     OnReturnAddress(ModuleInfo* module, size_t original_address, size_t translated_address);

private:
	bool        HandleBreakpoint(void* address, Context* context);
	void        OnInstrumentModuleLoaded(void* module, ModuleInfo* target_module);
	ModuleInfo* IsInstrumentModule(const char* module_name);
	void        InstrumentAllLoadedModules();
	void        InstrumentModule(ModuleInfo* module);
	void        ClearInstrumentation(ModuleInfo* module);
	bool        TryExecuteInstrumented(Context* context, char* address);
	size_t      GetTranslatedAddress(ModuleInfo* module, size_t address);
	void        TranslateBasicBlock(char*                                     address,
									ModuleInfo*                               module,
									std::set<char*>*                          queue,
									std::list<std::pair<uint32_t, uint32_t>>* offset_fixes);
	void        TranslateBasicBlockRecursive(char* address, ModuleInfo* module);
	void        FixOffsetOrEnqueue(ModuleInfo*                               module,
								   uint32_t                                  bb,
								   uint32_t                                  jmp_offset,
								   std::set<char*>*                          queue,
								   std::list<std::pair<uint32_t, uint32_t>>* offset_fixes);
	void        InvalidInstruction(ModuleInfo* module);

	// needed to support cross-module linking
	// on module unloads / reloads
	void InvalidateCrossModuleLink(CrossModuleLink* link);
	void FixCrossModuleLink(CrossModuleLink* link);
	void FixCrossModuleLinks(ModuleInfo* module);
	void InvalidateCrossModuleLinks(ModuleInfo* module);
	void InvalidateCrossModuleLinks();
	void ClearCrossModuleLinks(ModuleInfo* module);
	void ClearCrossModuleLinks();

	void PatchModuleEntries(ModuleInfo* module);

	// functions related to indirect jump/call instrumentation
	void InitGlobalJumptable(ModuleInfo* module);
	void InstrumentIndirect(ModuleInfo*             module,
							Instruction&            inst,
							size_t                  instruction_address,
							IndirectInstrumentation mode,
							size_t                  bb_address);

	// returns the indirect instrumentation mode that should be used for a particular call
	// can be overriden
	virtual IndirectInstrumentation ShouldInstrumentIndirect(ModuleInfo*  module,
															 Instruction& inst,
															 size_t       instruction_address);

	size_t AddTranslatedJump(ModuleInfo*            module,
							 ModuleInfo*            target_module,
							 size_t                 original_target,
							 size_t                 actual_target,
							 size_t                 list_head_offset,
							 IndirectBreakpoinInfo& breakpoint_info,
							 bool                   global_indirect);
	bool HandleIndirectJMPBreakpoint(void* address, Context* context);

	void PatchPointersLocal(char* buf, size_t size, std::unordered_map<size_t, size_t>& search_replace, bool commit_code, ModuleInfo* module);
	template <typename T>
	void PatchPointersLocalT(char* buf, size_t size, std::unordered_map<size_t, size_t>& search_replace, bool commit_code, ModuleInfo* module);
	template <typename T>
	void CommitValueAtomicT(ModuleInfo* module, size_t start_offset);
	void InstrumentMainModule(const TargetModule& module);
	void InitUnwindGenerator();

private:
	IndirectInstrumentation indirect_instrumentation_mode;

	bool instrument_cross_module_calls;
	bool patch_return_addresses;
	bool persist_instrumentation_data;

	bool trace_basic_blocks;
	bool trace_module_entries;

	bool generate_unwind;

	// these could be indexed by module1 and module2 for performance
	// but the assumption for now is that there won't be too many of
	// them so a flat structure shoudl be ok for now
	std::list<CrossModuleLink> cross_module_links;

	bool instrumentation_disabled;
	bool instrument_modules_on_load;

	PatchModuleEntriesValue patch_module_entries;

	friend class Asssembler;
	friend class X86Assembler;
	friend class Arm64Assembler;
	friend class ModuleInfo;
	friend class UnwindGenerator;
#if defined(_WIN64)
	friend class WinUnwindGenerator;
#elif __APPLE__
	friend class UnwindGeneratorMacOS;
#endif
};


struct IndirectBreakpoinInfo
{
	size_t list_head;
	size_t source_bb;
#ifdef ARM64
	uint8_t branch_register;
#endif
};

class ModuleInfo
{
public:
	ModuleInfo();
	void ClearInstrumentation();

	std::string             module_name;
	void*                   module_header;
	size_t                  min_address;
	size_t                  max_address;
	size_t                  code_size;
	bool                    loaded;
	bool                    instrumented;
	bool                    main_module;
	std::list<AddressRange> executable_ranges;

	size_t instrumented_code_size;
	size_t instrumented_code_allocated;
	char*  instrumented_code_local;
	char*  instrumented_code_remote;
	char*  instrumented_code_remote_previous;

	std::unordered_map<uint32_t, uint32_t> basic_blocks;

	size_t br_indirect_newtarget_global;

	// per callsite jumplist breakpoint
	// from breakpoint address to list head offset
	std::unordered_map<size_t, IndirectBreakpoinInfo> br_indirect_newtarget_list;

	size_t jumptable_offset;
	size_t jumptable_address_offset;

	std::unordered_set<size_t>         invalid_instructions;
	std::unordered_map<size_t, size_t> tracepoints;

	std::unordered_set<size_t> entry_offsets;

	UnwindData* unwind_data;

	// clients can use this to store additional data
	// about the module
	void* client_data;
};

#endif  // TINYINST_H
