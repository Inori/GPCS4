#pragma once

#include "SceCommon.h"
#include "SceVideoOut.h"

#include "../Gve/GveDeviceInfo.h"

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
class ScePresenter;


class SceGnmDriver
{
	friend class SceVideoOut;
public:
	SceGnmDriver(std::shared_ptr<SceVideoOut>& videoOut);
	~SceGnmDriver();

	int submitCommandBuffers(uint32_t count,
		void *dcbGpuAddrs[], uint32_t *dcbSizesInBytes,
		void *ccbGpuAddrs[], uint32_t *ccbSizesInBytes);

	int submitAndFlipCommandBuffers(uint32_t count,
		void *dcbGpuAddrs[], uint32_t *dcbSizesInBytes,
		void *ccbGpuAddrs[], uint32_t *ccbSizesInBytes,
		uint32_t videoOutHandle, uint32_t displayBufferIndex,
		uint32_t flipMode, int64_t flipArg);

	int sceGnmSubmitDone(void);

	bool createPresenter(uint32_t imageCount);

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

private:
	std::shared_ptr<SceVideoOut> m_videoOut;

	RcPtr<gve::GveInstance>       m_instance;
	RcPtr<gve::GvePhysicalDevice> m_physDevice;
	RcPtr<gve::GveDevice>         m_device;
	RcPtr<gve::GveSwapChain>      m_swapchain;
	RcPtr<gve::GveContext>        m_context;

	std::shared_ptr<ScePresenter> m_presenter;
};

}  //sce