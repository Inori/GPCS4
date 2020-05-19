#include "GnmCommandBuffer.h"

#include "../Violet/VltDevice.h"
#include "../Violet/VltContext.h"
#include "../Violet/VltCmdList.h"
#include "../Violet/VltPresenter.h"
#include "../Sce/SceVideoOut.h"
#include "../Sce/SceGpuQueue.h"


#include "Platform/PlatformUtils.h"

GnmCommandBuffer::GnmCommandBuffer(
	const sce::SceGpuQueueDevice& device,
	const RcPtr<vlt::VltContext>& context) :
	m_device(device.device),
	m_context(context),
	m_presenter(device.presenter),
	m_videoOut(device.videoOut),
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

RcPtr<vlt::VltCmdList> GnmCommandBuffer::recordEnd()
{
	return std::exchange(m_cmdList, nullptr);
}

void GnmCommandBuffer::emuWriteGpuLabel(EventWriteSource selector, void* label, uint64_t value)
{
	do 
	{
		if (!label)
		{
			break;
		}

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

	} while (false);
}
