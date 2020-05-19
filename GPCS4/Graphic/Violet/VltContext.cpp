#include "VltContext.h"

#include "VltBuffer.h"
#include "VltCmdList.h"
#include "VltDescriptor.h"
#include "VltDevice.h"
#include "VltFrameBuffer.h"
#include "VltImage.h"
#include "VltPipelineManager.h"
#include "VltRenderPass.h"
#include "VltResourceObjects.h"
#include "VltSampler.h"
#include "VltShader.h"
#include "VltStaging.h"
#include "VltFormat.h"

namespace vlt
{
;

VltContext::VltContext(const RcPtr<VltDevice>& device) :
	m_device(device),
	m_objects(&m_device->m_resObjects),
	m_cmd(nullptr),
	m_descPool(nullptr),
	m_staging(new VltStagingBufferAllocator(device))
{
}

VltContext::~VltContext()
{
}

void VltContext::beginRecording(const RcPtr<VltCmdList>& commandBuffer)
{
	m_cmd = commandBuffer;

	m_cmd->beginRecording();

	// The current state of the internal command buffer is
	// undefined, so we have to bind and set up everything
	// before any draw or dispatch command is recorded.
	m_flags.clr(
		VltContextFlag::GpRenderPassBound,
		VltContextFlag::GpXfbActive,
		VltContextFlag::GpClearRenderTargets);

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

RcPtr<VltCmdList> VltContext::endRecording()
{
	renderPassUnbindFramebuffer();

	m_cmd->endRecording();

	return std::exchange(m_cmd, nullptr);
}

void VltContext::setViewports(uint32_t viewportCount, const VkViewport* viewports, const VkRect2D* scissorRects)
{
	auto& vp = m_state.dy.viewport;
	if (viewportCount != vp.count)
	{
		m_flags.set(VltContextFlag::GpDirtyPipelineState);
	}

	for (uint32_t i = 0; i != viewportCount; ++i)
	{
		vp.viewports[i] = viewports[i];
		vp.scissors[i]  = scissorRects[i];
	}
	vp.count = viewportCount;

	m_flags.set(VltContextFlag::GpDirtyViewport);
}

void VltContext::setInputLayout(
	uint32_t                  bindingCount,
	const VltVertexBinding*   bindings,
	uint32_t                  attributeCount,
	const VltVertexAttribute* attributes)
{
	m_flags.set(
		VltContextFlag::GpDirtyPipelineState,
		VltContextFlag::GpDirtyVertexBuffers);

	for (uint32_t i = 0; i < bindingCount; i++)
	{
		m_state.gp.state.vi.setBinding(i, bindings[i]);
	}

	for (uint32_t i = 0; i < attributeCount; i++)
	{
		m_state.gp.state.vi.setAttribute(i, attributes[i]);
	}
	
	// Clear bindings
	for (uint32_t i = bindingCount; i < m_state.gp.state.vi.bindingCount(); i++)
	{
		m_state.gp.state.vi.setBinding(i, VltVertexBinding());
	}
	// Clear attributes
	for (uint32_t i = attributeCount; i < m_state.gp.state.vi.attributeCount(); i++)
	{
		m_state.gp.state.vi.setAttribute(i, VltVertexAttribute());
	}

	m_state.gp.state.vi.setInputCount(bindingCount, attributeCount);
}

void VltContext::setInputAssemblyState(const VltInputAssemblyInfo& iaState)
{
	m_state.gp.state.ia = iaState;
	m_flags.set(VltContextFlag::GpDirtyPipelineState);
}

void VltContext::setRasterizerState(const VltRasterizationInfo& rsState)
{
	m_state.gp.state.rs = rsState;
	m_flags.set(VltContextFlag::GpDirtyPipelineState);
}

void VltContext::setMultiSampleState(const VltMultisampleInfo& msState)
{
	m_state.gp.state.ms = msState;
	m_flags.set(VltContextFlag::GpDirtyPipelineState);
}

void VltContext::setDepthStencilState(const VltDepthStencilInfo& dsState)
{
	VkBool32 depthBoundsEnable = m_state.gp.state.ds.enableDepthBoundsTest();
	m_state.gp.state.ds = dsState;
	m_state.gp.state.ds.setEnableDepthBoundsTest(depthBoundsEnable);
	m_flags.set(VltContextFlag::GpDirtyPipelineState);
}

void VltContext::setDepthBounds(VltDepthBounds depthBounds)
{
	if (m_state.dy.depthBounds != depthBounds)
	{
		m_state.dy.depthBounds = depthBounds;
		m_flags.set(VltContextFlag::GpDirtyDepthBounds);
	}

	if (m_state.gp.state.ds.enableDepthBoundsTest() != depthBounds.enableDepthBounds)
	{
		m_state.gp.state.ds.setEnableDepthBoundsTest(depthBounds.enableDepthBounds);
		m_flags.set(VltContextFlag::GpDirtyPipelineState);
	}
}

void VltContext::setLogicOpState(const VltLogicOp& lo)
{
	m_state.gp.state.cb.setLogicalOp(lo);
	m_flags.set(VltContextFlag::GpDirtyPipelineState);
}

void VltContext::setBlendMode(
	uint32_t                       attachment,
	const VltColorBlendAttachment& blendMode)
{
	m_state.gp.state.cb.setBlendMode(attachment, blendMode);
	m_flags.set(VltContextFlag::GpDirtyPipelineState);
}

void VltContext::setBlendMask(
	uint32_t                     attachment,
	const VkColorComponentFlags& colorMask)
{
	m_state.gp.state.cb.setColorWriteMask(attachment, colorMask);
	m_flags.set(VltContextFlag::GpDirtyPipelineState);
}

void VltContext::bindRenderTargets(const VltRenderTargets& renderTargets)
{
	m_state.om.renderTargets = renderTargets;

	resetRenderPassOps(renderTargets, m_state.om.renderPassOps);

	if (!m_state.om.framebuffer || !m_state.om.framebuffer->matchTargets(renderTargets))
	{
		m_flags.set(VltContextFlag::GpDirtyFramebuffer);
	}
	else
	{
		m_flags.clr(VltContextFlag::GpDirtyFramebuffer);
	}
}

void VltContext::bindShader(VkShaderStageFlagBits stage, const RcPtr<VltShader>& shader)
{
	RcPtr<VltShader>* shaderStage;

	switch (stage)
	{
	case VK_SHADER_STAGE_VERTEX_BIT:	shaderStage = &m_state.gp.shaders.vs;	break;
	case VK_SHADER_STAGE_FRAGMENT_BIT:	shaderStage = &m_state.gp.shaders.fs;	break;
	case VK_SHADER_STAGE_COMPUTE_BIT:	shaderStage = &m_state.cp.shaders.cs;	break;
	//case VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT:    shaderStage = &m_state.gp.shaders.tcs; break;
	//case VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT: shaderStage = &m_state.gp.shaders.tes; break;
	//case VK_SHADER_STAGE_GEOMETRY_BIT:                shaderStage = &m_state.gp.shaders.gs;  break;
	default:
		return;
	}

	*shaderStage = shader;

	if (stage != VK_SHADER_STAGE_COMPUTE_BIT)
	{
		m_flags.set(
			VltContextFlag::GpDirtyPipeline,
			VltContextFlag::GpDirtyPipelineState,
			VltContextFlag::GpDirtyResources);
	}
	else
	{
		m_flags.set(
			VltContextFlag::CpDirtyPipeline,
			VltContextFlag::CpDirtyPipelineState,
			VltContextFlag::CpDirtyResources);
	}
}

void VltContext::bindIndexBuffer(const VltBufferSlice& buffer, VkIndexType indexType)
{
	m_state.vi.indexBuffer = buffer;
	m_state.vi.indexType   = indexType;

	m_flags.set(VltContextFlag::GpDirtyIndexBuffer);
}

void VltContext::bindVertexBuffer(uint32_t binding, const VltBufferSlice& buffer, uint32_t stride)
{
	m_state.vi.vertexBuffers[binding] = buffer;
	m_state.vi.vertexStrides[binding] = stride;

	m_flags.set(VltContextFlag::GpDirtyVertexBuffers);
}

void VltContext::bindSampler(uint32_t regSlot, const RcPtr<VltSampler>& sampler)
{
	m_res[regSlot].sampler = sampler;
	m_flags.set(VltContextFlag::GpDirtyResources);
}

void VltContext::bindResourceBuffer(uint32_t regSlot, const VltBufferSlice& buffer)
{
	m_res[regSlot].buffer = buffer;
	m_flags.set(VltContextFlag::GpDirtyResources);

	// TODO:
	// We need one binding functions to set GpDirtyDescriptorBinding in order to update resource layout
	// currently I just set it here, but it should be fixed
	m_flags.set(VltContextFlag::GpDirtyDescriptorBinding);
}

void VltContext::bindResourceView(uint32_t                    regSlot,
								  const RcPtr<VltImageView>&  imageView,
								  const RcPtr<VltBufferView>& bufferView)
{
	m_res[regSlot].imageView  = imageView;
	m_res[regSlot].bufferView = bufferView;
	m_flags.set(VltContextFlag::GpDirtyResources);
}

void VltContext::draw(
	uint32_t vertexCount,
	uint32_t instanceCount,
	uint32_t firstVertex,
	uint32_t firstInstance)
{
	commitGraphicsState<false, false>();

	m_cmd->cmdDraw(vertexCount, instanceCount, firstVertex, firstInstance);
}

void VltContext::drawIndexed(
	uint32_t indexCount,
	uint32_t instanceCount,
	uint32_t firstIndex,
	uint32_t vertexOffset,
	uint32_t firstInstance)
{
	commitGraphicsState<true, false>();

	m_cmd->cmdDrawIndexed(indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
}

void VltContext::dispatch(
	uint32_t x, 
	uint32_t y, 
	uint32_t z)
{
	if (commitComputeState())
	{
		m_cmd->cmdDispatch(x, y, z);

		VkMemoryBarrier barrier = {};
		barrier.sType           = VK_STRUCTURE_TYPE_MEMORY_BARRIER;
		barrier.pNext           = nullptr;
		barrier.srcAccessMask   = VK_ACCESS_SHADER_WRITE_BIT;
		barrier.dstAccessMask   = VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT;

		m_cmd->cmdPipelineBarrier(VltCmdType::ExecBuffer, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT,
								  0, 1, &barrier, 0, nullptr, 0, nullptr);
	}
}

void VltContext::clearRenderTarget(
	const RcPtr<VltImageView>& targetView,
	VkImageAspectFlags         clearAspects,
	const VkClearValue&        clearValue)
{
	updateFrameBuffer();

	const auto& tgtImgInfo = targetView->imageInfo();

	// Prepare attachment ops
	VltColorAttachmentOps colorOp = {};
	colorOp.loadOp                = VK_ATTACHMENT_LOAD_OP_LOAD;
	colorOp.loadLayout            = targetView->imageInfo().layout;
	colorOp.storeOp               = VK_ATTACHMENT_STORE_OP_STORE;
	colorOp.storeLayout           = targetView->imageInfo().layout;

	VltDepthAttachmentOps depthOp = {};
	depthOp.loadOpD               = VK_ATTACHMENT_LOAD_OP_LOAD;
	depthOp.loadOpS               = VK_ATTACHMENT_LOAD_OP_LOAD;
	depthOp.loadLayout            = targetView->imageInfo().layout;
	depthOp.storeOpD              = VK_ATTACHMENT_STORE_OP_STORE;
	depthOp.storeOpS              = VK_ATTACHMENT_STORE_OP_STORE;
	depthOp.storeLayout           = targetView->imageInfo().layout;

	if (clearAspects & VK_IMAGE_ASPECT_COLOR_BIT)
	{
		colorOp.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	}

	if (clearAspects & VK_IMAGE_ASPECT_DEPTH_BIT)
	{
		depthOp.loadOpD = VK_ATTACHMENT_LOAD_OP_CLEAR;
	}

	if (clearAspects & VK_IMAGE_ASPECT_STENCIL_BIT)
	{
		depthOp.loadOpS = VK_ATTACHMENT_LOAD_OP_CLEAR;
	}

	if (clearAspects == targetView->info().aspect &&
		tgtImgInfo.type != VK_IMAGE_TYPE_3D)
	{
		colorOp.loadLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		depthOp.loadLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	}

	int32_t attachmentIndex = -1;
	// Only image which covers the entire framebuffer
	// can be cleared using render pass
	if (m_state.om.framebuffer->isFullSize(targetView))
	{
		attachmentIndex = m_state.om.framebuffer->findAttachment(targetView);
	}

	if (attachmentIndex >= 0)
	{
		// If the target to be cleared is one of the framebuffer's attachment

		if (m_flags.test(VltContextFlag::GpRenderPassBound))
		{
			// If we are already in a render pass scope,
			// perform the clear by simply emit a clear attachment command.

			// For color images,
			// the attachment index for the current subpass is
			// equal to the render pass attachment index.
			VkClearAttachment clearInfo = {};
			clearInfo.aspectMask        = clearAspects;
			clearInfo.colorAttachment   = attachmentIndex;
			clearInfo.clearValue        = clearValue;

			VkClearRect clearRect        = {};
			clearRect.rect.offset.x      = 0;
			clearRect.rect.offset.y      = 0;
			clearRect.rect.extent.width  = tgtImgInfo.extent.width;
			clearRect.rect.extent.height = tgtImgInfo.extent.height;
			clearRect.baseArrayLayer     = 0;
			clearRect.layerCount         = tgtImgInfo.numLayers;

			m_cmd->cmdClearAttachments(1, &clearInfo, 1, &clearRect);
		}
		else
		{
			// If we are not in a render pass scope,
			// perform the clear upon next render pass begin.
			if (clearAspects & VK_IMAGE_ASPECT_COLOR_BIT)
			{
				m_state.om.renderPassOps.colorOps[attachmentIndex] = colorOp;
			}
			if (clearAspects & VK_IMAGE_ASPECT_DEPTH_BIT)
			{
				m_state.om.renderPassOps.depthOps.loadOpD  = depthOp.loadOpD;
				m_state.om.renderPassOps.depthOps.storeOpD = depthOp.storeOpD;
			}
			if (clearAspects & VK_IMAGE_ASPECT_STENCIL_BIT)
			{
				m_state.om.renderPassOps.depthOps.loadOpS  = depthOp.loadOpS;
				m_state.om.renderPassOps.depthOps.storeOpS = depthOp.storeOpS;
			}

		    if (clearAspects & (VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT))
			{
				m_state.om.renderPassOps.depthOps.loadLayout  = depthOp.loadLayout;
				m_state.om.renderPassOps.depthOps.storeLayout = depthOp.storeLayout;

				// If both depth and stencil are to be cleared, the entire buffer
				// can be safely cleared.
				if (m_state.om.renderPassOps.depthOps.loadOpD == VK_ATTACHMENT_LOAD_OP_CLEAR &&
					m_state.om.renderPassOps.depthOps.loadOpS == VK_ATTACHMENT_LOAD_OP_CLEAR)
				{
					m_state.om.renderPassOps.depthOps.loadLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				}
			}

			m_state.om.clearValues[attachmentIndex] = clearValue;
			m_flags.set(VltContextFlag::GpClearRenderTargets);
		}
	}
	else
	{
		// If the target to clear is not in current framebuffer,
		// we create a temporary framebuffer,
		// then still use render pass to perform the clear.
		leaveRenderPassScope();

		VltRenderTargets     attachments = {};
		VltRenderPassOps     ops         = {};
		VkAccessFlags        srcAccess   = {};
		VkPipelineStageFlags srcStages   = {};

		if (clearAspects & VK_IMAGE_ASPECT_COLOR_BIT)
		{
			attachments.color[0].view   = targetView;
			attachments.color[0].layout = targetView->pickLayout(VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);

			ops.colorOps[0] = colorOp;

			srcAccess = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			srcStages = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		}
		else
		{
			attachments.depth.view   = targetView;
			attachments.depth.layout = targetView->pickLayout(VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

			ops.depthOps = depthOp;

			srcAccess = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
			srcStages = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
		}

		auto framebuffer = m_device->createFrameBuffer(attachments);
		renderPassBindFramebuffer(framebuffer, ops, 1, &clearValue);

		renderPassUnbindFramebuffer();

		VkMemoryBarrier barrier = {};
		barrier.sType           = VK_STRUCTURE_TYPE_MEMORY_BARRIER;
		barrier.srcAccessMask   = srcAccess;
		m_cmd->cmdPipelineBarrier(
			VltCmdType::ExecBuffer,
			srcStages, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
			0,
			1, &barrier,
			0, nullptr,
			0, nullptr);
	}
}

void VltContext::copyBuffer(
	const RcPtr<VltBuffer>& dstBuffer,
	VkDeviceSize            dstOffset,
	const RcPtr<VltBuffer>& srcBuffer,
	VkDeviceSize            srcOffset,
	VkDeviceSize            numBytes)
{
	leaveRenderPassScope();

	auto srcSlice = srcBuffer->slice();
	auto dstSlice = dstBuffer->slice();

	VkBufferCopy region = { 0 };
	region.srcOffset    = srcOffset;
	region.dstOffset    = dstOffset;
	region.size         = numBytes;
	m_cmd->cmdCopyBuffer(VltCmdType::ExecBuffer, srcSlice.buffer, dstSlice.buffer, 1, &region);

	// Not sure if we need a barrier here....
	//fullPipelineBarrier();
}

void VltContext::copyBufferToImage(
	const RcPtr<VltImage>&   dstImage,
	VkImageSubresourceLayers dstSubresource,
	VkOffset3D               dstOffset,
	VkExtent3D               dstExtent,
	const RcPtr<VltBuffer>&  srcBuffer,
	VkDeviceSize             srcOffset,
	VkExtent2D               srcExtent)
{
	leaveRenderPassScope();

	VkBufferImageCopy region = {};
	region.bufferOffset      = srcOffset;
	region.bufferRowLength   = srcExtent.width;
	region.bufferImageHeight = srcExtent.height;
	region.imageSubresource  = dstSubresource;
	region.imageOffset       = dstOffset;
	region.imageExtent       = dstExtent;

	auto          srcSlice               = srcBuffer->slice();
	VkImageLayout dstImageLayoutTransfer = dstImage->pickLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	m_cmd->cmdCopyBufferToImage(VltCmdType::ExecBuffer, srcSlice.buffer, dstImage->handle(), dstImageLayoutTransfer, 1, &region);
}

void VltContext::updateBuffer(const RcPtr<VltBuffer>& buffer,
							  VkDeviceSize            offset,
							  VkDeviceSize            size,
							  const void*             data)
{
	leaveRenderPassScope();

	auto  stagingSlice = m_staging->alloc(size, CACHE_LINE_SIZE);
	auto stagingHandle = stagingSlice.getHandle();
	std::memcpy(stagingHandle.mapPtr, data, size);

	copyBuffer(buffer, offset, stagingSlice.buffer(), stagingSlice.offset(), size);

	m_cmd->trackResource(stagingSlice.buffer());
}

void VltContext::updateImage(
	const RcPtr<VltImage>&          image,
	const VkImageSubresourceLayers& subresources,
	VkOffset3D                      imageOffset,
	VkExtent3D                      imageExtent,
	const void*                     data,
	VkDeviceSize                    pitchPerRow,
	VkDeviceSize                    pitchPerLayer)
{
	leaveRenderPassScope();

	auto imgInfo    = image->info();
	auto formatInfo = image->formatInfo();
	
	uint32_t imageSize     = util::computeImageDataSize(imgInfo.format, imgInfo.extent);
	auto     stagingSlice  = m_staging->alloc(imageSize, CACHE_LINE_SIZE);
	auto     stagingHandle = stagingSlice.getHandle();

	std::memcpy(stagingHandle.mapPtr, data, imageSize);
	//auto blockCount = util::computeBlockCount(imgInfo.extent, formatInfo->blockSize);
	//util::packImageData(
	//	stagingHandle.mapPtr,
	//	data, blockCount,
	//	formatInfo->elementSize,
	//	pitchPerRow,
	//	pitchPerLayer);

	VkImageLayout transferLayout = image->pickLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

	// Note:
	// From dxvk, the initial layout can be VK_IMAGE_LAYOUT_UNDEFINED
	// only when the subresources covers the entire image.
	// But from vulkan spec, it seems the old layout can be
	// VK_IMAGE_LAYOUT_UNDEFINED at any chance as long as we don't
	// care the initial content.

	transitionImageLayout(image->handle(),
						  VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
						  0, VK_ACCESS_TRANSFER_WRITE_BIT,
						  VK_IMAGE_LAYOUT_UNDEFINED, transferLayout);

	VkExtent2D copyExtent = { 0, 0 };
	copyBufferToImage(image, subresources, imageOffset, imageExtent,
					  stagingSlice.buffer(), stagingSlice.offset(), copyExtent);

	transitionImageLayout(image->handle(),
						  VK_PIPELINE_STAGE_TRANSFER_BIT, imgInfo.stages,
						  VK_ACCESS_TRANSFER_WRITE_BIT, imgInfo.access,
						  transferLayout, imgInfo.layout);

	m_cmd->trackResource(stagingSlice.buffer());
}

void VltContext::uploadBuffer(
	const RcPtr<VltBuffer>& buffer,
	const void*             data)
{
	// TODO:
	// Use transfer queue to upload data
	updateBuffer(buffer, 0, buffer->length(), data);
}

void VltContext::uploadImage(
	const RcPtr<VltImage>&          image,
	const VkImageSubresourceLayers& subresources,
	const void*                     data,
	VkDeviceSize                    pitchPerRow,
	VkDeviceSize                    pitchPerLayer)
{
	auto imgInfo = image->info();
	// TODO:
	// Use transfer queue to upload data
	updateImage(image, subresources, { 0, 0, 0 }, imgInfo.extent, data, pitchPerRow, pitchPerLayer);
}

void VltContext::transitionImageLayout(
	VkImage              image,
	VkPipelineStageFlags srcStage,
	VkPipelineStageFlags dstStage,
	VkAccessFlags        srcAccess,
	VkAccessFlags        dstAccess,
	VkImageLayout        oldLayout,
	VkImageLayout        newLayout)
{
	VkImageMemoryBarrier barrier            = {};
	barrier.sType                           = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout                       = oldLayout;
	barrier.newLayout                       = newLayout;
	barrier.srcQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
	barrier.image                           = image;
	barrier.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseMipLevel   = 0;
	barrier.subresourceRange.levelCount     = 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount     = 1;
	barrier.srcAccessMask                   = srcAccess;
	barrier.dstAccessMask                   = dstAccess;

	m_cmd->cmdPipelineBarrier(
		VltCmdType::ExecBuffer,
		srcStage, dstStage,
		0,
		0, nullptr,
		0, nullptr,
		1, &barrier);
}

void VltContext::fullPipelineBarrier()
{
	// This should be only used for debugging purpose.

	VkAccessFlags fullpipeSrcAccessMask = VK_ACCESS_INDIRECT_COMMAND_READ_BIT |
										  VK_ACCESS_INDEX_READ_BIT |
										  VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT |
										  VK_ACCESS_UNIFORM_READ_BIT |
										  VK_ACCESS_INPUT_ATTACHMENT_READ_BIT |
										  VK_ACCESS_SHADER_READ_BIT |
										  VK_ACCESS_SHADER_WRITE_BIT |
										  VK_ACCESS_COLOR_ATTACHMENT_READ_BIT |
										  VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT |
										  VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT |
										  VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT |
										  VK_ACCESS_TRANSFER_READ_BIT |
										  VK_ACCESS_TRANSFER_WRITE_BIT |
										  VK_ACCESS_HOST_READ_BIT |
										  VK_ACCESS_HOST_WRITE_BIT;
	VkAccessFlags fullpipeDstAccessMask = VK_ACCESS_INDIRECT_COMMAND_READ_BIT |
										  VK_ACCESS_INDEX_READ_BIT |
										  VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT |
										  VK_ACCESS_UNIFORM_READ_BIT |
										  VK_ACCESS_INPUT_ATTACHMENT_READ_BIT |
										  VK_ACCESS_SHADER_READ_BIT |
										  VK_ACCESS_SHADER_WRITE_BIT |
										  VK_ACCESS_COLOR_ATTACHMENT_READ_BIT |
										  VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT |
										  VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT |
										  VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT |
										  VK_ACCESS_TRANSFER_READ_BIT |
										  VK_ACCESS_TRANSFER_WRITE_BIT |
										  VK_ACCESS_HOST_READ_BIT |
										  VK_ACCESS_HOST_WRITE_BIT;

	VkMemoryBarrier barrier = {};
	barrier.sType           = VK_STRUCTURE_TYPE_MEMORY_BARRIER;
	barrier.pNext           = nullptr;
	barrier.srcAccessMask   = fullpipeSrcAccessMask;
	barrier.dstAccessMask   = fullpipeDstAccessMask;

	m_cmd->cmdPipelineBarrier(
		VltCmdType::ExecBuffer,
		VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
		0,
		1, &barrier,
		0, nullptr,
		0, nullptr);
}

void VltContext::updateFrameBuffer()
{
	do
	{
		if (!m_flags.test(VltContextFlag::GpDirtyFramebuffer))
		{
			break;
		}

		leaveRenderPassScope();

		m_state.om.framebuffer = m_device->createFrameBuffer(m_state.om.renderTargets);

		m_flags.clr(VltContextFlag::GpDirtyFramebuffer);
		// If the framebuffer is updated, then the render pass has changed,
		// thus we need to recompile pipeline.
		m_flags.set(VltContextFlag::GpDirtyPipelineState);
	} while (false);
}

void VltContext::resetRenderPassOps(
	const VltRenderTargets& renderTargets,
	VltRenderPassOps&       renderPassOps)
{
	VkPipelineStageFlags shaderStages =
		m_device->getShaderPipelineStages() 
		& ~VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;

	renderPassOps.barrier.srcStages = 
		shaderStages | 
		VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT | 
		VK_PIPELINE_STAGE_VERTEX_INPUT_BIT |
		VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT |
		VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT | 
		VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
		VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;

	renderPassOps.barrier.srcAccess = 
		VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | 
		VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT |
		VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | 
		VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT | 
		VK_ACCESS_INDIRECT_COMMAND_READ_BIT | 
		VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT | 
		VK_ACCESS_INDEX_READ_BIT |
		VK_ACCESS_UNIFORM_READ_BIT |
		VK_ACCESS_SHADER_READ_BIT |
		VK_ACCESS_SHADER_WRITE_BIT;

	if (m_device->features().extTransformFeedback.transformFeedback)
	{
		renderPassOps.barrier.srcStages |= 
			VK_PIPELINE_STAGE_TRANSFORM_FEEDBACK_BIT_EXT;
		renderPassOps.barrier.srcAccess |= 
			VK_ACCESS_TRANSFORM_FEEDBACK_WRITE_BIT_EXT | 
			VK_ACCESS_TRANSFORM_FEEDBACK_COUNTER_READ_BIT_EXT | 
			VK_ACCESS_TRANSFORM_FEEDBACK_COUNTER_WRITE_BIT_EXT;
	}

	renderPassOps.barrier.dstStages = 
		VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
	renderPassOps.barrier.dstAccess = 
		renderPassOps.barrier.srcAccess |
		VK_ACCESS_TRANSFER_READ_BIT | 
		VK_ACCESS_TRANSFER_WRITE_BIT;

	renderPassOps.depthOps = renderTargets.depth.view != nullptr ? 
	VltDepthAttachmentOps
	{
		VK_ATTACHMENT_LOAD_OP_LOAD,
		VK_ATTACHMENT_LOAD_OP_LOAD,
		renderTargets.depth.view->imageInfo().layout,
		VK_ATTACHMENT_STORE_OP_STORE,
		VK_ATTACHMENT_STORE_OP_STORE,
		renderTargets.depth.view->imageInfo().layout
	}
	: VltDepthAttachmentOps{};

	for (uint32_t i = 0; i < MaxNumRenderTargets; i++)
	{
		renderPassOps.colorOps[i] = renderTargets.color[i].view != nullptr ? 
		VltColorAttachmentOps
		{
			VK_ATTACHMENT_LOAD_OP_LOAD,
			renderTargets.color[i].view->imageInfo().layout,
			VK_ATTACHMENT_STORE_OP_STORE,
			renderTargets.color[i].view->imageInfo().layout
		}
		: VltColorAttachmentOps{};
	}

	// TODO provide a sane alternative for this
	if (renderPassOps.colorOps[0].loadLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR)
	{
		renderPassOps.colorOps[0].loadOp     = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		renderPassOps.colorOps[0].loadLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	}
}

void VltContext::renderPassBindFramebuffer(const RcPtr<VltFrameBuffer>& framebuffer,
										   const VltRenderPassOps&      ops,
										   uint32_t                     clearValueCount,
										   const VkClearValue*          clearValues)
{
	VkRenderPass renderPass = framebuffer->getRenderPassHandle(ops);

	VltFramebufferSize    fbSize            = framebuffer->size();
	VkRenderPassBeginInfo renderPassInfo    = {};
	renderPassInfo.sType                    = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass               = renderPass;
	renderPassInfo.framebuffer              = framebuffer->handle();
	renderPassInfo.renderArea.offset        = { 0, 0 };
	renderPassInfo.renderArea.extent.width  = fbSize.width;
	renderPassInfo.renderArea.extent.height = fbSize.height;

	renderPassInfo.clearValueCount = clearValueCount;
	renderPassInfo.pClearValues    = clearValues;

	m_cmd->cmdBeginRenderPass(&renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	m_cmd->trackResource(framebuffer);

	for (uint32_t i = 0; i < framebuffer->numAttachments(); i++)
	{
		m_cmd->trackResource(framebuffer->getAttachment(i).view);
		m_cmd->trackResource(framebuffer->getAttachment(i).view->image());
	}
}

void VltContext::renderPassUnbindFramebuffer()
{
	m_cmd->cmdEndRenderPass();
}

void VltContext::updateVertexBindings()
{
	VkDeviceSize                                   bindingCount = 0;
	std::array<VkBuffer, MaxNumVertexBindings>     buffers;
	std::array<VkDeviceSize, MaxNumVertexBindings> offsets;
	for (const auto& buffer : m_state.vi.vertexBuffers)
	{
		// break on first invalid buffer
		if (!buffer.defined())
		{
			break;
		}

		buffers[bindingCount] = buffer.buffer()->slice().buffer;
		offsets[bindingCount] = buffer.offset();
		++bindingCount;
	}

	if (bindingCount)
	{
		m_cmd->cmdBindVertexBuffers(0, bindingCount, buffers.data(), offsets.data());
	}
	
	m_flags.clr(VltContextFlag::GpDirtyVertexBuffers);
}

void VltContext::updateIndexBinding()
{
	do
	{
		if (!m_state.vi.indexBuffer.defined())
		{
			break;
		}

		VkBuffer     indexBuffer = m_state.vi.indexBuffer.buffer()->slice().buffer;
		VkDeviceSize offset      = m_state.vi.indexBuffer.offset();
		VkIndexType  type        = m_state.vi.indexType;
		m_cmd->cmdBindIndexBuffer(indexBuffer, offset, type);

		m_flags.clr(VltContextFlag::GpDirtyIndexBuffer);
	} while (false);
}

template <VkPipelineBindPoint BindPoint>
void VltContext::updateShaderResources(const VltPipelineLayout* pipelineLayout)
{

	uint32_t                          bindingCount = pipelineLayout->bindingCount();
	std::vector<VkWriteDescriptorSet> descriptorWrites;
	std::array<VkDescriptorBufferInfo, MaxNumActiveBindings> bufferInfos = {};
	std::array<VkDescriptorImageInfo, MaxNumActiveBindings>  imageInfos = {};

	auto& set = BindPoint == VK_PIPELINE_BIND_POINT_GRAPHICS ? 
		m_gpCtx.descriptorSet : m_cpCtx.descriptorSet;

	if (bindingCount != 0)
	{
		set = allocateDescriptorSet(pipelineLayout->descriptorSetLayout());
	}
	else
	{
		set = VK_NULL_HANDLE;
	}

	// TODO:
	// Update descriptor set using template

	for (uint32_t i = 0; i != bindingCount; ++i)
	{
		auto     binding = pipelineLayout->binding(i);
		uint32_t regSlot = binding.resSlot.regSlot;
		auto     res     = m_res[regSlot];

		switch (binding.resSlot.type)
		{
		case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
		{
			VkDescriptorBufferInfo bufferInfo = {};
			bufferInfo.buffer                 = res.buffer.getHandle().buffer;
			bufferInfo.offset                 = res.buffer.offset();
			bufferInfo.range                  = res.buffer.length();
			bufferInfos[i]                    = bufferInfo;

			VkWriteDescriptorSet writeSet = {};
			writeSet.sType                = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			writeSet.dstSet               = set;
			writeSet.dstBinding           = i;
			writeSet.dstArrayElement      = 0;
			writeSet.descriptorType       = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			writeSet.descriptorCount      = 1;
			writeSet.pBufferInfo          = &bufferInfos[i];
			descriptorWrites.push_back(writeSet);
		}
		break;
		case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER:
		{
			VkDescriptorBufferInfo bufferInfo = {};
			bufferInfo.buffer                 = res.buffer.getHandle().buffer;
			bufferInfo.offset                 = res.buffer.offset();
			bufferInfo.range                  = res.buffer.length();
			bufferInfos[i]                    = bufferInfo;

			VkWriteDescriptorSet writeSet = {};
			writeSet.sType                = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			writeSet.dstSet               = set;
			writeSet.dstBinding           = i;
			writeSet.dstArrayElement      = 0;
			writeSet.descriptorType       = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
			writeSet.descriptorCount      = 1;
			writeSet.pBufferInfo          = &bufferInfos[i];
			descriptorWrites.push_back(writeSet);
		}
		break;
		case VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE:
		{
			VkDescriptorImageInfo imageInfo = {};
			imageInfo.imageLayout           = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfo.imageView             = res.imageView->handle();
			imageInfo.sampler               = nullptr;
			imageInfos[i]                   = imageInfo;

			VkWriteDescriptorSet writeSet = {};
			writeSet.sType                = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			writeSet.dstSet               = set;
			writeSet.dstBinding           = i;
			writeSet.dstArrayElement      = 0;
			writeSet.descriptorType       = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
			writeSet.descriptorCount      = 1;
			writeSet.pImageInfo           = &imageInfos[i];
			descriptorWrites.push_back(writeSet);
		}
		break;
		case VK_DESCRIPTOR_TYPE_SAMPLER:
		{
			VkDescriptorImageInfo imageInfo = {};
			imageInfo.imageLayout           = VK_IMAGE_LAYOUT_UNDEFINED;
			imageInfo.imageView             = VK_NULL_HANDLE;
			imageInfo.sampler               = res.sampler->handle();
			imageInfos[i]                   = imageInfo;

			VkWriteDescriptorSet writeSet = {};
			writeSet.sType                = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			writeSet.dstSet               = set;
			writeSet.dstBinding           = i;
			writeSet.dstArrayElement      = 0;
			writeSet.descriptorType       = VK_DESCRIPTOR_TYPE_SAMPLER;
			writeSet.descriptorCount      = 1;
			writeSet.pImageInfo           = &imageInfos[i];
			descriptorWrites.push_back(writeSet);
		}
		break;
		default:
			break;
		}
	}

	// TODO:
	// Use vkUpdateDescriptorSetWithTemplate
	if (descriptorWrites.size())
	{
		m_cmd->updateDescriptorSets(descriptorWrites.size(), descriptorWrites.data());
	}
}

template <VkPipelineBindPoint BindPoint>
void VltContext::updateShaderDescriptorSetBinding(const VltPipelineLayout* layout, VkDescriptorSet set)
{
	if (set != VK_NULL_HANDLE)
	{
		VkPipelineLayout pipelineLayout = layout->pipelineLayout();

		m_cmd->cmdBindDescriptorSet(BindPoint, pipelineLayout, set, 0, nullptr);
	}
}

VkDescriptorSet VltContext::allocateDescriptorSet(VkDescriptorSetLayout layout)
{
	if (m_descPool == nullptr)
	{
		m_descPool = m_device->createDescriptorPool();
	}

	VkDescriptorSet set = m_descPool->alloc(layout);

	if (set == VK_NULL_HANDLE)
	{
		// The old descriptor pool is full, we trace it
		// and create a new one.
		m_cmd->trackDescriptorPool(std::move(m_descPool));

		m_descPool = m_device->createDescriptorPool();
		set        = m_descPool->alloc(layout);
	}

	return set;
}

void VltContext::updateGraphicsShaderResources()
{
	if (m_flags.test(VltContextFlag::GpDirtyResources))
	{
		updateShaderResources<VK_PIPELINE_BIND_POINT_GRAPHICS>(
			m_state.gp.pipeline->layout());
	}

	updateShaderDescriptorSetBinding<VK_PIPELINE_BIND_POINT_GRAPHICS>(
		m_state.gp.pipeline->layout(),
		m_gpCtx.descriptorSet);

	m_flags.clr(VltContextFlag::GpDirtyResources,
				VltContextFlag::GpDirtyDescriptorBinding);
}

bool VltContext::updateGraphicsPipeline()
{
	// Descriptor layout is bound with shaders
	m_state.gp.pipeline = m_objects->pipelineManager().createGraphicsPipeline(m_state.gp.shaders);
	m_flags.clr(VltContextFlag::GpDirtyPipeline);

	return true;
}

bool VltContext::updateGraphicsPipelineState()
{
	VltRenderPass* renderPass = m_state.om.framebuffer->getRenderPass();
	m_gpCtx.pipeline          = m_state.gp.pipeline->getPipelineHandle(m_state.gp.state, *renderPass);

	m_cmd->cmdBindPipeline(VK_PIPELINE_BIND_POINT_GRAPHICS, m_gpCtx.pipeline);

	m_flags.clr(VltContextFlag::GpDirtyPipelineState);

	return true;
}

bool VltContext::updateComputePipeline()
{
	m_state.cp.pipeline = m_objects->pipelineManager().createComputePipeline(m_state.cp.shaders);
	m_flags.clr(VltContextFlag::CpDirtyPipeline);
	return true;
}

bool VltContext::updateComputePipelineState()
{
	m_cpCtx.pipeline = m_state.cp.pipeline->getPipelineHandle(m_state.cp.state);

	m_cmd->cmdBindPipeline(VK_PIPELINE_BIND_POINT_COMPUTE, m_cpCtx.pipeline);

	m_flags.clr(VltContextFlag::CpDirtyPipelineState);

	return true;
}

void VltContext::enterRenderPassScope()
{
	if (!m_flags.test(VltContextFlag::GpRenderPassBound))
	{
		renderPassBindFramebuffer(
			m_state.om.framebuffer,
			m_state.om.renderPassOps,
			m_state.om.clearValues.size(),
			m_state.om.clearValues.data());

		// If there's a clear render target call issued
		// before this, the renderpass ops will be set to
		// clear state, we don't want to discard image contents 
		// if we have to reenter the current render pass,
		// so here we reset the renderpass ops.
		resetRenderPassOps(
			m_state.om.renderTargets,
			m_state.om.renderPassOps);

		m_flags.set(VltContextFlag::GpRenderPassBound);
	}
}

void VltContext::leaveRenderPassScope()
{
	if (m_flags.test(VltContextFlag::GpRenderPassBound))
	{
		renderPassUnbindFramebuffer();
		m_flags.clr(VltContextFlag::GpRenderPassBound);
	}
}

void VltContext::updateDynamicState()
{
	if (m_flags.test(VltContextFlag::GpDirtyViewport))
	{
		const auto& vp = m_state.dy.viewport;

		m_cmd->cmdSetViewport(0, vp.count, vp.viewports.data());
		m_cmd->cmdSetScissor(0, vp.count, vp.scissors.data());

		// Update viewport count, this will be used to create pipeline.
		m_state.gp.state.dy.setViewportCount(vp.count);
		m_flags.clr(VltContextFlag::GpDirtyViewport);
	}

	if (m_flags.test(VltContextFlag::GpDirtyDepthBounds))
	{
		VltDepthBounds& db = m_state.dy.depthBounds;
		m_cmd->cmdSetDepthBounds(db.minDepthBounds, db.maxDepthBounds);

		m_flags.clr(VltContextFlag::GpDirtyDepthBounds);
	}
}

template <bool Indexed, bool Indirect>
bool VltContext::commitGraphicsState()
{
	if (m_flags.test(VltContextFlag::GpDirtyFramebuffer))
	{
		updateFrameBuffer();
	}

	if (!m_flags.test(VltContextFlag::GpRenderPassBound))
	{
		enterRenderPassScope();
	}

	if (m_flags.test(VltContextFlag::GpDirtyPipeline))
	{
		updateGraphicsPipeline();
	}

	if (m_flags.test(VltContextFlag::GpDirtyVertexBuffers))
	{
		updateVertexBindings();
	}

	if (m_flags.test(VltContextFlag::GpDirtyIndexBuffer) && Indexed)
	{
		updateIndexBinding();
	}

	if (m_flags.any(
			VltContextFlag::GpDirtyViewport,
			VltContextFlag::GpDirtyBlendConstants,
			VltContextFlag::GpDirtyStencilRef,
			VltContextFlag::GpDirtyDepthBias,
			VltContextFlag::GpDirtyDepthBounds))
	{
		updateDynamicState();
	}

	if (m_flags.any(VltContextFlag::GpDirtyResources,
					VltContextFlag::GpDirtyDescriptorBinding))
	{
		updateGraphicsShaderResources();
	}

	if (m_flags.test(VltContextFlag::GpDirtyPipelineState))
	{
		updateGraphicsPipelineState();
	}

	return true;
}

bool VltContext::commitComputeState()
{
	if (m_flags.test(VltContextFlag::GpRenderPassBound))
	{
		leaveRenderPassScope();
	}

	if (m_flags.test(VltContextFlag::CpDirtyPipeline))
	{
		updateComputePipeline();
	}

	if (m_flags.any(
		VltContextFlag::CpDirtyResources,
		VltContextFlag::CpDirtyDescriptorBinding))
	{
		updateComputeShaderResources();
	}
		
	if (m_flags.test(VltContextFlag::CpDirtyPipelineState))
	{
		updateComputePipelineState();
	}

	return true;
}

void VltContext::updateComputeShaderResources()
{
	if (m_flags.test(VltContextFlag::CpDirtyResources))
	{
		updateShaderResources<VK_PIPELINE_BIND_POINT_COMPUTE>(
			m_state.cp.pipeline->layout());
	}

	updateShaderDescriptorSetBinding<VK_PIPELINE_BIND_POINT_COMPUTE>(
		m_state.cp.pipeline->layout(),
		m_cpCtx.descriptorSet);

	m_flags.clr(VltContextFlag::CpDirtyResources,
				VltContextFlag::CpDirtyDescriptorBinding);
}

}  // namespace vlt