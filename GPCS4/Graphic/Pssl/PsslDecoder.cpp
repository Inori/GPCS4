#include "PsslDecoder.h"


namespace pssl
{;

const uint32_t* PsslCodeSlice::ptrAt(uint32_t id) const
{
	LOG_ASSERT(m_ptr + id < m_end, "End of stream");
	return m_ptr + id;
}

uint32_t PsslCodeSlice::at(uint32_t id) const
{
	LOG_ASSERT(m_ptr + id < m_end, "End of stream");
	return m_ptr[id];
}

uint32_t PsslCodeSlice::read()
{
	LOG_ASSERT(m_ptr < m_end, "End of stream");
	return *(m_ptr++);
}

PsslCodeSlice PsslCodeSlice::take(uint32_t n) const
{
	LOG_ASSERT(m_ptr + n <= m_end, "End of stream");
	return PsslCodeSlice(m_ptr, m_ptr + n);
}

pssl::PsslCodeSlice PsslCodeSlice::skip(uint32_t n) const
{
	LOG_ASSERT(m_ptr + n <= m_end, "End of stream");
	return PsslCodeSlice(m_ptr + n, m_end);
}


//////////////////////////////////////////////////////////////////////////

PsslDecoder::PsslDecoder()
{
}

PsslDecoder::~PsslDecoder()
{
}

void PsslDecoder::decodeInstruction(PsslCodeSlice slice)
{
	
}

const PsslShaderInstruction& PsslDecoder::getInstruction()
{

}

} // namespace pssl