#pragma once

#include "GPCS4Common.h"
#include "GCNParser/Instruction.h"
#include <memory>

namespace pssl
{;


struct GCNInstruction
{
	std::unique_ptr<Instruction> instruction;
	bool hasLiteral;
	uint32_t literalConst;

	GCNInstruction(std::unique_ptr<Instruction> inst, bool hasLit, uint32_t litConst):
		instruction(std::move(inst)),
		hasLiteral(hasLit),
		literalConst(litConst)
	{}

	GCNInstruction(GCNInstruction& other):
		instruction(std::move(other.instruction)),
		hasLiteral(other.hasLiteral),
		literalConst(other.literalConst)
	{}

	GCNInstruction(GCNInstruction&& other):
		instruction(std::move(other.instruction)),
		hasLiteral(other.hasLiteral),
		literalConst(other.literalConst)
	{}

	~GCNInstruction()
	{}
};


}  // namespace pssl
