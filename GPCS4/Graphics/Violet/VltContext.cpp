#include "VltContext.h"

#include "VltBuffer.h"
#include "VltDevice.h"
#include "VltImage.h"
#include "VltGpuEvent.h"

namespace sce::vlt
{
	VltContext::VltContext(VltDevice* device) :
		m_device(device),
		m_common(&device->m_objects),
		m_execBarriers(VltCmdType::ExecBuffer),
		m_transBarriers(VltCmdType::TransferBuffer),
		m_initBarriers(VltCmdType::InitBuffer),
		m_transAcquires(VltCmdType::TransferBuffer),
		m_staging(device)
	{
	}

	VltContext::~VltContext()
	{
	}

	void VltContext::beginRecording(const Rc<VltCommandList>& cmdList)
	{
		m_cmd = cmdList;
		m_cmd->beginRecording();

		// The current state of the internal command buffer is
		// undefined, so we have to bind and set up everything
		// before any draw or dispatch command is recorded.
		m_flags.clr(
			VltContextFlag::GpRenderingActive,
			VltContextFlag::GpXfbActive,
			VltContextFlag::GpClearRenderTargets);

		m_flags.set(
			VltContextFlag::GpDirtyPipeline,
			VltContextFlag::GpDirtyPipelineState,
			VltContextFlag::GpDirtyResources,
			VltContextFlag::GpDirtyVertexBuffers,
			VltContextFlag::GpDirtyIndexBuffer,
			VltContextFlag::GpDirtyXfbBuffers,
			VltContextFlag::GpDirtyBlendConstants,
			VltContextFlag::GpDirtyStencilRef,
			VltContextFlag::GpDirtyViewport,
			VltContextFlag::GpDirtyDepthBias,
			VltContextFlag::GpDirtyDepthBounds,
			VltContextFlag::CpDirtyPipeline,
			VltContextFlag::CpDirtyPipelineState,
			VltContextFlag::CpDirtyResources,
			VltContextFlag::DirtyDrawBuffer);
	}

	Rc<VltCommandList> VltContext::endRecording()
	{
		m_execBarriers.recordCommands(m_cmd);

		m_cmd->endRecording();
		return std::exchange(m_cmd, nullptr);
	}

	void VltContext::bindRenderTarget(
		uint32_t             slot,
		const VltAttachment& target)
	{
		m_state.cb.renderTargets.color[slot] = target;
	}
	
	void VltContext::bindDepthRenderTarget(
		const VltAttachment& depthTarget)
	{
		m_state.cb.renderTargets.depth = depthTarget;
	}
	
	void VltContext::dispatch(
		uint32_t x,
		uint32_t y,
		uint32_t z)
	{
		if (this->commitComputeState())
		{
			m_cmd->cmdDispatch(x, y, z);
		}
	}

	void VltContext::transformImage(
		const Rc<VltImage>&            dstImage,
		const VkImageSubresourceRange& dstSubresources,
		VkImageLayout                  srcLayout,
		VkImageLayout                  dstLayout)
	{
		if (srcLayout != dstLayout)
		{
			m_execBarriers.recordCommands(m_cmd);

			m_execBarriers.accessImage(
				dstImage, dstSubresources,
				srcLayout,
				dstImage->info().stages,
				dstImage->info().access,
				dstLayout,
				dstImage->info().stages,
				dstImage->info().access);

			m_cmd->trackResource<VltAccess::Write>(dstImage);
		}
	}

	void VltContext::transformImage(
		const Rc<VltImage>&            dstImage,
		const VkImageSubresourceRange& dstSubresources,
		VkImageLayout                  srcLayout,
		VkPipelineStageFlags2          srcStages,
		VkAccessFlags2                 srcAccess,
		VkImageLayout                  dstLayout,
		VkPipelineStageFlags2          dstStages,
		VkAccessFlags2                 dstAccess)
	{
		if (srcLayout != dstLayout)
		{
			m_execBarriers.recordCommands(m_cmd);

			m_execBarriers.accessImage(
				dstImage, dstSubresources,
				srcLayout,
				srcStages,
				srcAccess,
				dstLayout,
				dstStages,
				dstAccess);

			m_cmd->trackResource<VltAccess::Write>(dstImage);
		}
	}

	template <bool Indexed, bool Indirect>
	bool VltContext::commitGraphicsState()
	{
		if (m_flags.test(VltContextFlag::GpDirtyPipeline))
		{
			if (unlikely(!this->updateGraphicsPipeline()))
				return false;
		}

		return true;
	}

	bool VltContext::updateGraphicsPipeline()
	{
		m_state.gp.pipeline = lookupGraphicsPipeline(m_state.gp.shaders);
		if (unlikely(m_state.gp.pipeline == nullptr))
		{
			m_state.gp.flags = VltGraphicsPipelineFlags();
			return false;
		}

		m_flags.clr(VltContextFlag::GpDirtyPipeline);
		return true;
	}

