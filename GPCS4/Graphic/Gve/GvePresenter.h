 #pragma once

#include "GveCommon.h"
#include <vector>

namespace gve
{;
	
class GveDevice;
class GveSwapChain;
class GveCmdList;

class GvePresenter
{
public:
	GvePresenter(GveDevice* device, GveSwapChain* swapchain);
	~GvePresenter();

	void present(const RcPtr<GveCmdList>& cmdList);

private:
	void createSyncObjects();

private:
	GveDevice* m_device;
	GveSwapChain* m_swapchain;
	uint32_t m_currentFrame = 0;
	std::vector<VkSemaphore> m_imageAvailableSemaphores;
	std::vector<VkSemaphore> m_renderFinishedSemaphores;
	std::vector<VkFence> m_inFlightFences;
};



}  // namespace gve