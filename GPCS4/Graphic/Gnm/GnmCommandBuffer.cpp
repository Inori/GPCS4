#include "GnmCommandBuffer.h"

#include "../Gve/GveDevice.h"
#include "../Gve/GveContext.h"
#include "../Gve/GveCmdList.h"

#include "../Sce/SceVideoOut.h"
#include "../Sce/ScePresenter.h"
#include "../Sce/SceGpuQueue.h"


#include "Platform/PlatformUtils.h"

GnmCommandBuffer::GnmCommandBuffer(
	const sce::SceGpuQueueDevice& device,
	const RcPtr<gve::GveContext>& context) :
	m_device(device.device),
	m_context(context),
	m_presenter(device.presenter),
	m_videoOut(device.videoOut),
	m_cmdList(nullptr)
{
}

GnmCommandBuffer::GnmCommandBuffer(const sce::SceGpuQueueDevice& device, const RcPtr<gve::GveContext>& context)
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
