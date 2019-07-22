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
	do 
	{
		if (m_pos + n > m_size)
		{
			LOG_ERR("Unexpected end of file %d", n);
			break;
		}

		std::memcpy(dst, m_data + m_pos, n);
		m_pos += n;
	} while (false);
}

void PsbcReader::skip(size_t n)
{
	do 
	{
		if (m_pos + n > m_size)
		{
			LOG_ERR("Unexpected end of file %d", n);
			break;
		}
		m_pos += n;
	} while (false);
}

PsbcReader PsbcReader::clone(size_t pos) const
{

}

PsbcReader PsbcReader::resize(size_t size) const
{
	LOG_ASSERT((size <= m_size), "Invalid size");
	return PsbcReader(m_data, size, m_pos);
}

void PsbcReader::store(std::ostream&& stream) const
{

}
