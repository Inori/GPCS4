#include "PsslKey.h"
#include "UtilString.h"

namespace pssl
{;

PsslKey::PsslKey(uint32_t crc, uint32_t hash)
{
	m_key = crc | uint64_t(hash) << 32;
}


PsslKey::PsslKey()
{

}

PsslKey::PsslKey(const PsslKey& key)
{
	m_key = key.m_key;
}

PsslKey::~PsslKey()
{
}

uint64_t PsslKey::toUint64() const
{
	return m_key;
}

std::string PsslKey::toString() const
{
	// TODO:
	// More meaningful name
	return UtilString::Format("%llX", m_key);
}

PsslKey& PsslKey::operator=(const PsslKey& other)
{
	m_key = other.m_key;
	return *this;
}

bool PsslKey::operator==(const PsslKey& other)
{
	return m_key == other.m_key;
}

}  // namespace pssl