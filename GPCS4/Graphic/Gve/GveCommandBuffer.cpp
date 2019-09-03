#include "GveCommandBuffer.h"

namespace gve
{;


GveCommandBuffer::GveCommandBuffer(const RcPtr<GveDevice>& device):
	m_device(device)
{
	bool success = initCommandBuffer();
	LOG_ASSERT(success, "init command buffer failed.");
}

GveCommandBuffer::~GveCommandBuffer()
{
	vkDestroyCommandPool(*m_device, m_pool, nullptr);
}

bool GveCommandBuffer::initCommandBuffer()
{
	bool ret = false;
	do 
	{
		auto phyDevice = m_device->getPhysicalDevice();
		GvePhysicalDeviceQueueFamilies families = phyDevice->findQueueFamilies();

		VkCommandPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.queueFamilyIndex = families.graphicsFamily;
		if (vkCreateCommandPool(*m_device, &poolInfo, nullptr, &m_pool) != VK_SUCCESS) 
		{
			LOG_ERR("failed to create graphics command pool!");
			break;
		}

		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = m_pool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = 1;

		if (vkAllocateCommandBuffers(*m_device, &allocInfo, &m_cmd) != VK_SUCCESS) 
		{
			LOG_ERR("failed to allocate command buffers!");
			break;
		}

		ret = true;
	} while (false);
	return ret;
}

}  // namespace gve