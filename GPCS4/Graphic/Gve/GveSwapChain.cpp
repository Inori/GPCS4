#include "GveSwapChain.h"
#include <algorithm>

namespace gve
{;

GveSwapChain::GveSwapChain(RcPtr<GveDevice>& logicDevice,
	std::shared_ptr<sce::SceVideoOut>& videoOut,
	uint32_t imageCount) :
	m_swapchain(VK_NULL_HANDLE),
	m_device(logicDevice),
	m_videoOut(videoOut)
{
	m_phyDevice = m_device->physicalDevice();
	createSwapChain(imageCount);
}

GveSwapChain::~GveSwapChain()
{
	vkDestroySwapchainKHR(*m_device, m_swapchain, nullptr);
}

VkSwapchainKHR GveSwapChain::handle() const
{
	return m_swapchain;
}

void GveSwapChain::createSwapChain(uint32_t imageCount)
{
	do 
	{
		GveInstance* instance = m_phyDevice->instance();
		VkDevice device = *m_device;
		VkSurfaceKHR surface = m_videoOut->getSurface(*instance);

		SwapChainSupportDetails swapChainSupport = querySwapChainSupport(*m_phyDevice, surface);

		VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
		VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
		VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

		//imageCount = swapChainSupport.capabilities.minImageCount + 1;
		if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
		{
			imageCount = swapChainSupport.capabilities.maxImageCount;
		}

		GvePhysicalDeviceQueueFamilies queueFamilies = m_phyDevice->findQueueFamilies();
		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(*m_phyDevice, queueFamilies.graphicsFamily, surface, &presentSupport);
		LOG_ASSERT(presentSupport == VK_TRUE, "graphics and present queue not same.");

		VkSwapchainCreateInfoKHR createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = surface;

		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = extent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;

		if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &m_swapchain) != VK_SUCCESS)
		{
			LOG_ERR("failed to create swap chain!");
			break;
		}

		vkGetSwapchainImagesKHR(device, m_swapchain, &imageCount, nullptr);
		m_swapChainImages.resize(imageCount);
		vkGetSwapchainImagesKHR(device, m_swapchain, &imageCount, m_swapChainImages.data());

		m_swapChainImageFormat = surfaceFormat.format;
		m_swapChainExtent = extent;

		createImageViews();

	} while (false);
}

void GveSwapChain::createImageViews()
{
	for (auto& image : m_swapChainImages)
	{
		VkImageViewCreateInfo viewInfo = {};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = image;
		viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format = m_swapChainImageFormat;
		viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = 1;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;

		VkImageView imageView;
		if (vkCreateImageView(*m_device, &viewInfo, nullptr, &imageView) != VK_SUCCESS) 
		{
			LOG_ERR("failed to create swapchain texture image view!");
		}

		m_swapChainImageViews.push_back(imageView);
	}
}

SwapChainSupportDetails GveSwapChain::querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface)
{
	SwapChainSupportDetails details;

	do 
	{
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

		if (formatCount == 0)
		{
			break;
		}
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());

		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

		if (presentModeCount == 0)
		{
			break;
		}
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());

	} while (false);

	return details;
}

VkSurfaceFormatKHR GveSwapChain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
	bool foundBest = false;
	VkSurfaceFormatKHR format;

	for (const auto& availableFormat : availableFormats) 
	{
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM &&
			availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) 
		{
			format = availableFormat;
			foundBest = true;
			break;
		}
	}

	if (!foundBest)
	{
		format = availableFormats[0];
	}

	return format;
}

VkPresentModeKHR GveSwapChain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
{
	VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;

	for (const auto& availablePresentMode : availablePresentModes) 
	{
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) 
		{
			bestMode = availablePresentMode;
			break;
		}
		else if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR) 
		{
			bestMode = availablePresentMode;
		}
	}

	return bestMode;
}

VkExtent2D GveSwapChain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
{
	VkExtent2D extend = {};
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) 
	{
		extend = capabilities.currentExtent;
	}
	else 
	{
		uint32_t width = 0, height = 0;
		m_videoOut->getFramebufferSize(width, height);

		VkExtent2D actualExtent = { width,height };

		actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
		actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

		extend = actualExtent;
	}
	return extend;
}


VkFormat GveSwapChain::imageFormat() const
{
	return m_swapChainImageFormat;
}

uint32_t GveSwapChain::imageCount() const
{
	return m_swapChainImages.size();
}

VkExtent2D GveSwapChain::extent() const
{
	return m_swapChainExtent;
}

VkImage GveSwapChain::getImage(uint32_t index)
{
	return m_swapChainImages[index];
}

VkImageView GveSwapChain::getImageView(uint32_t index)
{
	return m_swapChainImageViews[index];
}

VkResult GveSwapChain::acquireNextImage(VkSemaphore signal, VkFence fence, uint32_t& index)
{
	LOG_ASSERT(fence == VK_NULL_HANDLE, "TODO: only support null fence currently");
	VkResult result = vkAcquireNextImageKHR(*m_device, m_swapchain, UINT64_MAX, signal, VK_NULL_HANDLE, &index);
	return result;
}

}  // namespace gve