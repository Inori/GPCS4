#include "VltCmdList.h"
#include "VltDevice.h"

namespace sce::vlt
{
	VltCommandList::VltCommandList(VltDevice* device) :
		m_device(device),
		m_cmdBuffersUsed(0),
		m_debug(device)
	{
		const auto& graphicsQueue = m_device->queues().graphics;
		const auto& transferQueue = m_device->queues().transfer;

		VkFenceCreateInfo fenceInfo;
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.pNext = nullptr;
		fenceInfo.flags = 0;

		if (vkCreateFence(m_device->handle(), &fenceInfo, nullptr, &m_fence) != VK_SUCCESS)
			Logger::exception("DxvkCommandList: Failed to create fence");

		VkCommandPoolCreateInfo poolInfo;
		poolInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.pNext            = nullptr;
		poolInfo.flags            = 0;
		poolInfo.queueFamilyIndex = graphicsQueue.queueFamily;

		if (vkCreateCommandPool(m_device->handle(), &poolInfo, nullptr, &m_graphicsPool) != VK_SUCCESS)
			Logger::exception("DxvkCommandList: Failed to create graphics command pool");

		if (m_device->hasDedicatedTransferQueue())
		{
			poolInfo.queueFamilyIndex = transferQueue.queueFamily;

			if (vkCreateCommandPool(m_device->handle(), &poolInfo, nullptr, &m_transferPool) != VK_SUCCESS)
				Logger::exception("DxvkCommandList: Failed to create transfer command pool");
		}

		VkCommandBufferAllocateInfo cmdInfoGfx;
		cmdInfoGfx.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		cmdInfoGfx.pNext              = nullptr;
		cmdInfoGfx.commandPool        = m_graphicsPool;
		cmdInfoGfx.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		cmdInfoGfx.commandBufferCount = 1;

		VkCommandBufferAllocateInfo cmdInfoDma;
		cmdInfoDma.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		cmdInfoDma.pNext              = nullptr;
		cmdInfoDma.commandPool        = m_transferPool ? m_transferPool : m_graphicsPool;
		cmdInfoDma.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		cmdInfoDma.commandBufferCount = 1;

		if (vkAllocateCommandBuffers(m_device->handle(), &cmdInfoGfx, &m_execBuffer) != VK_SUCCESS || vkAllocateCommandBuffers(m_device->handle(), &cmdInfoGfx, &m_initBuffer) != VK_SUCCESS || vkAllocateCommandBuffers(m_device->handle(), &cmdInfoDma, &m_sdmaBuffer) != VK_SUCCESS)
			Logger::exception("DxvkCommandList: Failed to allocate command buffer");

		if (m_device->hasDedicatedTransferQueue())
		{
			VkSemaphoreCreateInfo semInfo;
			semInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
			semInfo.pNext = nullptr;
			semInfo.flags = 0;

			if (vkCreateSemaphore(m_device->handle(), &semInfo, nullptr, &m_sdmaSemaphore) != VK_SUCCESS)
				Logger::exception("DxvkCommandList: Failed to create semaphore");
		}

	}

	VltCommandList::~VltCommandList()
	{
		this->reset();

		vkDestroySemaphore(m_device->handle(), m_sdmaSemaphore, nullptr);

		vkDestroyCommandPool(m_device->handle(), m_graphicsPool, nullptr);
		vkDestroyCommandPool(m_device->handle(), m_transferPool, nullptr);

		vkDestroyFence(m_device->handle(), m_fence, nullptr);
	}

	VkResult VltCommandList::submit(
		VkSemaphore waitSemaphore,
		VkSemaphore wakeSemaphore)
	{
		const auto& graphics = m_device->queues().graphics;
		const auto& transfer = m_device->queues().transfer;

		DxvkQueueSubmission info = DxvkQueueSubmission();

		if (m_cmdBuffersUsed.test(DxvkCmdBuffer::TransferBuffer))
		{
			info.cmdBuffers[info.cmdBufferCount++] = m_sdmaBuffer;

			if (m_device->hasDedicatedTransferQueue())
			{
				info.wakeSync[info.wakeCount++] = m_sdmaSemaphore;
				VkResult status                 = submitToQueue(transfer.queueHandle, VK_NULL_HANDLE, info);

				if (status != VK_SUCCESS)
					return status;

				info                          = DxvkQueueSubmission();
				info.waitSync[info.waitCount] = m_sdmaSemaphore;
				info.waitMask[info.waitCount] = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
				info.waitCount += 1;
			}
		}

		if (m_cmdBuffersUsed.test(DxvkCmdBuffer::InitBuffer))
			info.cmdBuffers[info.cmdBufferCount++] = m_initBuffer;
		if (m_cmdBuffersUsed.test(DxvkCmdBuffer::ExecBuffer))
			info.cmdBuffers[info.cmdBufferCount++] = m_execBuffer;

		if (waitSemaphore)
		{
			info.waitSync[info.waitCount] = waitSemaphore;
			info.waitMask[info.waitCount] = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
			info.waitCount += 1;
		}

		if (wakeSemaphore)
			info.wakeSync[info.wakeCount++] = wakeSemaphore;

		return submitToQueue(graphics.queueHandle, m_fence, info);
	}

