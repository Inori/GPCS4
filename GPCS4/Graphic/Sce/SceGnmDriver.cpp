#include "SceGnmDriver.h"
#include "sce_errors.h"

#include <set>

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

	m_pipeMgr = std::make_unique<GvePipelineManager>(m_device.ptr());
	m_resMgr = std::make_unique<GveResourceManager>(m_device);

	createSyncObjects(MAX_FRAMES_IN_FLIGHT);
}

SceGnmDriver::~SceGnmDriver()
{
	m_commandBuffers.clear();
	m_commandParsers.clear();
	m_frameBuffers.clear();
	m_contexts.clear();
}

bool SceGnmDriver::initDriver(uint32_t bufferNum)
{
	
	m_swapchain = new GveSwapChain(m_device, m_videoOut, bufferNum);

	createFrameBuffers(bufferNum);

	createContexts(bufferNum);

	createCommandParsers(bufferNum);

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

		auto cmdBuffer = m_commandParsers[displayBufferIndex]->getCommandBuffer()->getCmdBuffer();
		submitCommandBufferAndPresent(cmdBuffer);
	
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
	VkSurfaceKHR surface = m_videoOut->getSurface(*m_instance);

	SwapChainSupportDetails swapChainSupport = GveSwapChain::querySwapChainSupport(*device, surface);
	swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	
	const VkPhysicalDeviceFeatures& supportedFeatures = device->features().core.features;

	return  swapChainAdequate && supportedFeatures.samplerAnisotropy;
}

void SceGnmDriver::createFrameBuffers(uint32_t count)
{
	VkExtent2D extent = m_swapchain->extent();

	GveRenderPassFormat format;
	format.colorFormat = m_swapchain->imageFormat();
	m_renderPass = m_device->createRenderPass(format);

	for (uint32_t i = 0; i != count; ++i)
	{
		auto imageView = m_swapchain->getImageView(i);
		auto frameBuffer = m_device->createFrameBuffer(m_renderPass->handle(), imageView, extent);
		m_frameBuffers.push_back(frameBuffer);
	}
}

void SceGnmDriver::createContexts(uint32_t count)
{
	GveContextParam param;
	param.pipeMgr = m_pipeMgr.get();
	param.renderPass = m_renderPass;

	for (uint32_t i = 0; i != count; ++i)
	{
		auto context = m_device->createContext(param);
		m_contexts.push_back(context);
	}
}

void SceGnmDriver::createCommandParsers(uint32_t count)
{
	// Initialize command buffers and command parsers
	// according to bufferNum
	m_commandBuffers.resize(count);
	for (uint32_t i = 0; i != count; ++i)
	{
		GveRenderTarget target = { m_frameBuffers[i] };
		m_commandBuffers[i] = std::make_shared<GnmCommandBufferDraw>(m_device, m_contexts[i], m_resMgr.get(), target);
	}
	
	m_commandParsers.resize(count);
	for (uint32_t i = 0; i != count; ++i)
	{
		m_commandParsers[i] = std::make_unique<GnmCmdStream>(m_commandBuffers[i]);
	}
}

void SceGnmDriver::createSyncObjects(uint32_t framesInFlight)
{
	m_imageAvailableSemaphores.resize(framesInFlight);
	m_renderFinishedSemaphores.resize(framesInFlight);
	m_inFlightFences.resize(framesInFlight);

	VkSemaphoreCreateInfo semaphoreInfo = {};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo = {};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (size_t i = 0; i < framesInFlight; i++) 
	{
		if (vkCreateSemaphore(*m_device, &semaphoreInfo, nullptr, &m_imageAvailableSemaphores[i]) != VK_SUCCESS ||
			vkCreateSemaphore(*m_device, &semaphoreInfo, nullptr, &m_renderFinishedSemaphores[i]) != VK_SUCCESS ||
			vkCreateFence(*m_device, &fenceInfo, nullptr, &m_inFlightFences[i]) != VK_SUCCESS) 
		{
			LOG_ERR("failed to create synchronization objects for a frame!");
		}
	}
}

void SceGnmDriver::submitCommandBufferAndPresent(const RcPtr<GveCommandBuffer>& cmdBuffer)
{
	vkWaitForFences(*m_device, 1, &m_inFlightFences[m_currentFrame], VK_TRUE, UINT64_MAX);

	uint32_t imageIndex;
	VkResult result = m_swapchain->acquireNextImage(m_imageAvailableSemaphores[m_currentFrame], VK_NULL_HANDLE, imageIndex);

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = { m_imageAvailableSemaphores[m_currentFrame] };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;

	VkCommandBuffer cb = cmdBuffer->execBufferHandle();
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &cb;

	VkSemaphore signalSemaphores[] = { m_renderFinishedSemaphores[m_currentFrame] };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	vkResetFences(*m_device, 1, &m_inFlightFences[m_currentFrame]);

	auto queues = m_device->queues();
	if (vkQueueSubmit(queues.graphics.queueHandle, 1, &submitInfo, m_inFlightFences[m_currentFrame]) != VK_SUCCESS)
	{
		LOG_ERR("failed to submit draw command buffer!");
	}

	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = { m_swapchain->handle() };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;

	presentInfo.pImageIndices = &imageIndex;

	result = vkQueuePresentKHR(queues.graphics.queueHandle, &presentInfo);

	m_currentFrame = (m_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

}  //sce