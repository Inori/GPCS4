#include "VltCmdList.h"

#include "VltDevice.h"

namespace sce::vlt
{
	VltCommandList::VltCommandList(VltDevice* device, VltQueueType queueType) :
		m_device(device),
		m_queueType(queueType),
		m_cmdBuffersUsed(0),
		m_descriptorPoolTracker(device)
	{
		const auto& execQueue     = m_queueType == VltQueueType::Graphics
										? m_device->queues().graphics
										: m_device->queues().compute;
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
		poolInfo.queueFamilyIndex = execQueue.queueFamily;

		if (vkCreateCommandPool(m_device->handle(), &poolInfo, nullptr, &m_execPool) != VK_SUCCESS)
			Logger::exception("DxvkCommandList: Failed to create execute command pool");

		if (m_device->hasDedicatedTransferQueue())
		{
			poolInfo.queueFamilyIndex = transferQueue.queueFamily;

			if (vkCreateCommandPool(m_device->handle(), &poolInfo, nullptr, &m_transferPool) != VK_SUCCESS)
				Logger::exception("DxvkCommandList: Failed to create transfer command pool");
		}

		VkCommandBufferAllocateInfo cmdInfoExec;
		cmdInfoExec.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		cmdInfoExec.pNext              = nullptr;
		cmdInfoExec.commandPool        = m_execPool;
		cmdInfoExec.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		cmdInfoExec.commandBufferCount = 1;

		VkCommandBufferAllocateInfo cmdInfoDma;
		cmdInfoDma.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		cmdInfoDma.pNext              = nullptr;
		cmdInfoDma.commandPool        = m_transferPool ? m_transferPool : m_execPool;
		cmdInfoDma.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		cmdInfoDma.commandBufferCount = 1;


		if (vkAllocateCommandBuffers(m_device->handle(), &cmdInfoExec, &m_execBuffer) != VK_SUCCESS ||
			vkAllocateCommandBuffers(m_device->handle(), &cmdInfoExec, &m_initBuffer) != VK_SUCCESS ||
			vkAllocateCommandBuffers(m_device->handle(), &cmdInfoDma, &m_transBuffer) != VK_SUCCESS)
			Logger::exception("DxvkCommandList: Failed to allocate command buffer");

		if (m_device->hasDedicatedTransferQueue())
		{
			VkSemaphoreCreateInfo semInfo;
			semInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
			semInfo.pNext = nullptr;
			semInfo.flags = 0;

			if (vkCreateSemaphore(m_device->handle(), &semInfo, nullptr, &m_transSemaphore) != VK_SUCCESS)
				Logger::exception("DxvkCommandList: Failed to create semaphore");
		}
	}

	VltCommandList::~VltCommandList()
	{
		this->reset();

		vkDestroySemaphore(m_device->handle(), m_transSemaphore, nullptr);

		vkDestroyCommandPool(m_device->handle(), m_execPool, nullptr);
		vkDestroyCommandPool(m_device->handle(), m_transferPool, nullptr);

		vkDestroyFence(m_device->handle(), m_fence, nullptr);
	}

	VkResult VltCommandList::submit(
		VkSemaphore waitSemaphore,
		VkSemaphore wakeSemaphore)
	{
		const auto& exec     = m_queueType == VltQueueType::Graphics
								   ? m_device->queues().graphics
								   : m_device->queues().compute;
		const auto& transfer = m_device->queues().transfer;

		VltQueueSubmission info = VltQueueSubmission();

		if (m_cmdBuffersUsed.test(VltCmdType::TransferBuffer))
		{
			info.cmdBuffers[info.cmdBufferCount++] = m_transBuffer;

			if (m_device->hasDedicatedTransferQueue())
			{
				info.wakeSync.emplace_back(
					populateSemaphoreSubmit(m_transSemaphore, 0, VK_PIPELINE_STAGE_TRANSFER_BIT));
				VkResult status = submitToQueue(transfer.queueHandle, VK_NULL_HANDLE, info);

				if (status != VK_SUCCESS)
					return status;

				info = VltQueueSubmission();
				info.waitSync.emplace_back(
					populateSemaphoreSubmit(m_transSemaphore, 0, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT));
			}
		}

		if (m_cmdBuffersUsed.test(VltCmdType::InitBuffer))
			info.cmdBuffers[info.cmdBufferCount++] = m_initBuffer;
		if (m_cmdBuffersUsed.test(VltCmdType::ExecBuffer))
			info.cmdBuffers[info.cmdBufferCount++] = m_execBuffer;

		if (waitSemaphore)
		{
			info.waitSync.emplace_back(
				populateSemaphoreSubmit(waitSemaphore, 0, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT));
		}

		if (wakeSemaphore)
		{
			info.wakeSync.emplace_back(
				populateSemaphoreSubmit(wakeSemaphore, 0, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT));
		}

		return submitToQueue(exec.queueHandle, m_fence, info);
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

		if ((m_execPool && vkResetCommandPool(m_device->handle(), m_execPool, 0) != VK_SUCCESS) ||
			(m_transferPool && vkResetCommandPool(m_device->handle(), m_transferPool, 0) != VK_SUCCESS))
			Logger::err("DxvkCommandList: Failed to reset command buffer");

		if (vkBeginCommandBuffer(m_execBuffer, &info) != VK_SUCCESS ||
			vkBeginCommandBuffer(m_initBuffer, &info) != VK_SUCCESS ||
			vkBeginCommandBuffer(m_transBuffer, &info) != VK_SUCCESS)
			Logger::err("DxvkCommandList: Failed to begin command buffer");

		if (vkResetFences(m_device->handle(), 1, &m_fence) != VK_SUCCESS)
			Logger::err("DxvkCommandList: Failed to reset fence");

		// Unconditionally mark the exec buffer as used. There
		// is virtually no use case where this isn't correct.
		m_cmdBuffersUsed = VltCmdType::ExecBuffer;
	}