	VkResult VltCommandList::synchronize()
	{
		VkResult status = VK_TIMEOUT;

		while (status == VK_TIMEOUT)
		{
			status = vkWaitForFences(
				m_device->handle(), 1, &m_fence, VK_FALSE,
				1'000'000'000ull);
		}

		return status;
	}

	void VltCommandList::beginRecording()
	{
		VkCommandBufferBeginInfo info;
		info.sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		info.pNext            = nullptr;
		info.flags            = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		info.pInheritanceInfo = nullptr;

		if ((m_graphicsPool && vkResetCommandPool(m_device->handle(), m_graphicsPool, 0) != VK_SUCCESS) || (m_transferPool && vkResetCommandPool(m_device->handle(), m_transferPool, 0) != VK_SUCCESS))
			Logger::err("DxvkCommandList: Failed to reset command buffer");

		if (vkBeginCommandBuffer(m_execBuffer, &info) != VK_SUCCESS || vkBeginCommandBuffer(m_initBuffer, &info) != VK_SUCCESS || vkBeginCommandBuffer(m_sdmaBuffer, &info) != VK_SUCCESS)
			Logger::err("DxvkCommandList: Failed to begin command buffer");

		if (vkResetFences(m_device->handle(), 1, &m_fence) != VK_SUCCESS)
			Logger::err("DxvkCommandList: Failed to reset fence");

		// Unconditionally mark the exec buffer as used. There
		// is virtually no use case where this isn't correct.
		m_cmdBuffersUsed = DxvkCmdBuffer::ExecBuffer;
	}

	void VltCommandList::endRecording()
	{
		if (vkEndCommandBuffer(m_execBuffer) != VK_SUCCESS || vkEndCommandBuffer(m_initBuffer) != VK_SUCCESS || vkEndCommandBuffer(m_sdmaBuffer) != VK_SUCCESS)
			Logger::err("DxvkCommandList::endRecording: Failed to record command buffer");
	}

	void VltCommandList::reset()
	{
		//// Free resources and other objects
		//// that are no longer in use
		//m_resources.reset();

		//// Recycle heavy Vulkan objects
		//m_descriptorPoolTracker.reset();

		//// Return buffer memory slices
		//m_bufferTracker.reset();

		//// Return query and event handles
		//m_gpuQueryTracker.reset();
		//m_gpuEventTracker.reset();

		//// Less important stuff
		//m_signalTracker.reset();
	}

	VkResult VltCommandList::submitToQueue(
		VkQueue                    queue,
		VkFence                    fence,
		const DxvkQueueSubmission& info)
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

	void VltCommandList::updateDescriptorSets(uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites)
	{
		vkUpdateDescriptorSets(m_device->handle(),
							   descriptorWriteCount, pDescriptorWrites,
							   0, nullptr);
	}

	void VltCommandList::updateDescriptorSetWithTemplate(VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorTemplate, const void* data)
	{
		vkUpdateDescriptorSetWithTemplate(m_device->handle(),
										  descriptorSet, descriptorTemplate, data);
	}

	void VltCommandList::cmdResetQuery(VkQueryPool queryPool, uint32_t queryId, VkEvent event)
	{
		if (event == VK_NULL_HANDLE)
		{
			vkResetQueryPool(
				m_device->handle(), queryPool, queryId, 1);
		}
		else
		{
			m_cmdBuffersUsed.set(DxvkCmdBuffer::InitBuffer);

			vkResetEvent(
				m_device->handle(), event);

			vkCmdResetQueryPool(
				m_initBuffer, queryPool, queryId, 1);

			vkCmdSetEvent(m_initBuffer,
						  event, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT);
		}
	}

	void VltCommandList::cmdBeginDebugUtilsLabel(VkDebugUtilsLabelEXT* labelInfo)
	{
		m_debug.cmdBeginDebugUtilsLabel(m_execBuffer, labelInfo);
	}

	void VltCommandList::cmdEndDebugUtilsLabel()
	{
		m_debug.cmdEndDebugUtilsLabel(m_execBuffer);
	}

	void VltCommandList::cmdInsertDebugUtilsLabel(VkDebugUtilsLabelEXT* labelInfo)
	{
		m_debug.cmdInsertDebugUtilsLabel(m_execBuffer, labelInfo);
	}

}  // namespace sce::vlt