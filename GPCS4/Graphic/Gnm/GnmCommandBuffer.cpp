#include "GnmCommandBuffer.h"
#include "../Gve/GveContext.h"
#include "../Gve/GveCmdList.h"
#include "../Gve/GveDevice.h"

#include "Platform/PlatformUtils.h"

GnmCommandBuffer::GnmCommandBuffer(const RcPtr<gve::GveDevice>& device):
	m_device(device),
	m_cmd(m_device ? m_device->createCmdList() : nullptr),
	m_context(m_device ? m_device->createContext() : nullptr)
{
}

GnmCommandBuffer::~GnmCommandBuffer()
{
}

RcPtr<gve::GveCmdList> GnmCommandBuffer::getCmdList()
{
	return m_cmd;
}

void GnmCommandBuffer::emuWriteGpuLabel(EventWriteSource selector, void* label, uint64_t value)
{
	if (selector == kEventWriteSource32BitsImmediate)
	{
		*(uint32_t*)label = value;
	}
	else if (selector == kEventWriteSource64BitsImmediate)
	{
		*(uint64_t*)label = value;
	}
	else
	{
		*(uint64_t*)label = UtilProcess::GetProcessTimeCounter();
	}
}
