#include "GnmCommandBuffer.h"

#include "../Gve/GveDevice.h"
#include "../Gve/GveContext.h"
#include "../Gve/GveCmdList.h"
#include "../Gve/GveSwapChain.h"

#include "Platform/PlatformUtils.h"

GnmCommandBuffer::GnmCommandBuffer(
	const RcPtr<gve::GveDevice>&             device,
	const RcPtr<gve::GveContext>&            context,
	const RcPtr<gve::GveSwapChain>&          swapchain,
	const std::shared_ptr<sce::SceVideoOut>& videoOut):
	m_device(device),
	m_context(context),
	m_swapchain(swapchain),
	m_videoOut(videoOut),
	m_cmdList(nullptr)
{
}

GnmCommandBuffer::~GnmCommandBuffer()
{
}

void GnmCommandBuffer::recordBegin(uint32_t displayBufferIndex)
{
	m_displayBufferIndex = displayBufferIndex;
}

RcPtr<gve::GveCmdList> GnmCommandBuffer::recordEnd()
{
	return m_cmdList;
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
