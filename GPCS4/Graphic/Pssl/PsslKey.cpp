#include "PsslKey.h"

namespace pssl
{;

PsslKey::PsslKey(uint32_t crc, uint32_t hash)
{
	m_key = crc | uint64_t(hash) << 32;
}


PsslKey::~PsslKey()
{
}

uint64_t PsslKey::getKey() const
{
	return m_key;
}

bool PsslKey::operator==(const PsslKey& other)
{
	return m_key == other.m_key;
}

}  // namespace pssl