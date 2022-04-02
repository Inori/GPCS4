#pragma once

#include "VltCommon.h"

namespace sce::vlt
{
	/**
     * \brief Descriptor info
     * 
     * Stores information that is required to
     * update a single resource descriptor.
     */
	union VltDescriptorInfo
	{
		VkDescriptorImageInfo  image;
		VkDescriptorBufferInfo buffer;
		VkBufferView           texelBuffer;
	};
}  // namespace sce::vlt