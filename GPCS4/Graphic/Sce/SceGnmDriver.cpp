#include "SceGnmDriver.h"

#include "SceGpuQueue.h"
#include "UtilMath.h"
#include "sce_errors.h"

#include "../Gnm/GnmCmdStream.h"
#include "../Gnm/GnmCommandBufferDraw.h"
#include "../Gnm/GnmCommandBufferDummy.h"
#include "../GraphicShared.h"
#include "../Violet/VltCmdList.h"
#include "../Violet/VltImage.h"
#include "../Violet/VltInstance.h"
#include "../Violet/VltPhysicalDevice.h"
#include "../Violet/VltPresenter.h"

LOG_CHANNEL(Graphic.Sce.SceGnmDriver);

using namespace vlt;

namespace sce
{;

SceGnmDriver::SceGnmDriver(std::shared_ptr<SceVideoOut>& videoOut) :
	m_videoOut(videoOut)
{
	bool success = initGnmDriver();
	LOG_ASSERT(success == true, "init Gnm Driver failed.");
}

SceGnmDriver::~SceGnmDriver()
{
	// TODO:
	// Correct the reference count maintenance, thus we don't
	// need to handle release order manually.
	// e.g. wrap VkSurfaceKHR in a class with reference count

	m_graphicsQueue.reset();
	// Release Presenter before VideoOut
	m_presenter = nullptr;
	// Release VideoOut before GnmDriver.
	m_videoOut.reset();
}

bool SceGnmDriver::initGnmDriver()
{
	bool ret = false;
	do
	{
		// Instance
		auto extensions = m_videoOut->getExtensions();
		m_instance      = violetCreateInstance(extensions);
		if (!m_instance)
		{
			LOG_ERR("create vulkan instance failed.");
			break;
		}

		// Physical device
		VkSurfaceKHR windowSurface = m_videoOut->getWindowSurface(*m_instance);
		auto         phyDevices    = m_instance->enumPhysicalDevices();
		if (!pickPhysicalDevice(phyDevices, windowSurface))
		{
			LOG_ERR("pick physical device failed.");
			break;
		}

		// Logical device
		auto featuresToEnable = getEnableFeatures(m_physDevice);
		m_device              = m_physDevice->createLogicalDevice(featuresToEnable);
		if (!m_device)
		{
			LOG_ERR("create logical device failed.");
			break;
		}

		ret = true;
	} while (false);
	return ret;
}

bool SceGnmDriver::pickPhysicalDevice(
	const std::vector<RcPtr<vlt::VltPhysicalDevice>>& devices,
	VkSurfaceKHR                                      surface)
{
	auto iter = std::find_if(devices.begin(), devices.end(), 
		[this, surface](const auto& device) { return isDeviceSuitable(device, surface); });

	bool found = iter != devices.end();
	if (found)
	{
		m_physDevice = *iter;
	}
	
	return found;
}

bool SceGnmDriver::isDeviceSuitable(const RcPtr<vlt::VltPhysicalDevice>& device,
									VkSurfaceKHR                         surface)
{
	bool ret = false;
	do
	{
		if (!device)
		{
			break;
		}

		auto requiredFeatures = getEnableFeatures(device);
		if (!device->checkFeatureSupport(requiredFeatures))
		{
			break;
		}

		if (!checkPresentSupport(device, surface))
		{
			break;
		}

		ret = true;
	} while (false);
	return ret;
}

VltDeviceFeatures SceGnmDriver::getEnableFeatures(const RcPtr<vlt::VltPhysicalDevice>& device)
{
	VltDeviceFeatures required  = {};
	auto              supported = device->features();

	// Setup all required features to be enabled here.

	required.core.features.samplerAnisotropy  = supported.core.features.samplerAnisotropy;
	required.core.features.shaderInt64        = VK_TRUE;
	required.core.features.geometryShader     = supported.core.features.geometryShader;
	required.core.features.tessellationShader = supported.core.features.tessellationShader;

	return required;
}

bool SceGnmDriver::checkPresentSupport(const RcPtr<vlt::VltPhysicalDevice>& device, VkSurfaceKHR surface)
{
	VkBool32 presentSupport = false;
	auto     queueFamilies  = device->findQueueFamilies();
	// TODO:
	// Currently, I only support graphics queue with present support.
	vkGetPhysicalDeviceSurfaceSupportKHR(*device, queueFamilies.graphics, surface, &presentSupport);
	return presentSupport;
}

int SceGnmDriver::submitCommandBuffers(uint32_t  count,
									   void*     dcbGpuAddrs[],
									   uint32_t* dcbSizesInBytes,
									   void*     ccbGpuAddrs[],
									   uint32_t* ccbSizesInBytes)
{
	return submitAndFlipCommandBuffers(count,
									   dcbGpuAddrs, dcbSizesInBytes,
									   ccbGpuAddrs, ccbSizesInBytes,
									   SCE_VIDEO_HANDLE_MAIN, 0, 0, 0);
}

int SceGnmDriver::submitAndFlipCommandBuffers(uint32_t  count,
											  void*     dcbGpuAddrs[],
											  uint32_t* dcbSizesInBytes,
											  void*     ccbGpuAddrs[],
											  uint32_t* ccbSizesInBytes,
											  uint32_t  videoOutHandle,
											  uint32_t  displayBufferIndex,
											  uint32_t  flipMode,
											  int64_t   flipArg)
{
	// There's only one hardware graphics queue for most of modern GPUs, including the one on PS4.
	// Thus a PS4 game will call submit function to submit command buffers sequentially,
	// and normally in one same thread.
	// We just emulate the GPU, parsing and executing one command buffer per call.

	// TODO:
	// For real PS4 system, the submit call is asynchronous.
	// Thus future development, we should record vulkan command buffer asynchronously too,
	// reducing time period of the submit call.

	LOG_ASSERT(count == 1, "Currently only support 1 cmdbuff at one call.");

	SceGpuCommand cmd = {};
	cmd.buffer        = dcbGpuAddrs[0];
	cmd.size          = dcbSizesInBytes[0];
	auto cmdList      = m_graphicsQueue->record(cmd, displayBufferIndex);

	submitPresent(cmdList);

	return SCE_OK;
}

void SceGnmDriver::submitPresent(const RcPtr<vlt::VltCmdList>& cmdList)
{
	do
	{
		if (!cmdList)
		{
			break;
		}

		PresenterSync presentSync = m_presenter->getSyncObjects();

		uint32_t imageIndex = 0;
		VkResult status     = m_presenter->acquireNextImage(presentSync.acquire, VK_NULL_HANDLE, imageIndex);
		if (status != VK_SUCCESS)
		{
			break;
		}

		SceGpuSubmission gpuSubmission = {};
		gpuSubmission.cmdList          = cmdList;
		gpuSubmission.wait             = presentSync.acquire;
		gpuSubmission.wake             = presentSync.present;
		m_graphicsQueue->submit(gpuSubmission);

		VltPresentInfo presentation;
		presentation.presenter = m_presenter;
		presentation.waitSync  = gpuSubmission.wake;
		m_device->presentImage(presentation);

	} while (false);
}

int SceGnmDriver::sceGnmSubmitDone(void)
{
	m_videoOut->processEvents();
	return SCE_OK;
}

bool SceGnmDriver::createGraphicsQueue(uint32_t imageCount)
{
	bool ret = false;
	do
	{
		if (!imageCount)
		{
			break;
		}

		auto deviceQueue = m_device->queues();
		auto sizeInfo    = m_videoOut->getSize();

		// Create presenter first.
		// The graphics queue need to access swapchain image as render target
		// during record.
		PresenterDesc desc      = {};
		desc.windowSurface      = m_videoOut->getWindowSurface(*m_instance);
		desc.presentQueue       = deviceQueue.graphics.queueHandle;
		desc.imageExtent.width  = sizeInfo.frameWidth;
		desc.imageExtent.height = sizeInfo.frameHeight;
		desc.imageCount         = imageCount;
		desc.fullScreen         = false;

		m_presenter = new VltPresenter(m_device, desc);
		if (!m_presenter)
		{
			break;
		}

		// Create the only graphics queue.
		SceGpuQueueDevice gfxDevice = {};
		gfxDevice.device            = m_device;
		gfxDevice.presenter         = m_presenter;
		gfxDevice.videoOut          = m_videoOut;
		m_graphicsQueue             = std::make_unique<SceGpuQueue>(gfxDevice, SceQueueType::Graphics);

		ret = true;
	} while (false);
	return ret;
}

uint32_t SceGnmDriver::mapComputeQueue(uint32_t pipeId,
									   uint32_t queueId,
									   void*    ringBaseAddr,
									   uint32_t ringSizeInDW,
									   void*    readPtrAddr)
{
	int vqueueId = SCE_GNM_ERROR_UNKNOWN;
	do
	{
		if (pipeId >= MaxPipeId)
		{
			vqueueId = SCE_GNM_ERROR_COMPUTEQUEUE_INVALID_PIPE_ID;
			break;
		}

		if (queueId >= MaxQueueId)
		{
			vqueueId = SCE_GNM_ERROR_COMPUTEQUEUE_INVALID_QUEUE_ID;
			break;
		}

		if ((uintptr_t)ringBaseAddr % 256 != 0)
		{
			vqueueId = SCE_GNM_ERROR_COMPUTEQUEUE_INVALID_RING_BASE_ADDR;
			break;
		}

		if (!::util::isPowerOfTwo(ringSizeInDW))
		{
			vqueueId = SCE_GNM_ERROR_COMPUTEQUEUE_INVALID_RING_SIZE;
			break;
		}

		if ((uintptr_t)readPtrAddr % 4 != 0)
		{
			vqueueId = SCE_GNM_ERROR_COMPUTEQUEUE_INVALID_READ_PTR_ADDR;
			break;
		}

		*(uint32_t*)readPtrAddr = 0;

		vqueueId = VQueueIdBegin + pipeId * MaxPipeId + queueId;
		if (vqueueId >= MaxComputeQueueCount)
		{
			LOG_ERR("vqueueId is larger than max queue count.");
			break;
		}

		SceGpuQueueDevice cptDevice = {};
		cptDevice.device            = m_device;
		cptDevice.presenter         = nullptr;
		cptDevice.videoOut          = nullptr;

		uint32_t vqueueIndex        = vqueueId - VQueueIdBegin;
		m_computeQueues[vqueueIndex] = std::make_unique<SceGpuQueue>(cptDevice, SceQueueType::Compute);

	} while (false);

	return vqueueId;
}

void SceGnmDriver::unmapComputeQueue(uint32_t vqueueId)
{
	do
	{
		if (vqueueId >= MaxComputeQueueCount)
		{
			LOG_ERR("vqueueId is larger than max queue count.");
			break;
		}

		uint32_t vqueueIndex = vqueueId - VQueueIdBegin;
		m_computeQueues[vqueueIndex].reset();

	} while (false);
}

void SceGnmDriver::dingDong(
	uint32_t vqueueId,
	uint32_t nextStartOffsetInDw)
{
}

}  // namespace sce