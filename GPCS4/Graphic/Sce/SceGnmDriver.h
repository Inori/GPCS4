#pragma once

#include "GPCS4Common.h"
#include "SceVideoOut.h"
#include <memory>

namespace gve
{;
class GveInstance;
class GvePhysicalDevice;
class GveDevice;
class GveSwapChain;
class GvePresenter;
}  // namespace gve

class GnmCmdStream;
class GnmCommandBuffer;

namespace sce
{;

class SceVideoOut;


class SceGnmDriver
{
public:
	SceGnmDriver(std::shared_ptr<SceVideoOut>& videoOut);
	~SceGnmDriver();

	bool initDriver(uint32_t bufferNum);

	int submitCommandBuffers(uint32_t count,
		void *dcbGpuAddrs[], uint32_t *dcbSizesInBytes,
		void *ccbGpuAddrs[], uint32_t *ccbSizesInBytes);

	int submitAndFlipCommandBuffers(uint32_t count,
		void *dcbGpuAddrs[], uint32_t *dcbSizesInBytes,
		void *ccbGpuAddrs[], uint32_t *ccbSizesInBytes,
		uint32_t videoOutHandle, uint32_t displayBufferIndex,
		uint32_t flipMode, int64_t flipArg);

	int sceGnmSubmitDone(void);

private:

	void createCommandParsers(uint32_t count);

private:
	std::shared_ptr<SceVideoOut> m_videoOut;
	std::vector<std::unique_ptr<GnmCmdStream>> m_commandParsers;
	std::vector<std::shared_ptr<GnmCommandBuffer>> m_commandBuffers;

	RcPtr<gve::GveInstance> m_instance;
	RcPtr<gve::GvePhysicalDevice> m_physDevice;
	RcPtr<gve::GveDevice> m_device;
	RcPtr<gve::GveSwapChain> m_swapchain;
	std::unique_ptr<gve::GvePresenter> m_presenter;
};

}  //sce