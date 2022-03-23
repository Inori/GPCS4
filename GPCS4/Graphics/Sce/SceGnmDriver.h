#pragma once

#include "SceCommon.h"
#include "SceVideoOut.h"

#include <array>
#include <memory>


namespace sce
{;

class SceVideoOut;
class SceGpuQueue;

// Valid vqueue id should be positive value.
constexpr uint32_t VQueueIdBegin        = 1;
constexpr uint32_t MaxPipeId            = 7;  // Some docs say it should be 7, others say it should be 3. Fuck that.
constexpr uint32_t MaxQueueId           = 8;
constexpr uint32_t MaxComputeQueueCount = MaxPipeId * MaxQueueId;

class SceGnmDriver
{
	friend class SceVideoOut;

public:
	SceGnmDriver(std::shared_ptr<SceVideoOut>& videoOut);
	~SceGnmDriver();

	/// Graphics
	
	int submitCommandBuffers(uint32_t  count,
							 void*     dcbGpuAddrs[],
							 uint32_t* dcbSizesInBytes,
							 void*     ccbGpuAddrs[],
							 uint32_t* ccbSizesInBytes);

	int submitAndFlipCommandBuffers(uint32_t  count,
									void*     dcbGpuAddrs[],
									uint32_t* dcbSizesInBytes,
									void*     ccbGpuAddrs[],
									uint32_t* ccbSizesInBytes,
									uint32_t  videoOutHandle,
									uint32_t  displayBufferIndex,
									uint32_t  flipMode,
									int64_t   flipArg);

	int sceGnmSubmitDone(void);

	/// Compute

	uint32_t mapComputeQueue(
		uint32_t pipeId,
		uint32_t queueId,
		void*    ringBaseAddr,
		uint32_t ringSizeInDW,
		void*    readPtrAddr);

	void unmapComputeQueue(uint32_t vqueueId);

	void dingDong(
		uint32_t vqueueId,
		uint32_t nextStartOffsetInDw);

private:
	bool initGnmDriver();

	void createGraphicsQueue();

	void submitPresent();

private:
	std::shared_ptr<SceVideoOut> m_videoOut;

	std::unique_ptr<SceGpuQueue>                                   m_graphicsQueue;
	std::array<std::unique_ptr<SceGpuQueue>, MaxComputeQueueCount> m_computeQueues;
};

}  // namespace sce