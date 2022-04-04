#include "ScePresenter.h"

#include "SceVideoOut.h"

#include "Violet/VltDevice.h"
#include "Violet/VltFormat.h"
#include "Violet/VltLog.h"

#include <algorithm>

namespace sce
{
	using namespace vlt;

	ScePresenter::ScePresenter(const PresenterDevice& device,
							   const PresenterDesc&   desc) :
		m_device(device),
		m_surface(device.surface)
	{
		if (recreateSwapChain(desc) != VK_SUCCESS)
			Logger::exception("Failed to create swap chain");
	}

	ScePresenter::~ScePresenter()
	{
		destroySwapchain();
	}

	PresenterInfo ScePresenter::info() const
	{
		return m_info;
	}

	PresenterImage ScePresenter::getImage(uint32_t index) const
	{
		return m_images.at(index);
	}

	VkResult ScePresenter::acquireNextImage(PresenterSync& sync, uint32_t& index)
	{
		sync = m_semaphores.at(m_frameIndex);

		VkResult result = vkAcquireNextImageKHR(m_device.device,
												m_swapchain, std::numeric_limits<uint64_t>::max(),
												sync.acquire, VK_NULL_HANDLE, &m_imageIndex);

		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
			return result;

		index = m_imageIndex;
		return result;
	}

	VkResult ScePresenter::presentImage()
	{
		PresenterSync sync = m_semaphores.at(m_frameIndex);

		VkPresentInfoKHR info;
		info.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		info.pNext              = nullptr;
		info.waitSemaphoreCount = 1;
		info.pWaitSemaphores    = &sync.present;
		info.swapchainCount     = 1;
		info.pSwapchains        = &m_swapchain;
		info.pImageIndices      = &m_imageIndex;
		info.pResults           = nullptr;

		VkResult status = vkQueuePresentKHR(m_device.queue, &info);

		m_frameIndex += 1;
		m_frameIndex %= m_semaphores.size();

		return status;
	}

	VkResult ScePresenter::recreateSwapChain(const PresenterDesc& desc)
	{
		if (m_swapchain)
			destroySwapchain();

		// Query surface capabilities. Some properties might
		// have changed, including the size limits and supported
		// present modes, so we'll just query everything again.
		VkSurfaceCapabilitiesKHR        caps;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR>   modes;

		VkResult status;
		if ((status = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
				 m_device.adapter, m_surface, &caps)) != VK_SUCCESS)
		{
			if (status == VK_ERROR_SURFACE_LOST_KHR)
			{
				// TODO:
				// In this case, we should destroy the surface and recreate one.
				Logger::exception("surface lost.");
			}
		}

		if ((status = getSupportedFormats(formats, desc)) != VK_SUCCESS)
			return status;

		if ((status = getSupportedPresentModes(modes, desc)) != VK_SUCCESS)
			return status;

		// Select actual swap chain properties and create swap chain
		m_info.format      = pickFormat(formats.size(), formats.data(), desc.numFormats, desc.formats);
		m_info.presentMode = pickPresentMode(modes.size(), modes.data(), desc.numPresentModes, desc.presentModes);
		m_info.imageExtent = pickImageExtent(caps, desc.imageExtent);
		m_info.imageCount  = pickImageCount(caps, m_info.presentMode, desc.imageCount);

		if (!m_info.imageExtent.width || !m_info.imageExtent.height)
		{
			m_info.imageCount = 0;
			m_info.format     = { VK_FORMAT_UNDEFINED, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
			return VK_SUCCESS;
		}

		VkSwapchainCreateInfoKHR swapInfo;
		swapInfo.sType                 = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapInfo.pNext                 = nullptr;
		swapInfo.flags                 = 0;
		swapInfo.surface               = m_surface;
		swapInfo.minImageCount         = m_info.imageCount;
		swapInfo.imageFormat           = m_info.format.format;
		swapInfo.imageColorSpace       = m_info.format.colorSpace;
		swapInfo.imageExtent           = m_info.imageExtent;
		swapInfo.imageArrayLayers      = 1;
		swapInfo.imageUsage            = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
		swapInfo.imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE;
		swapInfo.queueFamilyIndexCount = 0;
		swapInfo.pQueueFamilyIndices   = nullptr;
		swapInfo.preTransform          = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
		swapInfo.compositeAlpha        = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		swapInfo.presentMode           = m_info.presentMode;
		swapInfo.clipped               = VK_TRUE;
		swapInfo.oldSwapchain          = VK_NULL_HANDLE;

		Logger::info(util::str::formatex(
			"Presenter: Actual swap chain properties:"
			"\n  Format:       ",
			m_info.format.format,
			"\n  Present mode: ", m_info.presentMode,
			"\n  Buffer size:  ", m_info.imageExtent.width, "x", m_info.imageExtent.height,
			"\n  Image count:  ", m_info.imageCount));

		if ((status = vkCreateSwapchainKHR(m_device.device,
										   &swapInfo, nullptr, &m_swapchain)) != VK_SUCCESS)
			return status;

		createImageViews();
		createSemaphores();

		// Invalidate indices
		m_imageIndex = 0;
		m_frameIndex = 0;
		return VK_SUCCESS;
	}

