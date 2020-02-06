#pragma once

#include "GPCS4Common.h"
#include <vulkan/vulkan.h>
#include "../SpirV/spirv.hpp"

#ifdef GPCS4_DEBUG

// Dump shader to file
#define PSSL_DUMP_SHADER

// Enable some debug features during shader compile
#define PSSL_COMPILER_DEBUG

#endif