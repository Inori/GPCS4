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
	m_cmd->endRecording();
}

void GveContex::setViewport(const VkViewport& viewport, const VkRect2D& scissorRect)
{
	setViewports(1, &viewport, &scissorRect);
}

void GveContex::setViewports(uint32_t viewportCount, const VkViewport* viewports, const VkRect2D* scissorRects)
{
	m_state.viewports.clear();
	m_state.viewports.assign(viewports, viewports + viewportCount);

	m_state.scissors.clear();
	m_state.scissors.assign(scissorRects, scissorRects + viewportCount);
}

void GveContex::setInputLayout(uint32_t attributeCount, const VkVertexInputAttributeDescription* attributes, 
	uint32_t bindingCount, const VkVertexInputBindingDescription* bindings)
{
	memset(&m_state.vi.state, 0, sizeof(m_state.vi.state));
	m_state.vi.state.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	m_state.vi.state.pVertexAttributeDescriptions = attributes;
	m_state.vi.state.vertexAttributeDescriptionCount = attributeCount;
	m_state.vi.state.pVertexBindingDescriptions = bindings;
	m_state.vi.state.vertexBindingDescriptionCount = bindingCount;
}

void GveContex::setPrimitiveType(VkPrimitiveTopology topology)
{
	memset(&m_state.ia, 0, sizeof(m_state.ia));
	m_state.ia.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	m_state.ia.pNext = nullptr;
	m_state.ia.topology = topology;
	m_state.ia.primitiveRestartEnable = VK_FALSE;
}

void GveContex::setRasterizerState(const GveRasterizerState& state)
{
	memset(&m_state.rs, 0, sizeof(m_state.rs));
	m_state.rs.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	m_state.rs.depthClampEnable = state.depthClampEnable;
	m_state.rs.rasterizerDiscardEnable = VK_FALSE;
	m_state.rs.polygonMode = state.polygonMode;
	m_state.rs.lineWidth = 1.0f;
	m_state.rs.cullMode = state.cullMode;
	m_state.rs.frontFace = state.frontFace;
	m_state.rs.depthBiasEnable = state.depthBiasEnable;
}

void GveContex::setMultiSampleState(const GveMultisampleState& state)
{
	memset(&m_state.ms, 0, sizeof(m_state.ms));
	m_state.ms.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	m_state.ms.rasterizationSamples = static_cast<VkSampleCountFlagBits>(state.sampleCount);
	m_state.ms.sampleShadingEnable = state.sampleShadingEnable;
}

void GveContex::setBlendControl(const GveBlendControl& blendCtl)
{
	memset(&m_state.cba, 0, sizeof(m_state.cba));
	memset(&m_state.cb, 0, sizeof(m_state.cb));
	m_state.cba.blendEnable = blendCtl.enableBlending;
	m_state.cba.colorWriteMask = blendCtl.writeMask;
	m_state.cba.srcColorBlendFactor = blendCtl.colorSrcFactor;
	m_state.cba.dstColorBlendFactor = blendCtl.colorDstFactor;
	m_state.cba.colorBlendOp = blendCtl.colorBlendOp;
	m_state.cba.srcAlphaBlendFactor = blendCtl.alphaSrcFactor;
	m_state.cba.dstAlphaBlendFactor = blendCtl.alphaDstFactor;
	m_state.cba.alphaBlendOp = blendCtl.alphaBlendOp;

	m_state.cb.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	m_state.cb.logicOpEnable = VK_FALSE;
	m_state.cb.logicOp = VK_LOGIC_OP_COPY;
	m_state.cb.attachmentCount = 1;
	m_state.cb.pAttachments = &m_state.cba;
	m_state.cb.blendConstants[0] = 0.0f;
	m_state.cb.blendConstants[1] = 0.0f;
	m_state.cb.blendConstants[2] = 0.0f;
	m_state.cb.blendConstants[3] = 0.0f;
}

void GveContex::bindRenderTargets(const GveRenderTarget& target)
{
	m_renderTarget = target;
}

void GveContex::bindShader(VkShaderStageFlagBits stage, const RcPtr<GveShader>& shader)
{
	switch (stage)
	{
	case VK_SHADER_STAGE_VERTEX_BIT:
		m_shaders.vs = shader;
		break;
	case VK_SHADER_STAGE_FRAGMENT_BIT:
		m_shaders.fs = shader;
		break;
	default:
		break;
	}
}

void GveContex::bindIndexBuffer(const RcPtr<GveBuffer>& buffer, VkIndexType indexType)
{
	m_state.vi.indexBuffer = buffer;
	m_state.vi.indexType = indexType;
}

void GveContex::bindVertexBuffer(uint32_t binding, const RcPtr<GveBuffer>& buffer, uint32_t stride)
{
	m_state.vi.vertexBuffers[binding] = buffer;
	m_state.vi.vertexStrides[binding] = stride;
}

