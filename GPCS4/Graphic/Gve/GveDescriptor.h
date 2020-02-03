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
	VkBufferView texelBuffer;
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


  /**
   * \brief Descriptor pool tracker
   * 
   * Tracks descriptor pools that are either full
   * or no longer needed by the Gve context. The
   * command list will reset and recycle all pools
   * once it has completed execution on the GPU.
   */
class GveDescriptorPoolTracker
{

public:
	GveDescriptorPoolTracker(GveDevice* device);
	~GveDescriptorPoolTracker();

	/**
     * \brief Adds a descriptor pool to track
     * \param [in] pool The descriptor pool
     */
	void trackDescriptorPool(RcPtr<GveDescriptorPool>&& pool);

	/**
     * \brief Resets event tracker
     * 
     * Resets all tracked descriptor pools
     * and returns them to the device.
     */
	void reset();

private:
	GveDevice* m_device;

	std::vector<RcPtr<GveDescriptorPool>> m_pools;
};

}  // namespace gve