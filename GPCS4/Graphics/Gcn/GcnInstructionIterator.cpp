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

	void GcnInstructionIterator::advanceProgramCounter(const GcnShaderInstruction& ins)
	{
		m_programCounter += ins.length;
	}

	uint32_t GcnInstructionIterator::getBranchTarget(const GcnShaderInstruction& ins)
	{
		auto     sopp   = gcnInstructionAs<GcnShaderInstSOPP>(ins);
		uint32_t target = static_cast<uint32_t>(static_cast<int32_t>(m_programCounter) +
												static_cast<int32_t>(sopp.control.simm << 2) + 4);
		return target;
	}

}  // namespace sce::gcn