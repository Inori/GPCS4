#pragma once

#include "VltCommon.h"

#include <algorithm>

namespace vlt::util
{;
	
/**
 * \brief Computes mip level extent
 *
 * \param [in] size Base mip level extent
 * \param [in] level mip level to compute
 * \returns Extent of the given mip level
 */
inline VkExtent3D computeMipLevelExtent(VkExtent3D size, uint32_t level) 
{
	size.width = std::max(1u, size.width >> level);
	size.height = std::max(1u, size.height >> level);
	size.depth = std::max(1u, size.depth >> level);
	return size;
}

/**
 * \brief Compute block count for compressed images
 * 
 * Convenience function to compute the size, in
 * blocks, of compressed images subresources.
 * \param [in] extent The image size
 * \param [in] blockSize Size of a pixel block
 * \returns Number of blocks in the image
 */
inline VkExtent3D computeBlockCount(VkExtent3D extent, VkExtent3D blockSize)
{
	return VkExtent3D{
		(extent.width + blockSize.width - 1) / blockSize.width,
		(extent.height + blockSize.height - 1) / blockSize.height,
		(extent.depth + blockSize.depth - 1) / blockSize.depth
	};
}

inline uint32_t getComponentIndex(
	VkComponentSwizzle component,
	uint32_t           identity)
{
	switch (component)
	{
	case VK_COMPONENT_SWIZZLE_R:
		return 0;
	case VK_COMPONENT_SWIZZLE_G:
		return 1;
	case VK_COMPONENT_SWIZZLE_B:
		return 2;
	case VK_COMPONENT_SWIZZLE_A:
		return 3;
	default:
		return identity; /* identity, zero, one */
	}
}

/**
 * \brief Computes number of pixels or blocks of an image
 * 
 * Basically returns the product of width, height and depth.
 * \param [in] extent Image extent, in pixels or blocks
 * \returns Flattened number of pixels or blocks
 */
inline uint32_t flattenImageExtent(VkExtent3D extent)
{
	return extent.width * extent.height * extent.depth;
}

/**
 * \brief Computes image data size, in bytes
 * 
 * Convenience method that can be used to compute the number
 * of bytes required to store image data in a given format.
 * \param [in] format The image format
 * \param [in] extent Image size, in pixels
 * \returns Data size, in bytes
 */
VkDeviceSize computeImageDataSize(VkFormat format, VkExtent3D extent);

/**
 * \brief Writes tightly packed image data to a buffer
 * 
 * \param [in] dstBytes Destination buffer pointer
 * \param [in] srcBytes Pointer to source data
 * \param [in] blockCount Number of blocks to copy
 * \param [in] blockSize Number of bytes per block
 * \param [in] pitchPerRow Number of bytes between rows
 * \param [in] pitchPerLayer Number of bytes between layers
 */
void packImageData(
	void*        dstBytes,
	const void*  srcBytes,
	VkExtent3D   blockCount,
	VkDeviceSize blockSize,
	VkDeviceSize pitchPerRow,
	VkDeviceSize pitchPerLayer);

}  // namespace vlt