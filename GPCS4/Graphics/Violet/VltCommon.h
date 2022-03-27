#pragma once

#include "GPCS4Common.h"
#include "UtilString.h"
#include "UtilFlag.h"
#include "VltRc.h"
#include "VltLog.h"

#include <vulkan/vulkan.h>


#ifdef GPCS4_DEBUG
	#define VLT_VALIDATION_AND_DEBUG 1
	#define VLT_DUMP_SHADER          1
	#define VLT_DEBUG_MARKER         1
#endif  // GPCS4_DEBUG

