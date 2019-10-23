#include "GveContext.h"
#include "GveCommandBuffer.h"
#include "GveFrameBuffer.h"
#include "GveShader.h"


namespace gve
{;

GveContex::GveContex(const RcPtr<GveDevice>& device):
	m_device(device)
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

}

void GveContex::setInputLayout(uint32_t attributeCount, const VkVertexInputAttributeDescription* attributes, uint32_t bindingCount, const VkVertexInputBindingDescription* bindings)
{

}

void GveContex::bindShader(VkShaderStageFlagBits stage, const RcPtr<GveShader>& shader)
{

}

void GveContex::bindIndexBuffer(const GveBuffer& buffer, VkIndexType indexType)
{

}

void GveContex::bindVertexBuffer(uint32_t binding, const GveBuffer& buffer, uint32_t stride)
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

} // namespace gve