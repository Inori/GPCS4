#include "GveContext.h"
#include "GveCommandBuffer.h"
#include "GveFrameBuffer.h"



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

} // namespace gve