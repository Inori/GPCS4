#pragma once

#include "GPCS4Common.h"


namespace pssl
{;

class PsslKey
{
public:
	PsslKey();
	PsslKey(const PsslKey& key);
	PsslKey(uint32_t crc, uint32_t hash);
	~PsslKey();

	uint64_t getKey() const;

	bool operator == (const PsslKey& other);

	PsslKey& operator = (const PsslKey& other);

private:
	uint64_t m_key;
};


}  // namespace pssl
