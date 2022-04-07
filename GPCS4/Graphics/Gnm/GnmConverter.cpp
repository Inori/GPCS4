#include "GnmConverter.h"

LOG_CHANNEL(Graphic.Gnm.GnmConverter);

namespace sce::Gnm::cvt
{
	
	VkFormat convertZFormat(ZFormat zfmt)
	{
		VkFormat format = VK_FORMAT_UNDEFINED;
		switch (zfmt)
		{
		case kZFormatInvalid:
			format = VK_FORMAT_UNDEFINED;
			break;
		case kZFormat16:
			// seems there's no half float point image format in vulkan.
			LOG_WARN("half float z format detected.");
			format = VK_FORMAT_D16_UNORM;
			break;
		case kZFormat32Float:
			format = VK_FORMAT_D32_SFLOAT;
			break;
		}
		return format;
	}

	VkFormat convertDataFormat(DataFormat dataFormat)
	{

	}

	VkSampleCountFlagBits convertNumFragments(NumFragments numFragments)
	{
		VkSampleCountFlagBits sampleCount = VK_SAMPLE_COUNT_FLAG_BITS_MAX_ENUM;
		switch (numFragments)
		{
		case kNumFragments1:
			sampleCount = VK_SAMPLE_COUNT_1_BIT;
			break;
		case kNumFragments2:
			sampleCount = VK_SAMPLE_COUNT_2_BIT;
			break;
		case kNumFragments4:
			sampleCount = VK_SAMPLE_COUNT_4_BIT;
			break;
		case kNumFragments8:
			sampleCount = VK_SAMPLE_COUNT_8_BIT;
			break;
		}
		return sampleCount;
	}

}  // namespace sce::Gnm::cvt