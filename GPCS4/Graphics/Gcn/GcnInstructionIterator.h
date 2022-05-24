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

		// TODO:
		// Remove this virtual method
		virtual void processInstruction(
			const GcnShaderInstruction& ins);

	protected:

		void advanceProgramCounter(const GcnShaderInstruction& ins);
		void resetProgramCounter();

		uint32_t getBranchTarget(const GcnShaderInstruction& ins);

	protected:
		// PC pointer, will be updated after processing each instruction.
		uint32_t m_programCounter = 0;
	};


}  // namespace sce::gcn