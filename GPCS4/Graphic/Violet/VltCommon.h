#pragma once

#include "GPCS4Common.h"

#ifdef GPCS4_DEBUG
	#define VLT_VALIDATION_LAYERS_ENABLE
	#define VLT_DUMP_SHADER
	#define VLT_DEBUG_MARKER
#endif  // GPCS4_DEBUG

#include <vulkan/vulkan.h>


constexpr size_t CACHE_LINE_SIZE = 64;


