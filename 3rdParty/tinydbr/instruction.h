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

#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#ifdef ARM64
#include "third_party/reil/reil/aarch64/decoder.h"
#else

#include <Zydis/Zydis.h>

struct ZydisInstruction
{
	ZydisDecodedInstruction instruction;
	ZydisDecodedOperand     operands[ZYDIS_MAX_OPERAND_COUNT];
};

#endif

enum InstructionClass {
  INVALID = 0,
  RET,
  IJUMP,
  ICALL,
  OTHER,
};

struct Instruction
{
	size_t           address;
	size_t           length;
	bool             bbend;
	InstructionClass iclass;

#ifdef ARM64
	reil::aarch64::decoder::Instruction instr;
#else
	ZydisInstruction zinst;
#endif
	Instruction() :
		address(0),
		length(0),
		bbend(false),
		iclass(InstructionClass::INVALID),
#ifdef ARM64
		instr({})
#else
		zinst({})
#endif
	{
	}
};

#endif  // INSTRUCTION_H
