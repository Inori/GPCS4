#include "GveDescriptor.h"
#include "GveDevice.h"

LOG_CHANNEL(Graphic.Gve.GveDescriptor);

namespace gve
{;


GveDescriptorPool::GveDescriptorPool(GveDevice* device):
	m_device(device)
{
	constexpr uint32_t MaxSets = 16;

	std::array<VkDescriptorPoolSize, 3> pools = 
	{ {
		  { VK_DESCRIPTOR_TYPE_SAMPLER,                MaxSets * 2 },
		  { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,          MaxSets * 3 },
		  { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,         MaxSets * 3 }
	} };

	VkDescriptorPoolCreateInfo info;
	info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	info.pNext = nullptr;
	info.flags = 0;
	info.maxSets = MaxSets;
	info.poolSizeCount = pools.size();
	info.pPoolSizes = pools.data();

	if (vkCreateDescriptorPool(*m_device, &info, nullptr, &m_pool) != VK_SUCCESS)
	{
		LOG_ERR("Failed to create descriptor pool");
	}
}

GveDescriptorPool::~GveDescriptorPool()
{
	vkDestroyDescriptorPool(*m_device, m_pool, nullptr);
}


VkDescriptorSet GveDescriptorPool::alloc(VkDescriptorSetLayout layout)
{
	VkDescriptorSet set = VK_NULL_HANDLE;
	do 
	{
		VkDescriptorSetAllocateInfo info;
		info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		info.pNext = nullptr;
		info.descriptorPool = m_pool;
		info.descriptorSetCount = 1;
		info.pSetLayouts = &layout;
		VkResult result = vkAllocateDescriptorSets(*m_device, &info, &set);
		if (result != VK_SUCCESS)
		{
			LOG_ERR("allocate descriptor set failed, error %d", result);
			break;
		}

	} while (false);

	return set;
}

void GveDescriptorPool::reset()
{
	vkResetDescriptorPool(*m_device, m_pool, 0);
}

///

GveDescriptorPoolTracker::GveDescriptorPoolTracker(GveDevice* device):
	m_device(device)
{
}

GveDescriptorPoolTracker::~GveDescriptorPoolTracker()
{
}


void GveDescriptorPoolTracker::trackDescriptorPool(RcPtr<GveDescriptorPool>&& pool)
{
	m_pools.push_back(std::move(pool));
}

void GveDescriptorPoolTracker::reset()
{
	for (const auto& pool : m_pools)
	{
		pool->reset();
		m_device->recycleDescriptorPool(pool);
	}

	m_pools.clear();
}

}  // namespace gve