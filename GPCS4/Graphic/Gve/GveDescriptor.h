#pragma once

#include "GveCommon.h"

namespace gve
{;

class GveDevice;

/**
 * \brief Descriptor info
 *
 * Stores information that is required to
 * update a single resource descriptor.
 */
union GveDescriptorInfo 
{
	VkDescriptorImageInfo  image;
	VkDescriptorBufferInfo buffer;
};


class GveDescriptorPool : public RcObject
{
public:
	GveDescriptorPool(GveDevice* device);
	~GveDescriptorPool();

	VkDescriptorSet alloc(VkDescriptorSetLayout layout);

	void reset();

private:

	GveDevice* m_device;
	VkDescriptorPool m_pool = VK_NULL_HANDLE;
};

}  // namespace gve