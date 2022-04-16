#pragma once

#include "VltCommon.h"
#include "VltDebugUtil.h"
#include "VltLifetime.h"
#include "VltSignal.h"
#include "VltDescriptor.h"

namespace sce::vlt
{
	class VltDevice;

	/**
     * \brief Command buffer flags
     * 
     * A set of flags used to specify which of
     * the command buffers need to be submitted.
     */
	enum class VltCmdType : uint32_t
	{
		InitBuffer     = 0,
		ExecBuffer     = 1,
		TransferBuffer = 2,
		ComputeBuffer  = 3,
	};

	using VltCmdBufferFlags = util::Flags<VltCmdType>;

	/**
     * \brief Queue submission info
     *
     * Convenience struct that holds data for
     * actual command submissions. Internal use
     * only, array sizes are based on need.
     */
	struct VltQueueSubmission
	{
		uint32_t             waitCount;
		VkSemaphore          waitSync[2];
		VkPipelineStageFlags waitMask[2];
		uint32_t             wakeCount;
		VkSemaphore          wakeSync[2];
		uint32_t             cmdBufferCount;
		VkCommandBuffer      cmdBuffers[4];
	};

	/**
     * \brief DXVK command list
     * 
     * Stores a command buffer that a context can use to record Vulkan
     * commands. The command list shall also reference the resources
     * used by the recorded commands for automatic lifetime tracking.
     * When the command list has completed execution, resources that
     * are no longer used may get destroyed.
     */
	class VltCommandList : public RcObject
	{

	public:
		VltCommandList(VltDevice* device);
		virtual ~VltCommandList();

		/**
         * \brief Submits command list
         * 
         * \param [in] queue Device queue
         * \param [in] waitSemaphore Semaphore to wait on
         * \param [in] wakeSemaphore Semaphore to signal
         * \returns Submission status
         */
		VkResult submit(
			VkSemaphore waitSemaphore,
			VkSemaphore wakeSemaphore);

		/**
         * \brief Synchronizes command buffer execution
         * 
         * Waits for the fence associated with
         * this command buffer to get signaled.
         * \returns Synchronization status
         */
		VkResult synchronize();

		/**
         * \brief Begins recording
         * 
         * Resets the command buffer and
         * begins command buffer recording.
         */
		void beginRecording();

		/**
         * \brief Ends recording
         * 
         * Ends command buffer recording, making
         * the command list ready for submission.
         * \param [in] stats Stat counters
         */
		void endRecording();

		/**
         * \brief Adds a resource to track
         * 
         * Adds a resource to the internal resource tracker.
         * Resources will be kept alive and "in use" until
         * the device can guarantee that the submission has
         * completed.
         */
		template <VltAccess Access>
		void trackResource(Rc<VltResource> rc)
		{
			m_resources.trackResource<Access>(std::move(rc));
		}

		/**
         * \brief Tracks a descriptor pool
         * \param [in] pool The descriptor pool
         */
		void trackDescriptorPool(Rc<VltDescriptorPool> pool)
		{
			m_descriptorPoolTracker.trackDescriptorPool(pool);
		}

		/**
         * \brief Tracks a GPU event
         * 
         * The event will be returned to its event pool
         * after the command buffer has finished executing.
         * \param [in] handle Event handle
         */
		//void trackGpuEvent(DxvkGpuEventHandle handle)
		//{
		//	m_gpuEventTracker.trackEvent(handle);
		//}

		/**
         * \brief Tracks a GPU query
         * 
         * The query handle will be returned to its allocator
         * after the command buffer has finished executing.
         * \param [in] handle Event handle
         */
		//void trackGpuQuery(DxvkGpuQueryHandle handle)
		//{
		//	m_gpuQueryTracker.trackQuery(handle);
		//}

		/**
         * \brief Queues signal
         * 
         * The signal will be notified once the command
         * buffer has finished executing on the GPU.
         * \param [in] signal The signal
         * \param [in] value Signal value
         */
		void queueSignal(const Rc<util::sync::Signal>& signal, uint64_t value)
		{
			m_signalTracker.add(signal, value);
		}

