#pragma once

#include "SceCommon.h"
#include "SceVideoOut.h"

#include <memory>

namespace gve
{;
class GveInstance;
class GvePhysicalDevice;
class GveDevice;
class GveSwapChain;
class GvePresenter;
class GveContext;
}  // namespace gve

class GnmCmdStream;
class GnmCommandBuffer;

namespace sce
{;

class SceVideoOut;


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

private:

	bool initGnmDriver();

	bool initGraphics();

	bool initCompute();


private:
	std::shared_ptr<SceVideoOut> m_videoOut;

	RcPtr<gve::GveInstance>       m_instance;
	RcPtr<gve::GvePhysicalDevice> m_physDevice;
	RcPtr<gve::GveDevice>         m_device;
	RcPtr<gve::GveSwapChain>      m_swapchain;
	RcPtr<gve::GveContext>        m_context;

	std::unique_ptr<GnmCmdStream>     m_commandParser;
	std::unique_ptr<GnmCommandBuffer> m_graphicsCmdBuffer;
};

}  //sce