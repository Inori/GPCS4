#pragma once

#include "GveCommon.h"
#include "UtilFlag.h"

namespace gve
{;

enum class GveFormatFlag
{
	BlockCompressed = 0,  ///< Image format is block compressed
	SampledUInt     = 1,  ///< Sampled type is an unsigned integer type
	SampledSInt     = 2,  ///< Sampled type is a signed integer type
	ColorSpaceSrgb  = 3,  ///< Non-linear SRGB color format
};

using GveFormatFlags = Flags<GveFormatFlag>;

/**
   * \brief Format info structure
   * 
   * Provides some useful information
   * about a Vulkan image format. 
   */
struct GveFormatInfo
{
	/// Size of an element in this format. For compressed
	/// formats, this is the size of a block, in bytes.
	VkDeviceSize elementSize = 0;

	/// Available image aspect flags
	VkImageAspectFlags aspectMask = 0;

	/// Some other format info flags
	GveFormatFlags flags = 0;

	/// Size, in pixels, of a compressed block. For
	/// non-block formats, all these values are 1.
	VkExtent3D blockSize = { 1, 1, 1 };
};

const GveFormatInfo* imageFormatInfo(VkFormat format);
	
}  // namespace gve