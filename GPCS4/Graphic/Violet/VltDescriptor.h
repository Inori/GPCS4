#pragma once

#include "VltCommon.h"

namespace vlt
{;

class VltDevice;

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
	VkBufferView texelBuffer;
};


class VltDescriptorPool : public RcObject
{
public:
	VltDescriptorPool(VltDevice* device);
	~VltDescriptorPool();

	VkDescriptorSet alloc(VkDescriptorSetLayout layout);

	void reset();

private:
	VltDevice*       m_device;
	VkDescriptorPool m_pool = VK_NULL_HANDLE;
};


  /**
   * \brief Descriptor pool tracker
   * 
   * Tracks descriptor pools that are either full
   * or no longer needed by the Violet context. The
   * command list will reset and recycle all pools
   * once it has completed execution on the GPU.
   */
class VltDescriptorPoolTracker
{

public:
	VltDescriptorPoolTracker(VltDevice* device);
	~VltDescriptorPoolTracker();

	/**
     * \brief Adds a descriptor pool to track
     * \param [in] pool The descriptor pool
     */
	void trackDescriptorPool(RcPtr<VltDescriptorPool>&& pool);

	/**
     * \brief Resets event tracker
     * 
     * Resets all tracked descriptor pools
     * and returns them to the device.
     */
	void reset();

private:
	VltDevice* m_device;

	std::vector<RcPtr<VltDescriptorPool>> m_pools;
};

}  // namespace vlt