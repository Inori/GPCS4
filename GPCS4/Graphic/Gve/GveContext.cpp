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

void GveContex::bindShader(VkShaderStageFlagBits stage, const RcPtr<GveShader>& shader)
{

}

} // namespace gve