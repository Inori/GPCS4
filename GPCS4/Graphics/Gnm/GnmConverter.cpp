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

	VkFormat convertVideoOutPixelFormat(SceVideoOutPixelFormat format)
	{
		VkFormat result = VK_FORMAT_UNDEFINED;
		switch (format)
		{
		case SCE_VIDEO_OUT_PIXEL_FORMAT_B8_G8_R8_A8_SRGB:
			result = VK_FORMAT_B8G8R8A8_SRGB;
			break;
		case SCE_VIDEO_OUT_PIXEL_FORMAT_B16_G16_R16_A16_FLOAT:
			result = VK_FORMAT_R16G16B16A16_SFLOAT;
			break;
		case SCE_VIDEO_OUT_PIXEL_FORMAT_R8_G8_B8_A8_SRGB:
			result = VK_FORMAT_A8B8G8R8_SRGB_PACK32;
			break;
		case SCE_VIDEO_OUT_PIXEL_FORMAT_B10_G10_R10_A2:
			result = VK_FORMAT_A2R10G10B10_UINT_PACK32;
			break;
		case SCE_VIDEO_OUT_PIXEL_FORMAT_B10_G10_R10_A2_SRGB:
		case SCE_VIDEO_OUT_PIXEL_FORMAT_B10_G10_R10_A2_BT2020_PQ:
			LOG_ASSERT(false, "format not supported.");
			break;
		}
		return result;
	}

	DataFormat convertDataFormatFromVideoOutPixelFormat(SceVideoOutPixelFormat format)
	{
		DataFormat result = {};
		switch (format)
		{
		case SCE_VIDEO_OUT_PIXEL_FORMAT_B8_G8_R8_A8_SRGB:
			result = kDataFormatB8G8R8A8UnormSrgb;
			break;
		case SCE_VIDEO_OUT_PIXEL_FORMAT_B16_G16_R16_A16_FLOAT:
		case SCE_VIDEO_OUT_PIXEL_FORMAT_R8_G8_B8_A8_SRGB:
		case SCE_VIDEO_OUT_PIXEL_FORMAT_B10_G10_R10_A2:
		case SCE_VIDEO_OUT_PIXEL_FORMAT_B10_G10_R10_A2_SRGB:
		case SCE_VIDEO_OUT_PIXEL_FORMAT_B10_G10_R10_A2_BT2020_PQ:
			LOG_ASSERT(false, "format not supported.");
			break;
		}
		return result;
	}

}  // namespace sce::Gnm::cvt