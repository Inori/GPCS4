#pragma once

#include "SceCommon.h"

#include <memory>
#include <mutex>
#include <vector>

namespace sce
{
	namespace vlt
	{
		class VltDevice;
	}  // namespace vlt

	class SceGpuQueue;
	class SceObjects;

	/**
	 * \brief Compute queue
	 * 
	 * Manage a ring buffer for compute command
	 * and submit to gpu queue.
	 */
	class SceComputeQueue
	{
	public:
		SceComputeQueue(vlt::VltDevice* device,
						SceObjects&     objects,
						void*           ringBaseAddr,
						uint32_t        ringSizeInDW,
						void*           readPtrAddr);
		~SceComputeQueue();

		void dingDong(uint32_t nextStartOffsetInDw);

	private:
		void submitCommand();

	private:
		uint32_t* m_ringBegin;
		uint32_t* m_ringEnd;
		uint32_t* m_ringCmd;
		uint32_t* m_offsetPtr;

		std::mutex                   m_mutex;
		std::unique_ptr<SceGpuQueue> m_queue;
	};


}  // namespace sce