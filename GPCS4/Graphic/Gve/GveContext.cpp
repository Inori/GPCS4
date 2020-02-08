#include "GveContext.h"

#include "GveBuffer.h"
#include "GveCmdList.h"
#include "GveDescriptor.h"
#include "GveDevice.h"
#include "GveFrameBuffer.h"
#include "GveImage.h"
#include "GvePipelineManager.h"
#include "GveRenderPass.h"
#include "GveResourceObjects.h"
#include "GveSampler.h"
#include "GveShader.h"
#include "GveStaging.h"

namespace gve
{;

GveContext::GveContext(const RcPtr<GveDevice>& device) :
	m_device(device),
	m_objects(&m_device->m_resObjects),
	m_cmd(nullptr),
	m_descPool(nullptr),
	m_stagingAlloc(std::make_unique<GveStagingBufferAllocator>(device))
{
}

GveContext::~GveContext()
{
}

void GveContext::beginRecording(const RcPtr<GveCmdList>& commandBuffer)
{
	m_cmd = commandBuffer;

	m_cmd->beginRecording();

	// The current state of the internal command buffer is
	// undefined, so we have to bind and set up everything
	// before any draw or dispatch command is recorded.
	m_flags.clr(
		GveContextFlag::GpRenderPassBound,
		GveContextFlag::GpXfbActive,
		GveContextFlag::GpClearRenderTargets);

	m_flags.set(
		GveContextFlag::GpDirtyFramebuffer,
		GveContextFlag::GpDirtyPipeline,
		GveContextFlag::GpDirtyPipelineState,
		GveContextFlag::GpDirtyResources,
		GveContextFlag::GpDirtyVertexBuffers,
		GveContextFlag::GpDirtyIndexBuffer,
		GveContextFlag::GpDirtyXfbBuffers,
		GveContextFlag::GpDirtyBlendConstants,
		GveContextFlag::GpDirtyStencilRef,
		GveContextFlag::GpDirtyViewport,
		GveContextFlag::GpDirtyDepthBias,
		GveContextFlag::GpDirtyDepthBounds,

		GveContextFlag::CpDirtyPipeline,
		GveContextFlag::CpDirtyPipelineState,
		GveContextFlag::CpDirtyResources,
		GveContextFlag::DirtyDrawBuffer);
}

RcPtr<GveCmdList> GveContext::endRecording()
{
	endRenderPass();

	m_cmd->endRecording();

	m_stagingAlloc->trim();

	return std::exchange(m_cmd, nullptr);
}

void GveContext::setViewport(const VkViewport& viewport, const VkRect2D& scissorRect)
{
	setViewports(1, &viewport, &scissorRect);
}

void GveContext::setViewports(uint32_t viewportCount, const VkViewport* viewports, const VkRect2D* scissorRects)
{
	auto& vp = m_state.dy.vp;
	if (viewportCount != vp.count)
	{
		m_flags.set(GveContextFlag::GpDirtyPipelineState);
	}

	for (uint32_t i = 0; i != viewportCount; ++i)
	{
		vp.viewports[i] = viewports[i];
		vp.scissors[i]  = scissorRects[i];
	}
	vp.count = viewportCount;

	m_flags.set(GveContextFlag::GpDirtyViewport);
}

void GveContext::setVertexInputLayout(const GveVertexInputInfo& viState)
{
	m_state.gp.states.vi = viState;
	m_flags.set(GveContextFlag::GpDirtyPipelineState);
}

void GveContext::setInputAssemblyState(const GveInputAssemblyInfo& iaState)
{
	m_state.gp.states.ia = iaState;
	m_flags.set(GveContextFlag::GpDirtyPipelineState);
}

void GveContext::setRasterizerState(const GveRasterizationInfo& rsState)
{
	m_state.gp.states.rs = rsState;
	m_flags.set(GveContextFlag::GpDirtyPipelineState);
}

void GveContext::setMultiSampleState(const GveMultisampleInfo& msState)
{
	m_state.gp.states.ms = msState;
	m_flags.set(GveContextFlag::GpDirtyPipelineState);
}

void GveContext::setDepthStencilState(const GveDepthStencilInfo& dsState)
{
	m_state.gp.states.ds = dsState;
	m_flags.set(GveContextFlag::GpDirtyPipelineState);
}

void GveContext::setColorBlendState(const GveColorBlendInfo& blendCtl)
{
	m_state.gp.states.cb = blendCtl;
	m_flags.set(GveContextFlag::GpDirtyPipelineState);
}

void GveContext::bindRenderTargets(const GveAttachment* color, uint32_t count)
{
	do
	{
		if (!color || !count)
		{
			break;
		}

		if (m_state.om.framebuffer && m_state.om.framebuffer->matchColorTargets(color, count))
		{
			break;
		}

		std::memcpy(m_state.om.renderTargets.color, color, sizeof(GveAttachment) * count);
		m_flags.set(GveContextFlag::GpDirtyFramebuffer);

	} while (false);
}

void GveContext::bindDepthRenderTarget(const GveAttachment& depth)
{
	do
	{
		if (m_state.om.framebuffer && m_state.om.framebuffer->matchDepthTarget(depth))
		{
			break;
		}

		m_state.om.renderTargets.depth = depth;
		m_flags.set(GveContextFlag::GpDirtyFramebuffer);

	} while (false);
}

void GveContext::bindShader(VkShaderStageFlagBits stage, const RcPtr<GveShader>& shader)
{
	RcPtr<GveShader>* shaderStage;

	switch (stage)
	{
	case VK_SHADER_STAGE_VERTEX_BIT:					shaderStage = &m_state.gp.shaders.vs;  break;
	case VK_SHADER_STAGE_FRAGMENT_BIT:					shaderStage = &m_state.gp.shaders.fs;  break;
	//case VK_SHADER_STAGE_COMPUTE_BIT:                 shaderStage = &m_state.cp.shaders.cs;  break;
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
			GveContextFlag::GpDirtyPipeline,
			GveContextFlag::GpDirtyPipelineState,
			GveContextFlag::GpDirtyResources);
	}
	else
	{
		m_flags.set(
			GveContextFlag::CpDirtyPipeline,
			GveContextFlag::CpDirtyPipelineState,
			GveContextFlag::CpDirtyResources);
	}
}