		/**
         * \brief Notifies resources and signals
         */
		void notifyObjects()
		{
			m_resources.notify();
			m_signalTracker.notify();
		}

		/**
         * \brief Resets the command list
         * 
         * Resets the internal command buffer of the command list and
         * marks all tracked resources as unused. When submitting the
         * command list to the device, this method will be called once
         * the command list completes execution.
         */
		void reset();

		void updateDescriptorSets(
			uint32_t                    descriptorWriteCount,
			const VkWriteDescriptorSet* pDescriptorWrites);

		void updateDescriptorSetWithTemplate(
			VkDescriptorSet            descriptorSet,
			VkDescriptorUpdateTemplate descriptorTemplate,
			const void*                data);

		void cmdBeginConditionalRendering(
			const VkConditionalRenderingBeginInfoEXT* pConditionalRenderingBegin)
		{
			vkCmdBeginConditionalRenderingEXT(
				m_execBuffer, pConditionalRenderingBegin);
		}

		void cmdEndConditionalRendering()
		{
			vkCmdEndConditionalRenderingEXT(m_execBuffer);
		}

		void cmdBeginQuery(
			VkQueryPool         queryPool,
			uint32_t            query,
			VkQueryControlFlags flags)
		{
			vkCmdBeginQuery(m_execBuffer,
							queryPool, query, flags);
		}

		void cmdBeginQueryIndexed(
			VkQueryPool         queryPool,
			uint32_t            query,
			VkQueryControlFlags flags,
			uint32_t            index)
		{
			vkCmdBeginQueryIndexedEXT(
				m_execBuffer, queryPool, query, flags, index);
		}

		void cmdBeginRenderPass(
			const VkRenderPassBeginInfo* pRenderPassBegin,
			VkSubpassContents            contents)
		{
			vkCmdBeginRenderPass(m_execBuffer,
								 pRenderPassBegin, contents);
		}

		void cmdBeginTransformFeedback(
			uint32_t            firstBuffer,
			uint32_t            bufferCount,
			const VkBuffer*     counterBuffers,
			const VkDeviceSize* counterOffsets)
		{
			vkCmdBeginTransformFeedbackEXT(m_execBuffer,
										   firstBuffer, bufferCount, counterBuffers, counterOffsets);
		}

		void cmdBindDescriptorSet(
			VkPipelineBindPoint pipeline,
			VkPipelineLayout    pipelineLayout,
			VkDescriptorSet     descriptorSet,
			uint32_t            dynamicOffsetCount,
			const uint32_t*     pDynamicOffsets)
		{
			vkCmdBindDescriptorSets(m_execBuffer,
									pipeline, pipelineLayout, 0, 1,
									&descriptorSet, dynamicOffsetCount, pDynamicOffsets);
		}

		void cmdBindIndexBuffer(
			VkBuffer     buffer,
			VkDeviceSize offset,
			VkIndexType  indexType)
		{
			vkCmdBindIndexBuffer(m_execBuffer,
								 buffer, offset, indexType);
		}

		void cmdBindPipeline(
			VkPipelineBindPoint pipelineBindPoint,
			VkPipeline          pipeline)
		{
			vkCmdBindPipeline(m_execBuffer,
							  pipelineBindPoint, pipeline);
		}

		void cmdBindTransformFeedbackBuffers(
			uint32_t            firstBinding,
			uint32_t            bindingCount,
			const VkBuffer*     pBuffers,
			const VkDeviceSize* pOffsets,
			const VkDeviceSize* pSizes)
		{
			vkCmdBindTransformFeedbackBuffersEXT(m_execBuffer,
												 firstBinding, bindingCount, pBuffers, pOffsets, pSizes);
		}

		void cmdBindVertexBuffers(
			uint32_t            firstBinding,
			uint32_t            bindingCount,
			const VkBuffer*     pBuffers,
			const VkDeviceSize* pOffsets)
		{
			vkCmdBindVertexBuffers(m_execBuffer,
								   firstBinding, bindingCount, pBuffers, pOffsets);
		}

