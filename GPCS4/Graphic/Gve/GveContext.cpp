#include "GveContext.h"
#include "GveCommandBuffer.h"
#include "GveFrameBuffer.h"
#include "GveShader.h"


namespace gve
{;

GveContex::GveContex(const RcPtr<GveDevice>& device):
	m_device(device)
{
	m_command = m_device->createCommandBuffer();
}

GveContex::~GveContex()
{

}

void GveContex::initState()
{
	m_command->beginRecording();
}

void GveContex::submit()
{
	m_command->endRecording();
}

void GveContex::bindShader(VkShaderStageFlagBits stage, const RcPtr<GveShader>& shader)
{

}

} // namespace gve