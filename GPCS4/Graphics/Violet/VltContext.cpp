#include "VltContext.h"

#include "VltBuffer.h"
#include "VltDevice.h"
#include "VltImage.h"
#include "VltGpuEvent.h"
#include "VltDescriptor.h"

LOG_CHANNEL("Graphic.Violet");

namespace sce::vlt
{
	VltContext::VltContext(VltDevice* device) :
		m_device(device),
		m_common(&device->m_objects),
		m_execBarriers(VltCmdType::ExecBuffer),
		m_execAcquires(VltCmdType::ExecBuffer),
		m_transBarriers(VltCmdType::TransferBuffer),
		m_transAcquires(VltCmdType::TransferBuffer),
		m_initBarriers(VltCmdType::InitBuffer),
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
			VltContextFlag::GpRenderPassBound,
			VltContextFlag::GpXfbActive);

		m_flags.set(
			VltContextFlag::GpDirtyFramebuffer,
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
		this->spillRenderPass();

		m_execBarriers.recordCommands(m_cmd);
		m_transBarriers.recordCommands(m_cmd);
		m_initBarriers.recordCommands(m_cmd);

		m_cmd->endRecording();
		return std::exchange(m_cmd, nullptr);
	}

	void VltContext::flushCommandList()
	{
		auto commandList = this->endRecording();
		auto queueType   = commandList->type();

		m_device->submitCommandList(
			commandList,
			VK_NULL_HANDLE,
			VK_NULL_HANDLE);

		this->beginRecording(
			m_device->createCommandList(queueType));
	}

	void VltContext::bindRenderTargets(const VltRenderTargets& targets)
	{
		// Set up default render pass ops
		m_state.cb.renderTargets = targets;

		this->resetRenderPassOps(
			m_state.cb.renderTargets,
			m_state.cb.renderPassOps);

		if (!m_state.cb.framebufferInfo.hasTargets(targets))
		{
			// Create a new framebuffer object next
			// time we start rendering something
			m_flags.set(VltContextFlag::GpDirtyFramebuffer);
		}
		else
		{
			// Don't redundantly spill the render pass if
			// the same render targets are bound again
			m_flags.clr(VltContextFlag::GpDirtyFramebuffer);
		}
	}
	

	void VltContext::bindIndexBuffer(
		const VltBufferSlice& buffer,
		VkIndexType           indexType)
	{
		m_state.vi.indexBuffer = buffer;
		m_state.vi.indexType   = indexType;

		m_flags.set(VltContextFlag::GpDirtyIndexBuffer);
	}

	void VltContext::bindVertexBuffer(
		uint32_t              binding,
		const VltBufferSlice& buffer,
		uint32_t              stride)
	{
		m_state.vi.vertexBuffers[binding] = buffer;
		m_flags.set(VltContextFlag::GpDirtyVertexBuffers);

		if (unlikely(!buffer.defined()))
			stride = 0;

		if (unlikely(m_state.vi.vertexStrides[binding] != stride))
		{
			m_state.vi.vertexStrides[binding] = stride;
			m_flags.set(VltContextFlag::GpDirtyPipelineState);
		}
	}

	void VltContext::pushConstants(
		uint32_t    offset,
		uint32_t    size,
		const void* data)
	{
		std::memcpy(&m_state.pc.data[offset], data, size);

		m_flags.set(VltContextFlag::DirtyPushConstants);
	}

	void VltContext::draw(
		uint32_t vertexCount,
		uint32_t instanceCount,
		uint32_t firstVertex,
		uint32_t firstInstance)
	{
		if (this->commitGraphicsState<false, false>())
		{
			m_cmd->cmdDraw(
				vertexCount, instanceCount,
				firstVertex, firstInstance);
		}
	}

	void VltContext::drawIndexed(
		uint32_t indexCount,
		uint32_t instanceCount,
		uint32_t firstIndex,
		uint32_t vertexOffset,
		uint32_t firstInstance)
	{
		if (this->commitGraphicsState<true, false>())
		{
			m_cmd->cmdDrawIndexed(
				indexCount, instanceCount,
				firstIndex, vertexOffset,
				firstInstance);
		}
	}
	
	void VltContext::dispatch(
		uint32_t x,
		uint32_t y,
		uint32_t z)
	{
		if (this->commitComputeState())
		{
			this->commitComputePrevBarriers();

			m_cmd->cmdDispatch(x, y, z);

			this->commitComputePostBarriers();
		}
	}

	void VltContext::changeImageLayout(
		const Rc<VltImage>& image,
		VkImageLayout       layout)
	{
		if (image->info().layout != layout)
		{
			this->spillRenderPass();

			VkImageSubresourceRange subresources;
			subresources.aspectMask     = image->formatInfo()->aspectMask;
			subresources.baseArrayLayer = 0;
			subresources.baseMipLevel   = 0;
			subresources.layerCount     = image->info().numLayers;
			subresources.levelCount     = image->info().mipLevels;

			if (m_execBarriers.isImageDirty(image, subresources, VltAccess::Write))
				m_execBarriers.recordCommands(m_cmd);

			m_execBarriers.accessImage(image, subresources,
									   image->info().layout,
									   image->info().stages,
									   image->info().access,
									   layout,
									   image->info().stages,
									   image->info().access);

			image->updateLayout(layout);
		}
	}

	void VltContext::transformImage(
		const Rc<VltImage>&            dstImage,
		const VkImageSubresourceRange& dstSubresources,
		VkImageLayout                  srcLayout,
		VkImageLayout                  dstLayout)
	{
		this->spillRenderPass();

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

	void VltContext::copyBuffer(
		const Rc<VltBuffer>& dstBuffer,
		VkDeviceSize         dstOffset,
		const Rc<VltBuffer>& srcBuffer,
		VkDeviceSize         srcOffset,
		VkDeviceSize         numBytes)
	{
		if (numBytes == 0)
			return;

		this->spillRenderPass();

		auto dstSlice = dstBuffer->getSliceHandle(dstOffset, numBytes);
		auto srcSlice = srcBuffer->getSliceHandle(srcOffset, numBytes);

		if (m_execBarriers.isBufferDirty(srcSlice, VltAccess::Read) || 
			m_execBarriers.isBufferDirty(dstSlice, VltAccess::Write))
			m_execBarriers.recordCommands(m_cmd);

		VkBufferCopy bufferRegion;
		bufferRegion.srcOffset = srcSlice.offset;
		bufferRegion.dstOffset = dstSlice.offset;
		bufferRegion.size      = dstSlice.length;

		m_cmd->cmdCopyBuffer(VltCmdType::ExecBuffer,
							 srcSlice.handle, dstSlice.handle, 1, &bufferRegion);

		m_execBarriers.accessBuffer(srcSlice,
									VK_PIPELINE_STAGE_TRANSFER_BIT,
									VK_ACCESS_TRANSFER_READ_BIT,
									srcBuffer->info().stages,
									srcBuffer->info().access);

		m_execBarriers.accessBuffer(dstSlice,
									VK_PIPELINE_STAGE_TRANSFER_BIT,
									VK_ACCESS_TRANSFER_WRITE_BIT,
									dstBuffer->info().stages,
									dstBuffer->info().access);

		m_cmd->trackResource<VltAccess::Write>(dstBuffer);
		m_cmd->trackResource<VltAccess::Read>(srcBuffer);
	}

	void VltContext::copyBufferToImage(
		const Rc<VltImage>&      dstImage,
		VkImageSubresourceLayers dstSubresource,
		VkOffset3D               dstOffset,
		VkExtent3D               dstExtent,
		const Rc<VltBuffer>&     srcBuffer,
		VkDeviceSize             srcOffset,
		VkExtent2D               srcExtent)
	{
		this->spillRenderPass();

		auto srcSlice = srcBuffer->getSliceHandle(srcOffset, 0);

		// We may copy to only one aspect of a depth-stencil image,
		// but pipeline barriers need to have all aspect bits set
		auto dstFormatInfo = dstImage->formatInfo();

		auto dstSubresourceRange       = vutil::makeSubresourceRange(dstSubresource);
		dstSubresourceRange.aspectMask = dstFormatInfo->aspectMask;

		if (m_execBarriers.isImageDirty(dstImage, dstSubresourceRange, VltAccess::Write) ||
			m_execBarriers.isBufferDirty(srcSlice, VltAccess::Read))
			m_execBarriers.recordCommands(m_cmd);

		// Initialize the image if the entire subresource is covered
		VkImageLayout dstImageLayoutInitial  = dstImage->info().layout;
		VkImageLayout dstImageLayoutTransfer = dstImage->pickLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

		if (dstImage->isFullSubresource(dstSubresource, dstExtent))
			dstImageLayoutInitial = VK_IMAGE_LAYOUT_UNDEFINED;

		m_execAcquires.accessImage(
			dstImage, dstSubresourceRange,
			dstImageLayoutInitial, 0, 0,
			dstImageLayoutTransfer,
			VK_PIPELINE_STAGE_TRANSFER_BIT,
			VK_ACCESS_TRANSFER_WRITE_BIT);

		m_execAcquires.recordCommands(m_cmd);

		VkBufferImageCopy copyRegion;
		copyRegion.bufferOffset      = srcSlice.offset;
		copyRegion.bufferRowLength   = srcExtent.width;
		copyRegion.bufferImageHeight = srcExtent.height;
		copyRegion.imageSubresource  = dstSubresource;
		copyRegion.imageOffset       = dstOffset;
		copyRegion.imageExtent       = dstExtent;

		m_cmd->cmdCopyBufferToImage(VltCmdType::ExecBuffer,
									srcSlice.handle, dstImage->handle(),
									dstImageLayoutTransfer, 1, &copyRegion);

		m_execBarriers.accessImage(
			dstImage, dstSubresourceRange,
			dstImageLayoutTransfer,
			VK_PIPELINE_STAGE_TRANSFER_BIT,
			VK_ACCESS_TRANSFER_WRITE_BIT,
			dstImage->info().layout,
			dstImage->info().stages,
			dstImage->info().access);

		m_execBarriers.accessBuffer(srcSlice,
									VK_PIPELINE_STAGE_TRANSFER_BIT,
									VK_ACCESS_TRANSFER_READ_BIT,
									srcBuffer->info().stages,
									srcBuffer->info().access);

		m_cmd->trackResource<VltAccess::Write>(dstImage);
		m_cmd->trackResource<VltAccess::Read>(srcBuffer);
	}


	void VltContext::updateIndexBufferBinding()
	{
		m_flags.clr(VltContextFlag::GpDirtyIndexBuffer);

		if (m_state.vi.indexBuffer.defined())
		{
			auto bufferInfo = m_state.vi.indexBuffer.getDescriptor();

			m_cmd->cmdBindIndexBuffer(
				bufferInfo.buffer.buffer,
				bufferInfo.buffer.offset,
				m_state.vi.indexType);
		}
		else
		{
			m_cmd->cmdBindIndexBuffer(
				m_common->dummyResources().bufferHandle(),
				0, VK_INDEX_TYPE_UINT32);
		}
	}

	void VltContext::updateVertexBufferBindings()
	{
		m_flags.clr(VltContextFlag::GpDirtyVertexBuffers);

		if (unlikely(!m_state.gp.state.il.bindingCount()))
			return;

		std::array<VkBuffer, MaxNumVertexBindings>     buffers;
		std::array<VkDeviceSize, MaxNumVertexBindings> offsets;

		// Set buffer handles and offsets for active bindings
		for (uint32_t i = 0; i < m_state.gp.state.il.bindingCount(); i++)
		{
			uint32_t binding = m_state.gp.state.ilBindings[i].binding();

			if (likely(m_state.vi.vertexBuffers[binding].defined()))
			{
				auto vbo = m_state.vi.vertexBuffers[binding].getDescriptor();

				buffers[i] = vbo.buffer.buffer;
				offsets[i] = vbo.buffer.offset;
			}
			else
			{
				buffers[i] = m_common->dummyResources().bufferHandle();
				offsets[i] = 0;
			}
		}

		// Vertex bindigs get remapped when compiling the
		// pipeline, so this actually does the right thing
		m_cmd->cmdBindVertexBuffers(
			0, m_state.gp.state.il.bindingCount(),
			buffers.data(), offsets.data());
	}

	
	void VltContext::updateDynamicState()
	{
		if (!m_gpActivePipeline)
			return;

		if (m_flags.test(VltContextFlag::GpDirtyViewport))
		{
			m_flags.clr(VltContextFlag::GpDirtyViewport);

			uint32_t viewportCount = m_state.gp.state.rs.viewportCount();
			m_cmd->cmdSetViewport(0, viewportCount, m_state.vp.viewports.data());
			m_cmd->cmdSetScissor(0, viewportCount, m_state.vp.scissorRects.data());
		}

		if (m_flags.all(VltContextFlag::GpDirtyBlendConstants,
						VltContextFlag::GpDynamicBlendConstants))
		{
			m_flags.clr(VltContextFlag::GpDirtyBlendConstants);
			m_cmd->cmdSetBlendConstants(&m_state.dyn.blendConstants.r);
		}

		if (m_flags.all(VltContextFlag::GpDirtyStencilRef,
						VltContextFlag::GpDynamicStencilRef))
		{
			m_flags.clr(VltContextFlag::GpDirtyStencilRef);

			m_cmd->cmdSetStencilReference(
				VK_STENCIL_FRONT_AND_BACK,
				m_state.dyn.stencilReference);
		}

		if (m_flags.all(VltContextFlag::GpDirtyDepthBias,
						VltContextFlag::GpDynamicDepthBias))
		{
			m_flags.clr(VltContextFlag::GpDirtyDepthBias);

			m_cmd->cmdSetDepthBias(
				m_state.dyn.depthBias.depthBiasConstant,
				m_state.dyn.depthBias.depthBiasClamp,
				m_state.dyn.depthBias.depthBiasSlope);
		}

		if (m_flags.all(VltContextFlag::GpDirtyDepthBounds,
						VltContextFlag::GpDynamicDepthBounds))
		{
			m_flags.clr(VltContextFlag::GpDirtyDepthBounds);

			m_cmd->cmdSetDepthBoundsTestEnable(
				m_state.gp.state.ds.enableDepthBoundsTest());

			m_cmd->cmdSetDepthBounds(
				m_state.dyn.depthBounds.minDepthBounds,
				m_state.dyn.depthBounds.maxDepthBounds);
		}
	}

	template <bool Indexed, bool Indirect>
	bool VltContext::commitGraphicsState()
	{
		if (m_flags.test(VltContextFlag::GpDirtyFramebuffer))
			this->updateFramebuffer();
			
		if (m_flags.test(VltContextFlag::GpDirtyPipeline))
			if (unlikely(!this->updateGraphicsPipeline()))
				return false;

		if (!m_flags.test(VltContextFlag::GpRenderPassBound))
			this->startRenderPass();

		if (m_flags.test(VltContextFlag::GpDirtyIndexBuffer) && Indexed)
			this->updateIndexBufferBinding();

		if (m_flags.test(VltContextFlag::GpDirtyVertexBuffers))
			this->updateVertexBufferBindings();

		if (m_flags.any(
				VltContextFlag::GpDirtyResources,
				VltContextFlag::GpDirtyDescriptorBinding))
			this->updateGraphicsShaderResources();

		if (m_flags.test(VltContextFlag::GpDirtyPipelineState))
			if (unlikely(!this->updateGraphicsPipelineState()))
				return false;

		if (m_flags.any(
				VltContextFlag::GpDirtyViewport,
				VltContextFlag::GpDirtyBlendConstants,
				VltContextFlag::GpDirtyStencilRef,
				VltContextFlag::GpDirtyDepthBias,
				VltContextFlag::GpDirtyDepthBounds))
			this->updateDynamicState();

		if (m_flags.test(VltContextFlag::DirtyPushConstants))
			this->updatePushConstants<VK_PIPELINE_BIND_POINT_GRAPHICS>();

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

	bool VltContext::updateGraphicsPipelineState()
	{
		// Set up vertex buffer strides for active bindings
		for (uint32_t i = 0; i < m_state.gp.state.il.bindingCount(); i++)
		{
			const uint32_t binding = m_state.gp.state.ilBindings[i].binding();
			m_state.gp.state.ilBindings[i].setStride(m_state.vi.vertexStrides[binding]);
		}

		// Check which dynamic states need to be active. States that
		// are not dynamic will be invalidated in the command buffer.
		m_flags.clr(VltContextFlag::GpDynamicBlendConstants,
					VltContextFlag::GpDynamicDepthBias,
					VltContextFlag::GpDynamicDepthBounds,
					VltContextFlag::GpDynamicStencilRef);

		m_flags.set(m_state.gp.state.useDynamicBlendConstants()
						? VltContextFlag::GpDynamicBlendConstants
						: VltContextFlag::GpDirtyBlendConstants);

		m_flags.set(m_state.gp.state.useDynamicDepthBias()
						? VltContextFlag::GpDynamicDepthBias
						: VltContextFlag::GpDirtyDepthBias);

		m_flags.set(m_state.gp.state.useDynamicDepthBounds()
						? VltContextFlag::GpDynamicDepthBounds
						: VltContextFlag::GpDirtyDepthBounds);

		m_flags.set(m_state.gp.state.useDynamicStencilRef()
						? VltContextFlag::GpDynamicStencilRef
						: VltContextFlag::GpDirtyStencilRef);

		// Retrieve and bind actual Vulkan pipeline handle
		m_gpActivePipeline = m_state.gp.pipeline->getPipelineHandle(
			m_state.gp.state);

		if (unlikely(!m_gpActivePipeline))
			return false;

		m_cmd->cmdBindPipeline(
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			m_gpActivePipeline);

		m_flags.clr(VltContextFlag::GpDirtyPipelineState);
		return true;
	}

	void VltContext::setViewports(
		uint32_t          viewportCount,
		const VkViewport* viewports,
		const VkRect2D*   scissorRects)
	{
		if (m_state.gp.state.rs.viewportCount() != viewportCount)
		{
			m_state.gp.state.rs.setViewportCount(viewportCount);
			m_flags.set(VltContextFlag::GpDirtyPipelineState);
		}

		for (uint32_t i = 0; i < viewportCount; i++)
		{
			m_state.vp.viewports[i]    = viewports[i];
			m_state.vp.scissorRects[i] = scissorRects[i];

			// Vulkan viewports are not allowed to have a width or
			// height of zero, so we fall back to a dummy viewport
			// and instead set an empty scissor rect, which is legal.
			if (viewports[i].width == 0.0f || viewports[i].height == 0.0f)
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

	void VltContext::setBlendConstants(
		VltBlendConstants blendConstants)
	{
		if (m_state.dyn.blendConstants != blendConstants)
		{
			m_state.dyn.blendConstants = blendConstants;
			m_flags.set(VltContextFlag::GpDirtyBlendConstants);
		}
	}

	void VltContext::setDepthBias(
		VltDepthBias depthBias)
	{
		if (m_state.dyn.depthBias != depthBias)
		{
			m_state.dyn.depthBias = depthBias;
			m_flags.set(VltContextFlag::GpDirtyDepthBias);
		}
	}

	void VltContext::setDepthBounds(VltDepthBounds depthBounds)
	{
		if (m_state.dyn.depthBounds != depthBounds)
		{
			m_state.dyn.depthBounds = depthBounds;
			m_flags.set(VltContextFlag::GpDirtyDepthBounds);
		}

		if (m_state.gp.state.ds.enableDepthBoundsTest() != depthBounds.enableDepthBounds)
		{
			m_state.gp.state.ds.setEnableDepthBoundsTest(depthBounds.enableDepthBounds);
			m_flags.set(VltContextFlag::GpDirtyPipelineState);
		}
	}

	void VltContext::setStencilReference(uint32_t reference)
	{
		if (m_state.dyn.stencilReference != reference)
		{
			m_state.dyn.stencilReference = reference;
			m_flags.set(VltContextFlag::GpDirtyStencilRef);
		}
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

	void VltContext::bindResourceView(
		uint32_t                 slot,
		const Rc<VltImageView>&  imageView,
		const Rc<VltBufferView>& bufferView)
	{
		m_rc[slot].imageView   = imageView;
		m_rc[slot].bufferView  = bufferView;
		m_rc[slot].bufferSlice = bufferView != nullptr
									 ? bufferView->slice()
									 : VltBufferSlice();
		m_flags.set(
			VltContextFlag::CpDirtyResources,
			VltContextFlag::GpDirtyResources);
	}

	void VltContext::bindResourceSampler(
		uint32_t              slot,
		const Rc<VltSampler>& sampler)
	{
		m_rc[slot].sampler = sampler;
	
		m_flags.set(
			VltContextFlag::CpDirtyResources,
			VltContextFlag::GpDirtyResources);
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

	void VltContext::setInputAssemblyState(
		const VltInputAssemblyState& ia)
	{
		m_state.gp.state.ia = VltIaInfo(
			ia.primitiveTopology,
			ia.primitiveRestart,
			ia.patchVertexCount);

		m_flags.set(VltContextFlag::GpDirtyPipelineState);
	}

	void VltContext::setInputLayout(
		uint32_t                  attributeCount,
		const VltVertexAttribute* attributes,
		uint32_t                  bindingCount,
		const VltVertexBinding*   bindings)
	{
		m_flags.set(
			VltContextFlag::GpDirtyPipelineState,
			VltContextFlag::GpDirtyVertexBuffers);

		for (uint32_t i = 0; i < attributeCount; i++)
		{
			m_state.gp.state.ilAttributes[i] = VltIlAttribute(
				attributes[i].location, attributes[i].binding,
				attributes[i].format, attributes[i].offset);
		}

		for (uint32_t i = attributeCount; i < m_state.gp.state.il.attributeCount(); i++)
			m_state.gp.state.ilAttributes[i] = VltIlAttribute();

		for (uint32_t i = 0; i < bindingCount; i++)
		{
			m_state.gp.state.ilBindings[i] = VltIlBinding(
				bindings[i].binding, 0, bindings[i].inputRate,
				bindings[i].fetchRate);
		}

		for (uint32_t i = bindingCount; i < m_state.gp.state.il.bindingCount(); i++)
			m_state.gp.state.ilBindings[i] = VltIlBinding();

		m_state.gp.state.il = VltIlInfo(attributeCount, bindingCount);
	}

	void VltContext::setRasterizerState(
		const VltRasterizerState& rs)
	{
		m_state.gp.state.rs = VltRsInfo(
			rs.depthClipEnable,
			rs.depthBiasEnable,
			rs.polygonMode,
			rs.cullMode,
			rs.frontFace,
			m_state.gp.state.rs.viewportCount(),
			rs.sampleCount);

		m_flags.set(VltContextFlag::GpDirtyPipelineState);
	}

	void VltContext::setMultisampleState(
		const VltMultisampleState& ms)
	{
		m_state.gp.state.ms = VltMsInfo(
			m_state.gp.state.ms.sampleCount(),
			ms.sampleMask,
			ms.enableAlphaToCoverage);

		m_flags.set(VltContextFlag::GpDirtyPipelineState);
	}

	void VltContext::setDepthStencilState(
		const VltDepthStencilState& ds)
	{
		m_state.gp.state.ds = VltDsInfo(
			ds.enableDepthTest,
			ds.enableDepthWrite,
			m_state.gp.state.ds.enableDepthBoundsTest(),
			ds.enableStencilTest,
			ds.depthCompareOp);

		m_state.gp.state.dsFront = VltDsStencilOp(ds.stencilOpFront);
		m_state.gp.state.dsBack  = VltDsStencilOp(ds.stencilOpBack);

		m_flags.set(VltContextFlag::GpDirtyPipelineState);
	}

	void VltContext::setLogicOpState(
		const VltLogicOpState& lo)
	{
		m_state.gp.state.cb = VltCbInfo(
			lo.enableLogicOp,
			lo.logicOp);

		m_flags.set(VltContextFlag::GpDirtyPipelineState);
	}

	void VltContext::setBlendMode(
		uint32_t attachment, const VltBlendMode& blendMode)
	{
		m_state.gp.state.cbBlend[attachment] = VltCbAttachmentBlend(
			blendMode.enableBlending,
			blendMode.colorSrcFactor,
			blendMode.colorDstFactor,
			blendMode.colorBlendOp,
			blendMode.alphaSrcFactor,
			blendMode.alphaDstFactor,
			blendMode.alphaBlendOp,
			blendMode.writeMask);

		m_flags.set(VltContextFlag::GpDirtyPipelineState);
	}

	void VltContext::clearRenderTarget(
		const Rc<VltImageView>& imageView,
		VkImageAspectFlags      clearAspects,
		VkClearValue            clearValue)
	{
		// Make sure the color components are ordered correctly
		if (clearAspects & VK_IMAGE_ASPECT_COLOR_BIT)
		{
			clearValue.color = vutil::swizzleClearColor(clearValue.color,
														vutil::invertComponentMapping(imageView->info().swizzle));
		}

		// Check whether the render target view is an attachment
		// of the current framebuffer and is included entirely.
		// If not, we need to create a temporary framebuffer.
		int32_t attachmentIndex = -1;

		if (m_state.cb.framebufferInfo.isFullSize(imageView))
			attachmentIndex = m_state.cb.framebufferInfo.findAttachment(imageView);

		if (attachmentIndex >= 0 && m_flags.test(VltContextFlag::GpRenderPassBound))
		{
			uint32_t colorIndex = std::max(0, m_state.cb.framebufferInfo.getColorAttachmentIndex(attachmentIndex));

			VkClearAttachment clearInfo;
			clearInfo.aspectMask      = clearAspects;
			clearInfo.colorAttachment = colorIndex;
			clearInfo.clearValue      = clearValue;

			VkClearRect clearRect;
			clearRect.rect.offset.x      = 0;
			clearRect.rect.offset.y      = 0;
			clearRect.rect.extent.width  = imageView->mipLevelExtent(0).width;
			clearRect.rect.extent.height = imageView->mipLevelExtent(0).height;
			clearRect.baseArrayLayer     = 0;
			clearRect.layerCount         = imageView->info().numLayers;

			m_cmd->cmdClearAttachments(1, &clearInfo, 1, &clearRect);
		}
		else
		{
			this->deferClear(imageView, clearAspects, clearValue);
		}
	}

	void VltContext::emitRenderTargetReadbackBarrier()
	{
		emitMemoryBarrier(VK_DEPENDENCY_BY_REGION_BIT,
						  VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
						  VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
						  VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT |
						  VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
						  VK_ACCESS_SHADER_READ_BIT);
	}

	void VltContext::updateBuffer(
		const Rc<VltBuffer>& buffer,
		VkDeviceSize         offset,
		VkDeviceSize         size,
		const void*          data)
	{
		
		this->spillRenderPass();

		VltBufferSliceHandle bufferSlice = buffer->getSliceHandle(offset, size);
		VltCmdType           cmdBuffer   = VltCmdType::ExecBuffer;

		if (m_execBarriers.isBufferDirty(bufferSlice, VltAccess::Write))
			m_execBarriers.recordCommands(m_cmd);

		// Vulkan specifies that small amounts of data (up to 64kB) can
		// be copied to a buffer directly if the size is a multiple of
		// four. Anything else must be copied through a staging buffer.
		// We'll limit the size to 4kB in order to keep command buffers
		// reasonably small, we do not know how much data apps may upload.
		if ((size <= 4096) && ((size & 0x3) == 0) && ((offset & 0x3) == 0))
		{
			m_cmd->cmdUpdateBuffer(
				cmdBuffer,
				bufferSlice.handle,
				bufferSlice.offset,
				bufferSlice.length,
				data);
		}
		else
		{
			auto stagingSlice  = m_staging.alloc(CACHE_LINE_SIZE, size);
			auto stagingHandle = stagingSlice.getSliceHandle();

			std::memcpy(stagingHandle.mapPtr, data, size);

			VkBufferCopy region;
			region.srcOffset = stagingHandle.offset;
			region.dstOffset = bufferSlice.offset;
			region.size      = size;

			m_cmd->cmdCopyBuffer(cmdBuffer,
								 stagingHandle.handle, bufferSlice.handle, 1, &region);

			m_cmd->trackResource<VltAccess::Read>(stagingSlice.buffer());
		}

		m_execBarriers.accessBuffer(
			bufferSlice,
			VK_PIPELINE_STAGE_TRANSFER_BIT,
			VK_ACCESS_TRANSFER_WRITE_BIT,
			buffer->info().stages,
			buffer->info().access);

		m_cmd->trackResource<VltAccess::Write>(buffer);
	}

	void VltContext::updateImage(
		const Rc<VltImage>&             image,
		const VkImageSubresourceLayers& subresources,
		VkOffset3D                      imageOffset,
		VkExtent3D                      imageExtent,
		const void*                     data,
		VkDeviceSize                    pitchPerRow,
		VkDeviceSize                    pitchPerLayer)
	{
		this->spillRenderPass();

		// Upload data through a staging buffer. Special care needs to
		// be taken when dealing with compressed image formats: Rather
		// than copying pixels, we'll be copying blocks of pixels.
		const VltFormatInfo* formatInfo = image->formatInfo();

		// Align image extent to a full block. This is necessary in
		// case the image size is not a multiple of the block size.
		VkExtent3D elementCount = vutil::computeBlockCount(
			imageExtent, formatInfo->blockSize);
		elementCount.depth *= subresources.layerCount;

		// Allocate staging buffer memory for the image data. The
		// pixels or blocks will be tightly packed within the buffer.
		auto stagingSlice  = m_staging.alloc(CACHE_LINE_SIZE,
											 formatInfo->elementSize * vutil::flattenImageExtent(elementCount));
		auto stagingHandle = stagingSlice.getSliceHandle();
		vutil::packImageData(stagingHandle.mapPtr, data,
							 elementCount, formatInfo->elementSize,
							 pitchPerRow, pitchPerLayer);

		// Prepare the image layout. If the given extent covers
		// the entire image, we may discard its previous contents.
		auto subresourceRange       = vutil::makeSubresourceRange(subresources);
		subresourceRange.aspectMask = formatInfo->aspectMask;

		if (m_execBarriers.isImageDirty(image, subresourceRange, VltAccess::Write))
			m_execBarriers.recordCommands(m_cmd);

		// Initialize the image if the entire subresource is covered
		VkImageLayout imageLayoutInitial  = image->info().layout;
		VkImageLayout imageLayoutTransfer = image->pickLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

		if (image->isFullSubresource(subresources, imageExtent))
			imageLayoutInitial = VK_IMAGE_LAYOUT_UNDEFINED;

		m_execAcquires.accessImage(
			image, subresourceRange,
			imageLayoutInitial, 0, 0,
			imageLayoutTransfer,
			VK_PIPELINE_STAGE_TRANSFER_BIT,
			VK_ACCESS_TRANSFER_WRITE_BIT);

		m_execAcquires.recordCommands(m_cmd);

		// Copy contents of the staging buffer into the image.
		// Since our source data is tightly packed, we do not
		// need to specify any strides.
		VkBufferImageCopy region;
		region.bufferOffset      = stagingHandle.offset;
		region.bufferRowLength   = 0;
		region.bufferImageHeight = 0;
		region.imageSubresource  = subresources;
		region.imageOffset       = imageOffset;
		region.imageExtent       = imageExtent;

		m_cmd->cmdCopyBufferToImage(VltCmdType::ExecBuffer,
									stagingHandle.handle, image->handle(),
									imageLayoutTransfer, 1, &region);

		// Transition image back into its optimal layout
		m_execBarriers.accessImage(
			image, subresourceRange,
			imageLayoutTransfer,
			VK_PIPELINE_STAGE_TRANSFER_BIT,
			VK_ACCESS_TRANSFER_WRITE_BIT,
			image->info().layout,
			image->info().stages,
			image->info().access);

		m_cmd->trackResource<VltAccess::Write>(image);
		m_cmd->trackResource<VltAccess::Read>(stagingSlice.buffer());
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

		uint32_t acquireQueueFamily = m_cmd->type() == VltQueueType::Graphics
										 ? m_device->queues().graphics.queueFamily
										 : m_device->queues().compute.queueFamily;

		m_transBarriers.releaseBuffer(
			m_initBarriers, bufferSlice,
			m_device->queues().transfer.queueFamily,
			VK_PIPELINE_STAGE_TRANSFER_BIT,
			VK_ACCESS_TRANSFER_WRITE_BIT,
			acquireQueueFamily,
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

		uint32_t acquireQueueFamily = m_cmd->type() == VltQueueType::Graphics
										  ? m_device->queues().graphics.queueFamily
										  : m_device->queues().compute.queueFamily;

		// Transfer ownership to graphics queue
		m_transBarriers.releaseImage(m_initBarriers,
									 image, vutil::makeSubresourceRange(subresources),
									 m_device->queues().transfer.queueFamily,
									 image->pickLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL),
									 VK_PIPELINE_STAGE_TRANSFER_BIT,
									 VK_ACCESS_TRANSFER_WRITE_BIT,
									 acquireQueueFamily,
									 image->info().layout,
									 image->info().stages,
									 image->info().access);

		m_cmd->trackResource<VltAccess::Write>(image);
		m_cmd->trackResource<VltAccess::Read>(stagingSlice.buffer());
	}

	void VltContext::downloadBuffer(
		const Rc<VltBuffer>& buffer,
		void*                data)
	{
		auto bufferSlice = buffer->getSliceHandle();

		auto stagingSlice  = m_staging.alloc(bufferSlice.length, CACHE_LINE_SIZE);
		auto stagingHandle = stagingSlice.getSliceHandle();

		VkBufferCopy region;
		region.srcOffset = bufferSlice.offset;
		region.dstOffset = stagingHandle.offset ;
		region.size      = bufferSlice.length;

		m_cmd->cmdCopyBuffer(VltCmdType::ExecBuffer,
							 bufferSlice.handle, stagingHandle.handle, 1, &region);

		flushCommandList();
		// Wait for copying done before memcpy
		// TODO:
		// This should could be batched.
		m_device->waitForIdle();

		std::memcpy(data, stagingHandle.mapPtr, bufferSlice.length);
	}

	void VltContext::initBuffer(
		const Rc<VltBuffer>& buffer)
	{
		auto slice = buffer->getSliceHandle();

		m_cmd->cmdFillBuffer(VltCmdType::InitBuffer,
							 slice.handle, slice.offset,
							 util::align(slice.length, 4), 0);

		m_initBarriers.accessBuffer(slice,
									VK_PIPELINE_STAGE_TRANSFER_BIT,
									VK_ACCESS_TRANSFER_WRITE_BIT,
									buffer->info().stages,
									buffer->info().access);

		m_cmd->trackResource<VltAccess::Write>(buffer);
	}

	void VltContext::initImage(
		const Rc<VltImage>&            image,
		const VkImageSubresourceRange& subresources,
		VkImageLayout                  initialLayout)
	{
		if (initialLayout == VK_IMAGE_LAYOUT_PREINITIALIZED)
		{
			m_initBarriers.accessImage(image, subresources,
									   initialLayout, 0, 0,
									   image->info().layout,
									   image->info().stages,
									   image->info().access);

			m_cmd->trackResource<VltAccess::None>(image);
		}
		else
		{
			VkImageLayout clearLayout = image->pickLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

			// transform image to clear layout
			m_execAcquires.accessImage(image, subresources,
									   initialLayout, 0, 0, clearLayout,
									   VK_PIPELINE_STAGE_TRANSFER_BIT,
									   VK_ACCESS_TRANSFER_WRITE_BIT);
			m_execAcquires.recordCommands(m_cmd);

			auto formatInfo = image->formatInfo();

			if (formatInfo->flags.any(VltFormatFlag::BlockCompressed, VltFormatFlag::MultiPlane))
			{
				LOG_FIXME("init compressed or multi plane image is not supported.");
			}
			else
			{
				if (subresources.aspectMask & (VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT))
				{
					VkClearDepthStencilValue value = {};

					m_cmd->cmdClearDepthStencilImage(image->handle(),
													 clearLayout, &value, 1, &subresources);
				}
				else
				{
					VkClearColorValue value = {};

					m_cmd->cmdClearColorImage(image->handle(),
											  clearLayout, &value, 1, &subresources);
				}
			}

			// transform image back to default layout
			m_execBarriers.accessImage(image, subresources,
									   clearLayout,
									   VK_PIPELINE_STAGE_TRANSFER_BIT,
									   VK_ACCESS_TRANSFER_WRITE_BIT,
									   image->info().layout,
									   image->info().stages,
									   image->info().access);

			m_cmd->trackResource<VltAccess::Write>(image);
		}
	}

	void VltContext::setBarrierControl(VltBarrierControlFlags control)
	{
		m_barrierControl = control;
	}

	void VltContext::signalGpuEvent(
		const Rc<VltGpuEvent>&  event,
		const VkDependencyInfo* dependencyInfo)
	{
		this->endRecording();

		VltGpuEventHandle handle = m_common->eventPool().allocEvent();

		m_cmd->cmdSetEvent2(handle.event, dependencyInfo);

		m_cmd->trackGpuEvent(event->reset(handle));
		m_cmd->trackResource<VltAccess::None>(event);
	}

	void VltContext::signal(
		const Rc<util::sync::Signal>& signal,
		uint64_t                      value)
	{
		m_cmd->queueSignal(signal, value);
	}

	void VltContext::signalSemaphore(const VltSemaphoreSubmission& submission)
	{
		m_cmd->signalSemaphore(submission);
	}

	void VltContext::waitSemaphore(const VltSemaphoreSubmission& submission)
	{
		m_cmd->waitSemaphore(submission);
	}

	void VltContext::beginDebugLabel(VkDebugUtilsLabelEXT* label)
	{
		if (!m_device->instance()->extensions().extDebugUtils)
			return;

		m_cmd->cmdBeginDebugUtilsLabel(label);
	}

	void VltContext::endDebugLabel()
	{
		if (!m_device->instance()->extensions().extDebugUtils)
			return;

		m_cmd->cmdEndDebugUtilsLabel();
	}

	void VltContext::insertDebugLabel(VkDebugUtilsLabelEXT* label)
	{
		if (!m_device->instance()->extensions().extDebugUtils)
			return;

		m_cmd->cmdInsertDebugUtilsLabel(label);
	}

	void VltContext::startRenderPass()
	{
		if (!m_flags.test(VltContextFlag::GpRenderPassBound))
		{
			this->applyRenderTargetLoadLayouts();
			this->flushClears(true);

			m_flags.set(
				VltContextFlag::GpRenderPassBound,
				VltContextFlag::GpDirtyPipeline,
				VltContextFlag::GpDirtyPipelineState,
				VltContextFlag::GpDirtyVertexBuffers,
				VltContextFlag::GpDirtyIndexBuffer,
				VltContextFlag::GpDirtyXfbBuffers,
				VltContextFlag::GpDirtyBlendConstants,
				VltContextFlag::GpDirtyStencilRef,
				VltContextFlag::GpDirtyViewport,
				VltContextFlag::GpDirtyDepthBias,
				VltContextFlag::GpDirtyDepthBounds,
				VltContextFlag::DirtyPushConstants);

			this->renderPassBindFramebuffer(
				m_state.cb.framebufferInfo,
				m_state.cb.renderPassOps);

			// Track the final layout of each render target
			this->applyRenderTargetStoreLayouts();

			// Don't discard image contents if we have
			// to spill the current render pass
			this->resetRenderPassOps(
				m_state.cb.renderTargets,
				m_state.cb.renderPassOps);
		}
	}

	void VltContext::spillRenderPass()
	{
		if (m_flags.test(VltContextFlag::GpRenderPassBound))
		{
			m_flags.clr(VltContextFlag::GpRenderPassBound);

			this->renderPassUnbindFramebuffer();
			this->transitionRenderTargetLayouts(m_execBarriers);

			m_execBarriers.recordCommands(m_cmd);
		}
		else
		{
			// Execute deferred clears if necessary
			this->flushClears(false);
		}
	}

	void VltContext::renderPassEmitInitBarriers(
		const VltFramebufferInfo& framebufferInfo,
		const VltRenderPassOps&   ops)
	{
		// If any of the involved images are dirty, emit all pending barriers now.
		// Otherwise, skip this step so that we can more efficiently batch barriers.
		for (uint32_t i = 0; i < framebufferInfo.numAttachments(); i++)
		{
			const auto& attachment = framebufferInfo.getAttachment(i);

			if (m_execBarriers.isImageDirty(
					attachment.view->image(),
					attachment.view->imageSubresources(),
					VltAccess::Write))
			{
				m_execBarriers.recordCommands(m_cmd);
				break;
			}
		}

		// Transition all images to the render layout as necessary
		const auto& depthAttachment = framebufferInfo.getDepthTarget();

		if (depthAttachment.layout != ops.depthOps.loadLayout && depthAttachment.view != nullptr)
		{
			VkImageAspectFlags depthAspects = depthAttachment.view->info().aspect;

			VkPipelineStageFlags depthStages =
				VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT |
				VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
			VkAccessFlags depthAccess = 0;

			if (((depthAspects & VK_IMAGE_ASPECT_DEPTH_BIT) && ops.depthOps.loadOpD == VK_ATTACHMENT_LOAD_OP_LOAD) || 
				((depthAspects & VK_IMAGE_ASPECT_STENCIL_BIT) && ops.depthOps.loadOpS == VK_ATTACHMENT_LOAD_OP_LOAD))
				depthAccess |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;

			if (((depthAspects & VK_IMAGE_ASPECT_DEPTH_BIT) && ops.depthOps.loadOpD != VK_ATTACHMENT_LOAD_OP_LOAD) || 
				((depthAspects & VK_IMAGE_ASPECT_STENCIL_BIT) && ops.depthOps.loadOpS != VK_ATTACHMENT_LOAD_OP_LOAD) || 
				(depthAttachment.layout != VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL))
				depthAccess |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

			if (depthAttachment.layout != VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
			{
				depthStages |= m_device->getShaderPipelineStages();
				depthAccess |= VK_ACCESS_SHADER_READ_BIT;
			}

			m_execBarriers.accessImage(
				depthAttachment.view->image(),
				depthAttachment.view->imageSubresources(),
				ops.depthOps.loadLayout,
				depthStages, 0,
				depthAttachment.layout,
				depthStages, depthAccess);
		}

		for (uint32_t i = 0; i < MaxNumRenderTargets; i++)
		{
			const auto& colorAttachment = framebufferInfo.getColorTarget(i);

			if (colorAttachment.layout != ops.colorOps[i].loadLayout && colorAttachment.view != nullptr)
			{
				VkAccessFlags colorAccess = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

				if (ops.colorOps[i].loadOp == VK_ATTACHMENT_LOAD_OP_LOAD)
					colorAccess |= VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;

				m_execBarriers.accessImage(
					colorAttachment.view->image(),
					colorAttachment.view->imageSubresources(),
					ops.colorOps[i].loadLayout,
					VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, 0,
					colorAttachment.layout,
					VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
					colorAccess);
			}
		}

		// Unconditionally emit barriers here. We need to do this
		// even if there are no layout transitions, since we don't
		// track resource usage during render passes.
		m_execBarriers.recordCommands(m_cmd);
	}

	void VltContext::renderPassEmitPostBarriers(
		const VltFramebufferInfo& framebufferInfo,
		const VltRenderPassOps&   ops)
	{
		const auto& depthAttachment = framebufferInfo.getDepthTarget();

		if (depthAttachment.view != nullptr)
		{
			if (depthAttachment.layout != ops.depthOps.storeLayout)
			{
				m_execBarriers.accessImage(
					depthAttachment.view->image(),
					depthAttachment.view->imageSubresources(),
					depthAttachment.layout,
					VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT |
						VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
					VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT |
						VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
					ops.depthOps.storeLayout,
					depthAttachment.view->imageInfo().stages,
					depthAttachment.view->imageInfo().access);
			}
			else
			{
				VkAccessFlags srcAccess = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;

				if (depthAttachment.layout != VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL)
					srcAccess |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

				m_execBarriers.accessMemory(
					VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT |
						VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
					srcAccess,
					depthAttachment.view->imageInfo().stages,
					depthAttachment.view->imageInfo().access);
			}
		}

		for (uint32_t i = 0; i < MaxNumRenderTargets; i++)
		{
			const auto& colorAttachment = framebufferInfo.getColorTarget(i);

			if (colorAttachment.view != nullptr)
			{
				if (colorAttachment.layout != ops.colorOps[i].storeLayout)
				{
					m_execBarriers.accessImage(
						colorAttachment.view->image(),
						colorAttachment.view->imageSubresources(),
						colorAttachment.layout,
						VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
						VK_ACCESS_COLOR_ATTACHMENT_READ_BIT |
							VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
						ops.colorOps[i].storeLayout,
						colorAttachment.view->imageInfo().stages,
						colorAttachment.view->imageInfo().access);
				}
				else
				{
					m_execBarriers.accessMemory(
						VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
						VK_ACCESS_COLOR_ATTACHMENT_READ_BIT |
							VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
						colorAttachment.view->imageInfo().stages,
						colorAttachment.view->imageInfo().access);
				}
			}
		}

		// Do not flush barriers here. This is intended since
		// we pre-record them when binding the framebuffer.
	}

	void VltContext::renderPassBindFramebuffer(
		const VltFramebufferInfo& framebufferInfo,
		const VltRenderPassOps&   ops)
	{
		const VltFramebufferSize fbSize = framebufferInfo.size();

		this->renderPassEmitInitBarriers(framebufferInfo, ops);
		this->renderPassEmitPostBarriers(framebufferInfo, ops);

		uint32_t colorInfoCount = 0;

		std::array<VkRenderingAttachmentInfoKHR, MaxNumRenderTargets> colorInfos;

		for (uint32_t i = 0; i < MaxNumRenderTargets; i++)
		{
			const auto& colorTarget = framebufferInfo.getColorTarget(i);
			colorInfos[i]           = { VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO_KHR };

			if (colorTarget.view != nullptr)
			{
				colorInfos[i].imageView   = colorTarget.view->handle();
				colorInfos[i].imageLayout = colorTarget.layout;
				colorInfos[i].loadOp      = ops.colorOps[i].loadOp;
				colorInfos[i].storeOp     = VK_ATTACHMENT_STORE_OP_STORE;

				if (ops.colorOps[i].loadOp == VK_ATTACHMENT_LOAD_OP_CLEAR)
					colorInfos[i].clearValue.color = ops.colorOps[i].clearValue;

				colorInfoCount = i + 1;
			}
		}

		VkRenderingAttachmentInfoKHR depthInfo           = { VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO_KHR };
		VkImageAspectFlags           depthStencilAspects = 0;

		if (framebufferInfo.getDepthTarget().view != nullptr)
		{
			const auto& depthTarget = framebufferInfo.getDepthTarget();
			depthStencilAspects     = depthTarget.view->info().aspect;
			depthInfo.imageView     = depthTarget.view->handle();
			depthInfo.imageLayout   = depthTarget.layout;
			depthInfo.loadOp        = ops.depthOps.loadOpD;
			depthInfo.storeOp       = VK_ATTACHMENT_STORE_OP_STORE;

			if (ops.depthOps.loadOpD == VK_ATTACHMENT_LOAD_OP_CLEAR)
				depthInfo.clearValue.depthStencil.depth = ops.depthOps.clearValue.depth;
		}

		VkRenderingAttachmentInfoKHR stencilInfo = depthInfo;

		if (framebufferInfo.getDepthTarget().view != nullptr)
		{
			stencilInfo.loadOp  = ops.depthOps.loadOpS;
			stencilInfo.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

			if (ops.depthOps.loadOpS == VK_ATTACHMENT_LOAD_OP_CLEAR)
				stencilInfo.clearValue.depthStencil.stencil = ops.depthOps.clearValue.stencil;
		}

		VkRenderingInfoKHR renderingInfo = { VK_STRUCTURE_TYPE_RENDERING_INFO_KHR };
		renderingInfo.renderArea.offset  = VkOffset2D{ 0, 0 };
		renderingInfo.renderArea.extent  = VkExtent2D{ fbSize.width, fbSize.height };
		renderingInfo.layerCount         = fbSize.layers;

		if (colorInfoCount)
		{
			renderingInfo.colorAttachmentCount = colorInfoCount;
			renderingInfo.pColorAttachments    = colorInfos.data();
		}

		if (depthStencilAspects & VK_IMAGE_ASPECT_DEPTH_BIT)
			renderingInfo.pDepthAttachment = &depthInfo;

		if (depthStencilAspects & VK_IMAGE_ASPECT_STENCIL_BIT)
			renderingInfo.pStencilAttachment = &stencilInfo;

		m_cmd->cmdBeginRendering(&renderingInfo);

		for (uint32_t i = 0; i < framebufferInfo.numAttachments(); i++)
		{
			m_cmd->trackResource<VltAccess::None>(framebufferInfo.getAttachment(i).view);
			m_cmd->trackResource<VltAccess::Write>(framebufferInfo.getAttachment(i).view->image());
		}
	}

	void VltContext::renderPassUnbindFramebuffer()
	{
		m_cmd->cmdEndRendering();

		// If there are pending layout transitions, execute them immediately
		// since the backend expects images to be in the store layout after
		// a render pass instance. This is expected to be rare.
		if (m_execBarriers.hasResourceBarriers())
			m_execBarriers.recordCommands(m_cmd);
	}

	bool VltContext::commitComputeState()
	{
		if (m_flags.test(VltContextFlag::GpRenderPassBound))
			this->spillRenderPass();

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

		if (m_flags.test(VltContextFlag::DirtyPushConstants))
		{
			this->updatePushConstants<VK_PIPELINE_BIND_POINT_COMPUTE>();
		}	

		return true;
	}

	bool VltContext::updateComputePipeline()
	{
		m_state.cp.pipeline = lookupComputePipeline(m_state.cp.shaders);

		if (unlikely(m_state.cp.pipeline == nullptr))
			return false;

		if (m_state.cp.pipeline->layout()->pushConstRange().size)
			m_flags.set(VltContextFlag::DirtyPushConstants);

		m_flags.clr(VltContextFlag::CpDirtyPipeline);
		return true;
	}

	template <VkPipelineBindPoint BindPoint>
	void VltContext::updateShaderDescriptorSetBinding(
		VkDescriptorSet set, const VltPipelineLayout* layout)
	{
		if (set)
		{
			std::array<uint32_t, MaxNumActiveBindings> offsets;

			for (uint32_t i = 0; i < layout->dynamicBindingCount(); i++)
			{
				const auto& binding = layout->dynamicBinding(i);
				const auto& res     = m_rc[binding.slot];

				offsets[i] = res.bufferSlice.defined()
								 ? res.bufferSlice.getDynamicOffset()
								 : 0;
			}

			m_cmd->cmdBindDescriptorSet(BindPoint,
										layout->pipelineLayout(), set,
										layout->dynamicBindingCount(),
										offsets.data());
		}
	}

	template <VkPipelineBindPoint BindPoint>
	void sce::vlt::VltContext::updatePushConstants()
	{
		m_flags.clr(VltContextFlag::DirtyPushConstants);

		auto layout = BindPoint == VK_PIPELINE_BIND_POINT_GRAPHICS
						  ? m_state.gp.pipeline->layout()
						  : m_state.cp.pipeline->layout();

		if (!layout)
			return;

		VkPushConstantRange pushConstRange = layout->pushConstRange();
		if (!pushConstRange.size)
			return;

		m_cmd->cmdPushConstants(
			layout->pipelineLayout(),
			pushConstRange.stageFlags,
			pushConstRange.offset,
			pushConstRange.size,
			&m_state.pc.data[pushConstRange.offset]);
	}

	template <VkPipelineBindPoint BindPoint>
	void VltContext::updateShaderResources(
		const VltPipelineLayout* layout)
	{
		std::array<VltDescriptorInfo, MaxNumActiveBindings> descriptors;

		// Assume that all bindings are active as a fast path
		VltBindingMask bindMask;
		bindMask.setFirst(layout->bindingCount());

		for (uint32_t i = 0; i < layout->bindingCount(); i++)
		{
			const auto& binding = layout->binding(i);
			const auto& res     = m_rc[binding.slot];

			switch (binding.type)
			{
			case VK_DESCRIPTOR_TYPE_SAMPLER:
				if (res.sampler != nullptr)
				{
					descriptors[i].image.sampler     = res.sampler->handle();
					descriptors[i].image.imageView   = VK_NULL_HANDLE;
					descriptors[i].image.imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				}
				else
				{
					descriptors[i].image = m_common->dummyResources().samplerDescriptor();
				}
				break;

			case VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE:
				if (res.imageView != nullptr && res.imageView->handle(binding.view) != VK_NULL_HANDLE)
				{
					descriptors[i].image.sampler     = VK_NULL_HANDLE;
					descriptors[i].image.imageView   = res.imageView->handle(binding.view);
					descriptors[i].image.imageLayout = res.imageView->imageInfo().layout;
				}
				else
				{
					bindMask.clr(i);
					descriptors[i].image = m_common->dummyResources().imageViewDescriptor(binding.view, true);
				}
				break;

			case VK_DESCRIPTOR_TYPE_STORAGE_IMAGE:
				if (res.imageView != nullptr && res.imageView->handle(binding.view) != VK_NULL_HANDLE)
				{
					descriptors[i].image.sampler     = VK_NULL_HANDLE;
					descriptors[i].image.imageView   = res.imageView->handle(binding.view);
					descriptors[i].image.imageLayout = res.imageView->imageInfo().layout;
				}
				else
				{
					bindMask.clr(i);
					descriptors[i].image = m_common->dummyResources().imageViewDescriptor(binding.view, false);
				}
				break;

			case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
				if (res.sampler != nullptr && res.imageView != nullptr && res.imageView->handle(binding.view) != VK_NULL_HANDLE)
				{
					descriptors[i].image.sampler     = res.sampler->handle();
					descriptors[i].image.imageView   = res.imageView->handle(binding.view);
					descriptors[i].image.imageLayout = res.imageView->imageInfo().layout;
				}
				else
				{
					bindMask.clr(i);
					descriptors[i].image = m_common->dummyResources().imageSamplerDescriptor(binding.view);
				}
				break;

			case VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER:
				if (res.bufferView != nullptr)
				{
					res.bufferView->updateView();
					descriptors[i].texelBuffer = res.bufferView->handle();
				}
				else
				{
					bindMask.clr(i);
					descriptors[i].texelBuffer = m_common->dummyResources().bufferViewDescriptor();
				}
				break;

			case VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER:
				if (res.bufferView != nullptr)
				{
					res.bufferView->updateView();
					descriptors[i].texelBuffer = res.bufferView->handle();
				}
				else
				{
					bindMask.clr(i);
					descriptors[i].texelBuffer = m_common->dummyResources().bufferViewDescriptor();
				}
				break;

			case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
				if (res.bufferSlice.defined())
				{
					descriptors[i] = res.bufferSlice.getDescriptor();
				}
				else
				{
					descriptors[i].buffer = m_common->dummyResources().bufferDescriptor();
				}
				break;

			case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER:
				if (res.bufferSlice.defined())
				{
					descriptors[i] = res.bufferSlice.getDescriptor();
				}
				else
				{
					bindMask.clr(i);
					descriptors[i].buffer = m_common->dummyResources().bufferDescriptor();
				}
				break;

			case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC:
				if (res.bufferSlice.defined())
				{
					descriptors[i]               = res.bufferSlice.getDescriptor();
					descriptors[i].buffer.offset = 0;
				}
				else
				{
					descriptors[i].buffer = m_common->dummyResources().bufferDescriptor();
				}
				break;

			default:
				Logger::err(util::str::formatex("VltContext: Unhandled descriptor type: ", binding.type));
			}
		}

		// Allocate and update descriptor set
		auto& set = BindPoint == VK_PIPELINE_BIND_POINT_GRAPHICS
						? m_gpSet
						: m_cpSet;

		if (layout->bindingCount())
		{
			set = allocateDescriptorSet(layout->descriptorSetLayout());

			m_cmd->updateDescriptorSetWithTemplate(set,
												   layout->descriptorTemplate(), descriptors.data());
		}
		else
		{
			set = VK_NULL_HANDLE;
		}

		// Select the active binding mask to update
		auto& refMask = BindPoint == VK_PIPELINE_BIND_POINT_GRAPHICS
							? m_state.gp.state.bsBindingMask
							: m_state.cp.state.bsBindingMask;

		// If some resources are not bound, we may need to
		// update spec constants and rebind the pipeline
		if (refMask != bindMask)
		{
			refMask = bindMask;

			m_flags.set(BindPoint == VK_PIPELINE_BIND_POINT_GRAPHICS
							? VltContextFlag::GpDirtyPipelineState
							: VltContextFlag::CpDirtyPipelineState);
		}
	}

	VkDescriptorSet VltContext::allocateDescriptorSet(VkDescriptorSetLayout layout)
	{
		if (m_descPool == nullptr)
			m_descPool = m_device->createDescriptorPool();

		VkDescriptorSet set = m_descPool->alloc(layout);

		if (set == VK_NULL_HANDLE)
		{
			m_cmd->trackDescriptorPool(std::move(m_descPool));

			m_descPool = m_device->createDescriptorPool();
			set        = m_descPool->alloc(layout);
		}

		return set;
	}

	bool VltContext::updateComputePipelineState()
	{
		m_cpActivePipeline = m_state.cp.pipeline->getPipelineHandle(m_state.cp.state);

		if (unlikely(!m_cpActivePipeline))
			return false;

		m_cmd->cmdBindPipeline(
			VK_PIPELINE_BIND_POINT_COMPUTE,
			m_cpActivePipeline);

		m_flags.clr(VltContextFlag::CpDirtyPipelineState);
		return true;
	}

	void VltContext::updateComputeShaderResources()
	{
		if ((m_flags.test(VltContextFlag::CpDirtyResources)) || 
			(m_state.cp.pipeline->layout()->hasStaticBufferBindings()))
			this->updateShaderResources<VK_PIPELINE_BIND_POINT_COMPUTE>(m_state.cp.pipeline->layout());

		this->updateShaderDescriptorSetBinding<VK_PIPELINE_BIND_POINT_COMPUTE>(
			m_cpSet, m_state.cp.pipeline->layout());

		m_flags.clr(VltContextFlag::CpDirtyResources,
					VltContextFlag::CpDirtyDescriptorBinding);
	}

	void VltContext::updateGraphicsShaderResources()
	{
		if ((m_flags.test(VltContextFlag::GpDirtyResources)) || 
			(m_state.gp.pipeline->layout()->hasStaticBufferBindings()))
			this->updateShaderResources<VK_PIPELINE_BIND_POINT_GRAPHICS>(m_state.gp.pipeline->layout());

		this->updateShaderDescriptorSetBinding<VK_PIPELINE_BIND_POINT_GRAPHICS>(
			m_gpSet, m_state.gp.pipeline->layout());

		m_flags.clr(VltContextFlag::GpDirtyResources,
					VltContextFlag::GpDirtyDescriptorBinding);
	}

	VltGraphicsPipeline* VltContext::lookupGraphicsPipeline(
		const VltGraphicsPipelineShaders& shaders)
	{
		auto idx = shaders.hash() % m_gpLookupCache.size();

		if (unlikely(!m_gpLookupCache[idx] || !shaders.eq(m_gpLookupCache[idx]->shaders())))
			m_gpLookupCache[idx] = m_common->pipelineManager().createGraphicsPipeline(shaders);

		return m_gpLookupCache[idx];
	}

	VltComputePipeline* VltContext::lookupComputePipeline(
		const VltComputePipelineShaders& shaders)
	{
		auto idx = shaders.hash() % m_cpLookupCache.size();

		if (unlikely(!m_cpLookupCache[idx] || !shaders.eq(m_cpLookupCache[idx]->shaders())))
			m_cpLookupCache[idx] = m_common->pipelineManager().createComputePipeline(shaders);

		return m_cpLookupCache[idx];
	}

	void VltContext::commitComputePrevBarriers()
	{
		auto layout = m_state.cp.pipeline->layout();

		bool requiresBarrier = false;

		for (uint32_t i = 0; i < layout->bindingCount() && !requiresBarrier; i++)
		{
			if (m_state.cp.state.bsBindingMask.test(i))
			{
				const VltDescriptorSlot      binding = layout->binding(i);
				const VltShaderResourceSlot& slot    = m_rc[binding.slot];

				VltAccessFlags dstAccess = VltAccess::Read;
				VltAccessFlags srcAccess = 0;

				switch (binding.type)
				{
				case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER:
					if (binding.access & VK_ACCESS_SHADER_WRITE_BIT)
						dstAccess.set(VltAccess::Write);
					/* fall through */

				case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
				case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC:
					srcAccess = m_execBarriers.getBufferAccess(
						slot.bufferSlice.getSliceHandle());
					break;

				case VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER:
					if (binding.access & VK_ACCESS_SHADER_WRITE_BIT)
						dstAccess.set(VltAccess::Write);
					/* fall through */

				case VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER:
					srcAccess = m_execBarriers.getBufferAccess(
						slot.bufferView->getSliceHandle());
					break;

				case VK_DESCRIPTOR_TYPE_STORAGE_IMAGE:
					if (binding.access & VK_ACCESS_SHADER_WRITE_BIT)
						dstAccess.set(VltAccess::Write);
					/* fall through */

				case VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE:
				case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
					srcAccess = m_execBarriers.getImageAccess(
						slot.imageView->image(),
						slot.imageView->imageSubresources());
					break;

				default:
					/* nothing to do */;
				}

				if (srcAccess == 0)
				{
					continue;
				}

				// Skip write-after-write barriers if explicitly requested
				if ((m_barrierControl.test(VltBarrierControl::IgnoreWriteAfterWrite)) &&
					(m_execBarriers.getSrcStages() == VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT) &&
					(srcAccess.test(VltAccess::Write)) &&
					(dstAccess.test(VltAccess::Write)))
				{
					continue;
				}

				requiresBarrier = (srcAccess | dstAccess).test(VltAccess::Write);
			}
		}

		if (requiresBarrier)
			m_execBarriers.recordCommands(m_cmd);
	}

	void VltContext::commitComputePostBarriers()
	{
		auto layout = m_state.cp.pipeline->layout();

		for (uint32_t i = 0; i < layout->bindingCount(); i++)
		{
			if (m_state.cp.state.bsBindingMask.test(i))
			{
				const VltDescriptorSlot      binding = layout->binding(i);
				const VltShaderResourceSlot& slot    = m_rc[binding.slot];

				VkPipelineStageFlags stages = VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
				VkAccessFlags        access = VK_ACCESS_SHADER_READ_BIT;

				switch (binding.type)
				{
				case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER:
					if (binding.access & VK_ACCESS_SHADER_WRITE_BIT)
						access |= VK_ACCESS_SHADER_WRITE_BIT;
					/* fall through */

				case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
				case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC:
					m_execBarriers.accessBuffer(
						slot.bufferSlice.getSliceHandle(),
						stages, access,
						slot.bufferSlice.bufferInfo().stages,
						slot.bufferSlice.bufferInfo().access);
					break;

				case VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER:
					if (binding.access & VK_ACCESS_SHADER_WRITE_BIT)
						access |= VK_ACCESS_SHADER_WRITE_BIT;
					/* fall through */

				case VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER:
					m_execBarriers.accessBuffer(
						slot.bufferView->getSliceHandle(),
						stages, access,
						slot.bufferView->bufferInfo().stages,
						slot.bufferView->bufferInfo().access);
					break;

				case VK_DESCRIPTOR_TYPE_STORAGE_IMAGE:
					if (binding.access & VK_ACCESS_SHADER_WRITE_BIT)
						access |= VK_ACCESS_SHADER_WRITE_BIT;
					/* fall through */

				case VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE:
				case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
					m_execBarriers.accessImage(
						slot.imageView->image(),
						slot.imageView->imageSubresources(),
						slot.imageView->imageInfo().layout,
						stages, access,
						slot.imageView->imageInfo().layout,
						slot.imageView->imageInfo().stages,
						slot.imageView->imageInfo().access);
					break;

				default:
					/* nothing to do */;
				}
			}
		}
	}

	void VltContext::updateFramebuffer()
	{
		if (m_flags.test(VltContextFlag::GpDirtyFramebuffer))
		{
			m_flags.clr(VltContextFlag::GpDirtyFramebuffer);

			this->spillRenderPass();

			VltFramebufferInfo fbInfo = makeFramebufferInfo(m_state.cb.renderTargets);
			this->updateRenderTargetLayouts(fbInfo, m_state.cb.framebufferInfo);

			// Update relevant graphics pipeline state
			m_state.gp.state.ms.setSampleCount(fbInfo.getSampleCount());
			m_state.gp.state.rt        = fbInfo.getRtInfo();
			m_state.cb.framebufferInfo = fbInfo;

			for (uint32_t i = 0; i < MaxNumRenderTargets; i++)
			{
				const Rc<VltImageView>& attachment = fbInfo.getColorTarget(i).view;

				VkComponentMapping mapping = attachment != nullptr
												 ? vutil::invertComponentMapping(attachment->info().swizzle)
												 : VkComponentMapping();

				m_state.gp.state.cbSwizzle[i] = VltCbAttachmentSwizzle(mapping);
			}

			m_flags.set(VltContextFlag::GpDirtyPipelineState);
		}
	}

	void VltContext::resetRenderPassOps(
		const VltRenderTargets& renderTargets,
		VltRenderPassOps&       renderPassOps)
	{
		if (renderTargets.depth.view != nullptr)
		{
			renderPassOps.depthOps = VltDepthAttachmentOps{
				VK_ATTACHMENT_LOAD_OP_LOAD, VK_ATTACHMENT_LOAD_OP_LOAD,
				renderTargets.depth.layout, renderTargets.depth.layout
			};
		}
		else
		{
			renderPassOps.depthOps = VltDepthAttachmentOps{};
		}

		for (uint32_t i = 0; i < MaxNumRenderTargets; i++)
		{
			if (renderTargets.color[i].view != nullptr)
			{
				renderPassOps.colorOps[i] = VltColorAttachmentOps{
					VK_ATTACHMENT_LOAD_OP_LOAD,
					renderTargets.color[i].layout,
					renderTargets.color[i].layout
				};
			}
			else
			{
				renderPassOps.colorOps[i] = VltColorAttachmentOps{};
			}
		}
	}

	VltFramebufferInfo VltContext::makeFramebufferInfo(
		const VltRenderTargets& renderTargets)
	{
		return VltFramebufferInfo(renderTargets, m_device->getDefaultFramebufferSize());
	}

	void VltContext::updateRenderTargetLayouts(
		const VltFramebufferInfo& newFb,
		const VltFramebufferInfo& oldFb)
	{
		VltRenderTargetLayouts layouts = {};

		for (uint32_t i = 0; i < MaxNumRenderTargets; i++)
		{
			if (newFb.getColorTarget(i).view != nullptr)
				layouts.color[i] = newFb.getColorTarget(i).view->imageInfo().layout;
		}

		if (newFb.getDepthTarget().view != nullptr)
			layouts.depth = newFb.getDepthTarget().view->imageInfo().layout;

		// Check whether any of the previous attachments have been moved
		// around or been rebound with a different view. This may help
		// reduce the number of image layout transitions between passes.
		for (uint32_t i = 0; i < MaxNumRenderTargets; i++)
		{
			const VltAttachment& oldAttachment = oldFb.getColorTarget(i);

			if (oldAttachment.view != nullptr)
			{
				bool found = false;

				for (uint32_t j = 0; j < MaxNumRenderTargets && !found; j++)
				{
					const VltAttachment& newAttachment = newFb.getColorTarget(j);

					found = newAttachment.view == oldAttachment.view ||
							(newAttachment.view != nullptr &&
							 newAttachment.view->image() == oldAttachment.view->image() &&
							 newAttachment.view->subresources() == oldAttachment.view->subresources());

					if (found)
						layouts.color[j] = m_rtLayouts.color[i];
				}
			}
		}

		const VltAttachment& oldAttachment = oldFb.getDepthTarget();

		if (oldAttachment.view != nullptr)
		{
			const VltAttachment& newAttachment = newFb.getDepthTarget();

			bool found = newAttachment.view == oldAttachment.view ||
						 (newAttachment.view != nullptr &&
						  newAttachment.view->image() == oldAttachment.view->image() &&
						  newAttachment.view->subresources() == oldAttachment.view->subresources());

			if (found)
				layouts.depth = m_rtLayouts.depth;
		}

		m_rtLayouts = layouts;
	}

	void VltContext::applyRenderTargetLoadLayouts()
	{
		for (uint32_t i = 0; i < MaxNumRenderTargets; i++)
			m_state.cb.renderPassOps.colorOps[i].loadLayout = m_rtLayouts.color[i];

		m_state.cb.renderPassOps.depthOps.loadLayout = m_rtLayouts.depth;
	}

	void VltContext::applyRenderTargetStoreLayouts()
	{
		for (uint32_t i = 0; i < MaxNumRenderTargets; i++)
			m_rtLayouts.color[i] = m_state.cb.renderPassOps.colorOps[i].storeLayout;

		m_rtLayouts.depth = m_state.cb.renderPassOps.depthOps.storeLayout;
	}

	void VltContext::transitionRenderTargetLayouts(VltBarrierSet& barriers)
	{
		for (uint32_t i = 0; i < MaxNumRenderTargets; i++)
		{
			const VltAttachment& color = m_state.cb.framebufferInfo.getColorTarget(i);

			if (color.view != nullptr)
			{
				this->transitionColorAttachment(barriers, color, m_rtLayouts.color[i]);
				m_rtLayouts.color[i] = color.view->imageInfo().layout;
			}
		}

		const VltAttachment& depth = m_state.cb.framebufferInfo.getDepthTarget();

		if (depth.view != nullptr)
		{
			this->transitionDepthAttachment(barriers, depth, m_rtLayouts.depth);
			m_rtLayouts.depth = depth.view->imageInfo().layout;
		}
	}

	void VltContext::transitionColorAttachment(VltBarrierSet&       barriers,
											   const VltAttachment& attachment,
											   VkImageLayout        oldLayout)
	{
		if (oldLayout != attachment.view->imageInfo().layout)
		{
			barriers.accessImage(
				attachment.view->image(),
				attachment.view->imageSubresources(), oldLayout,
				VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
				VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
				attachment.view->imageInfo().layout,
				attachment.view->imageInfo().stages,
				attachment.view->imageInfo().access);

			m_cmd->trackResource<VltAccess::Write>(attachment.view->image());
		}
	}

	void VltContext::transitionDepthAttachment(VltBarrierSet&       barriers,
											   const VltAttachment& attachment,
											   VkImageLayout        oldLayout)
	{
		if (oldLayout != attachment.view->imageInfo().layout)
		{
			barriers.accessImage(
				attachment.view->image(),
				attachment.view->imageSubresources(), oldLayout,
				VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT |
					VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
				oldLayout != VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL
					? VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT
					: 0,
				attachment.view->imageInfo().layout,
				attachment.view->imageInfo().stages,
				attachment.view->imageInfo().access);

			m_cmd->trackResource<VltAccess::Write>(attachment.view->image());
		}
	}

	void VltContext::emitMemoryBarrier(
		VkDependencyFlags     flags,
		VkPipelineStageFlags2 srcStages,
		VkAccessFlags2        srcAccess,
		VkPipelineStageFlags2 dstStages,
		VkAccessFlags2        dstAccess)
	{
		this->spillRenderPass();

		VkMemoryBarrier2 barrier;
		barrier.sType         = VK_STRUCTURE_TYPE_MEMORY_BARRIER_2;
		barrier.pNext         = nullptr;
		barrier.srcStageMask  = srcStages;
		barrier.srcAccessMask = srcAccess;
		barrier.dstStageMask  = dstStages;
		barrier.dstAccessMask = dstAccess;

		VkDependencyInfo info;
		info.sType                    = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
		info.pNext                    = nullptr;
		info.dependencyFlags          = flags;
		info.memoryBarrierCount       = 1;
		info.pMemoryBarriers          = &barrier;
		info.bufferMemoryBarrierCount = 0;
		info.pBufferMemoryBarriers    = nullptr;
		info.imageMemoryBarrierCount  = 0;
		info.pImageMemoryBarriers     = nullptr;

		m_cmd->cmdPipelineBarrier2(
			VltCmdType::ExecBuffer, &info);
	}

	void VltContext::performClear(
		const Rc<VltImageView>& imageView,
		int32_t                 attachmentIndex,
		VkImageAspectFlags      discardAspects,
		VkImageAspectFlags      clearAspects,
		VkClearValue            clearValue)
	{
		VltColorAttachmentOps colorOp;
		colorOp.loadOp      = VK_ATTACHMENT_LOAD_OP_LOAD;
		colorOp.loadLayout  = imageView->imageInfo().layout;
		colorOp.storeLayout = imageView->imageInfo().layout;

		VltDepthAttachmentOps depthOp;
		depthOp.loadOpD     = VK_ATTACHMENT_LOAD_OP_LOAD;
		depthOp.loadOpS     = VK_ATTACHMENT_LOAD_OP_LOAD;
		depthOp.loadLayout  = imageView->imageInfo().layout;
		depthOp.storeLayout = imageView->imageInfo().layout;

		if (clearAspects & VK_IMAGE_ASPECT_COLOR_BIT)
			colorOp.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		else if (discardAspects & VK_IMAGE_ASPECT_COLOR_BIT)
			colorOp.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;

		if (clearAspects & VK_IMAGE_ASPECT_DEPTH_BIT)
			depthOp.loadOpD = VK_ATTACHMENT_LOAD_OP_CLEAR;
		else if (discardAspects & VK_IMAGE_ASPECT_DEPTH_BIT)
			depthOp.loadOpD = VK_ATTACHMENT_LOAD_OP_DONT_CARE;

		if (clearAspects & VK_IMAGE_ASPECT_STENCIL_BIT)
			depthOp.loadOpS = VK_ATTACHMENT_LOAD_OP_CLEAR;
		else if (discardAspects & VK_IMAGE_ASPECT_DEPTH_BIT)
			depthOp.loadOpS = VK_ATTACHMENT_LOAD_OP_DONT_CARE;

		if (attachmentIndex >= 0 && !m_state.cb.framebufferInfo.isWritable(attachmentIndex, clearAspects | discardAspects))
		{
			// Do not fold the clear/discard into the render pass if any of the affected aspects
			// isn't writable. We can only hit this particular path when starting a render pass,
			// so we can safely manipulate load layouts here.
			int32_t       colorIndex   = m_state.cb.framebufferInfo.getColorAttachmentIndex(attachmentIndex);
			VkImageLayout renderLayout = m_state.cb.framebufferInfo.getAttachment(attachmentIndex).layout;

			if (colorIndex < 0)
			{
				depthOp.loadLayout                           = m_state.cb.renderPassOps.depthOps.loadLayout;
				depthOp.storeLayout                          = renderLayout;
				m_state.cb.renderPassOps.depthOps.loadLayout = renderLayout;
			}
			else
			{
				colorOp.loadLayout                                       = m_state.cb.renderPassOps.colorOps[colorIndex].loadLayout;
				colorOp.storeLayout                                      = renderLayout;
				m_state.cb.renderPassOps.colorOps[colorIndex].loadLayout = renderLayout;
			}

			attachmentIndex = -1;
		}

		bool is3D = imageView->imageInfo().type == VK_IMAGE_TYPE_3D;

		if ((clearAspects | discardAspects) == imageView->info().aspect && !is3D)
		{
			colorOp.loadLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			depthOp.loadLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		}

		if (attachmentIndex < 0)
		{
			if (m_execBarriers.isImageDirty(imageView->image(), imageView->imageSubresources(), VltAccess::Write))
				m_execBarriers.recordCommands(m_cmd);

			// Set up a temporary render pass to execute the clear
			VkImageLayout imageLayout = ((clearAspects | discardAspects) & VK_IMAGE_ASPECT_COLOR_BIT)
											? imageView->pickLayout(VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
											: imageView->pickLayout(VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

			VkRenderingAttachmentInfoKHR attachmentInfo = { VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO_KHR };
			attachmentInfo.imageView                    = imageView->handle();
			attachmentInfo.imageLayout                  = imageLayout;
			attachmentInfo.storeOp                      = VK_ATTACHMENT_STORE_OP_STORE;
			attachmentInfo.clearValue                   = clearValue;

			VkRenderingAttachmentInfoKHR stencilInfo = attachmentInfo;

			VkExtent3D extent = imageView->mipLevelExtent(0);

			VkRenderingInfoKHR renderingInfo = { VK_STRUCTURE_TYPE_RENDERING_INFO_KHR };
			renderingInfo.renderArea.extent  = { extent.width, extent.height };
			renderingInfo.layerCount         = imageView->info().numLayers;

			VkImageLayout loadLayout;
			VkImageLayout storeLayout;

			VkPipelineStageFlags clearStages = 0;
			VkAccessFlags        clearAccess = 0;

			if ((clearAspects | discardAspects) & VK_IMAGE_ASPECT_COLOR_BIT)
			{
				clearStages |= VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
				clearAccess |= VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

				attachmentInfo.loadOp = colorOp.loadOp;

				renderingInfo.colorAttachmentCount = 1;
				renderingInfo.pColorAttachments    = &attachmentInfo;

				loadLayout  = colorOp.loadLayout;
				storeLayout = colorOp.storeLayout;
			}
			else
			{
				clearStages |= VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT |
							   VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
				clearAccess |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

				if (imageView->info().aspect & VK_IMAGE_ASPECT_DEPTH_BIT)
				{
					renderingInfo.pDepthAttachment = &attachmentInfo;
					attachmentInfo.loadOp          = depthOp.loadOpD;
				}

				if (imageView->info().aspect & VK_IMAGE_ASPECT_STENCIL_BIT)
				{
					renderingInfo.pStencilAttachment = &stencilInfo;
					stencilInfo.loadOp               = depthOp.loadOpS;
				}

				loadLayout  = depthOp.loadLayout;
				storeLayout = depthOp.storeLayout;
			}

			if (loadLayout != imageLayout)
			{
				m_execAcquires.accessImage(
					imageView->image(),
					imageView->imageSubresources(),
					loadLayout, clearStages, 0,
					imageLayout, clearStages, clearAccess);

				m_execAcquires.recordCommands(m_cmd);
			}

			m_cmd->cmdBeginRendering(&renderingInfo);
			m_cmd->cmdEndRendering();

			m_execBarriers.accessImage(
				imageView->image(),
				imageView->imageSubresources(),
				imageLayout, clearStages, clearAccess,
				storeLayout,
				imageView->imageInfo().stages,
				imageView->imageInfo().access);

			m_cmd->trackResource<VltAccess::None>(imageView);
			m_cmd->trackResource<VltAccess::Write>(imageView->image());
		}
		else
		{
			// Perform the operation when starting the next render pass
			if ((clearAspects | discardAspects) & VK_IMAGE_ASPECT_COLOR_BIT)
			{
				uint32_t colorIndex = m_state.cb.framebufferInfo.getColorAttachmentIndex(attachmentIndex);

				m_state.cb.renderPassOps.colorOps[colorIndex].loadOp = colorOp.loadOp;
				if (m_state.cb.renderPassOps.colorOps[colorIndex].loadOp != VK_ATTACHMENT_LOAD_OP_LOAD && !is3D)
					m_state.cb.renderPassOps.colorOps[colorIndex].loadLayout = VK_IMAGE_LAYOUT_UNDEFINED;

				m_state.cb.renderPassOps.colorOps[colorIndex].clearValue = clearValue.color;
			}

			if ((clearAspects | discardAspects) & VK_IMAGE_ASPECT_DEPTH_BIT)
			{
				m_state.cb.renderPassOps.depthOps.loadOpD          = depthOp.loadOpD;
				m_state.cb.renderPassOps.depthOps.clearValue.depth = clearValue.depthStencil.depth;
			}

			if ((clearAspects | discardAspects) & VK_IMAGE_ASPECT_STENCIL_BIT)
			{
				m_state.cb.renderPassOps.depthOps.loadOpS            = depthOp.loadOpS;
				m_state.cb.renderPassOps.depthOps.clearValue.stencil = clearValue.depthStencil.stencil;
			}

			if ((clearAspects | discardAspects) & (VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT))
			{
				if (m_state.cb.renderPassOps.depthOps.loadOpD != VK_ATTACHMENT_LOAD_OP_LOAD &&
					m_state.cb.renderPassOps.depthOps.loadOpS != VK_ATTACHMENT_LOAD_OP_LOAD)
					m_state.cb.renderPassOps.depthOps.loadLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			}
		}
	}

	void VltContext::deferClear(
		const Rc<VltImageView>& imageView,
		VkImageAspectFlags      clearAspects,
		VkClearValue            clearValue)
	{
		for (auto& entry : m_deferredClears)
		{
			if (entry.imageView->matchesView(imageView))
			{
				entry.imageView = imageView;
				entry.discardAspects &= ~clearAspects;
				entry.clearAspects |= clearAspects;

				if (clearAspects & VK_IMAGE_ASPECT_COLOR_BIT)
					entry.clearValue.color = clearValue.color;
				if (clearAspects & VK_IMAGE_ASPECT_DEPTH_BIT)
					entry.clearValue.depthStencil.depth = clearValue.depthStencil.depth;
				if (clearAspects & VK_IMAGE_ASPECT_STENCIL_BIT)
					entry.clearValue.depthStencil.stencil = clearValue.depthStencil.stencil;

				return;
			}
			else if (entry.imageView->checkSubresourceOverlap(imageView))
			{
				this->spillRenderPass();
				break;
			}
		}

		m_deferredClears.push_back({ imageView, 0, clearAspects, clearValue });
	}

	void VltContext::deferDiscard(
		const Rc<VltImageView>& imageView,
		VkImageAspectFlags      discardAspects)
	{
		for (auto& entry : m_deferredClears)
		{
			if (entry.imageView->matchesView(imageView))
			{
				entry.imageView = imageView;
				entry.discardAspects |= discardAspects;
				entry.clearAspects &= ~discardAspects;
				return;
			}
			else if (entry.imageView->checkSubresourceOverlap(imageView))
			{
				this->spillRenderPass();
				break;
			}
		}

		m_deferredClears.push_back({ imageView, discardAspects });
	}

	void VltContext::flushClears(bool useRenderPass)
	{
		for (const auto& clear : m_deferredClears)
		{
			int32_t attachmentIndex = -1;

			if (useRenderPass && m_state.cb.framebufferInfo.isFullSize(clear.imageView))
				attachmentIndex = m_state.cb.framebufferInfo.findAttachment(clear.imageView);

			this->performClear(clear.imageView, attachmentIndex,
							   clear.discardAspects, clear.clearAspects, clear.clearValue);
		}

		m_deferredClears.clear();
	}


}  // namespace sce::vlt