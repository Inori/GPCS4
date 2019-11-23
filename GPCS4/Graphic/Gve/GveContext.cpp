#include "GveContext.h"
#include "GveCmdList.h"
#include "GveFrameBuffer.h"
#include "GveShader.h"
#include "GvePipelineManager.h"
#include "GveResourceManager.h"
#include "GveBuffer.h"
#include "GveImage.h"
#include "GveSampler.h"
#include "GveDevice.h"
#include "GveRenderPass.h"


namespace gve
{;


GveContex::GveContex(const RcPtr<GveDevice>& device) :
	m_device(device)
{
}

GveContex::~GveContex()
{

}

void GveContex::beginRecording(const RcPtr<GveCmdList>& commandBuffer)
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

RcPtr<GveCmdList> GveContex::endRecording()
{
	return std::exchange(m_cmd, nullptr);
}

void GveContex::setViewport(const VkViewport& viewport, const VkRect2D& scissorRect)
{
	setViewports(1, &viewport, &scissorRect);
}

void GveContex::setViewports(uint32_t viewportCount, const VkViewport* viewports, const VkRect2D* scissorRects)
{
	auto& vp = m_state.gp.states.vp;
	if (viewportCount != vp.viewportCount())
	{
		m_flags.set(GveContextFlag::GpDirtyPipelineState);
	}

	vp.clear();
	for (uint32_t i = 0; i != viewportCount; ++i)
	{
		vp.addViewport(viewports[i]);
		vp.addScissor(scissorRects[i]);
	}

	m_flags.set(GveContextFlag::GpDirtyViewport);
}

void GveContex::setVertexInputLayout(const GveVertexInputInfo& viState)
{
	m_state.gp.states.vi = viState;
	m_flags.set(GveContextFlag::GpDirtyPipelineState);
}

void GveContex::setInputAssemblyState(const GveInputAssemblyInfo& iaState)
{
	m_state.gp.states.ia = iaState;
	m_flags.set(GveContextFlag::GpDirtyPipelineState);
}

void GveContex::setRasterizerState(const GveRasterizationInfo& rsState)
{
	m_state.gp.states.rs = rsState;
	m_flags.set(GveContextFlag::GpDirtyPipelineState);
}

void GveContex::setMultiSampleState(const GveMultisampleInfo& msState)
{
	m_state.gp.states.ms = msState;
	m_flags.set(GveContextFlag::GpDirtyPipelineState);
}

void GveContex::setDepthStencilState(const GveDepthStencilInfo& dsState)
{
	m_state.gp.states.ds = dsState;
	m_flags.set(GveContextFlag::GpDirtyPipelineState);
}

void GveContex::setColorBlendState(const GveColorBlendInfo& blendCtl)
{
	m_flags.set(GveContextFlag::GpDirtyPipelineState);
}

void GveContex::bindRenderTargets(const GveRenderTargets& targets)
{
	m_state.om.framebuffer = m_device->createFrameBuffer(targets);

	m_flags.set(GveContextFlag::GpDirtyFramebuffer);
}

void GveContex::bindShader(VkShaderStageFlagBits stage, const RcPtr<GveShader>& shader)
{
	RcPtr<GveShader>* shaderStage;

	switch (stage) 
	{
		case VK_SHADER_STAGE_VERTEX_BIT:                  shaderStage = &m_state.gp.shaders.vs;  break;
		case VK_SHADER_STAGE_FRAGMENT_BIT:                shaderStage = &m_state.gp.shaders.fs;  break;
		//case VK_SHADER_STAGE_COMPUTE_BIT:                 shaderStage = &m_state.cp.shaders.cs;  break;
		//case VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT:    shaderStage = &m_state.gp.shaders.tcs; break;
		//case VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT: shaderStage = &m_state.gp.shaders.tes; break;
		//case VK_SHADER_STAGE_GEOMETRY_BIT:                shaderStage = &m_state.gp.shaders.gs;  break;
		default: return;
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

void GveContex::bindIndexBuffer(const RcPtr<GveBuffer>& buffer, VkIndexType indexType)
{
	m_state.vi.indexBuffer = buffer;
	m_state.vi.indexType = indexType;

	m_flags.set(GveContextFlag::GpDirtyIndexBuffer);
}

void GveContex::bindVertexBuffer(uint32_t binding, const RcPtr<GveBuffer>& buffer, uint32_t stride)
{
	m_state.vi.vertexBuffers[binding] = buffer;
	m_state.vi.vertexStrides[binding] = stride;

	m_flags.set(GveContextFlag::GpDirtyVertexBuffers);
}

void GveContex::bindSampler(uint32_t regSlot, const RcPtr<GveSampler>& sampler)
{
	m_res[regSlot].sampler = sampler;
	m_flags.set(GveContextFlag::GpDirtyResources);
}

void GveContex::bindResourceBuffer(uint32_t regSlot, const RcPtr<GveBuffer>& buffer)
{
	m_res[regSlot].buffer = buffer;
	m_flags.set(GveContextFlag::GpDirtyResources);

	// TODO:
	// We need one binding functions to set GpDirtyDescriptorBinding in order to update resource layout
	// currently I just set it here, but it should be fixed
	m_flags.set(GveContextFlag::GpDirtyDescriptorBinding);
}

void GveContex::bindResourceView(uint32_t regSlot, 
	const RcPtr<GveImageView>& imageView, 
	const RcPtr<GveBufferView>& bufferView)
{
	m_res[regSlot].imageView = imageView;
	m_res[regSlot].bufferView = bufferView;
	m_flags.set(GveContextFlag::GpDirtyResources);
}

void GveContex::drawIndex(uint32_t indexCount, uint32_t firstIndex)
{

}


void GveContex::copyBuffer(VkBuffer dstBuffer, VkBuffer srcBuffer, VkDeviceSize size)
{
	VkCommandBuffer commandBuffer = m_cmd->cmdBeginSingleTimeCommands();

	VkBufferCopy copyRegion = { 0 };
	copyRegion.size = size;
	m_cmd->cmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);
	auto queues = m_device->queues();
	m_cmd->cmdEndSingleTimeCommands(commandBuffer, queues.graphics.queueHandle);
}

void GveContex::copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height)
{
	VkCommandBuffer commandBuffer = m_cmd->cmdBeginSingleTimeCommands();

	VkBufferImageCopy region = {};
	region.bufferOffset = 0;
	region.bufferRowLength = 0;
	region.bufferImageHeight = 0;
	region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	region.imageSubresource.mipLevel = 0;
	region.imageSubresource.baseArrayLayer = 0;
	region.imageSubresource.layerCount = 1;
	region.imageOffset = { 0, 0, 0 };
	region.imageExtent = {
		width,
		height,
		1
	};

	vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

	auto queues = m_device->queues();
	m_cmd->cmdEndSingleTimeCommands(commandBuffer, queues.graphics.queueHandle);
}

void GveContex::updateBuffer(const RcPtr<GveBuffer>& buffer, 
	VkDeviceSize offset, VkDeviceSize size, const void* data)
{
	
}

void GveContex::updateImage(const RcPtr<GveImage>& buffer, VkDeviceSize offset, VkDeviceSize size, const void* data)
{

}

void GveContex::transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout)
{
	VkCommandBuffer commandBuffer = m_cmd->cmdBeginSingleTimeCommands();

	VkImageMemoryBarrier barrier = {};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = oldLayout;
	barrier.newLayout = newLayout;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = image;
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;

	VkPipelineStageFlags sourceStage;
	VkPipelineStageFlags destinationStage;

	if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) 
	{
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

		sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	}
	else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) 
	{
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
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
		1, &barrier
	);

	auto queues = m_device->queues();
	m_cmd->cmdEndSingleTimeCommands(commandBuffer, queues.graphics.queueHandle);
}

} // namespace gve