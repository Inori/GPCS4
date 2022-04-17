#include "SceGpuQueue.h"

#include "Gnm/GnmCommandBufferDispatch.h"
#include "Gnm/GnmCommandBufferDraw.h"
#include "Gnm/GnmCommandBufferDummy.h"
#include "Gnm/GnmCommandProcessor.h"
#include "Violet/VltDevice.h"
#include "Violet/VltCmdList.h"

LOG_CHANNEL(Graphic.Sce.SceGpuQueue);

namespace sce
{
	using namespace Gnm;
	using namespace vlt;

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

	Rc<VltCommandList>
	SceGpuQueue::record(const SceGpuCommand& cmd)
	{
		return m_cp->processCommandBuffer(cmd.buffer, cmd.size);
	}

	void SceGpuQueue::submit(const SceGpuSubmission& submission)
	{
		m_device->submitCommandList(
			submission.cmdList,
			submission.wait,
			submission.wake);
	}

	void SceGpuQueue::synchronize()
	{
		m_device->syncSubmission();
	}

	void SceGpuQueue::createQueue(SceQueueType type)
	{
		m_cp = std::make_unique<GnmCommandProcessor>();

		if (type == SceQueueType::Graphics)
		{
			m_cmdProducer = std::make_unique<GnmCommandBufferDraw>(m_device);
		}
		else
		{
			m_cmdProducer = std::make_unique<GnmCommandBufferDispatch>(m_device);
		}

#ifdef GPCS4_NO_GRAPHICS
		m_cmdProducer = std::make_unique<GnmCommandBufferDummy>(m_device);
#endif

		m_cp->attachCommandBuffer(m_cmdProducer.get());
	}

}  // namespace sce