void GveContex::bindSampler(uint32_t regSlot, const RcPtr<GveSampler>& sampler)
{
	m_res[regSlot].sampler = sampler;
}

void GveContex::bindResourceBuffer(uint32_t regSlot, const RcPtr<GveBuffer>& buffer)
{
	m_res[regSlot].buffer = buffer;
}

void GveContex::bindResourceView(uint32_t regSlot, 
	const RcPtr<GveImageView>& imageView, 
	const RcPtr<GveBufferView>& bufferView)
{
	m_res[regSlot].imageView = imageView;
	m_res[regSlot].bufferView = bufferView;
}

void GveContex::drawIndex(uint32_t indexCount, uint32_t firstIndex)
{
	if (s_pipeline == VK_NULL_HANDLE)
	{
		auto pipeline = m_pipeMgr->getGraphicsPipeline(m_shaders);
		s_pipeline = pipeline->getPipelineHandle(m_state, *m_renderPass);
		s_layout = pipeline->getLayout();
		s_descPool = m_device->createDescriptorPool();
	}

	if (m_descSet == VK_NULL_HANDLE)
	{
		m_descSet = s_descPool->alloc(s_layout->descriptorSetLayout());
		uint32_t bindingCount = s_layout->bindingCount();
		std::vector<VkWriteDescriptorSet> descriptorWrites;
		for (uint32_t i = 0; i != bindingCount; ++i)
		{
			auto binding = s_layout->binding(i);
			uint32_t regSlot = binding.resSlot.regSlot;
			auto res = m_res[regSlot];
			switch (binding.resSlot.type)
			{
			case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
			{
				VkDescriptorBufferInfo bufferInfo = {};
				bufferInfo.buffer = res.buffer->handle();
				bufferInfo.offset = 0;
				bufferInfo.range = res.buffer->size();

				VkWriteDescriptorSet writeSet = {};
				writeSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				writeSet.dstSet = m_descSet;
				writeSet.dstBinding = i;
				writeSet.dstArrayElement = 0;
				writeSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				writeSet.descriptorCount = 1;
				writeSet.pBufferInfo = &bufferInfo;
				descriptorWrites.push_back(writeSet);
			}
				break;
			case VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE:
			{
				VkDescriptorImageInfo imageInfo = {};
				imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				imageInfo.imageView = res.imageView->handle();
				imageInfo.sampler = nullptr;

				VkWriteDescriptorSet writeSet = {};
				writeSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				writeSet.dstSet = m_descSet;
				writeSet.dstBinding = i;
				writeSet.dstArrayElement = 0;
				writeSet.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
				writeSet.descriptorCount = 1;
				writeSet.pImageInfo = &imageInfo;
				descriptorWrites.push_back(writeSet);
			}
				break;
			case VK_DESCRIPTOR_TYPE_SAMPLER:
			{
				VkDescriptorImageInfo imageInfo = {};
				imageInfo.imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				imageInfo.imageView = VK_NULL_HANDLE;
				imageInfo.sampler = res.sampler->handle();

				VkWriteDescriptorSet writeSet = {};
				writeSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				writeSet.dstSet = m_descSet;
				writeSet.dstBinding = i;
				writeSet.dstArrayElement = 0;
				writeSet.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
				writeSet.descriptorCount = 1;
				writeSet.pImageInfo = &imageInfo;
				descriptorWrites.push_back(writeSet);
			}
				break;
			default:
				break;
			}
		}

		vkUpdateDescriptorSets(*m_device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}


	VkRenderPassBeginInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = m_renderPass->handle();
	renderPassInfo.framebuffer = m_renderTarget.frameBuffer->handle();
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = m_renderTarget.frameBuffer->extent();
	VkClearValue clearColor = { 0.5f, 0.5f, 0.5f, 1.0f };
	renderPassInfo.clearValueCount = 1;
	renderPassInfo.pClearValues = &clearColor;

	m_cmd->cmdBeginRenderPass(&renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	m_cmd->cmdBindPipeline(VK_PIPELINE_BIND_POINT_GRAPHICS, s_pipeline);

	VkBuffer vertexBuffers[] = { m_state.vi.vertexBuffers[0]->handle() };
	VkDeviceSize offsets[] = { 0 };
	m_cmd->cmdBindVertexBuffers(0, 1, vertexBuffers, offsets);

	m_cmd->cmdBindIndexBuffer(m_state.vi.indexBuffer->handle(), 0, m_state.vi.indexType);

	m_cmd->cmdBindDescriptorSet(VK_PIPELINE_BIND_POINT_GRAPHICS, s_layout->pipelineLayout(), m_descSet, 0, nullptr);

	m_cmd->cmdDrawIndexed(m_state.vi.indexBuffer->size()/2, 1, 0, 0, 0);

	m_cmd->cmdEndRenderPass();
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

	if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

		sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	}
	else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	}
	else {
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