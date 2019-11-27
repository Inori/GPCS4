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
	GveSwapChain(const RcPtr<GveDevice>& logicDevice,
		std::shared_ptr<sce::SceVideoOut>& videoOut,
		uint32_t imageCount);
	~GveSwapChain();

	VkSwapchainKHR handle() const;

	VkFormat imageFormat() const;

	// Image count is not guaranteed to be equal with display buffer count
	uint32_t imageCount() const;

	VkExtent2D extent() const;

	VkImage getImage(uint32_t index);

	VkImageView getImageView(uint32_t index);

	VkResult acquireNextImage(VkSemaphore signal, VkFence fence, uint32_t& index);

	static SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);
private:
	void createSwapChain(uint32_t imageCount);
	void createImageViews();
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

private:
	VkSwapchainKHR m_swapchain;
	RcPtr<GvePhysicalDevice> m_phyDevice;
	RcPtr<GveDevice> m_device;
	std::shared_ptr<sce::SceVideoOut> m_videoOut;

	std::vector<VkImage> m_swapChainImages;
	std::vector<VkImageView> m_swapChainImageViews;

	VkFormat m_swapChainImageFormat;
	VkExtent2D m_swapChainExtent;

};


}  // namespace gve
