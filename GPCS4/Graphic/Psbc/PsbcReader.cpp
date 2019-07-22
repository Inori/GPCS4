#include "PsbcReader.h"


PsbcReader::PsbcReader(const uint8_t* data, size_t size):
	PsbcReader(data, size, 0)
{
}

PsbcReader::~PsbcReader()
{
}

std::string PsbcReader::readString()
{
	std::string result;

	while (m_data[m_pos] != '\0')
	{
		result.push_back(m_data[m_pos++]);
	}
	m_pos++;

	return result;
}

void PsbcReader::read(void* dst, size_t n)
{
	LOG_ASSERT((m_pos + n <= m_size), "Unexpected end of file %d", n);
	std::memcpy(dst, m_data + m_pos, n);
	m_pos += n;
}

void PsbcReader::skip(size_t n)
{
	LOG_ASSERT((m_pos + n <= m_size), "Unexpected end of file %d", n);
	m_pos += n;
}

PsbcReader PsbcReader::clone(size_t pos) const
{
	LOG_ASSERT((pos <= m_size), "Invalid offset %d", pos);
	return PsbcReader(m_data + pos, m_size - pos);
}

PsbcReader PsbcReader::resize(size_t size) const
{
	LOG_ASSERT((size <= m_size), "Invalid size");
	return PsbcReader(m_data, size, m_pos);
}

void PsbcReader::store(std::ostream&& stream) const
{
	stream.write((const char*)m_data, m_size);
}
