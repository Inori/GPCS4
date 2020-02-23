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
	renderPassUnbindFramebuffer();

	m_cmd->endRecording();

	// m_stagingAlloc->trim();

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

void GveContext::setVertexInputState(const GveVertexInputInfo& viState)
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


void GveContext::copyBuffer(
	const RcPtr<GveBuffer>& dstBuffer,
	VkDeviceSize            dstOffset,
	const RcPtr<GveBuffer>& srcBuffer,
	VkDeviceSize            srcOffset,
	VkDeviceSize            numBytes)
{
	leaveRenderPassScope();
	
	auto         srcSlice = srcBuffer->slice();
	auto         dstSlice = dstBuffer->slice();

	VkBufferCopy region = { 0 };
	region.srcOffset    = srcOffset;
	region.dstOffset    = dstOffset;
	region.size         = numBytes;
	m_cmd->cmdCopyBuffer(GveCmdType::ExecBuffer, srcSlice.buffer, dstSlice.buffer, 1, &region);

	auto            buffInfo = dstBuffer->info();
	VkMemoryBarrier barrier = {};
	barrier.sType           = VK_STRUCTURE_TYPE_MEMORY_BARRIER;
	barrier.pNext           = nullptr;
	barrier.srcAccessMask   = VK_ACCESS_TRANSFER_WRITE_BIT;
	barrier.dstAccessMask    = buffInfo.access;

	//m_cmd->cmdPipelineBarrier(
	//	GveCmdType::ExecBuffer,
	//	VK_PIPELINE_STAGE_TRANSFER_BIT, buffInfo.stages,
	//	0,
	//	1, &barrier,
	//	0, nullptr,
	//	0, nullptr);

	fullPipelineBarrier();
}

void GveContext::copyBufferToImage(
	const RcPtr<GveImage>&   dstImage,
	VkImageSubresourceLayers dstSubresource,
	VkOffset3D               dstOffset,
	VkExtent3D               dstExtent,
	const RcPtr<GveBuffer>&  srcBuffer,
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

	auto srcSlice = srcBuffer->slice();
	VkImageLayout dstImageLayoutTransfer = dstImage->pickLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	m_cmd->cmdCopyBufferToImage(GveCmdType::ExecBuffer, srcSlice.buffer, dstImage->handle(), dstImageLayoutTransfer, 1, &region);
}

void GveContext::updateBuffer(const RcPtr<GveBuffer>& buffer,
							  VkDeviceSize            offset,
							  VkDeviceSize            size,
							  const void*             data)
{
	leaveRenderPassScope();

	auto  stagingSlice = m_stagingAlloc->alloc(size, CACHE_LINE_SIZE);
	void* stagingData  = stagingSlice.mapPtr(0);
	std::memcpy(stagingData, data, size);

	copyBuffer(buffer, offset, stagingSlice.buffer(), 0, size);
}

void GveContext::updateImage(
	const RcPtr<GveImage>&          image,
	const VkImageSubresourceLayers& subresources,
	VkOffset3D                      imageOffset,
	VkExtent3D                      imageExtent,
	const void*                     data,
	VkDeviceSize                    size)
{
	leaveRenderPassScope();

	auto  stagingSlice = m_stagingAlloc->alloc(size, CACHE_LINE_SIZE);
	void* stagingData  = stagingSlice.mapPtr(0);
	std::memcpy(stagingData, data, size);

	auto          imgInfo   = image->info();

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
}

void GveContext::uploadBuffer(
	const RcPtr<GveBuffer>& buffer,
	const void*             data)
{
}

void GveContext::uploadImage(
	const RcPtr<GveImage>&          image,
	const VkImageSubresourceLayers& subresources,
	const void*                     data,
	VkDeviceSize                    pitchPerRow,
	VkDeviceSize                    pitchPerLayer)
{
}

void GveContext::transitionImageLayout(
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
		GveCmdType::ExecBuffer,
		srcStage, dstStage, 
		0, 
		0, nullptr, 
		0, nullptr, 
		1, &barrier);
}

void GveContext::fullPipelineBarrier()
{

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
		GveCmdType::ExecBuffer,
		VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
		0,
		1, &barrier,
		0, nullptr,
		0, nullptr);
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

void GveContext::renderPassBindFramebuffer()
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

void GveContext::renderPassUnbindFramebuffer()
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

		buffers[bindingCount] = buffer.buffer()->slice().buffer;
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

		VkBuffer indexBuffer = m_state.vi.indexBuffer.buffer()->slice().buffer;
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
			bufferInfo.buffer                 = res.buffer.slice().buffer;
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

void GveContext::enterRenderPassScope()
{
	if (!m_flags.test(GveContextFlag::GpRenderPassBound))
	{
		renderPassBindFramebuffer();
	}
}

void GveContext::leaveRenderPassScope()
{
	if (m_flags.test(GveContextFlag::GpRenderPassBound))
	{
		renderPassUnbindFramebuffer();
	}
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
	if (m_flags.test(GveContextFlag::GpDirtyFramebuffer))
	{
		updateFrameBuffer();
	}

	if (!m_flags.test(GveContextFlag::GpRenderPassBound))
	{
		enterRenderPassScope();
	}

	if (m_flags.test(GveContextFlag::GpDirtyPipeline))
	{
		updateGraphicsPipeline();
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