	VltGraphicsPipeline* VltContext::lookupGraphicsPipeline(
		const VltGraphicsPipelineShaders& shaders)
	{
		auto idx = shaders.hash() % m_gpLookupCache.size();

		if (unlikely(!m_gpLookupCache[idx] || !shaders.eq(m_gpLookupCache[idx]->shaders())))
			m_gpLookupCache[idx] = m_common->pipelineManager().createGraphicsPipeline(shaders);

		return m_gpLookupCache[idx];
	}

	void VltContext::setViewports(
		uint32_t          viewportCount,
		const VkViewport* viewports)
	{
		if (m_state.gp.state.rs.viewportCount() != viewportCount)
		{
			m_state.gp.state.rs.setViewportCount(viewportCount);
			m_flags.set(VltContextFlag::GpDirtyPipelineState);
		}

		for (uint32_t i = 0; i < viewportCount; i++)
		{
			m_state.vp.viewports[i] = viewports[i];

			// Vulkan viewports are not allowed to have a width
			// of zero (but zero height is allowed),
			// so we fall back to a dummy viewport
			// and instead set an empty scissor rect, which is legal.
			if (viewports[i].width == 0.0f)
			{
				m_state.vp.viewports[i] = VkViewport{
					0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f
				};
				m_state.vp.scissorRects[i] = VkRect2D{
					VkOffset2D{ 0, 0 },
					VkExtent2D{ 0, 0 }
				};
			}
		}

		m_flags.set(VltContextFlag::GpDirtyViewport);
	}

	void VltContext::setScissors(
		uint32_t        scissorCount,
		const VkRect2D* scissorRects)
	{
		// Assume count of scissor and viewport are always equal.
		// In fact, these's only one scissor for Gnm
		if (m_state.gp.state.rs.viewportCount() != scissorCount)
		{
			m_state.gp.state.rs.setViewportCount(scissorCount);
			m_flags.set(VltContextFlag::GpDirtyPipelineState);
		}

		for (uint32_t i = 0; i < scissorCount; i++)
		{
			m_state.vp.scissorRects[i] = scissorRects[i];
		}

		m_flags.set(VltContextFlag::GpDirtyScissor);
	}

	void VltContext::bindResourceBuffer(
		uint32_t              slot,
		const VltBufferSlice& buffer)
	{
		if (likely(!m_rc[slot].bufferSlice.matches(buffer)))
		{
			m_flags.set(
				VltContextFlag::CpDirtyResources,
				VltContextFlag::GpDirtyResources);
		}
		else
		{
			m_flags.set(
				VltContextFlag::CpDirtyDescriptorBinding,
				VltContextFlag::GpDirtyDescriptorBinding);
		}

		m_rc[slot].bufferSlice = buffer;
	}

	void VltContext::bindShader(
		VkShaderStageFlagBits stage,
		const Rc<VltShader>&  shader)
	{
		Rc<VltShader>* shaderStage;

		// clang-format off
		switch (stage) 
		{
		  case VK_SHADER_STAGE_VERTEX_BIT:                  shaderStage = &m_state.gp.shaders.vs;  break;
		  case VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT:    shaderStage = &m_state.gp.shaders.tcs; break;
		  case VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT: shaderStage = &m_state.gp.shaders.tes; break;
		  case VK_SHADER_STAGE_GEOMETRY_BIT:                shaderStage = &m_state.gp.shaders.gs;  break;
		  case VK_SHADER_STAGE_FRAGMENT_BIT:                shaderStage = &m_state.gp.shaders.fs;  break;
		  case VK_SHADER_STAGE_COMPUTE_BIT:                 shaderStage = &m_state.cp.shaders.cs;  break;
		  default: return;
		}
		// clang-format on

		*shaderStage = shader;

		if (stage == VK_SHADER_STAGE_COMPUTE_BIT)
		{
			m_flags.set(
				VltContextFlag::CpDirtyPipeline,
				VltContextFlag::CpDirtyPipelineState,
				VltContextFlag::CpDirtyResources);
		}
		else
		{
			m_flags.set(
				VltContextFlag::GpDirtyPipeline,
				VltContextFlag::GpDirtyPipelineState,
				VltContextFlag::GpDirtyResources);
		}
	}

	void VltContext::uploadBuffer(
		const Rc<VltBuffer>& buffer,
		const void*          data)
	{
		auto bufferSlice = buffer->getSliceHandle();

		auto stagingSlice  = m_staging.alloc(bufferSlice.length, CACHE_LINE_SIZE);
		auto stagingHandle = stagingSlice.getSliceHandle();
		std::memcpy(stagingHandle.mapPtr, data, bufferSlice.length);

		VkBufferCopy region;
		region.srcOffset = stagingHandle.offset;
		region.dstOffset = bufferSlice.offset;
		region.size      = bufferSlice.length;

		m_cmd->cmdCopyBuffer(VltCmdType::TransferBuffer,
							 stagingHandle.handle, bufferSlice.handle, 1, &region);

		m_transBarriers.releaseBuffer(
			m_initBarriers, bufferSlice,
			m_device->queues().transfer.queueFamily,
			VK_PIPELINE_STAGE_TRANSFER_BIT,
			VK_ACCESS_TRANSFER_WRITE_BIT,
			m_device->queues().graphics.queueFamily,
			buffer->info().stages,
			buffer->info().access);

		m_cmd->trackResource<VltAccess::Read>(stagingSlice.buffer());
		m_cmd->trackResource<VltAccess::Write>(buffer);
	}

