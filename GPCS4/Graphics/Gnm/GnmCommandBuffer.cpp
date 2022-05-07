#include "GnmCommandBuffer.h"
#include "Emulator.h"
#include "VirtualGPU.h"
#include "PlatProcess.h"

#include "Violet/VltCmdList.h"
#include "Violet/VltDevice.h"
#include "Sce/SceGpuQueue.h"

using namespace sce::vlt;

namespace sce::Gnm
{

	GnmCommandBuffer::GnmCommandBuffer(vlt::VltDevice* device) :
		m_device(device),
		m_factory(device)
	{
	}

	GnmCommandBuffer::~GnmCommandBuffer()
	{
	}

	void GnmCommandBuffer::beginRecording()
	{
		auto queueType = m_queueType == SceQueueType::Graphics
							 ? VltQueueType::Graphics
							 : VltQueueType::Compute;

		m_tracker     = &(GPU().resourceTracker());
		m_initializer = std::make_unique<GnmInitializer>(m_device, queueType);
		m_context     = m_device->createContext();

		m_context->beginRecording(
			m_device->createCommandList(queueType));
	}

	vlt::Rc<vlt::VltCommandList>
	GnmCommandBuffer::endRecording()
	{
		return m_context->endRecording();
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
				*(uint64_t*)label = plat::GetProcessTimeCounter();
			}

		} while (false);
	}

}  // namespace sce::Gnm