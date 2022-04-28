#include "GcnInstructionIterator.h"
#include "GcnInstruction.h"
#include "GcnDecoder.h"

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

	uint32_t GcnInstructionIterator::getBranchTarget(const GcnShaderInstruction& ins)
	{
		auto     sopp   = gcnInstructionAs<GcnShaderInstSOPP>(ins);
		uint32_t target = m_programCounter + sopp.control.simm * 4 + 4;
		return target;
	}

}  // namespace sce::gcn