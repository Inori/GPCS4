#include "GCNInstructionIterator.h"
#include "GCNParser/ParserSI.h"

LOG_CHANNEL(Graphic.Pssl.GCNInstructionIterator);

namespace pssl
{;

GCNInstructionIterator::GCNInstructionIterator()
{
}

GCNInstructionIterator::~GCNInstructionIterator()
{
}

void GCNInstructionIterator::updateProgramCounter(const GCNInstruction& ins)
{
	uint32_t insLength = ins.instruction->GetInstructionLength();

	if (insLength == sizeof(uint32_t))
	{
		m_programCounter += insLength;
		if (ins.hasLiteral)
		{
			m_programCounter += sizeof(ins.literalConst);
		}
	}
	else if (insLength == sizeof(uint64_t))
	{
		m_programCounter += insLength;
	}
	else
	{
		LOG_ASSERT(false, "error instruction length %d", insLength);
	}
}

}  // namespace pssl