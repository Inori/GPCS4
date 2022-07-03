#include "SceGnmDriver.h"

#include "Emulator.h"
#include "VirtualGPU.h"
#include "SceVideoOut.h"
#include "SceGpuQueue.h"
#include "SceComputeQueue.h"
#include "SceSwapchain.h"
#include "SceResourceTracker.h"
#include "SceObjects.h"
#include "UtilMath.h"
#include "sce_errors.h"

#include "Gnm/GnmLabelManager.h"
#include "Gnm/GnmCommandBufferDraw.h"
#include "Gnm/GnmCommandBufferDummy.h"
#include "Gnm/GnmCommandProcessor.h"
#include "Violet/VltAdapter.h"
#include "Violet/VltDevice.h"
#include "Violet/VltInstance.h"


LOG_CHANNEL(Graphic.Sce.SceGnmDriver);

extern "C" void glfwPollEvents(void);

namespace sce
{
	using namespace vlt;
	using namespace Gnm;

	SceGnmDriver::SceGnmDriver()
	{
		bool success = initGnmDriver();
		LOG_ASSERT(success == true, "init Gnm Driver failed.");
	}

	SceGnmDriver::~SceGnmDriver()
	{
		destroyGpuQueues();
	}

	bool SceGnmDriver::initGnmDriver()
	{
		bool ret = false;
		do
		{
			if (!createVltDevice())
			{
				LOG_ERR("create vlt device failed.");
				break;
			}

			// A GPU must have a graphics queue by default.
			createGraphicsQueue();
			ret = true;
		} while (false);
		return ret;
	}

	bool SceGnmDriver::createVltDevice()
	{
		bool ret = false;
		do
		{
			m_instance = new VltInstance();

			// adapters are ranked internally by their power
			// typically first one is the most powerful GPU in system
			m_adapter = m_instance->enumAdapters(0);
			if (m_adapter == nullptr)
			{
				break;
			}

			m_device = m_adapter->createDevice(m_instance);
			if (m_device == nullptr)
			{
				break;
			}

			m_objects = std::make_unique<SceObjects>(m_device.ptr());

			ret = true;
		}while(false);
		return ret;
	}

	void SceGnmDriver::createSwapchain(
		SceVideoOut*         videoOut,
		const PresenterDesc& desc
	)
	{
		SceSwapchainDevice device = {};
		device.adapter            = m_adapter->handle();  // adapter with present capability
		device.device             = m_device.ptr();
		device.videoOut           = videoOut;
		m_swapchain               = std::make_unique<SceSwapchain>(device, desc);
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
										   0, 0, 0, 0);
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
		// Thus for future development, we should record vulkan command buffer asynchronously too,
		// reducing time period of the submit call.

		LOG_ASSERT(count == 1, "Currently only support 1 cmdbuff at one call.");

		// track current display buffer
		// so that we can find it during command buffer recording
		// and use it as render target.
		trackRenderTarget(displayBufferIndex);

		SceGpuCommand cmd = {};
		cmd.buffer        = dcbGpuAddrs[0];
		cmd.size          = dcbSizesInBytes[0];
		m_graphicsQueue->record(cmd);

		submitPresent(displayBufferIndex);

		downloadResource();
		// clear resource tracker every frame
		cleanupFrame();

		return SCE_OK;
	}

	void SceGnmDriver::submitPresent(uint32_t imageIndex)
	{
		SceGpuSubmission submission = {};
		submission.wait             = VK_NULL_HANDLE;
		submission.wake             = VK_NULL_HANDLE;
		m_graphicsQueue->submit(submission);

		// wait for submit done.
		m_graphicsQueue->synchronize();

		// present the display buffer
		m_swapchain->present(imageIndex);
	}

	int SceGnmDriver::sceGnmSubmitDone(void)
	{
		// Gnm::submitDone() is the place to hint the PS4 OS that
		// all the currently running GPU tasks (graphics and compute) are done for a frame,
		// such that the OS get the permission to do some extra stuffs.
		//
		// Since we use a window to emulate the hardware display, we need a place
		// to process the window event.
		// Currently I didn't find a very good place, so I place it here.
		
		glfwPollEvents();

		// TODO:
		// Execute the Gnm::DrawCommandBuffer::InitializeDefaultHardwareState command.

		return SCE_OK;
	}

	void SceGnmDriver::createGraphicsQueue()
	{
		// Create the only graphics queue.
		m_graphicsQueue = std::make_unique<SceGpuQueue>(
			SceQueueType::Graphics, m_device.ptr(), *m_objects);
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

			vqueueId = VQueueIdBegin + pipeId * MaxPipeId + queueId;
			if (vqueueId >= MaxComputeQueueCount)
			{
				LOG_ERR("vqueueId is larger than max queue count.");
				break;
			}

			uint32_t vqueueIndex         = vqueueId - VQueueIdBegin;
			m_computeQueues[vqueueIndex] = std::make_unique<SceComputeQueue>(m_device.ptr(),
																			 ringBaseAddr,
																			 ringSizeInDW,
																			 readPtrAddr);
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
		uint32_t vqueueIndex = vqueueId - VQueueIdBegin;
		m_computeQueues[vqueueIndex]->dingDong(nextStartOffsetInDw);
	}

	void SceGnmDriver::destroyGpuQueues()
	{
		m_graphicsQueue.reset();

		for (auto& compQueue : m_computeQueues)
		{
			if (compQueue)
			{
				compQueue.reset();
			}
		}
	}

	void SceGnmDriver::trackRenderTarget(uint32_t index)
	{
		auto& tracker = GPU().resourceTracker();

		auto& renderTarget = m_swapchain->getImage(index);

		tracker.track(renderTarget);
	}

	void SceGnmDriver::cleanupFrame()
	{
		// Reset global objects.

		auto& tracker = GPU().resourceTracker();
		tracker.reset();

		auto& labelMgr = GPU().labelManager();
		labelMgr.reset();
	}

	void SceGnmDriver::downloadResource()
	{
		// Download the resource from vulkan back to it's
		// Gnm object.
		// This has two purpose:
		// 1. After the download, we can then safely
		//    release vulkan object, no matter the Gnm
		//    resource is released by game or not.
		//    If it is released after current frame,
		//    we are fine, the cost is a useless memory copy.
	    //    If it is still used by the next frame,
		//    we just create a new vulkan object using previous
		//    content downloaded here. The cost is a vulkan object
		//    creation.
		// 2. If the game access the Gnm object on CPU side,
		//    we are fine.
		//    
		//    This need to be optimized a lot.

		auto& tracker = GPU().resourceTracker();

		auto context = m_device->createContext();
		context->beginRecording(
			m_device->createCommandList(VltQueueType::Graphics));

		tracker.download(context.ptr());

		m_device->submitCommandList(
			context->endRecording(),
			VK_NULL_HANDLE,
			VK_NULL_HANDLE);
		m_device->syncSubmission();
	}

}  // namespace sce