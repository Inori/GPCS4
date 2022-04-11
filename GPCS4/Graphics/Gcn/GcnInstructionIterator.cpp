#include "GcnInstructionIterator.h"
#include "GcnInstruction.h"

namespace sce::gcn
{
	GcnInstructionIterator::GcnInstructionIterator()
	{
	}

	GcnInstructionIterator::~GcnInstructionIterator()
	{
	}

	void GcnInstructionIterator::updateProgramCounter(const GcnShaderInstruction& ins)
	{
		m_programCounter += ins.length;
	}

}  // namespace sce::gcn