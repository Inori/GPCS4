#include "SceSwapchain.h"

#include "ScePresenter.h"
#include "SceSwapchainBlitter.h"
#include "SceVideoOut.h"

#include "Gnm/GnmConverter.h"
#include "Violet/VltContext.h"
#include "Violet/VltDevice.h"
#include "Violet/VltImage.h"

using namespace sce::vlt;
using namespace sce::Gnm;

namespace sce
{
	SceSwapchain::SceSwapchain(
		const SceSwapchainDevice& device,
		const PresenterDesc&      desc) :
		m_device(device),
		m_context(device.device->createContext()),
		m_blitter(new SceSwapchainBlitter(device.device))
	{
		createPresenter(desc);

		createSwapImageViews();

		createRenderTargets();
	}

	SceSwapchain::~SceSwapchain()
	{
	}

	const sce::SceRenderTarget& SceSwapchain::getImage(uint32_t index)
	{
		return m_renderTargets[index];
	}

	void SceSwapchain::present(uint32_t index)
	{
		auto& device = m_device.device;

		PresenterSync sync       = {};
		uint32_t      imageIndex = 0;

		VkResult status = m_presenter->acquireNextImage(sync, imageIndex);

		// Resolve back buffer if it is multisampled. We
		// only have to do it only for the first frame.
		m_context->beginRecording(
			device->createCommandList());

		// Get the render target which is draw to by commands from game.
		auto& target = m_renderTargets[index];

		// Transform render target to SHADER_READ layout
		// so that we can copy it to swapchain.
		// Note that the content must be preserved.
		m_context->transformImage(
			target.image,
			target.image->getAvailableSubresources(),
			VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
			VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		// Record draw commands to copy render target image to swapchain image.
		m_blitter->presentImage(m_context.ptr(),
								m_imageViews.at(imageIndex), VkRect2D(),
								target.imageView, VkRect2D());

		auto cmdList = m_context->endRecording();

		// Submit and present image to window.
		device->submitCommandList(cmdList,
								  sync.acquire, sync.present);

		device->presentImage(m_presenter);
	}

	void SceSwapchain::createPresenter(const PresenterDesc& desc)
	{
		VkInstance      instance = m_device.device->instance()->handle();
		PresenterDevice device   = {};
		device.adapter           = m_device.adapter;
		device.device            = m_device.device->handle();
		device.queue             = m_device.device->queues().graphics.queueHandle;
		device.surface           = m_device.videoOut->getSurface(instance);

		m_presenter = new ScePresenter(device, desc);
	}

	void SceSwapchain::createRenderTargets()
	{
		// Create display buffer backend render targets

		auto&       device             = m_device.device;
		auto        videoOut           = m_device.videoOut;
		uint32_t    displayBufferCount = videoOut->displayBufferCount();
		auto        gpuMode            = GPU().mode();
		const auto& attribute          = videoOut->displayBufferAttribute();

		Gnm::DataFormat format = cvt::convertDataFormatFromVideoOutPixelFormat(
			(SceVideoOutPixelFormat)attribute.pixelFormat);
		Gnm::TileMode tileMode;
		GpuAddress::computeSurfaceTileMode(
			gpuMode,                                         // NEO or base
			&tileMode,                                       // Tile mode pointer
			GpuAddress::kSurfaceTypeColorTargetDisplayable,  // Surface type
			format,                                          // Surface format
			1);                                              // Elements per pixel

		RenderTargetSpec spec;
		spec.init();
		spec.m_width                        = attribute.width;
		spec.m_height                       = attribute.height;
		spec.m_pitch                        = 0;
		spec.m_numSlices                    = 1;
		spec.m_colorFormat                  = format;
		spec.m_colorTileModeHint            = tileMode;
		spec.m_minGpuMode                   = gpuMode;
		spec.m_numSamples                   = Gnm::kNumSamples1;
		spec.m_numFragments                 = Gnm::kNumFragments1;
		spec.m_flags.enableCmaskFastClear   = 0;
		spec.m_flags.enableFmaskCompression = 0;

		m_renderTargets.clear();
		m_renderTargets.resize(displayBufferCount);

		m_context->beginRecording(
			device->createCommandList());

		for (uint32_t i = 0; i != displayBufferCount; ++i)
		{
			SceRenderTarget& target        = m_renderTargets[i];
			const auto&      displayBuffer = videoOut->getDisplayBuffer(i);

			VltImageCreateInfo imageInfo;
			imageInfo.type          = VK_IMAGE_TYPE_2D;
			imageInfo.format        = cvt::convertVideoOutPixelFormat((SceVideoOutPixelFormat)attribute.pixelFormat);
			imageInfo.flags         = 0;
			imageInfo.sampleCount   = VK_SAMPLE_COUNT_1_BIT;
			imageInfo.extent        = { attribute.width, attribute.height, 1 };
			imageInfo.numLayers     = 1;
			imageInfo.mipLevels     = 1;
			imageInfo.usage         = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
			imageInfo.stages        = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
			imageInfo.access        = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_SHADER_READ_BIT;
			imageInfo.tiling        = VK_IMAGE_TILING_OPTIMAL;  // use linear tiling so that we can use memory alias with a buffer object
			imageInfo.layout        = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

			VltImageViewCreateInfo viewInfo;
			viewInfo.type      = VK_IMAGE_VIEW_TYPE_2D;
			viewInfo.format    = imageInfo.format;
			viewInfo.usage     = imageInfo.usage;
			viewInfo.aspect    = VK_IMAGE_ASPECT_COLOR_BIT;
			viewInfo.minLevel  = 0;
			viewInfo.numLevels = 1;
			viewInfo.minLayer  = 0;
			viewInfo.numLayers = 1;

			target.image     = device->createImage(imageInfo, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
			target.imageView = device->createImageView(target.image, viewInfo);

			target.renderTarget.init(&spec);
			target.renderTarget.setAddresses(displayBuffer.address, nullptr, nullptr);

			// Initialize the image so that we can use it. Clearing
			// to black prevents garbled output for the first frame.
			VkImageSubresourceRange subresources;
			subresources.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
			subresources.baseMipLevel   = 0;
			subresources.levelCount     = 1;
			subresources.baseArrayLayer = 0;
			subresources.layerCount     = 1;
			m_context->initImage(target.image,
								 subresources, VK_IMAGE_LAYOUT_UNDEFINED);
		}

		device->submitCommandList(
			m_context->endRecording(),
			VK_NULL_HANDLE,
			VK_NULL_HANDLE);

		device->syncSubmission();
	}

	void SceSwapchain::createSwapImageViews()
	{
		PresenterInfo info = m_presenter->info();

		m_imageViews.clear();
		m_imageViews.resize(info.imageCount);

		VltImageCreateInfo imageInfo;
		imageInfo.type        = VK_IMAGE_TYPE_2D;
		imageInfo.format      = info.format.format;
		imageInfo.flags       = 0;
		imageInfo.sampleCount = VK_SAMPLE_COUNT_1_BIT;
		imageInfo.extent      = { info.imageExtent.width, info.imageExtent.height, 1 };
		imageInfo.numLayers   = 1;
		imageInfo.mipLevels   = 1;
		imageInfo.usage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		imageInfo.stages      = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		imageInfo.access      = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
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

			Rc<VltImage> image =
				m_device.device->createImageFromVkImage(imageInfo, imageHandle);

			m_imageViews[i] =
				m_device.device->createImageView(image, viewInfo);
		}
	}

}  // namespace sce