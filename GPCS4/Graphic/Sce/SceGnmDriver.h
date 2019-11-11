#pragma once

#include "GPCS4Common.h"
#include "SceVideoOut.h"

#include "../Gnm/GnmCmdStream.h"
#include "../Gnm/GnmCommandBufferDraw.h"

#include "../Gve/GveInstance.h"
#include "../Gve/GveSwapChain.h"
#include "../Gve/GvePipelineManager.h"
#include "../Gve/GveResourceManager.h"

#include <memory>

namespace sce
{;

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

	RcPtr<gve::GvePhysicalDevice>  pickPhysicalDevice();
	bool isDeviceSuitable(RcPtr<gve::GvePhysicalDevice>& device);
	void createFrameBuffers(uint32_t count);
	void createContexts(uint32_t count);
	void createCommandParsers(uint32_t count);
	void createSyncObjects(uint32_t framesInFlight);

	void submitCommandBufferAndPresent(const RcPtr<gve::GveCommandBuffer>& cmdBuffer);

private:
	std::shared_ptr<SceVideoOut> m_videoOut;
	std::vector<std::unique_ptr<GnmCmdStream>> m_commandParsers;
	std::vector<std::shared_ptr<GnmCommandBuffer>> m_commandBuffers;

	RcPtr<gve::GveInstance> m_instance;
	RcPtr<gve::GvePhysicalDevice> m_physDevice;
	RcPtr<gve::GveDevice> m_device;
	RcPtr<gve::GveSwapChain> m_swapchain;
	RcPtr<gve::GveRenderPass> m_renderPass;
	std::vector<RcPtr<gve::GveFrameBuffer>> m_frameBuffers;
	std::vector<RcPtr<gve::GveContex>> m_contexts;

	std::unique_ptr<gve::GvePipelineManager> m_pipeMgr;
	std::unique_ptr<gve::GveResourceManager> m_resMgr;


	std::vector<VkSemaphore> m_imageAvailableSemaphores;
	std::vector<VkSemaphore> m_renderFinishedSemaphores;
	std::vector<VkFence> m_inFlightFences;
	uint32_t m_currentFrame = 0;
};

}  //sce