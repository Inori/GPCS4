#include "SceGnmDriver.h"

#include "Emulator/Emulator.h"
#include "VirtualGPU.h"
#include "SceVideoOut.h"
#include "SceGpuQueue.h"
#include "ScePresenter.h"
#include "SceResourceTracker.h"
#include "UtilMath.h"
#include "sce_errors.h"

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

	SceGnmDriver::SceGnmDriver()
	{
		bool success = initGnmDriver();
		LOG_ASSERT(success == true, "init Gnm Driver failed.");
	}

	SceGnmDriver::~SceGnmDriver()
	{
		destroyGpuQueues();

		m_presenter = nullptr;
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

			ret = true;
		}while(false);
		return ret;
	}

	void SceGnmDriver::createPresenter(
		SceVideoOut*         videoOut,
		const PresenterDesc& desc
	)
	{
		VkInstance      instance = m_device->instance()->handle();
		PresenterDevice device   = {};
		device.adapter           = m_adapter->handle();
		device.device            = m_device->handle();
		device.queue             = m_device->queues().graphics.queueHandle;
		device.surface           = videoOut->getSurface(instance);

		m_presenter = new ScePresenter(device, desc);

		createRenderTargetViews();
	}

	void SceGnmDriver::createRenderTargetViews()
	{
		PresenterInfo info = m_presenter->info();

		m_swapImages.clear();
		m_swapImages.resize(info.imageCount);

		VltImageCreateInfo imageInfo;
		imageInfo.type        = VK_IMAGE_TYPE_2D;
		imageInfo.format      = info.format.format;
		imageInfo.flags       = 0;
		imageInfo.sampleCount = VK_SAMPLE_COUNT_1_BIT;
		imageInfo.extent      = { info.imageExtent.width, info.imageExtent.height, 1 };
		imageInfo.numLayers   = 1;
		imageInfo.mipLevels   = 1;
		imageInfo.usage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		imageInfo.stages      = 0;
		imageInfo.access      = 0;
		imageInfo.tiling      = VK_IMAGE_TILING_OPTIMAL;
		imageInfo.layout      = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VltImageViewCreateInfo viewInfo;
		viewInfo.type      = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format    = info.format.format;
		viewInfo.usage     = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		viewInfo.aspect    = VK_IMAGE_ASPECT_COLOR_BIT;
		viewInfo.minLevel  = 0;
		viewInfo.numLevels = 1;
		viewInfo.minLayer  = 0;
		viewInfo.numLayers = 1;

		for (uint32_t i = 0; i < info.imageCount; i++)
		{
			VkImage imageHandle = m_presenter->getImage(i).image;

			m_swapImages[i].image     = m_device->createImageFromVkImage(imageInfo, imageHandle);
			m_swapImages[i].imageView = m_device->createImageView(m_swapImages[i].image, viewInfo);
		}
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

		// track currently display buffer
		// so that we can find it during command buffer recording
		// and use it as render target.
		trackSwapImage(displayBufferIndex);

		SceGpuCommand cmd = {};
		cmd.buffer        = dcbGpuAddrs[0];
		cmd.size          = dcbSizesInBytes[0];
		auto cmdList      = m_graphicsQueue->record(cmd);

		submitPresent(cmdList, displayBufferIndex);

		// clear resource tracker every frame
		resetResourceTracker();

		return SCE_OK;
	}

	void SceGnmDriver::submitPresent(
		const vlt::Rc<vlt::VltCommandList>& cmdList,
		uint32_t                            imageIndex)
	{
		do
		{
			PresenterSync sync = {};
			uint32_t      presentIndex = 0;

			m_presenter->acquireNextImage(sync, presentIndex);

			SceGpuSubmission submission = {};
			submission.cmdList          = cmdList;
			submission.wait             = sync.acquire;
			submission.wake             = sync.present;
			m_graphicsQueue->submit(submission);

			// TODO:
			// take image index into consideration
			m_graphicsQueue->present(m_presenter);

		} while (false);
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
			m_device.ptr(), SceQueueType::Graphics);
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

			uint32_t vqueueIndex         = vqueueId - VQueueIdBegin;
			m_computeQueues[vqueueIndex] = std::make_unique<SceGpuQueue>(
				m_device.ptr(), SceQueueType::Compute);

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

	void SceGnmDriver::trackSwapImage(uint32_t index)
	{
		auto& tracker   = GPU().resourceTracker();
		auto& swapImage = m_swapImages[index];
		auto& info      = swapImage.image->info();
		auto  gpuMode   = GPU().mode();

		Gnm::TileMode tileMode;

		// This should be ok to set a hard code format.
		// All the shit we do here is just to give the dummy renderTarget 
		// a non-zero dummy size, so that it can be find by the resource tracker.
		// we'll receive the real rendertarget during command buffer process.
		Gnm::DataFormat format = Gnm::kDataFormatB8G8R8A8UnormSrgb;
		GpuAddress::computeSurfaceTileMode(
			gpuMode,                                         // NEO or base
			&tileMode,                                       // Tile mode pointer
			GpuAddress::kSurfaceTypeColorTargetDisplayable,  // Surface type
			format,                                          // Surface format
			1);                                              // Elements per pixel

		Gnm::RenderTargetSpec spec = {};
		spec.init();
		spec.m_width                        = info.extent.width;
		spec.m_height                       = info.extent.height;
		spec.m_pitch                        = 0;
		spec.m_numSlices                    = 1;
		spec.m_colorFormat                  = format;
		spec.m_colorTileModeHint            = tileMode;
		spec.m_minGpuMode                   = gpuMode;
		spec.m_numSamples                   = Gnm::kNumSamples1;
		spec.m_numFragments                 = Gnm::kNumFragments1;
		spec.m_flags.enableCmaskFastClear   = 0;
		spec.m_flags.enableFmaskCompression = 0;

		SceRenderTarget renderTarget = {};
		renderTarget.renderTarget.init(&spec);
		renderTarget.image = swapImage.image;
		renderTarget.imageView = swapImage.imageView;

		tracker.track(renderTarget);
	}

	void SceGnmDriver::resetResourceTracker()
	{
		auto& tracker = GPU().resourceTracker();
		tracker.reset();
	}

}  // namespace sce