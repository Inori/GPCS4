#include "SceGnmDriver.h"
#include "sce_errors.h"
#include "../GraphicShared.h"

namespace sce
{;

using namespace gve;
const int MAX_FRAMES_IN_FLIGHT = 2;

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
	m_device = m_physDevice->createLogicalDevice();
	LOG_ASSERT(m_device != nullptr, "create logical device failed.");

	// Swap chain
	m_swapchain = new GveSwapChain(m_device, m_videoOut, 3);
}

SceGnmDriver::~SceGnmDriver()
{
	m_commandBuffers.clear();
	m_commandParsers.clear();

	m_videoOut->DestroySurface(*m_instance);
}

bool SceGnmDriver::initDriver(uint32_t bufferNum)
{

	createCommandParsers(bufferNum);

	return true;
}

int SceGnmDriver::submitCommandBuffers(uint32_t count, 
	void *dcbGpuAddrs[], uint32_t *dcbSizesInBytes,
	void *ccbGpuAddrs[], uint32_t *ccbSizesInBytes)
{
	return submitAndFlipCommandBuffers(count,
		dcbGpuAddrs, dcbSizesInBytes,
		ccbGpuAddrs, ccbSizesInBytes,
		SCE_VIDEO_HANDLE_MAIN, 0, 0, 0);
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
	m_videoOut->processEvents();
	return SCE_OK;
}

RcPtr<GvePhysicalDevice> SceGnmDriver::pickPhysicalDevice()
{
	RcPtr<GvePhysicalDevice> phyDevice;
	do 
	{
		if (!m_instance)
		{
			break;
		}

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

bool SceGnmDriver::isDeviceSuitable(RcPtr<GvePhysicalDevice>& device)
{
	bool swapChainAdequate = false;
	VkSurfaceKHR surface = m_videoOut->createSurface(*m_instance);

	SwapChainSupportDetails swapChainSupport = GveSwapChain::querySwapChainSupport(*device, surface);
	swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	
	const VkPhysicalDeviceFeatures& supportedFeatures = device->features().core.features;

	return  swapChainAdequate && supportedFeatures.samplerAnisotropy;
}


void SceGnmDriver::createCommandParsers(uint32_t count)
{
	// Initialize command buffers and command parsers
	// according to bufferNum
	m_commandBuffers.resize(count);
	for (uint32_t i = 0; i != count; ++i)
	{
		m_commandBuffers[i] = std::make_shared<GnmCommandBufferDraw>(m_device);
	}
	
	m_commandParsers.resize(count);
	for (uint32_t i = 0; i != count; ++i)
	{
		m_commandParsers[i] = std::make_unique<GnmCmdStream>(m_commandBuffers[i]);
	}
}

}  //sce