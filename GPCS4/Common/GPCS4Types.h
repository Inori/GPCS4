#pragma once

// GPCS4 Type defines

#include <cstdint>

// Note:
// The compiler shipped with PS4 SDK 
// treat "long" as 8 bytes width, 
// but the compiler used to compile GPCS4 (clang-cl) treat it as 4 bytes.
// So we need to take care when we declare PS4 structures and function signature:
// if there's a "long", we should use "ps4_long" instead.

typedef int64_t ps4_long;
typedef uint64_t ps4_ulong;