		void cmdBindVertexBuffers2(
			uint32_t            firstBinding,
			uint32_t            bindingCount,
			const VkBuffer*     pBuffers,
			const VkDeviceSize* pOffsets,
			const VkDeviceSize* pSizes,
			const VkDeviceSize* pStrides)
		{
			vkCmdBindVertexBuffers2EXT(m_execBuffer,
									   firstBinding, bindingCount, pBuffers, pOffsets,
									   pSizes, pStrides);
		}

		void cmdLaunchCuKernel(VkCuLaunchInfoNVX launchInfo)
		{
			vkCmdCuLaunchKernelNVX(m_execBuffer, &launchInfo);
		}

		void cmdBlitImage(
			VkImage            srcImage,
			VkImageLayout      srcImageLayout,
			VkImage            dstImage,
			VkImageLayout      dstImageLayout,
			uint32_t           regionCount,
			const VkImageBlit* pRegions,
			VkFilter           filter)
		{
			vkCmdBlitImage(m_execBuffer,
						   srcImage, srcImageLayout,
						   dstImage, dstImageLayout,
						   regionCount, pRegions, filter);
		}

		void cmdClearAttachments(
			uint32_t                 attachmentCount,
			const VkClearAttachment* pAttachments,
			uint32_t                 rectCount,
			const VkClearRect*       pRects)
		{
			vkCmdClearAttachments(m_execBuffer,
								  attachmentCount, pAttachments,
								  rectCount, pRects);
		}

		void cmdClearColorImage(
			VkImage                        image,
			VkImageLayout                  imageLayout,
			const VkClearColorValue*       pColor,
			uint32_t                       rangeCount,
			const VkImageSubresourceRange* pRanges)
		{
			vkCmdClearColorImage(m_execBuffer,
								 image, imageLayout, pColor,
								 rangeCount, pRanges);
		}

		void cmdClearDepthStencilImage(
			VkImage                         image,
			VkImageLayout                   imageLayout,
			const VkClearDepthStencilValue* pDepthStencil,
			uint32_t                        rangeCount,
			const VkImageSubresourceRange*  pRanges)
		{
			vkCmdClearDepthStencilImage(m_execBuffer,
										image, imageLayout, pDepthStencil,
										rangeCount, pRanges);
		}

		void cmdCopyBuffer(
			VltCmdType          cmdBuffer,
			VkBuffer            srcBuffer,
			VkBuffer            dstBuffer,
			uint32_t            regionCount,
			const VkBufferCopy* pRegions)
		{
			m_cmdBuffersUsed.set(cmdBuffer);

			vkCmdCopyBuffer(getCmdBuffer(cmdBuffer),
							srcBuffer, dstBuffer,
							regionCount, pRegions);
		}

		void cmdCopyBufferToImage(
			VltCmdType               cmdBuffer,
			VkBuffer                 srcBuffer,
			VkImage                  dstImage,
			VkImageLayout            dstImageLayout,
			uint32_t                 regionCount,
			const VkBufferImageCopy* pRegions)
		{
			m_cmdBuffersUsed.set(cmdBuffer);

			vkCmdCopyBufferToImage(getCmdBuffer(cmdBuffer),
								   srcBuffer, dstImage, dstImageLayout,
								   regionCount, pRegions);
		}

		void cmdCopyImage(
			VltCmdType         cmdBuffer,
			VkImage            srcImage,
			VkImageLayout      srcImageLayout,
			VkImage            dstImage,
			VkImageLayout      dstImageLayout,
			uint32_t           regionCount,
			const VkImageCopy* pRegions)
		{
			m_cmdBuffersUsed.set(cmdBuffer);

			vkCmdCopyImage(getCmdBuffer(cmdBuffer),
						   srcImage, srcImageLayout,
						   dstImage, dstImageLayout,
						   regionCount, pRegions);
		}

