#pragma once

#include "SceCommon.h"
#include "SceVideoOut.h"

#include "../Gve/GveDeviceInfo.h"

#include <array>
#include <memory>

namespace gve
{;
class GveInstance;
class GvePhysicalDevice;
class GveDevice;
class GvePresenter;
}  // namespace gve

class GnmCmdStream;
class GnmCommandBuffer;

namespace sce
{;

class SceVideoOut;
class SceGpuQueue;

constexpr uint32_t kMaxPipeId            = 7;  // Some doc say it should be 7, others say it should be 3. Fuck that.
constexpr uint32_t kMaxQueueId           = 8;
constexpr uint32_t kMaxComputeQueueCount = kMaxPipeId * kMaxQueueId;

class SceGnmDriver
{
	friend class SceVideoOut;

public:
	SceGnmDriver(std::shared_ptr<SceVideoOut>& videoOut);
	~SceGnmDriver();

	bool createGraphicsQueue(uint32_t imageCount);

	uint32_t mapComputeQueue(
		uint32_t pipeId,
		uint32_t queueId,
		void*    ringBaseAddr,
		uint32_t ringSizeInDW,
		void*    readPtrAddr);

	void unmapComputeQueue(uint32_t vqueueId);

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

private:
	bool initGnmDriver();

	bool pickPhysicalDevice(
		const std::vector<RcPtr<gve::GvePhysicalDevice>>& devices,
		VkSurfaceKHR                                      surface);

	bool isDeviceSuitable(
		const RcPtr<gve::GvePhysicalDevice>& device,
		VkSurfaceKHR                         surface);

	gve::GveDeviceFeatures getRequiredFeatures(
		const RcPtr<gve::GvePhysicalDevice>& device);

	bool checkPresentSupport(
		const RcPtr<gve::GvePhysicalDevice>& device,
		VkSurfaceKHR                         surface);

	void submitPresent();

private:
	std::shared_ptr<SceVideoOut> m_videoOut;

	RcPtr<gve::GveInstance>       m_instance;
	RcPtr<gve::GvePhysicalDevice> m_physDevice;
	RcPtr<gve::GveDevice>         m_device;
	RcPtr<gve::GvePresenter>      m_presenter;

	std::unique_ptr<SceGpuQueue>                                    m_graphicsQueue;
	std::array<std::unique_ptr<SceGpuQueue>, kMaxComputeQueueCount> m_computeQueues;
};

}  // namespace sce