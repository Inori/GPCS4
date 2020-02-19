#include "SceGpuQueue.h"

#include "../Gnm/GnmCmdStream.h"
#include "../Gnm/GnmCommandBufferDraw.h"
#include "../Gnm/GnmCommandBufferDispatch.h"
#include "../Gve/GveDevice.h"
#include "../Gve/GveContext.h"

namespace sce
{;

SceGpuQueue::SceGpuQueue(
	const SceGpuQueueDevice& device,
	SceQueueType             type) :
	m_device(device)
{
	createQueue(type);
}

SceGpuQueue::~SceGpuQueue()
{
}

void SceGpuQueue::submit(void* cmd, uint32_t size)
{
}

void SceGpuQueue::createQueue(SceQueueType type)
{
	m_context = m_device.device->createContext();
	m_cmdParser = std::make_unique<GnmCmdStream>();

	if (type == SceQueueType::Graphics)
	{
		m_cmdProcesser = std::make_unique<GnmCommandBufferDraw>(m_device, m_context);
	}
	else
	{
		m_cmdProcesser = std::make_unique<GnmCommandBufferDispatch>(m_device, m_context);
	}
	
	m_cmdParser->attachCommandBuffer(m_cmdProcesser.get());
}

}  // namespace sce