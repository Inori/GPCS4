#pragma once

#include "GcnCommon.h"
#include "GcnInstruction.h"

namespace sce::gcn
{
	bool isVop3BEncoding(GcnOpcode opcode);

	bool isUnconditionalBranch(const GcnShaderInstruction& ins);

	bool isConditionalBranch(const GcnShaderInstruction& ins);

	bool isBranchInstruction(const GcnShaderInstruction& ins);

	bool isForkInstruction(const GcnShaderInstruction& ins);
	/**
	 * \brief Is basic block terminate instruction
	 */
	bool isTerminateInstruction(const GcnShaderInstruction& ins);

	uint32_t calculateBranchTarget(uint32_t pc, const GcnShaderInstruction& ins);

}  // namespace sce::gcn