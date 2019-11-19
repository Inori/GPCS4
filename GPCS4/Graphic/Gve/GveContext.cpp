#include "GveContext.h"
#include "GveCommandBuffer.h"
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

VkPipeline GveContex::s_pipeline = VK_NULL_HANDLE;
GvePipelineLayout* GveContex::s_layout = nullptr;
RcPtr<gve::GveDescriptorPool> GveContex::s_descPool;

GveContex::GveContex(const RcPtr<GveDevice>& device, const GveContextParam& param) :
	m_device(device),
	m_pipeMgr(param.pipeMgr),
	m_renderPass(param.renderPass)
{
}

GveContex::~GveContex()
{

}

void GveContex::beginRecording(const RcPtr<GveCommandBuffer>& commandBuffer)
{
	m_cmd = commandBuffer;
	m_cmd->beginRecording();
}

void GveContex::endRecording()
{
	do 
	{
		if (!m_cmd)
		{
			break;
		}

		m_cmd->endRecording();
	} while (false);
	
}

void GveContex::setViewport(const VkViewport& viewport, const VkRect2D& scissorRect)
{
	setViewports(1, &viewport, &scissorRect);
}

void GveContex::setViewports(uint32_t viewportCount, const VkViewport* viewports, const VkRect2D* scissorRects)
{

}

void GveContex::setInputLayout(uint32_t attributeCount, const VkVertexInputAttributeDescription* attributes, 
	uint32_t bindingCount, const VkVertexInputBindingDescription* bindings)
{

}

void GveContex::setPrimitiveType(VkPrimitiveTopology topology)
{

}

void GveContex::setRasterizerState(const GveRasterizerState& state)
{

}

void GveContex::setMultiSampleState(const GveMultisampleState& state)
{

}

void GveContex::setBlendControl(const GveBlendControl& blendCtl)
{

}

void GveContex::bindRenderTargets(const GveRenderTargets& target)
{

}

void GveContex::bindShader(VkShaderStageFlagBits stage, const RcPtr<GveShader>& shader)
{

}

void GveContex::bindIndexBuffer(const RcPtr<GveBuffer>& buffer, VkIndexType indexType)
{

}

void GveContex::bindVertexBuffer(uint32_t binding, const RcPtr<GveBuffer>& buffer, uint32_t stride)
{

}

void GveContex::bindSampler(uint32_t regSlot, const RcPtr<GveSampler>& sampler)
{

}

void GveContex::bindResourceBuffer(uint32_t regSlot, const RcPtr<GveBuffer>& buffer)
{
	
}

void GveContex::bindResourceView(uint32_t regSlot, 
	const RcPtr<GveImageView>& imageView, 
	const RcPtr<GveBufferView>& bufferView)
{

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