		void cmdCopyImageToBuffer(
			VltCmdType               cmdBuffer,
			VkImage                  srcImage,
			VkImageLayout            srcImageLayout,
			VkBuffer                 dstBuffer,
			uint32_t                 regionCount,
			const VkBufferImageCopy* pRegions)
		{
			m_cmdBuffersUsed.set(cmdBuffer);

			vkCmdCopyImageToBuffer(getCmdBuffer(cmdBuffer),
								   srcImage, srcImageLayout, dstBuffer,
								   regionCount, pRegions);
		}

		void cmdCopyQueryPoolResults(
			VkQueryPool        queryPool,
			uint32_t           firstQuery,
			uint32_t           queryCount,
			VkBuffer           dstBuffer,
			VkDeviceSize       dstOffset,
			VkDeviceSize       stride,
			VkQueryResultFlags flags)
		{
			vkCmdCopyQueryPoolResults(m_execBuffer,
									  queryPool, firstQuery, queryCount,
									  dstBuffer, dstOffset, stride, flags);
		}

		void cmdDispatch(
			uint32_t x,
			uint32_t y,
			uint32_t z)
		{
			vkCmdDispatch(m_execBuffer, x, y, z);
		}

		void cmdDispatchIndirect(
			VkBuffer     buffer,
			VkDeviceSize offset)
		{
			vkCmdDispatchIndirect(
				m_execBuffer, buffer, offset);
		}

		void cmdDraw(
			uint32_t vertexCount,
			uint32_t instanceCount,
			uint32_t firstVertex,
			uint32_t firstInstance)
		{
			vkCmdDraw(m_execBuffer,
					  vertexCount, instanceCount,
					  firstVertex, firstInstance);
		}

		void cmdDrawIndirect(
			VkBuffer     buffer,
			VkDeviceSize offset,
			uint32_t     drawCount,
			uint32_t     stride)
		{
			vkCmdDrawIndirect(m_execBuffer,
							  buffer, offset, drawCount, stride);
		}

		void cmdDrawIndirectCount(
			VkBuffer     buffer,
			VkDeviceSize offset,
			VkBuffer     countBuffer,
			VkDeviceSize countOffset,
			uint32_t     maxDrawCount,
			uint32_t     stride)
		{
			vkCmdDrawIndirectCountKHR(m_execBuffer,
									  buffer, offset, countBuffer, countOffset, maxDrawCount, stride);
		}

		void cmdDrawIndexed(
			uint32_t indexCount,
			uint32_t instanceCount,
			uint32_t firstIndex,
			uint32_t vertexOffset,
			uint32_t firstInstance)
		{
			vkCmdDrawIndexed(m_execBuffer,
							 indexCount, instanceCount,
							 firstIndex, vertexOffset,
							 firstInstance);
		}

		void cmdDrawIndexedIndirect(
			VkBuffer     buffer,
			VkDeviceSize offset,
			uint32_t     drawCount,
			uint32_t     stride)
		{
			vkCmdDrawIndexedIndirect(m_execBuffer,
									 buffer, offset, drawCount, stride);
		}

		void cmdDrawIndexedIndirectCount(
			VkBuffer     buffer,
			VkDeviceSize offset,
			VkBuffer     countBuffer,
			VkDeviceSize countOffset,
			uint32_t     maxDrawCount,
			uint32_t     stride)
		{
			vkCmdDrawIndexedIndirectCountKHR(m_execBuffer,
											 buffer, offset, countBuffer, countOffset, maxDrawCount, stride);
		}

		void cmdDrawIndirectVertexCount(
			uint32_t     instanceCount,
			uint32_t     firstInstance,
			VkBuffer     counterBuffer,
			VkDeviceSize counterBufferOffset,
			uint32_t     counterOffset,
			uint32_t     vertexStride)
		{
			vkCmdDrawIndirectByteCountEXT(m_execBuffer,
										  instanceCount, firstInstance, counterBuffer,
										  counterBufferOffset, counterOffset, vertexStride);
		}

		void cmdEndQuery(
			VkQueryPool queryPool,
			uint32_t    query)
		{
			vkCmdEndQuery(m_execBuffer, queryPool, query);
		}

