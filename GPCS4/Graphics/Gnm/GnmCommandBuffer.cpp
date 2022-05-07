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

	Rc<VltCommandList> GnmCommandBuffer::finalize()
	{
		return m_context->endRecording();
	}

	void GnmCommandBuffer::initializeDefaultHardwareState()
	{
		m_tracker = &(GPU().resourceTracker());
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