	void VltCommandList::endRecording()
	{
		if (vkEndCommandBuffer(m_execBuffer) != VK_SUCCESS ||
			vkEndCommandBuffer(m_initBuffer) != VK_SUCCESS ||
			vkEndCommandBuffer(m_transBuffer) != VK_SUCCESS)
			Logger::err("DxvkCommandList::endRecording: Failed to record command buffer");
	}

	void VltCommandList::reset()
	{
		// Signal resources and events to
		// avoid stalling main thread
		m_signalTracker.reset();
		m_resources.reset();

		//// Recycle heavy Vulkan objects
		m_descriptorPoolTracker.reset();

		//// Return query and event handles
		m_gpuEventTracker.reset();
		m_semaphoreTracker.reset();
		//m_gpuQueryTracker.reset();
	}

	VkResult VltCommandList::submitToQueue(
		VkQueue                   queue,
		VkFence                   fence,
		const VltQueueSubmission& info)
	{
		std::array<VkCommandBufferSubmitInfo, 4> commands = {};
		std::transform(info.cmdBuffers, info.cmdBuffers + info.cmdBufferCount,
			commands.begin(), [](VkCommandBuffer cmd)
			{ 
				return VkCommandBufferSubmitInfo{
						VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO,
						nullptr,
						cmd,
						0
				};
			});

		VkSubmitInfo2 submitInfo;
		submitInfo.sType                    = VK_STRUCTURE_TYPE_SUBMIT_INFO_2;
		submitInfo.pNext                    = nullptr;
		submitInfo.flags                    = 0;
		submitInfo.waitSemaphoreInfoCount   = info.waitSync.size();
		submitInfo.pWaitSemaphoreInfos      = info.waitSync.data();
		submitInfo.commandBufferInfoCount   = info.cmdBufferCount;
		submitInfo.pCommandBufferInfos      = info.cmdBufferCount != 0 ? commands.data() : nullptr;
		submitInfo.signalSemaphoreInfoCount = info.wakeSync.size();
		submitInfo.pSignalSemaphoreInfos    = info.wakeSync.data();
	
		return vkQueueSubmit2(queue, 1, &submitInfo, fence);
	}

	void VltCommandList::signalSemaphore(const VltSemaphoreSubmission& submission)
	{
		submitSemaphore(submission, true);
	}

	void VltCommandList::waitSemaphore(const VltSemaphoreSubmission& submission)
	{
		submitSemaphore(submission, false);
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
			m_cmdBuffersUsed.set(VltCmdType::InitBuffer);

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
		m_device->vkd()->vkCmdBeginDebugUtilsLabelEXT(m_execBuffer, labelInfo);
	}

	void VltCommandList::cmdEndDebugUtilsLabel()
	{
		m_device->vkd()->vkCmdEndDebugUtilsLabelEXT(m_execBuffer);
	}

	void VltCommandList::cmdInsertDebugUtilsLabel(VkDebugUtilsLabelEXT* labelInfo)
	{
		m_device->vkd()->vkCmdInsertDebugUtilsLabelEXT(m_execBuffer, labelInfo);
	}

	VkSemaphoreSubmitInfo VltCommandList::populateSemaphoreSubmit(
		VkSemaphore           semaphore,
		uint64_t              value,
		VkPipelineStageFlags2 stageMask)
	{
		return VkSemaphoreSubmitInfo{
			VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
			nullptr,
			semaphore,
			value,
			stageMask,
			0,
		};
	}

	void VltCommandList::submitSemaphore(
		const VltSemaphoreSubmission& submission, bool signal)
	{
		auto semaInfo = populateSemaphoreSubmit(submission.semaphore->handle(),
												submission.value,
												submission.stageMask);

		const auto& exec = m_queueType == VltQueueType::Graphics
							   ? m_device->queues().graphics
							   : m_device->queues().compute;

		VkSubmitInfo2 submitInfo;
		submitInfo.sType                    = VK_STRUCTURE_TYPE_SUBMIT_INFO_2;
		submitInfo.pNext                    = nullptr;
		submitInfo.flags                    = 0;
		submitInfo.commandBufferInfoCount   = 0;
		submitInfo.pCommandBufferInfos      = nullptr;

		if (signal)
		{
			submitInfo.waitSemaphoreInfoCount   = 0;
			submitInfo.pWaitSemaphoreInfos      = nullptr;
			submitInfo.signalSemaphoreInfoCount = 1;
			submitInfo.pSignalSemaphoreInfos    = &semaInfo;
		}
		else
		{
			submitInfo.waitSemaphoreInfoCount   = 1;
			submitInfo.pWaitSemaphoreInfos      = &semaInfo;
			submitInfo.signalSemaphoreInfoCount = 0;
			submitInfo.pSignalSemaphoreInfos    = nullptr;
		}

		vkQueueSubmit2(exec.queueHandle, 1, &submitInfo, VK_NULL_HANDLE);

		m_semaphoreTracker.trackSemaphore(submission.semaphore);
	}

}  // namespace sce::vlt