#include "VltDescriptor.h"

#include "VltDevice.h"

#include <array>

namespace sce::vlt
{
	VltDescriptorPool::VltDescriptorPool(VltDevice* device) :
		m_device(device)
	{
		constexpr uint32_t MaxSets = 2048;

		std::array<VkDescriptorPoolSize, 9> pools = { { { VK_DESCRIPTOR_TYPE_SAMPLER, MaxSets * 2 },
														{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, MaxSets * 3 },
														{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, MaxSets / 8 },
														{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, MaxSets * 3 },
														{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, MaxSets / 8 },
														{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, MaxSets * 3 },
														{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, MaxSets / 8 },
														{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, MaxSets * 3 },
														{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, MaxSets * 2 } } };

		VkDescriptorPoolCreateInfo info;
		info.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		info.pNext         = nullptr;
		info.flags         = 0;
		info.maxSets       = MaxSets;
		info.poolSizeCount = pools.size();
		info.pPoolSizes    = pools.data();

		if (vkCreateDescriptorPool(m_device->handle(), &info, nullptr, &m_pool) != VK_SUCCESS)
			Logger::exception("DxvkDescriptorPool: Failed to create descriptor pool");
	}

	VltDescriptorPool::~VltDescriptorPool()
	{
		vkDestroyDescriptorPool(
			m_device->handle(), m_pool, nullptr);
	}

	VkDescriptorSet VltDescriptorPool::alloc(VkDescriptorSetLayout layout)
	{
		VkDescriptorSetAllocateInfo info;
		info.sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		info.pNext              = nullptr;
		info.descriptorPool     = m_pool;
		info.descriptorSetCount = 1;
		info.pSetLayouts        = &layout;

		VkDescriptorSet set = VK_NULL_HANDLE;
		if (vkAllocateDescriptorSets(m_device->handle(), &info, &set) != VK_SUCCESS)
			return VK_NULL_HANDLE;
		return set;
	}

	void VltDescriptorPool::reset()
	{
		vkResetDescriptorPool(
			m_device->handle(), m_pool, 0);
	}

	VltDescriptorPoolTracker::VltDescriptorPoolTracker(VltDevice* device) :
		m_device(device)
	{
	}

	VltDescriptorPoolTracker::~VltDescriptorPoolTracker()
	{
	}

	void VltDescriptorPoolTracker::trackDescriptorPool(Rc<VltDescriptorPool> pool)
	{
		m_pools.push_back(std::move(pool));
	}

	void VltDescriptorPoolTracker::reset()
	{
		for (const auto& pool : m_pools)
		{
			pool->reset();
			m_device->recycleDescriptorPool(pool);
		}

		m_pools.clear();
	}

}  // namespace sce::vlt