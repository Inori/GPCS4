#pragma once

#include "GPCS4Common.h"
#include <vulkan/vulkan.h>

#ifdef GPCS4_DEBUG

// Dump shader to file
#define PSSL_DUMP_SHADER

// Enable some debug features during shader compile
#define PSSL_COMPILER_DEBUG

// Enable spirv-cross analyze.
// Make the compiler output supported by spirv-cross.
// Note:
// This should be disabled when your are really running a game,
// or the output shader binary can not be used.
#define PSSL_SPIRV_CROSS_DEBUG

#endif