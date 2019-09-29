#pragma once

#include "GPCS4Common.h"
#include <string>


namespace pssl
{;

class PsslKey
{
public:
	PsslKey();
	PsslKey(const PsslKey& key);
	PsslKey(uint32_t crc, uint32_t hash);
	~PsslKey();

	uint64_t toUint64() const;

	std::string toString() const;

	bool operator == (const PsslKey& other);

	PsslKey& operator = (const PsslKey& other);

private:
	uint64_t m_key;
};


}  // namespace pssl
