#pragma once

#include "GPCS4Common.h"

#ifdef GPCS4_DEBUG
	#define VLT_VALIDATION_LAYERS_ENABLE 1
	#define VLT_DUMP_SHADER 1
	#define VLT_DEBUG_MARKER 1
#endif  // GPCS4_DEBUG

#include <vulkan/vulkan.h>


constexpr size_t CACHE_LINE_SIZE = 64;


