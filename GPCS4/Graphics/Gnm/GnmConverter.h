#pragma once

#include "GnmCommon.h"
#include "GnmDataFormat.h"
#include "SceVideoOut/sce_videoout_types.h"

namespace sce::Gnm::cvt
{
	VkFormat convertZFormat(ZFormat zfmt);

	VkSampleCountFlagBits 
		convertNumFragments(NumFragments numFragments);

	VkFormat convertDataFormat(DataFormat dataFormat);

	VkFormat convertVideoOutPixelFormat(SceVideoOutPixelFormat format);

	DataFormat convertDataFormatFromVideoOutPixelFormat(SceVideoOutPixelFormat format);

}  // namespace sce::Gnm