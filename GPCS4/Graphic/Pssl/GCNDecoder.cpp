#include "GCNDecoder.h"


namespace pssl
{;

const uint32_t* GCNCodeSlice::ptrAt(uint32_t id) const
{
	LOG_ASSERT(m_ptr + id < m_end, "End of stream");
	return m_ptr + id;
}

uint32_t GCNCodeSlice::at(uint32_t id) const
{
	LOG_ASSERT(m_ptr + id < m_end, "End of stream");
	return m_ptr[id];
}

uint32_t GCNCodeSlice::read()
{
	LOG_ASSERT(m_ptr < m_end, "End of stream");
	return *(m_ptr++);
}

GCNCodeSlice GCNCodeSlice::take(uint32_t n) const
{
	LOG_ASSERT(m_ptr + n <= m_end, "End of stream");
	return GCNCodeSlice(m_ptr, m_ptr + n);
}

pssl::GCNCodeSlice GCNCodeSlice::skip(uint32_t n) const
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

void GCNDecodeContext::decodeInstruction(GCNCodeSlice slice)
{
	
}

const PsslShaderInstruction& GCNDecodeContext::getInstruction()
{
	return m_instruction;
}

PsslInstructionEncoding GCNDecodeContext::getInstructionEncoding(uint32_t token)
{
	
}

} // namespace pssl