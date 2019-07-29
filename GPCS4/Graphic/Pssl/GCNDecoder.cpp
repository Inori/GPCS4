#include "GCNDecoder.h"


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

GCNDecodeContext::GCNDecodeContext()
{
}

GCNDecodeContext::~GCNDecodeContext()
{
}

void GCNDecodeContext::decodeInstruction(GCNCodeSlice& code)
{
	do
	{
		uint32_t token = code.frontDword();
		
		ParserSI::InstructionEncoding encoding = ParserSI::GetInstructionEncoding(token);
		
		uint32_t codeLenDw = ParserSI::GetInstructionLengthDwords(encoding);

		bool parseResult = false;
		Instruction* pInstruction = nullptr;
		if (codeLenDw == 1)
		{
			uint32_t tokenDword = code.readDword();
			parseResult = m_parser.Parse32(pInstruction, GDT_HW_GENERATION_SEAISLAND, tokenDword);
		}
		else if (codeLenDw == 2)
		{
			uint64_t tokenQword = code.readQword();
			parseResult = m_parser.Parse64(pInstruction, GDT_HW_GENERATION_SEAISLAND, tokenQword);
		}
		else
		{
			LOG_ERR("error gcn instruction length: %d", codeLenDw);
		}

		if (!parseResult)
		{
			LOG_ERR("parse instruction failed: %X", token);
		}

	} while (false);
}

const GCNInstruction& GCNDecodeContext::getInstruction()
{
	return m_instruction;
}


void GCNDecodeContext::freeInstruction()
{
	if (m_instruction.instruction)
	{
		delete m_instruction.instruction;
		m_instruction.instruction = nullptr;
	}
	m_instruction.hasLiteral = false;
	m_instruction.literalConst = 0;
}

} // namespace pssl