void GveContext::bindIndexBuffer(const GveBufferSlice& buffer, VkIndexType indexType)
{
	m_state.vi.indexBuffer = buffer;
	m_state.vi.indexType   = indexType;

	m_flags.set(GveContextFlag::GpDirtyIndexBuffer);
}

void GveContext::bindVertexBuffer(uint32_t binding, const GveBufferSlice& buffer, uint32_t stride)
{
	m_state.vi.vertexBuffers[binding] = buffer;
	m_state.vi.vertexStrides[binding] = stride;

	m_flags.set(GveContextFlag::GpDirtyVertexBuffers);
}

void GveContext::bindSampler(uint32_t regSlot, const RcPtr<GveSampler>& sampler)
{
	m_res[regSlot].sampler = sampler;
	m_flags.set(GveContextFlag::GpDirtyResources);
}

void GveContext::bindResourceBuffer(uint32_t regSlot, const GveBufferSlice& buffer)
{
	m_res[regSlot].buffer = buffer;
	m_flags.set(GveContextFlag::GpDirtyResources);

	// TODO:
	// We need one binding functions to set GpDirtyDescriptorBinding in order to update resource layout
	// currently I just set it here, but it should be fixed
	m_flags.set(GveContextFlag::GpDirtyDescriptorBinding);
}

void GveContext::bindResourceView(uint32_t regSlot,
								 const RcPtr<GveImageView>& imageView,
								 const RcPtr<GveBufferView>& bufferView)
{
	m_res[regSlot].imageView  = imageView;
	m_res[regSlot].bufferView = bufferView;
	m_flags.set(GveContextFlag::GpDirtyResources);
}

