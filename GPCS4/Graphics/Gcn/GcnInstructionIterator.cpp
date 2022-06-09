#include "GcnInstructionIterator.h"
#include "GcnInstruction.h"
#include "GcnDecoder.h"
#include "GcnInstructionUtil.h"

LOG_CHANNEL(Graphic.Gcn.GcnInstructionIterator);

namespace sce::gcn
{
	GcnInstructionIterator::GcnInstructionIterator()
	{
	}

	GcnInstructionIterator::~GcnInstructionIterator()
	{
	}

	void GcnInstructionIterator::advanceProgramCounter(const GcnShaderInstruction& ins)
	{
		m_programCounter += ins.length;
	}

	void GcnInstructionIterator::resetProgramCounter(uint32_t pc)
	{
		m_programCounter = pc;
	}

	uint32_t GcnInstructionIterator::getBranchTarget(const GcnShaderInstruction& ins)
	{
		return calculateBranchTarget(m_programCounter, ins);
	}

}  // namespace sce::gcn