#include "GnmGpuLabel.h"
#include "Violet/VltDevice.h"
#include "Violet/VltContext.h"
#include "Violet/VltSemaphore.h"

using namespace sce::vlt;

LOG_CHANNEL(Graphic.Gnm.GnmGpuLabel);

namespace sce::Gnm
{
	GnmGpuLabel::GnmGpuLabel(vlt::VltDevice* device,
							 void*           label) :
		m_device(device),
		m_label(label)
	{
	}

	GnmGpuLabel::~GnmGpuLabel()
	{
	}

	void GnmGpuLabel::set(uint64_t value)
	{
		// Currently not used.
	}

	void GnmGpuLabel::write(
		VltContext*         context,
		ReleaseMemEventType eventType,
		EventWriteSource    srcSelector,
		uint64_t            immValue)
	{
		LOG_ASSERT(m_semaphore == nullptr, "label is used repeatedly");

		VltSemaphoreCreateInfo info;
		info.semaphoreType = VK_SEMAPHORE_TYPE_TIMELINE;
		info.initialValue  = 0;
		m_semaphore        = m_device->createSemaphore(info);

		VltSemaphoreSubmission submission;
		submission.semaphore = m_semaphore;
		submission.stageMask = eventType == kReleaseMemEventCsDone
								   ? VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT
								   : VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT;
		submission.value     = immValue;

		// Queue the semaphore
		context->signalSemaphore(submission);
		 
		// Asynchronously set label value upon semaphore is signaled.
		// Record the returned future so that when the class is destructed,
		// we can make sure the label has been updated.
		m_future = std::async(std::launch::async, [this, srcSelector, immValue]()
			{ 
				m_semaphore->wait(immValue);

				if (srcSelector == kEventWriteSource32BitsImmediate)
					*reinterpret_cast<uint32_t*>(m_label) = immValue;
				else if (srcSelector == kEventWriteSource64BitsImmediate)
					*reinterpret_cast<uint64_t*>(m_label) = immValue; 
			});
	}

	void GnmGpuLabel::writeWithInterrupt(
		VltContext*         context,
		ReleaseMemEventType eventType,
		EventWriteSource    srcSelector,
		uint64_t            immValue)
	{
	}

	void GnmGpuLabel::wait(
		VltContext*     context,
		uint32_t        mask,
		WaitCompareFunc compareFunc,
		uint32_t        refValue)
	{
		// In Gnm, writeReleaseMemEvent must be called before waitOnAddress,
		// or the GPU will be blocked.
		LOG_ASSERT(m_semaphore != nullptr, "write is not called before wait.");
		LOG_ASSERT(compareFunc == kWaitCompareFuncEqual, "Only equal compareFunc is supported yet.");

		VltSemaphoreSubmission submission;
		submission.semaphore = m_semaphore;
		submission.stageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT;
		submission.value     = refValue & mask;
		context->waitSemaphore(submission);
	}

}  // namespace sce::Gnm