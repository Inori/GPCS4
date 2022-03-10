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

#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <cstddef>
#include <list>
#include <set>

#include "instruction.h"

struct IndirectBreakpoinInfo;
class TinyDBR;
class ModuleInfo;

class Assembler {
 public:
  Assembler(TinyDBR &tinyinst) : tinyinst_(tinyinst) {}
  virtual ~Assembler() = default;

  virtual void Init() = 0;

  virtual bool DecodeInstruction(Instruction &inst,
                                 const unsigned char *address,
                                 unsigned int max_size) = 0;
  virtual void FixInstructionAndOutput(
      ModuleInfo *module,
      Instruction &inst,
      const unsigned char *input,
      const unsigned char *input_address_remote,
      bool convert_call_to_jmp = false) = 0;
  virtual void HandleBasicBlockEnd(
      const char *address, ModuleInfo *module,
      std::set<char *> *queue,
      std::list<std::pair<uint32_t, uint32_t>> *offset_fixes,
      Instruction &inst,
      const char *code_ptr,
      size_t offset,
      size_t last_offset) = 0;

  virtual void JmpAddress(ModuleInfo *module, size_t address) = 0;
  virtual void Nop(ModuleInfo *module) = 0;
  virtual void Breakpoint(ModuleInfo *module) = 0;
  virtual void Crash(ModuleInfo *module) = 0;

  virtual void OffsetStack(ModuleInfo *module, int32_t offset) = 0;

  virtual bool IsRipRelative(ModuleInfo*  module,
							 Instruction& inst,
							 size_t       instruction_address,
							 size_t*      mem_address = nullptr)  = 0;
  virtual bool IsRspRelative(Instruction& inst,
							 size_t*      displacement = nullptr) = 0;

  virtual void TranslateJmp(ModuleInfo *module,
                            ModuleInfo *target_module,
                            size_t original_target,
                            IndirectBreakpoinInfo& breakpoint_info,
                            bool global_indirect,
                            size_t previous_offset) = 0;
  virtual void InstrumentLocalIndirect(ModuleInfo *module,
                                       Instruction &inst,
                                       size_t instruction_address,
                                       size_t bb_address) = 0;
  virtual void InstrumentGlobalIndirect(ModuleInfo *module,
                                        Instruction &inst,
                                        size_t instruction_address) = 0;
  virtual void FixOffset(ModuleInfo *module,
                 uint32_t jmp_offset,
                 uint32_t target_offset) = 0;

  virtual void PrintInstruction(const Instruction& inst) = 0;

 protected:
  TinyDBR &tinyinst_;
};

#endif  // ASSEMBLER_H