	void VltContext::uploadImage(
		const Rc<VltImage>&             image,
		const VkImageSubresourceLayers& subresources,
		const void*                     data,
		VkDeviceSize                    pitchPerRow,
		VkDeviceSize                    pitchPerLayer)
	{
		const VltFormatInfo* formatInfo = image->formatInfo();

		VkOffset3D imageOffset = { 0, 0, 0 };
		VkExtent3D imageExtent = image->mipLevelExtent(subresources.mipLevel);

		// Allocate staging buffer slice and copy data to it
		VkExtent3D elementCount = vutil::computeBlockCount(
			imageExtent, formatInfo->blockSize);
		elementCount.depth *= subresources.layerCount;

		auto stagingSlice  = m_staging.alloc(formatInfo->elementSize * vutil::flattenImageExtent(elementCount),
                                            CACHE_LINE_SIZE);
		auto stagingHandle = stagingSlice.getSliceHandle();

		vutil::packImageData(stagingHandle.mapPtr, data,
							 elementCount, formatInfo->elementSize,
							 pitchPerRow, pitchPerLayer);

		// Discard previous subresource contents
		m_transAcquires.accessImage(image,
									vutil::makeSubresourceRange(subresources),
									VK_IMAGE_LAYOUT_UNDEFINED, 0, 0,
									image->pickLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL),
									VK_PIPELINE_STAGE_TRANSFER_BIT,
									VK_ACCESS_TRANSFER_WRITE_BIT);

		m_transAcquires.recordCommands(m_cmd);

		// Perform copy on the transfer queue
		VkBufferImageCopy region;
		region.bufferOffset      = stagingHandle.offset;
		region.bufferRowLength   = 0;
		region.bufferImageHeight = 0;
		region.imageSubresource  = subresources;
		region.imageOffset       = imageOffset;
		region.imageExtent       = imageExtent;

		m_cmd->cmdCopyBufferToImage(VltCmdType::TransferBuffer,
									stagingHandle.handle, image->handle(),
									image->pickLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL),
									1, &region);

		// Transfer ownership to graphics queue
		m_transBarriers.releaseImage(m_initBarriers,
									 image, vutil::makeSubresourceRange(subresources),
									 m_device->queues().transfer.queueFamily,
									 image->pickLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL),
									 VK_PIPELINE_STAGE_TRANSFER_BIT,
									 VK_ACCESS_TRANSFER_WRITE_BIT,
									 m_device->queues().graphics.queueFamily,
									 image->info().layout,
									 image->info().stages,
									 image->info().access);

		m_cmd->trackResource<VltAccess::Write>(image);
		m_cmd->trackResource<VltAccess::Read>(stagingSlice.buffer());
	}

	void VltContext::signalGpuEvent(
		const Rc<VltGpuEvent>&  event,
		const VkDependencyInfo* dependencyInfo)
	{
		this->endRecording();

		m_cmd->cmdSetEvent2(event->handle(), dependencyInfo);

		m_cmd->trackResource<VltAccess::None>(event);
	}

	void VltContext::waitGpuEvent(
		const Rc<VltGpuEvent>&  event,
		const VkDependencyInfo* dependencyInfo)
	{
		VkEvent handle = event->handle();
		m_cmd->cmdWaitEvents2(1, &handle, dependencyInfo);
	}

	void VltContext::signal(
		const Rc<util::sync::Signal>& signal,
		uint64_t                      value)
	{
		m_cmd->queueSignal(signal, value);
	}

	void VltContext::beginRendering()
	{
	}

	void VltContext::endRendering()
	{
	}


	bool VltContext::commitComputeState()
	{
		if (m_flags.test(VltContextFlag::CpDirtyPipeline))
		{
			if (unlikely(!this->updateComputePipeline()))
				return false;
		}

		if (m_flags.any(
				VltContextFlag::CpDirtyResources,
				VltContextFlag::CpDirtyDescriptorBinding))
			this->updateComputeShaderResources();

		if (m_flags.test(VltContextFlag::CpDirtyPipelineState))
		{
			if (unlikely(!this->updateComputePipelineState()))
				return false;
		}

		return true;
	}

	bool VltContext::updateComputePipeline()
	{
	}

	bool VltContext::updateComputePipelineState()
	{
	}

	void VltContext::updateComputeShaderResources()
	{
	}

	void VltContext::updateGraphicsShaderResources()
	{
	}



}  // namespace sce::vlt