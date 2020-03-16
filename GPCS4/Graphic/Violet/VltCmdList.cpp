#include "VltCmdList.h"

#include "VltPhysicalDevice.h"

LOG_CHANNEL(Graphic.Violet.VltCmdList);

namespace vlt
{;

VltCmdList::VltCmdList(
	VltDevice*      device,
	VltPipelineType type) :
	m_device(device),
	m_pipelineType(type),
	m_descriptorPoolTracker(device)
{
	bool success = createCommandBuffer();
	LOG_ASSERT(success, "init command buffer failed.");
}

VltCmdList::~VltCmdList()
{
	destroyCommandBuffers();
}

bool VltCmdList::createCommandBuffer()
{
	bool ret = false;
	do
	{
		auto                           phyDevice = m_device->physicalDevice();
		VltPhysicalDeviceQueueFamilies families  = phyDevice->findQueueFamilies();

		VkFenceCreateInfo fenceInfo;
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.pNext = nullptr;
		fenceInfo.flags = 0;
		if (vkCreateFence(*m_device, &fenceInfo, nullptr, &m_fence) != VK_SUCCESS)
		{
			LOG_ERR("Failed to create fence");
			break;
		}
		
		// m_execPool can be allocated from either graphics queue or compute queue
		uint32_t execQueueFamily = m_pipelineType == VltPipelineType::Graphics ? 
			families.graphics : families.compute;

		VkCommandPoolCreateInfo poolInfo = {};
		poolInfo.sType                   = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.queueFamilyIndex        = execQueueFamily;
		if (vkCreateCommandPool(*m_device, &poolInfo, nullptr, &m_execPool) != VK_SUCCESS)
		{
			LOG_ERR("create command pool failed.");
			break;
		}

		if (m_device->hasDedicatedTransferQueue())
		{
			VkCommandPoolCreateInfo poolInfo = {};
			poolInfo.sType                   = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
			poolInfo.queueFamilyIndex        = families.transfer;
			if (vkCreateCommandPool(*m_device, &poolInfo, nullptr, &m_transferPool) != VK_SUCCESS)
			{
				LOG_ERR("create command pool failed.");
				break;
			}
		}

		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType                       = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool                 = m_execPool;
		allocInfo.level                       = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount          = 1;

		VkCommandBufferAllocateInfo allocInfoDma;
		allocInfoDma.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfoDma.pNext              = nullptr;
		allocInfoDma.commandPool        = m_transferPool ? m_transferPool : m_execPool;
		allocInfoDma.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfoDma.commandBufferCount = 1;

		if (vkAllocateCommandBuffers(*m_device, &allocInfo, &m_execBuffer) != VK_SUCCESS ||
			vkAllocateCommandBuffers(*m_device, &allocInfo, &m_initBuffer) != VK_SUCCESS ||
			vkAllocateCommandBuffers(*m_device, &allocInfoDma, &m_sdmaBuffer) != VK_SUCCESS)
		{
			LOG_ERR("failed to allocate command buffers!");
			break;
		}

		if (m_device->hasDedicatedTransferQueue())
		{
			VkSemaphoreCreateInfo semInfo;
			semInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
			semInfo.pNext = nullptr;
			semInfo.flags = 0;
			if (vkCreateSemaphore(*m_device, &semInfo, nullptr, &m_sdmaSemaphore) != VK_SUCCESS)
			{
				break;
			}
		}

		ret  = true;
	}while(false);
	return ret;
}

void VltCmdList::destroyCommandBuffers()
{
	reset();

	if (m_execPool)
	{
		vkDestroyCommandPool(*m_device, m_execPool, nullptr);
		m_execPool = VK_NULL_HANDLE;
	}

	if (m_transferPool)
	{
		vkDestroyCommandPool(*m_device, m_transferPool, nullptr);
		m_transferPool = VK_NULL_HANDLE;
	}

	vkDestroySemaphore(*m_device, m_sdmaSemaphore, nullptr);
	m_sdmaSemaphore = VK_NULL_HANDLE;
	vkDestroyFence(*m_device, m_fence, nullptr);
	m_fence = VK_NULL_HANDLE;
}


void VltCmdList::beginRecording()
{
	do
	{
		if ((m_execPool != VK_NULL_HANDLE && vkResetCommandPool(*m_device, m_execPool, 0) != VK_SUCCESS) ||
			(m_transferPool != VK_NULL_HANDLE && vkResetCommandPool(*m_device, m_transferPool, 0) != VK_SUCCESS))
		{
			LOG_ERR("reset command pool failed.");
			break;
		}

		VkCommandBufferBeginInfo info;
		info.sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		info.pNext            = nullptr;
		info.flags            = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		info.pInheritanceInfo = nullptr;

		if (vkBeginCommandBuffer(m_execBuffer, &info) != VK_SUCCESS ||
			vkBeginCommandBuffer(m_initBuffer, &info) != VK_SUCCESS ||
			vkBeginCommandBuffer(m_sdmaBuffer, &info) != VK_SUCCESS)
		{
			LOG_ERR("begin command buffer failed.");
			break;
		}

		if (vkResetFences(*m_device, 1, &m_fence) != VK_SUCCESS)
		{
			break;
		}

		// Unconditionally mark the exec buffer as used. There
		// is virtually no use case where this isn't correct.
		m_cmdTypeUsed = VltCmdType::ExecBuffer;
	} while (false);
}


void VltCmdList::endRecording()
{
	vkEndCommandBuffer(m_execBuffer);
	vkEndCommandBuffer(m_initBuffer);
	vkEndCommandBuffer(m_sdmaBuffer);
}


VkResult VltCmdList::submit(VkSemaphore wait, VkSemaphore wake)
{
	VltQueueSubmission submission = {};

	if (m_cmdTypeUsed.test(VltCmdType::ExecBuffer))
	{
		submission.cmdBuffers[submission.cmdBufferCount++] = m_execBuffer;
	}

	if (wait != VK_NULL_HANDLE)
	{
		submission.waitSync[submission.waitCount] = wait;
		submission.waitMask[submission.waitCount] = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
		++submission.waitCount;
	}

	if (wake != VK_NULL_HANDLE)
	{
		submission.wakeSync[submission.wakeCount++] = wake;
	}

	VltDeviceQueueSet queues      = m_device->queues();
	VkQueue           submitQueue = m_pipelineType == VltPipelineType::Graphics ?
		queues.graphics.queueHandle : 
		queues.compute.queueHandle;

	return submitToQueue(submitQueue, m_fence, submission);
}

VkResult VltCmdList::synchronize()
{
	return vkWaitForFences(*m_device, 1, &m_fence, VK_FALSE, UINT64_MAX);
}

void VltCmdList::reset()
{
	m_descriptorPoolTracker.reset();
	m_resourceTracker.reset();
}

VkResult VltCmdList::submitToQueue(
	VkQueue                   queue,
	VkFence                   fence,
	const VltQueueSubmission& info)
{
	VkSubmitInfo submitInfo;
	submitInfo.sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.pNext                = nullptr;
	submitInfo.waitSemaphoreCount   = info.waitCount;
	submitInfo.pWaitSemaphores      = info.waitSync;
	submitInfo.pWaitDstStageMask    = info.waitMask;
	submitInfo.commandBufferCount   = info.cmdBufferCount;
	submitInfo.pCommandBuffers      = info.cmdBuffers;
	submitInfo.signalSemaphoreCount = info.wakeCount;
	submitInfo.pSignalSemaphores    = info.wakeSync;

	return vkQueueSubmit(queue, 1, &submitInfo, fence);
}

VkCommandBuffer VltCmdList::selectCmdBuffer(VltCmdType cmdType) const
{
	VkCommandBuffer cmdBuffer = VK_NULL_HANDLE;
	if (cmdType == VltCmdType::ExecBuffer)
	{
		cmdBuffer = m_execBuffer;
	}
	else if (cmdType == VltCmdType::InitBuffer)
	{
		cmdBuffer = m_initBuffer;
	}
	else if (cmdType == VltCmdType::SdmaBuffer)
	{
		cmdBuffer = m_sdmaBuffer;
	}
	return cmdBuffer;
}

}  // namespace vlt