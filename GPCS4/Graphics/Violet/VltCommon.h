#pragma once

#include "GPCS4Common.h"
#include "UtilString.h"
#include "UtilFlag.h"
#include "VltRc.h"
#include "VltLog.h"

#include <vulkan/vulkan.h>

constexpr size_t CACHE_LINE_SIZE = 64;


#ifdef GPCS4_DEBUG
	#define VLT_VALIDATION_AND_DEBUG 1
	#define VLT_DUMP_SHADER          1
	#define VLT_DEBUG_LABEL          1
#endif  // GPCS4_DEBUG

