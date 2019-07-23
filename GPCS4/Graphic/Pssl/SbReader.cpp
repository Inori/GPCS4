#include "SbReader.h"


SbReader::SbReader(const uint8_t* data, size_t size):
	SbReader(data, size, 0)
{
}

SbReader::~SbReader()
{
}

std::string SbReader::readString()
{
	std::string result;

	while (m_data[m_pos] != '\0')
	{
		result.push_back(m_data[m_pos++]);
	}
	m_pos++;

	return result;
}

void SbReader::read(void* dst, size_t n)
{
	LOG_ASSERT((m_pos + n <= m_size), "Unexpected end of file %d", n);
	std::memcpy(dst, m_data + m_pos, n);
	m_pos += n;
}

void SbReader::skip(size_t n)
{
	LOG_ASSERT((m_pos + n <= m_size), "Unexpected end of file %d", n);
	m_pos += n;
}

SbReader SbReader::clone(size_t pos) const
{
	LOG_ASSERT((pos <= m_size), "Invalid offset %d", pos);
	return SbReader(m_data + pos, m_size - pos);
}

SbReader SbReader::resize(size_t size) const
{
	LOG_ASSERT((size <= m_size), "Invalid size");
	return SbReader(m_data, size, m_pos);
}

void SbReader::store(std::ostream&& stream) const
{
	stream.write((const char*)m_data, m_size);
}