	VkResult ScePresenter::getSupportedFormats(std::vector<VkSurfaceFormatKHR>& formats, const PresenterDesc& desc)
	{
		uint32_t numFormats = 0;

		VkPhysicalDeviceSurfaceInfo2KHR surfaceInfo;
		surfaceInfo.sType   = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SURFACE_INFO_2_KHR;
		surfaceInfo.pNext   = nullptr;
		surfaceInfo.surface = m_surface;

		VkResult status = vkGetPhysicalDeviceSurfaceFormatsKHR(
			m_device.adapter, m_surface, &numFormats, nullptr);

		if (status != VK_SUCCESS)
			return status;

		formats.resize(numFormats);

		status = vkGetPhysicalDeviceSurfaceFormatsKHR(
			m_device.adapter, m_surface, &numFormats, formats.data());

		return status;
	}

	VkResult ScePresenter::getSupportedPresentModes(std::vector<VkPresentModeKHR>& modes, const PresenterDesc& desc)
	{
		uint32_t numModes = 0;

		VkPhysicalDeviceSurfaceInfo2KHR surfaceInfo;
		surfaceInfo.sType   = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SURFACE_INFO_2_KHR;
		surfaceInfo.pNext   = nullptr;
		surfaceInfo.surface = m_surface;

		VkResult status = vkGetPhysicalDeviceSurfacePresentModesKHR(
			m_device.adapter, m_surface, &numModes, nullptr);

		if (status != VK_SUCCESS)
			return status;

		modes.resize(numModes);

		status = vkGetPhysicalDeviceSurfacePresentModesKHR(
			m_device.adapter, m_surface, &numModes, modes.data());

		return status;
	}

	VkResult ScePresenter::getSwapImages(std::vector<VkImage>& images)
	{
		uint32_t imageCount = 0;

		VkResult status = vkGetSwapchainImagesKHR(
			m_device.device, m_swapchain, &imageCount, nullptr);

		if (status != VK_SUCCESS)
			return status;

		images.resize(imageCount);

		return vkGetSwapchainImagesKHR(
			m_device.device, m_swapchain, &imageCount, images.data());
	}

	VkSurfaceFormatKHR ScePresenter::pickFormat(
		uint32_t                  numSupported,
		const VkSurfaceFormatKHR* pSupported,
		uint32_t                  numDesired,
		const VkSurfaceFormatKHR* pDesired)
	{
		if (numDesired > 0)
		{
			// If the implementation allows us to freely choose
			// the format, we'll just use the preferred format.
			if (numSupported == 1 && pSupported[0].format == VK_FORMAT_UNDEFINED)
				return pDesired[0];

			// If the preferred format is explicitly listed in
			// the array of supported surface formats, use it
			for (uint32_t i = 0; i < numDesired; i++)
			{
				for (uint32_t j = 0; j < numSupported; j++)
				{
					if (pSupported[j].format == pDesired[i].format && pSupported[j].colorSpace == pDesired[i].colorSpace)
						return pSupported[j];
				}
			}

			// If that didn't work, we'll fall back to a format
			// which has similar properties to the preferred one
			VltFormatFlags prefFlags = imageFormatInfo(pDesired[0].format)->flags;

			for (uint32_t j = 0; j < numSupported; j++)
			{
				auto currFlags = imageFormatInfo(pSupported[j].format)->flags;

				if ((currFlags & VltFormatFlag::ColorSpaceSrgb) == (prefFlags & VltFormatFlag::ColorSpaceSrgb))
					return pSupported[j];
			}
		}

		// Otherwise, fall back to the first supported format
		return pSupported[0];
	}

