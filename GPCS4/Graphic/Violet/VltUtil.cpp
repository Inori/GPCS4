#include "VltUtil.h"
#include "VltFormat.h"

namespace vlt::util
{;
	
VkDeviceSize computeImageDataSize(VkFormat format, VkExtent3D extent)
{
	const VltFormatInfo* formatInfo = imageFormatInfo(format);
	return formatInfo->elementSize * flattenImageExtent(computeBlockCount(extent, formatInfo->blockSize));
}

void packImageData(
	void*        dstBytes,
	const void*  srcBytes,
	VkExtent3D   blockCount,
	VkDeviceSize blockSize,
	VkDeviceSize pitchPerRow,
	VkDeviceSize pitchPerLayer)
{
	auto dstData = reinterpret_cast<char*>(dstBytes);
	auto srcData = reinterpret_cast<const char*>(srcBytes);

	const VkDeviceSize bytesPerRow   = blockCount.width * blockSize;
	const VkDeviceSize bytesPerLayer = blockCount.height * bytesPerRow;
	const VkDeviceSize bytesTotal    = blockCount.depth * bytesPerLayer;

	const bool directCopy = ((bytesPerRow == pitchPerRow) || (blockCount.height == 1)) && 
		                    ((bytesPerLayer == pitchPerLayer) || (blockCount.depth == 1));

	if (directCopy)
	{
		std::memcpy(dstData, srcData, bytesTotal);
	}
	else
	{
		for (uint32_t i = 0; i < blockCount.depth; i++)
		{
			for (uint32_t j = 0; j < blockCount.height; j++)
			{
				std::memcpy(
					dstData + j * bytesPerRow,
					srcData + j * pitchPerRow,
					bytesPerRow);
			}

			srcData += pitchPerLayer;
			dstData += bytesPerLayer;
		}
	}
}

}  // namespace vlt