		void cmdEndQueryIndexed(
			VkQueryPool queryPool,
			uint32_t    query,
			uint32_t    index)
		{
			vkCmdEndQueryIndexedEXT(
				m_execBuffer, queryPool, query, index);
		}

		void cmdEndRenderPass()
		{
			vkCmdEndRenderPass(m_execBuffer);
		}

		void cmdEndTransformFeedback(
			uint32_t            firstBuffer,
			uint32_t            bufferCount,
			const VkBuffer*     counterBuffers,
			const VkDeviceSize* counterOffsets)
		{
			vkCmdEndTransformFeedbackEXT(m_execBuffer,
										 firstBuffer, bufferCount, counterBuffers, counterOffsets);
		}

		void cmdFillBuffer(
			VltCmdType   cmdBuffer,
			VkBuffer     dstBuffer,
			VkDeviceSize dstOffset,
			VkDeviceSize size,
			uint32_t     data)
		{
			m_cmdBuffersUsed.set(cmdBuffer);

			vkCmdFillBuffer(getCmdBuffer(cmdBuffer),
							dstBuffer, dstOffset, size, data);
		}

		void cmdPipelineBarrier(
			VltCmdType                   cmdBuffer,
			VkPipelineStageFlags         srcStageMask,
			VkPipelineStageFlags         dstStageMask,
			VkDependencyFlags            dependencyFlags,
			uint32_t                     memoryBarrierCount,
			const VkMemoryBarrier*       pMemoryBarriers,
			uint32_t                     bufferMemoryBarrierCount,
			const VkBufferMemoryBarrier* pBufferMemoryBarriers,
			uint32_t                     imageMemoryBarrierCount,
			const VkImageMemoryBarrier*  pImageMemoryBarriers)
		{
			m_cmdBuffersUsed.set(cmdBuffer);

			vkCmdPipelineBarrier(getCmdBuffer(cmdBuffer),
								 srcStageMask, dstStageMask, dependencyFlags,
								 memoryBarrierCount, pMemoryBarriers,
								 bufferMemoryBarrierCount, pBufferMemoryBarriers,
								 imageMemoryBarrierCount, pImageMemoryBarriers);
		}

		void cmdPipelineBarrier2(
			VltCmdType              cmdBuffer,
			const VkDependencyInfo* pDependencyInfo)
		{
			m_cmdBuffersUsed.set(cmdBuffer);

			vkCmdPipelineBarrier2(getCmdBuffer(cmdBuffer),
								  pDependencyInfo);
		}

		void cmdPushConstants(
			VkPipelineLayout   layout,
			VkShaderStageFlags stageFlags,
			uint32_t           offset,
			uint32_t           size,
			const void*        pValues)
		{
			vkCmdPushConstants(m_execBuffer,
							   layout, stageFlags, offset, size, pValues);
		}

		void cmdResetQuery(
			VkQueryPool queryPool,
			uint32_t    queryId,
			VkEvent     event);

		void cmdResetQueryPool(
			VkQueryPool queryPool,
			uint32_t    firstQuery,
			uint32_t    queryCount)
		{
			m_cmdBuffersUsed.set(VltCmdType::InitBuffer);

			vkCmdResetQueryPool(m_initBuffer,
								queryPool, firstQuery, queryCount);
		}

		void cmdResolveImage(
			VkImage               srcImage,
			VkImageLayout         srcImageLayout,
			VkImage               dstImage,
			VkImageLayout         dstImageLayout,
			uint32_t              regionCount,
			const VkImageResolve* pRegions)
		{
			vkCmdResolveImage(m_execBuffer,
							  srcImage, srcImageLayout,
							  dstImage, dstImageLayout,
							  regionCount, pRegions);
		}

		void cmdUpdateBuffer(
			VltCmdType   cmdBuffer,
			VkBuffer     dstBuffer,
			VkDeviceSize dstOffset,
			VkDeviceSize dataSize,
			const void*  pData)
		{
			m_cmdBuffersUsed.set(cmdBuffer);

			vkCmdUpdateBuffer(getCmdBuffer(cmdBuffer),
							  dstBuffer, dstOffset, dataSize, pData);
		}

