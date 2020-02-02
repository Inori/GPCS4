#pragma once

#include "GPCS4Common.h"

namespace algo
{;

uint64_t MurmurHash(const void* key, int len);

uint64_t MurmurHash64A(const void* key, int len, uint64_t seed);

}  // namespace algo