	VkPresentModeKHR ScePresenter::pickPresentMode(
		uint32_t                numSupported,
		const VkPresentModeKHR* pSupported,
		uint32_t                numDesired,
		const VkPresentModeKHR* pDesired)
	{
		// Just pick the first desired and supported mode
		for (uint32_t i = 0; i < numDesired; i++)
		{
			for (uint32_t j = 0; j < numSupported; j++)
			{
				if (pSupported[j] == pDesired[i])
					return pSupported[j];
			}
		}

		// Guaranteed to be available
		return VK_PRESENT_MODE_FIFO_KHR;
	}

	VkExtent2D ScePresenter::pickImageExtent(
		const VkSurfaceCapabilitiesKHR& caps,
		VkExtent2D                      desired)
	{
		if (caps.currentExtent.width != std::numeric_limits<uint32_t>::max())
			return caps.currentExtent;

		VkExtent2D actual;
		actual.width  = std::clamp(desired.width, caps.minImageExtent.width, caps.maxImageExtent.width);
		actual.height = std::clamp(desired.height, caps.minImageExtent.height, caps.maxImageExtent.height);
		return actual;
	}

	uint32_t ScePresenter::pickImageCount(
		const VkSurfaceCapabilitiesKHR& caps,
		VkPresentModeKHR                presentMode,
		uint32_t                        desired)
	{
		uint32_t count = caps.minImageCount;

		if (presentMode != VK_PRESENT_MODE_IMMEDIATE_KHR)
			count = caps.minImageCount + 1;

		if (count < desired)
			count = desired;

		if (count > caps.maxImageCount && caps.maxImageCount != 0)
			count = caps.maxImageCount;

		return count;
	}

	VkResult ScePresenter::createImageViews()
	{
		VkResult status = VK_RESULT_MAX_ENUM;

		// Acquire images and create views
		std::vector<VkImage> images;

		if ((status = getSwapImages(images)) != VK_SUCCESS)
			return status;

		// Update actual image count
		m_info.imageCount = images.size();
		m_images.resize(m_info.imageCount);

		for (uint32_t i = 0; i < m_info.imageCount; i++)
		{
			m_images[i].image = images[i];

			VkImageViewCreateInfo viewInfo;
			viewInfo.sType      = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			viewInfo.pNext      = nullptr;
			viewInfo.flags      = 0;
			viewInfo.image      = images[i];
			viewInfo.viewType   = VK_IMAGE_VIEW_TYPE_2D;
			viewInfo.format     = m_info.format.format;
			viewInfo.components = VkComponentMapping{
				VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY,
				VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY
			};
			viewInfo.subresourceRange = {
				VK_IMAGE_ASPECT_COLOR_BIT,
				0, 1, 0, 1
			};

			if ((status = vkCreateImageView(m_device.device,
											&viewInfo, nullptr, &m_images[i].view)) != VK_SUCCESS)
				return status;
		}
		return status;
	}

	VkResult ScePresenter::createSemaphores()
	{
		VkResult status = VK_RESULT_MAX_ENUM;
		// Create one set of semaphores per swap image
		m_semaphores.resize(m_info.imageCount);

		for (uint32_t i = 0; i < m_semaphores.size(); i++)
		{
			VkSemaphoreCreateInfo semInfo;
			semInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
			semInfo.pNext = nullptr;
			semInfo.flags = 0;

			if ((status = vkCreateSemaphore(m_device.device,
											&semInfo, nullptr, &m_semaphores[i].acquire)) != VK_SUCCESS)
				return status;

			if ((status = vkCreateSemaphore(m_device.device,
											&semInfo, nullptr, &m_semaphores[i].present)) != VK_SUCCESS)
				return status;
		}

		return status;
	}

	void ScePresenter::destroySwapchain()
	{
		for (const auto& img : m_images)
			vkDestroyImageView(m_device.device, img.view, nullptr);

		for (const auto& sem : m_semaphores)
		{
			vkDestroySemaphore(m_device.device, sem.acquire, nullptr);
			vkDestroySemaphore(m_device.device, sem.present, nullptr);
		}

		vkDestroySwapchainKHR(m_device.device, m_swapchain, nullptr);

		m_images.clear();
		m_semaphores.clear();

		m_swapchain = VK_NULL_HANDLE;
	}

}  // namespace sce