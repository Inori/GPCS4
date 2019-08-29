#include "SceGnmDriver.h"
#include "sce_errors.h"

#include <set>

namespace sce
{;

using namespace gve;


const std::vector<const char*> deviceExtensions =
{
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};


SceGnmDriver::SceGnmDriver(std::shared_ptr<SceVideoOut>& videoOut):
	m_videoOut(videoOut)
{
	// Instance
	auto extensions = m_videoOut->getExtensions();
	m_instance = new GveInstance(extensions);

	// Physical device
	m_physDevice = pickPhysicalDevice();
	LOG_ASSERT(m_physDevice != nullptr, "pick physical device failed.");

	// Logical device
	VkSurfaceKHR surface = m_videoOut->getSurface(*m_instance);
	auto indices = m_physDevice->getSuitableQueueIndices(surface);
	m_device = m_physDevice->createLogicalDevice(indices, deviceExtensions);
	LOG_ASSERT(m_device != nullptr, "create logical device failed.");
}

SceGnmDriver::~SceGnmDriver()
{
	m_commandBuffers.clear();
	m_commandParsers.clear();
}

bool SceGnmDriver::initDriver(uint32_t bufferNum)
{
	m_commandBuffers.resize(bufferNum);
	for (auto& cmd : m_commandBuffers)
	{
		cmd = std::make_shared<GnmCommandBufferDraw>();
	}

	m_commandParsers.resize(bufferNum);
	for (uint32_t i = 0; i != bufferNum; ++i)
	{
		m_commandParsers[i] = std::make_unique<GnmCmdStream>(m_commandBuffers[i]);
	}

	m_swapchain = new GveSwapChain(m_physDevice, m_device, m_videoOut, bufferNum);
	return true;
}

int SceGnmDriver::submitAndFlipCommandBuffers(uint32_t count, 
	void *dcbGpuAddrs[], uint32_t *dcbSizesInBytes,
	void *ccbGpuAddrs[], uint32_t *ccbSizesInBytes,
	uint32_t videoOutHandle, uint32_t displayBufferIndex, 
	uint32_t flipMode, int64_t flipArg)
{
	int err = SCE_GNM_ERROR_UNKNOWN;
	do 
	{
		LOG_ASSERT(count == 1, "Currently only support only 1 cmdbuff.");

		auto& cmdParser = m_commandParsers[displayBufferIndex];
		if (!cmdParser->processCommandBuffer((uint32_t*)dcbGpuAddrs[0], dcbSizesInBytes[0]))
		{
			break;
		}
	
		err = SCE_OK;
	} while (false);
	return err;
}

int SceGnmDriver::sceGnmSubmitDone(void)
{
	return SCE_OK;
}

RcPtr<gve::GvePhysicalDevice> SceGnmDriver::pickPhysicalDevice()
{
	RcPtr<gve::GvePhysicalDevice> phyDevice;
	do 
	{
		if (!m_instance)
		{
			break;
		}

		VkSurfaceKHR surface = m_videoOut->getSurface(*m_instance);
		uint32_t devCount = m_instance->physicalDeviceCount();
		for (uint32_t i = 0; i != devCount; ++i)
		{
			RcPtr<GvePhysicalDevice> device = m_instance->getPhysicalDevice(i);
			if (isDeviceSuitable(device))
			{
				phyDevice = device;
				break;
			}
		}

	} while (false);
	return phyDevice;
}

bool SceGnmDriver::isDeviceSuitable(RcPtr<gve::GvePhysicalDevice>& device)
{
	bool extensionsSupported = checkDeviceExtensionSupport(device);

	bool swapChainAdequate = false;
	VkSurfaceKHR surface = m_videoOut->getSurface(*m_instance);
	if (extensionsSupported) 
	{
		SwapChainSupportDetails swapChainSupport = GveSwapChain::querySwapChainSupport(*device, surface);
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}

	VkPhysicalDeviceFeatures supportedFeatures = device->getFeatures();
	auto indices = device->getSuitableQueueIndices(surface);

	return indices.isComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
}

bool SceGnmDriver::checkDeviceExtensionSupport(RcPtr<gve::GvePhysicalDevice>& device)
{
	auto availableExtensions = device->getAvailableExtensions();
	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

	for (const auto& extension : availableExtensions) 
	{
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}

}  //sce