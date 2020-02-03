#include "GveCmdList.h"
#include "GvePhysicalDevice.h"

LOG_CHANNEL(Graphic.Gve.GveCmdList);

namespace gve
{;


GveCmdList::GveCmdList(GveDevice* device):
	m_device(device),
	m_descriptorPoolTracker(device)
{
	bool success = initCommandBuffer();
	LOG_ASSERT(success, "init command buffer failed.");
}

GveCmdList::~GveCmdList()
{
	vkDestroyCommandPool(*m_device, m_pool, nullptr);
}

VkCommandBuffer GveCmdList::execBufferHandle() const
{
	return m_execBuffer;
}

bool GveCmdList::initCommandBuffer()
{
	bool ret = false;
	do
	{
		auto phyDevice = m_device->physicalDevice();
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

		if (vkAllocateCommandBuffers(*m_device, &allocInfo, &m_execBuffer) != VK_SUCCESS)
		{
			LOG_ERR("failed to allocate command buffers!");
			break;
		}

		ret = true;
	} while (false);
	return ret;
}

void GveCmdList::beginRecording()
{
	do 
	{
		if (vkResetCommandPool(*m_device, m_pool, 0) != VK_SUCCESS)
		{
			LOG_ERR("reset command pool failed.");
			break;
		}

		VkCommandBufferBeginInfo info;
		info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		info.pNext = nullptr;
		info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		info.pInheritanceInfo = nullptr;
		if (vkBeginCommandBuffer(m_execBuffer, &info) != VK_SUCCESS)
		{
			LOG_ERR("begin command buffer failed.");
			break;
		}

	} while (false);
}

void GveCmdList::endRecording()
{
	vkEndCommandBuffer(m_execBuffer);
}

void GveCmdList::reset()
{
	m_descriptorPoolTracker.reset();
}

}  // namespace gve