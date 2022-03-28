#include "SceGpuQueue.h"

#include "Gnm/GnmCommandBufferDispatch.h"
#include "Gnm/GnmCommandBufferDraw.h"
#include "Gnm/GnmCommandBufferDummy.h"
#include "Gnm/GnmCommandProcessor.h"
#include "Violet/VltDevice.h"

LOG_CHANNEL(Graphic.Sce.SceGpuQueue);

namespace sce
{
	using namespace Gnm;

	SceGpuQueue::SceGpuQueue(
		vlt::VltDevice* device,
		SceQueueType    type) :
		m_device(device)
	{
		createQueue(type);
	}

	SceGpuQueue::~SceGpuQueue()
	{
	}

	void SceGpuQueue::record(const SceGpuCommand& cmd)
	{
		bool result = m_cp->processCommandBuffer(cmd.buffer, cmd.size);
		LOG_ERR_IF(result == false, "process command buffer failed.");

		// return m_cmdProcesser->recordEnd();
	}

	void SceGpuQueue::submit(const SceGpuSubmissionSync& sync)
	{
		//VltSubmitInfo submitInfo = {};
		//submitInfo.cmdList       = submission.cmdList;
		//submitInfo.waitSync      = submission.wait;
		//submitInfo.wakeSync      = submission.wake;

		//m_device.device->submitCommandList(submitInfo);
	}

	void SceGpuQueue::createQueue(SceQueueType type)
	{
		m_cp = std::make_unique<GnmCommandProcessor>();

		if (type == SceQueueType::Graphics)
		{
			m_cmdProducer = std::make_unique<GnmCommandBufferDraw>();
		}
		else
		{
			m_cmdProducer = std::make_unique<GnmCommandBufferDispatch>();
		}

#ifdef GPCS4_NO_GRAPHICS
		m_cmdProducer = std::make_unique<GnmCommandBufferDummy>();
#endif

		m_cp->attachCommandBuffer(m_cmdProducer.get());
	}

}  // namespace sce