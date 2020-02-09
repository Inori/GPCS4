#include "GCNDecoder.h"

LOG_CHANNEL(Graphic.Pssl.GCNDecoder);

namespace pssl
{;

const void* GCNCodeSlice::ptrAt(uint32_t id) const
{
	LOG_ASSERT(m_ptr + id < m_end, "End of stream");
	return m_ptr + id;
}

uint32_t GCNCodeSlice::at(uint32_t id) const
{
	LOG_ASSERT(m_ptr + id < m_end, "End of stream");
	return m_ptr[id];
}

uint32_t GCNCodeSlice::frontDword() const
{
	return *m_ptr;
}

uint64_t GCNCodeSlice::frontQword() const
{
	return *(uint64_t*)m_ptr;
}

uint32_t GCNCodeSlice::readDword()
{
	LOG_ASSERT(m_ptr < m_end, "End of stream");
	return *(m_ptr++);
}

uint64_t GCNCodeSlice::readQword()
{
	uint64_t qword = *(uint64_t*)m_ptr;
	m_ptr += 2;
	return qword;
}

GCNCodeSlice GCNCodeSlice::take(uint32_t n) const
{
	LOG_ASSERT(m_ptr + n <= m_end, "End of stream");
	return GCNCodeSlice(m_ptr, m_ptr + n);
}

GCNCodeSlice GCNCodeSlice::skip(uint32_t n) const
{
	LOG_ASSERT(m_ptr + n <= m_end, "End of stream");
	return GCNCodeSlice(m_ptr + n, m_end);
}


//////////////////////////////////////////////////////////////////////////

GCNDecodeContext::GCNDecodeContext():
	m_instruction {nullptr, false, 0}
{
}

GCNDecodeContext::~GCNDecodeContext()
{
	freeInstruction();
}

void GCNDecodeContext::decodeInstruction(GCNCodeSlice& code)
{
	do
	{
		freeInstruction();

		uint32_t encToken = code.frontDword();
		
		ParserSI::InstructionEncoding encoding = ParserSI::GetInstructionEncoding(encToken);
		uint32_t codeLenDw = ParserSI::GetInstructionLengthDwords(encoding);

		bool parseResult = false;
		std::unique_ptr<Instruction> pInstruction;
		bool hasLiteral = false;
		if (codeLenDw == 1)
		{
			uint32_t token = code.readDword();
			parseResult = m_parser.Parse(pInstruction, m_ps4HWGeneration, token, hasLiteral);
		}
		else if (codeLenDw == 2)
		{
			uint64_t token = code.readQword();
			parseResult = m_parser.Parse(pInstruction, m_ps4HWGeneration, token);
		}
		else
		{
			LOG_ERR("error gcn instruction length: %d", codeLenDw);
		}

		if (!parseResult)
		{
			LOG_ERR("parse instruction failed: %X", encToken);
		}

		m_instruction.instruction = std::move(pInstruction);
		m_instruction.hasLiteral = hasLiteral;
		if (hasLiteral)
		{
			m_instruction.literalConst = code.readDword();
		}

	} while (false);
}

GCNInstruction& GCNDecodeContext::getInstruction()
{
	return m_instruction;
}


void GCNDecodeContext::freeInstruction()
{
	if (m_instruction.instruction)
	{
		m_instruction.instruction.reset();
	}
	m_instruction.hasLiteral = false;
	m_instruction.literalConst = 0;
}

} // namespace pssl