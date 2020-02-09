#include "PsslSbReader.h"
#include <ostream>

LOG_CHANNEL(Graphic.Pssl.PsslSbReader);

namespace pssl
{;

PsslSbReader::PsslSbReader(const uint8_t* data, size_t size):
	PsslSbReader(data, size, 0)
{
}

PsslSbReader::~PsslSbReader()
{
}

std::string PsslSbReader::readString()
{
	std::string result;

	while (m_data[m_pos] != '\0')
	{
		result.push_back(m_data[m_pos++]);
	}
	m_pos++;

	return result;
}

void PsslSbReader::read(void* dst, size_t n)
{
	LOG_ASSERT((m_pos + n <= m_size), "Unexpected end of file %d", n);
	std::memcpy(dst, m_data + m_pos, n);
	m_pos += n;
}

void PsslSbReader::skip(size_t n)
{
	LOG_ASSERT((m_pos + n <= m_size), "Unexpected end of file %d", n);
	m_pos += n;
}

PsslSbReader PsslSbReader::clone(size_t pos) const
{
	LOG_ASSERT((pos <= m_size), "Invalid offset %d", pos);
	return PsslSbReader(m_data + pos, m_size - pos);
}

PsslSbReader PsslSbReader::resize(size_t size) const
{
	LOG_ASSERT((size <= m_size), "Invalid size");
	return PsslSbReader(m_data, size, m_pos);
}

void PsslSbReader::store(std::ostream&& stream) const
{
	stream.write((const char*)m_data, m_size);
}

}  // pssl