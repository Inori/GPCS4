#pragma once

#include "GveCommon.h"
#include "GveInstance.h"
#include "GveDevice.h"
#include "../Sce/SceVideoOut.h"

#include <vector>
#include <memory>

namespace gve
{;

struct SwapChainSupportDetails 
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

class GveSwapChain
{
public:
	GveSwapChain(RcPtr<GvePhysicalDevice>& phyDevice,
		RcPtr<GveDevice>& logicDevice,
		std::shared_ptr<sce::SceVideoOut>& videoOut);
	~GveSwapChain();

private:
	void createSwapChain();
	SwapChainSupportDetails querySwapChainSupport();
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

private:
	VkSwapchainKHR m_swapchain;
	RcPtr<GvePhysicalDevice> m_phyDevice;
	RcPtr<GveDevice> m_logicalDevice;
	std::shared_ptr<sce::SceVideoOut> m_videoOut;

	std::vector<VkImage> m_swapChainImages;
	VkFormat m_swapChainImageFormat;
	VkExtent2D m_swapChainExtent;

};


}  // namespace gve
