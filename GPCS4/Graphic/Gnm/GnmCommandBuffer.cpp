#include "GnmCommandBuffer.h"
#include "../Gve/GveCommandBuffer.h"
#include "../Gve/GveDevice.h"

GnmCommandBuffer::GnmCommandBuffer(const RcPtr<gve::GveDevice>& device, 
	const RcPtr<gve::GveContex>& context,
	gve::GveResourceManager* resMgr):
	m_device(device),
	m_context(context),
	m_resourceManager(resMgr)
{
	m_cmd = new gve::GveCommandBuffer(m_device);
}

GnmCommandBuffer::~GnmCommandBuffer()
{
}

RcPtr<gve::GveCommandBuffer> GnmCommandBuffer::getCmdBuffer()
{
	return m_cmd;
}
