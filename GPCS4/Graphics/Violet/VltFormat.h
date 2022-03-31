#pragma once

#include "VltCommon.h"

#include <array>

namespace sce::vlt
{
	enum class VltFormatFlag
	{
		BlockCompressed = 0,  ///< Image format is block compressed
		SampledUInt     = 1,  ///< Sampled type is an unsigned integer type
		SampledSInt     = 2,  ///< Sampled type is a signed integer type
		ColorSpaceSrgb  = 3,  ///< Non-linear SRGB color format
		MultiPlane      = 4,  ///< Multi-plane format
	};

	using VltFormatFlags = util::Flags<VltFormatFlag>;

	/**
   * \brief Planar format info
   */
	struct VltPlaneFormatInfo
	{
		/// Byte size of a pixel in the current plane
		VkDeviceSize elementSize = 0;
		/// Number of image pixels covered by a
		/// single pixel in the current plane
		VkExtent2D blockSize = { 1, 1 };
	};

	/**
   * \brief Format info structure
   * 
   * Provides some useful information
   * about a Vulkan image format. 
   */
	struct VltFormatInfo
	{
		/// Size of an element in this format. For compressed
		/// formats, this is the size of a block, in bytes.
		VkDeviceSize elementSize = 0;

		/// Available image aspect flags
		VkImageAspectFlags aspectMask = 0;

		/// Some other format info flags
		VltFormatFlags flags = 0;

		/// Size, in pixels, of a compressed block. For
		/// non-block formats, all these values are 1.
		VkExtent3D blockSize = { 1, 1, 1 };

		/// Plane info for multi-planar formats
		std::array<VltPlaneFormatInfo, 3> planes;
	};

	const VltFormatInfo* imageFormatInfo(VkFormat format);
}  // namespace sce::vlt