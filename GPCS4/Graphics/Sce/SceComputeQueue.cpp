#include "SceComputeQueue.h"
#include "SceGpuQueue.h"
#include "Violet/VltDevice.h"
#include "Violet/VltCmdList.h"

namespace sce
{

	SceComputeQueue::SceComputeQueue(
		vlt::VltDevice* device,
		void*           ringBaseAddr,
		uint32_t        ringSizeInDW,
		void*           readPtrAddr):
		m_ringBegin(reinterpret_cast<uint32_t*>(ringBaseAddr)),
		m_ringEnd(m_ringBegin + ringSizeInDW),
		m_ringCmd(m_ringBegin),
		m_offsetPtr(reinterpret_cast<uint32_t*>(readPtrAddr)),
		m_queue(std::make_unique<SceGpuQueue>(device, SceQueueType::Compute))
	{
		*m_offsetPtr = 0;
	}

	SceComputeQueue::~SceComputeQueue()
	{
	}

	void SceComputeQueue::dingDong(uint32_t nextStartOffsetInDw)
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		uint32_t* nextCmd = m_ringBegin + nextStartOffsetInDw;

		if (nextCmd > m_ringCmd)
		{
			// Normal case,
			// execute command in range [m_ringCmd, nextCmd]
			SceGpuCommand cmd = {};
			cmd.buffer        = m_ringCmd;
			cmd.size          = static_cast<uint32_t>(nextCmd - m_ringCmd) * sizeof(uint32_t);
			m_queue->record(cmd);
		}
		else
		{
			// Ring buffer overlaps,
			// we need to execute commands in range [m_ringCmd, m_ringEnd] and [m_ringBegin, nextCmd].

			if (m_ringCmd != m_ringEnd)
			{
				SceGpuCommand cmd = {};
				cmd.buffer        = m_ringCmd;
				cmd.size          = static_cast<uint32_t>(m_ringEnd - m_ringCmd) * sizeof(uint32_t);
				m_queue->record(cmd);
			}

			if (m_ringBegin != nextCmd)
			{
				SceGpuCommand cmd = {};
				cmd.buffer        = m_ringBegin;
				cmd.size          = static_cast<uint32_t>(nextCmd - m_ringBegin) * sizeof(uint32_t);
				m_queue->record(cmd);
			}
		}

		submitCommand();

		m_ringCmd    = nextCmd;
		*m_offsetPtr = static_cast<uint32_t>(m_ringCmd - m_ringBegin);
	}

	void SceComputeQueue::submitCommand()
	{
		SceGpuSubmission submission = {};
		submission.wait             = VK_NULL_HANDLE;
		submission.wake             = VK_NULL_HANDLE;
		m_queue->submit(submission);
	}

}  // namespace sce