		void cmdSetBlendConstants(const float blendConstants[4])
		{
			vkCmdSetBlendConstants(m_execBuffer, blendConstants);
		}

		void cmdSetDepthBias(
			float depthBiasConstantFactor,
			float depthBiasClamp,
			float depthBiasSlopeFactor)
		{
			vkCmdSetDepthBias(m_execBuffer,
							  depthBiasConstantFactor,
							  depthBiasClamp,
							  depthBiasSlopeFactor);
		}

		void cmdSetDepthBounds(
			float minDepthBounds,
			float maxDepthBounds)
		{
			vkCmdSetDepthBounds(m_execBuffer,
								minDepthBounds,
								maxDepthBounds);
		}

		void cmdSetEvent(
			VkEvent              event,
			VkPipelineStageFlags stages)
		{
			vkCmdSetEvent(m_execBuffer, event, stages);
		}

		void cmdSetEvent2(
			VkEvent                 event,
			const VkDependencyInfo* dependencyInfo)
		{
			vkCmdSetEvent2(m_execBuffer, event, dependencyInfo);
		}

		void cmdWaitEvents2(
			uint32_t                eventCount,
			VkEvent*                events,
			const VkDependencyInfo* pDependencyInfo)
		{
			vkCmdWaitEvents2(m_execBuffer, eventCount, events, pDependencyInfo);
		}

		void cmdSetScissor(
			uint32_t        firstScissor,
			uint32_t        scissorCount,
			const VkRect2D* scissors)
		{
			vkCmdSetScissor(m_execBuffer,
							firstScissor, scissorCount, scissors);
		}

		void cmdSetStencilReference(
			VkStencilFaceFlags faceMask,
			uint32_t           reference)
		{
			vkCmdSetStencilReference(m_execBuffer,
									 faceMask, reference);
		}

		void cmdSetViewport(
			uint32_t          firstViewport,
			uint32_t          viewportCount,
			const VkViewport* viewports)
		{
			vkCmdSetViewport(m_execBuffer,
							 firstViewport, viewportCount, viewports);
		}

		void cmdWriteTimestamp(
			VkPipelineStageFlagBits pipelineStage,
			VkQueryPool             queryPool,
			uint32_t                query)
		{
			vkCmdWriteTimestamp(m_execBuffer,
								pipelineStage, queryPool, query);
		}



		void cmdBeginDebugUtilsLabel(VkDebugUtilsLabelEXT* labelInfo);

		void cmdEndDebugUtilsLabel();

		void cmdInsertDebugUtilsLabel(VkDebugUtilsLabelEXT* labelInfo);

	private:
		VkCommandBuffer getCmdBuffer(VltCmdType cmdBuffer) const
		{
			if (cmdBuffer == VltCmdType::ExecBuffer)
				return m_execBuffer;
			if (cmdBuffer == VltCmdType::InitBuffer)
				return m_initBuffer;
			if (cmdBuffer == VltCmdType::TransferBuffer)
				return m_transBuffer;
			return VK_NULL_HANDLE;
		}

		VkResult submitToQueue(
			VkQueue                   queue,
			VkFence                   fence,
			const VltQueueSubmission& info);

	private:
		VltDevice* m_device;

		VkFence m_fence;

		VkCommandPool m_graphicsPool = VK_NULL_HANDLE;
		VkCommandPool m_transferPool = VK_NULL_HANDLE;

		VkCommandBuffer m_execBuffer  = VK_NULL_HANDLE;
		VkCommandBuffer m_initBuffer  = VK_NULL_HANDLE;
		VkCommandBuffer m_transBuffer = VK_NULL_HANDLE;

		VkSemaphore m_transSemaphore = VK_NULL_HANDLE;

		VltCmdBufferFlags m_cmdBuffersUsed;

		VltLifetimeTracker       m_resources;
		VltDescriptorPoolTracker m_descriptorPoolTracker;
		VltSignalTracker         m_signalTracker;
		//DxvkGpuQueryTracker       m_gpuQueryTracker;

		VltDebugUtil m_debug;
	};

}  // namespace sce::vlt