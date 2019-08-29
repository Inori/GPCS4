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

class GveSwapChain : public RcObject
{
public:
	GveSwapChain(RcPtr<GvePhysicalDevice>& phyDevice,
		RcPtr<GveDevice>& logicDevice,
		std::shared_ptr<sce::SceVideoOut>& videoOut,
		uint32_t imageCount);
	~GveSwapChain();

	VkFormat imageFormat() const;

	VkExtent2D extent() const;

	static SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);
private:
	void createSwapChain(uint32_t imageCount);
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
