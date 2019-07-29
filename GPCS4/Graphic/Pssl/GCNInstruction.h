#pragma once

#include "GPCS4Common.h"
#include "GCNParser/Instruction.h"

namespace pssl
{;


struct GCNInstruction
{
	Instruction* instruction;
	bool hasLiteral;
	uint32_t literalConst;
};


}  // namespace pssl
