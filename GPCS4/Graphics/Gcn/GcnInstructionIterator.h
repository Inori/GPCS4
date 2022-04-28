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

		virtual void processInstruction(
			const GcnShaderInstruction& ins) = 0;

	protected:

		void updateProgramCounter(const GcnShaderInstruction& ins);

		uint32_t getBranchTarget(const GcnShaderInstruction& ins);

	protected:
		// PC pointer, will be updated after processing each instruction.
		uint32_t m_programCounter = 0;
	};


}  // namespace sce::gcn