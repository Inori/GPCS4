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

} // namespace gve