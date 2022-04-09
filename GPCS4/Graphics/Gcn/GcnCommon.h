#pragma once

#include "GPCS4Common.h"
#include <vulkan/vulkan.h>
#include "SpirV/spirv.hpp"

#ifdef GPCS4_DEBUG

// Dump shader to file
#define GCN_DUMP_SHADER

// Enable some debug features during shader compile
#define GCN_COMPILER_DEBUG

#endif