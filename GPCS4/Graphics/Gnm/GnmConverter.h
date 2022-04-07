#pragma once

#include "GnmCommon.h"
#include "GnmDataFormat.h"

namespace sce::Gnm::cvt
{

	VkFormat convertZFormat(ZFormat zfmt);

	VkSampleCountFlagBits 
		convertNumFragments(NumFragments numFragments);

	VkFormat convertDataFormat(DataFormat dataFormat);

}  // namespace sce::Gnm