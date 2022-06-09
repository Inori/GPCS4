#pragma once

#include "GcnCommon.h"

namespace sce::gcn
{
	struct GcnShaderInstruction;

	class GcnInstructionIterator
	{
	public:
		GcnInstructionIterator();
		virtual ~GcnInstructionIterator();

	protected:

		void advanceProgramCounter(
			const GcnShaderInstruction& ins);
		void resetProgramCounter(
			uint32_t pc = 0);

		uint32_t getBranchTarget(
			const GcnShaderInstruction& ins);

	protected:
		// PC pointer, will be updated after processing each instruction.
		uint32_t m_programCounter = 0;
	};


}  // namespace sce::gcn