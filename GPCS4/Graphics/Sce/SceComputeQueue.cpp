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
		m_linearBuffer.resize(ringSizeInDW);
	}

	SceComputeQueue::~SceComputeQueue()
	{
	}

	void SceComputeQueue::dingDong(uint32_t nextStartOffsetInDw)
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		uint32_t* nextCmd = m_ringBegin + nextStartOffsetInDw;

		const void* command        = nullptr;
		uint32_t    cmdSizeInBytes = 0;

		if (nextCmd > m_ringCmd)
		{
			// Normal case,
			// execute command in range [m_ringCmd, nextCmd]
			command        = m_ringCmd;
			cmdSizeInBytes = static_cast<uint32_t>(nextCmd - m_ringCmd) * sizeof(uint32_t);
		}
		else
		{
			// Ring buffer overlaps,
			// we need to execute commands in range [m_ringCmd, m_ringEnd] and [m_ringBegin, nextCmd].
			// In such case, we copy the buffer into the cache making it linear.

			uint32_t cmdSizeInDwords = 0;
			if (m_ringCmd != m_ringEnd)
			{
				cmdSizeInDwords = static_cast<uint32_t>(m_ringEnd - m_ringCmd);
				std::memcpy(m_linearBuffer.data(), m_ringCmd, cmdSizeInDwords * sizeof(uint32_t));
			}

			if (m_ringBegin != nextCmd)
			{
				uint32_t sizeBeforeInDwords = static_cast<uint32_t>(nextCmd - m_ringBegin);
				std::memcpy(&m_linearBuffer[cmdSizeInDwords], m_ringBegin, sizeBeforeInDwords * sizeof(uint32_t));
				cmdSizeInDwords += sizeBeforeInDwords;
			}

			command        = m_linearBuffer.data();
			cmdSizeInBytes = cmdSizeInDwords * sizeof(uint32_t);
		}

		submitCommand(command, cmdSizeInBytes);

		m_ringCmd    = nextCmd;
		*m_offsetPtr = static_cast<uint32_t>(m_ringCmd - m_ringBegin);
	}

	void SceComputeQueue::submitCommand(const void* command, uint32_t size)
	{
		SceGpuCommand cmd = {};
		cmd.buffer        = command;
		cmd.size          = size;
		auto cmdList      = m_queue->record(cmd);

		SceGpuSubmission submission = {};
		submission.cmdList          = cmdList;
		submission.wait             = VK_NULL_HANDLE;
		submission.wake             = VK_NULL_HANDLE;
		m_queue->submit(submission);
	}

}  // namespace sce