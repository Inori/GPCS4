#include "SceGpuQueue.h"

#include "../Gnm/GnmCmdStream.h"
#include "../Gnm/GnmCommandBufferDraw.h"
#include "../Gnm/GnmCommandBufferDispatch.h"
#include "../Gve/GveDevice.h"
#include "../Gve/GveContext.h"
#include "../Gve/GveCmdList.h"
#include "../Gve/GvePresenter.h"

LOG_CHANNEL(Graphic.Sce.SceGpuQueue);

using namespace gve;

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


RcPtr<gve::GveCmdList> SceGpuQueue::record(
	const SceGpuCommand& cmd,
	uint32_t             displayBufferIndex)
{
	m_cmdProcesser->recordBegin(displayBufferIndex);

	bool result = m_cmdParser->processCommandBuffer(cmd.buffer, cmd.size);
	LOG_ERR_IF(result == false, "process command buffer failed.");

	return m_cmdProcesser->recordEnd();
}

void SceGpuQueue::submit(const SceGpuSubmission& submission)
{
	GveSubmitInfo submitInfo = {};
	submitInfo.cmdList       = submission.cmdList;
	submitInfo.waitSync      = submission.wait;
	submitInfo.wakeSync      = submission.wake;

	m_device.device->submitCommandList(submitInfo);
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