void GveContext::draw(
	uint32_t vertexCount,
	uint32_t instanceCount,
	uint32_t firstVertex,
	uint32_t firstInstance)
{
	commitGraphicsState<false, false>();

	m_cmd->cmdDraw(vertexCount, instanceCount, firstVertex, firstInstance);
}

void GveContext::drawIndexed(
	uint32_t indexCount,
	uint32_t instanceCount,
	uint32_t firstIndex,
	uint32_t vertexOffset,
	uint32_t firstInstance)
{
	commitGraphicsState<true, false>();

	m_cmd->cmdDrawIndexed(indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
}

void GveContext::copyBuffer(GveBufferSlice& dstBuffer, GveBufferSlice& srcBuffer, VkDeviceSize size)
{
	VkCommandBuffer commandBuffer = m_cmd->cmdBeginSingleTimeCommands();

	// TODO:
	// setup region correctly
	VkBufferCopy copyRegion = { 0 };
	copyRegion.size         = size;
	m_cmd->cmdCopyBuffer(commandBuffer, srcBuffer.handle(), dstBuffer.handle(), 1, &copyRegion);
	auto queues = m_device->queues();
	m_cmd->cmdEndSingleTimeCommands(commandBuffer, queues.graphics.queueHandle);
}

void GveContext::copyBufferToImage(
	const RcPtr<GveImage>& dstImage,
	GveBufferSlice& srcBuffer,
	uint32_t width, uint32_t height)
{
	VkCommandBuffer commandBuffer = m_cmd->cmdBeginSingleTimeCommands();

	VkBufferImageCopy region               = {};
	region.bufferOffset                    = srcBuffer.offset();
	region.bufferRowLength                 = 0;
	region.bufferImageHeight               = 0;
	region.imageSubresource.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
	region.imageSubresource.mipLevel       = 0;
	region.imageSubresource.baseArrayLayer = 0;
	region.imageSubresource.layerCount     = 1;
	region.imageOffset                     = { 0, 0, 0 };
	region.imageExtent                     = { width, height, 1 };

	m_cmd->cmdCopyBufferToImage(commandBuffer, srcBuffer.handle(), dstImage->handle(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
	auto queues = m_device->queues();
	m_cmd->cmdEndSingleTimeCommands(commandBuffer, queues.graphics.queueHandle);
}

void GveContext::updateBuffer(const RcPtr<GveBuffer>& buffer,
							 VkDeviceSize offset, VkDeviceSize size, const void* data)
{
	do
	{
		if (!data)
		{
			break;
		}

		if (buffer->info().usage & VK_BUFFER_USAGE_TRANSFER_DST_BIT)
		{
			auto stagingSlice = m_stagingAlloc->alloc(size, CACHE_LINE_SIZE);

			void* stagingData = stagingSlice.mapPtr(0);
			std::memcpy(stagingData, data, size);

			auto dstSlice = GveBufferSlice(buffer, offset, size);
			copyBuffer(dstSlice, stagingSlice, size);
		}
		else
		{
			void* bufferData = buffer->mapPtr(offset);
			std::memcpy(bufferData, data, size);
		}

	} while (false);
}

void GveContext::updateImage(const RcPtr<GveImage>& image,
							VkDeviceSize offset, VkDeviceSize size, const void* data)
{
	auto stagingSlice = m_stagingAlloc->alloc(size, CACHE_LINE_SIZE);

	void* stagingData = stagingSlice.mapPtr(0);
	std::memcpy(stagingData, data, size);

	transitionImageLayout(image->handle(), image->getFormat(), image->info().initialLayout, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	copyBufferToImage(image, stagingSlice, image->getWidth(), image->getHeight());
	transitionImageLayout(image->handle(), image->getFormat(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, image->info().layout);
}

void GveContext::transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout)
{
	VkCommandBuffer commandBuffer = m_cmd->cmdBeginSingleTimeCommands();

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

	VkPipelineStageFlags sourceStage;
	VkPipelineStageFlags destinationStage;

	if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
	{
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

		sourceStage      = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	}
	else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
	{
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		sourceStage      = VK_PIPELINE_STAGE_TRANSFER_BIT;
		destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	}
	else
	{
		throw std::invalid_argument("unsupported layout transition!");
	}

	vkCmdPipelineBarrier(
		commandBuffer,
		sourceStage, destinationStage,
		0,
		0, nullptr,
		0, nullptr,
		1, &barrier);

	auto queues = m_device->queues();
	m_cmd->cmdEndSingleTimeCommands(commandBuffer, queues.graphics.queueHandle);
}

void GveContext::updateFrameBuffer()
{
	do
	{
		if (m_state.om.framebuffer && m_state.om.framebuffer->matchRenderTargets(m_state.om.renderTargets))
		{
			break;
		}

		m_state.om.framebuffer = m_device->createFrameBuffer(m_state.om.renderTargets);

	} while (false);

	m_flags.clr(GveContextFlag::GpDirtyFramebuffer);
}

void GveContext::updateRenderPassOps(const GveRenderTargets& rts, GveRenderPassOps& ops)
{
	for (uint32_t i = 0; i != MaxNumRenderTargets; ++i)
	{
		if (rts.color[i].view == nullptr)
		{
			continue;
		}

		ops.colorOps[i].loadOp      = VK_ATTACHMENT_LOAD_OP_CLEAR;
		ops.colorOps[i].loadLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
		ops.colorOps[i].storeOp     = VK_ATTACHMENT_STORE_OP_STORE;
		ops.colorOps[i].storeLayout = rts.color[i].view->imageInfo().layout;
	}

	if (rts.depth.view != nullptr)
	{
		ops.depthOps.loadOpD     = VK_ATTACHMENT_LOAD_OP_CLEAR;
		ops.depthOps.loadLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
		ops.depthOps.storeOpD    = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		ops.depthOps.storeLayout = rts.depth.layout;
	}

	ops.barrier.srcStages = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	ops.barrier.srcAccess = 0;
	ops.barrier.dstStages = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	ops.barrier.dstAccess = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
}

void GveContext::beginRenderPass()
{
	updateRenderPassOps(m_state.om.renderTargets, m_state.om.omInfo.ops);

	auto& framebuffer       = m_state.om.framebuffer;
	VkRenderPass renderPass = framebuffer->getRenderPassHandle(m_state.om.omInfo.ops);

	VkExtent2D extend = framebuffer->getRenderExtent();

	VkRenderPassBeginInfo renderPassInfo = {};
	renderPassInfo.sType                 = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass            = renderPass;
	renderPassInfo.framebuffer           = framebuffer->handle();
	renderPassInfo.renderArea.offset     = { 0, 0 };
	renderPassInfo.renderArea.extent     = extend;

	// TODO:
	// Set clear value according to gnm
	std::array<VkClearValue, 2> clearValues = {};
	clearValues[0].color                    = { { 0.0f, 0.0f, 0.0f, 1.0f } };
	clearValues[1].depthStencil             = { 1.0f, 0 };

	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassInfo.pClearValues    = clearValues.data();

	m_cmd->cmdBeginRenderPass(&renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	m_flags.set(GveContextFlag::GpRenderPassBound);
}

void GveContext::endRenderPass()
{
	do
	{
		if (!m_flags.test(GveContextFlag::GpRenderPassBound))
		{
			break;
		}

		m_cmd->cmdEndRenderPass();
		m_flags.clr(GveContextFlag::GpRenderPassBound);
	} while (false);
}

void GveContext::updateVertexBindings()
{
	VkDeviceSize bindingCount = 0;
	std::array<VkBuffer, MaxNumVertexBindings> buffers;
	std::array<VkDeviceSize, MaxNumVertexBindings> offsets;
	for (const auto& buffer : m_state.vi.vertexBuffers)
	{
		// break on first invalid buffer
		if (!buffer.defined())
		{
			break;
		}

		buffers[bindingCount] = buffer.handle();
		offsets[bindingCount] = buffer.offset();
		++bindingCount;
	}

	m_cmd->cmdBindVertexBuffers(0, bindingCount, buffers.data(), offsets.data());

	m_flags.clr(GveContextFlag::GpDirtyVertexBuffers);
}

void GveContext::updateIndexBinding()
{
	do
	{
		if (!m_state.vi.indexBuffer.defined())
		{
			break;
		}

		VkBuffer indexBuffer = m_state.vi.indexBuffer.handle();
		VkDeviceSize offset  = m_state.vi.indexBuffer.offset();
		VkIndexType type     = m_state.vi.indexType;
		m_cmd->cmdBindIndexBuffer(indexBuffer, offset, type);

		m_flags.clr(GveContextFlag::GpDirtyIndexBuffer);
	} while (false);
}

template <VkPipelineBindPoint BindPoint>
void GveContext::updateShaderResources(const GvePipelineLayout* pipelineLayout, VkDescriptorSet& set)
{

	uint32_t bindingCount = pipelineLayout->bindingCount();
	std::vector<VkWriteDescriptorSet> descriptorWrites;

	if (bindingCount != 0)
	{
		set = allocateDescriptorSet(pipelineLayout->descriptorSetLayout());
	}
	else
	{
		set = VK_NULL_HANDLE;
	}

	for (uint32_t i = 0; i != bindingCount; ++i)
	{
		auto binding     = pipelineLayout->binding(i);
		uint32_t regSlot = binding.resSlot.regSlot;
		auto res         = m_res[regSlot];

		switch (binding.resSlot.type)
		{
		case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
		{
			VkDescriptorBufferInfo bufferInfo = {};
			bufferInfo.buffer                 = res.buffer.handle();
			bufferInfo.offset                 = res.buffer.offset();
			bufferInfo.range                  = res.buffer.length();

			VkWriteDescriptorSet writeSet = {};
			writeSet.sType                = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			writeSet.dstSet               = set;
			writeSet.dstBinding           = i;
			writeSet.dstArrayElement      = 0;
			writeSet.descriptorType       = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			writeSet.descriptorCount      = 1;
			writeSet.pBufferInfo          = &bufferInfo;
			descriptorWrites.push_back(writeSet);
		}
			break;
		case VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE:
		{
			VkDescriptorImageInfo imageInfo = {};
			imageInfo.imageLayout           = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfo.imageView             = res.imageView->handle();
			imageInfo.sampler               = nullptr;

			VkWriteDescriptorSet writeSet = {};
			writeSet.sType                = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			writeSet.dstSet               = set;
			writeSet.dstBinding           = i;
			writeSet.dstArrayElement      = 0;
			writeSet.descriptorType       = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
			writeSet.descriptorCount      = 1;
			writeSet.pImageInfo           = &imageInfo;
			descriptorWrites.push_back(writeSet);
		}
			break;
		case VK_DESCRIPTOR_TYPE_SAMPLER:
		{
			VkDescriptorImageInfo imageInfo = {};
			imageInfo.imageLayout           = VK_IMAGE_LAYOUT_UNDEFINED;
			imageInfo.imageView             = VK_NULL_HANDLE;
			imageInfo.sampler               = res.sampler->handle();

			VkWriteDescriptorSet writeSet = {};
			writeSet.sType                = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			writeSet.dstSet               = set;
			writeSet.dstBinding           = i;
			writeSet.dstArrayElement      = 0;
			writeSet.descriptorType       = VK_DESCRIPTOR_TYPE_SAMPLER;
			writeSet.descriptorCount      = 1;
			writeSet.pImageInfo           = &imageInfo;
			descriptorWrites.push_back(writeSet);
		}
			break;
		default:
			break;
		}
	}

	// TODO:
	// Use vkUpdateDescriptorSetWithTemplate
	m_cmd->updateDescriptorSets(descriptorWrites.size(), descriptorWrites.data());
}

template <VkPipelineBindPoint BindPoint>
void GveContext::updateShaderDescriptorSetBinding(const GvePipelineLayout* layout, VkDescriptorSet set)
{
	VkPipelineLayout pipelineLayout = layout->pipelineLayout();

	m_cmd->cmdBindDescriptorSet(BindPoint, pipelineLayout, set, 0, nullptr);
}

VkDescriptorSet GveContext::allocateDescriptorSet(VkDescriptorSetLayout layout)
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

void GveContext::updateGraphicsShaderResources()
{
	if (m_flags.test(GveContextFlag::GpDirtyResources))
	{
		updateShaderResources<VK_PIPELINE_BIND_POINT_GRAPHICS>(
			m_state.gp.pipeline->getLayout(), 
			m_gpCtx.descSet);
	}

	updateShaderDescriptorSetBinding<VK_PIPELINE_BIND_POINT_GRAPHICS>(
		m_state.gp.pipeline->getLayout(),
		m_gpCtx.descSet);

	m_flags.clr(GveContextFlag::GpDirtyResources,
				GveContextFlag::GpDirtyDescriptorBinding);
}

void GveContext::updateComputeDescriptorLayout()
{
	m_flags.clr(GveContextFlag::CpDirtyDescriptorBinding);
}

void GveContext::updateGraphicsPipeline()
{
	// Descriptor layout is bound with shaders
	m_state.gp.pipeline = m_objects->pipelineManager().getGraphicsPipeline(m_state.gp.shaders);
	m_flags.clr(GveContextFlag::GpDirtyPipeline);
}

void GveContext::updateGraphicsPipelineStates()
{
	GveRenderPass* renderPass = m_state.om.framebuffer->getRenderPass();
	m_gpCtx.pipeline          = m_state.gp.pipeline->getPipelineHandle(m_state.gp.states, *renderPass);

	m_cmd->cmdBindPipeline(VK_PIPELINE_BIND_POINT_GRAPHICS, m_gpCtx.pipeline);

	m_flags.clr(GveContextFlag::GpDirtyPipelineState);
}

void GveContext::updateComputePipeline()
{
}

void GveContext::updateComputePipelineStates()
{
}

void GveContext::updateDynamicState()
{
	if (m_flags.test(GveContextFlag::GpDirtyViewport))
	{
		const auto& vp = m_state.dy.vp;

		m_cmd->cmdSetViewport(0, vp.count, vp.viewports.data());
		m_cmd->cmdSetScissor(0, vp.count, vp.scissors.data());

		// Update viewport count, this will be used to create pipeline.
		m_state.gp.states.dy.setViewportCount(vp.count);
		m_flags.clr(GveContextFlag::GpDirtyViewport);
	}
}

template <bool Indexed, bool Indirect>
void GveContext::commitGraphicsState()
{
	if (m_flags.test(GveContextFlag::GpDirtyPipeline))
	{
		updateGraphicsPipeline();
	}

	if (m_flags.test(GveContextFlag::GpDirtyFramebuffer))
	{
		updateFrameBuffer();
	}

	if (!m_flags.test(GveContextFlag::GpRenderPassBound))
	{
		beginRenderPass();
	}

	if (m_flags.test(GveContextFlag::GpDirtyVertexBuffers))
	{
		updateVertexBindings();
	}

	if (m_flags.test(GveContextFlag::GpDirtyIndexBuffer) && Indexed)
	{
		updateIndexBinding();
	}

	if (m_flags.any(
			GveContextFlag::GpDirtyViewport,
			GveContextFlag::GpDirtyBlendConstants,
			GveContextFlag::GpDirtyStencilRef,
			GveContextFlag::GpDirtyDepthBias,
			GveContextFlag::GpDirtyDepthBounds))
	{
		updateDynamicState();
	}

	if (m_flags.any(GveContextFlag::GpDirtyResources,
					GveContextFlag::GpDirtyDescriptorBinding))
	{
		updateGraphicsShaderResources();
	}

	if (m_flags.test(GveContextFlag::GpDirtyPipelineState))
	{
		updateGraphicsPipelineStates();
	}
}

void GveContext::commitComputeState()
{
}

}  // namespace gve