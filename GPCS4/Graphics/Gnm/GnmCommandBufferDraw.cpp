#include "GnmCommandBufferDraw.h"

#include "Emulator.h"
#include "GnmBuffer.h"
#include "GnmConverter.h"
#include "GnmSampler.h"
#include "GnmSharpBuffer.h"
#include "GnmTexture.h"
#include "GpuAddress/GnmGpuAddress.h"
#include "VirtualGPU.h"

#include "Platform/PlatFile.h"
#include "Sce/SceResourceTracker.h"
#include "Sce/SceVideoOut.h"
#include "Violet/VltDevice.h"
#include "Violet/VltContext.h"
#include "Violet/VltImage.h"
#include "Violet/VltRenderTarget.h"

#include <algorithm>
#include <functional>

LOG_CHANNEL(Graphic.Gnm.GnmCommandBufferDraw);

using namespace sce::vlt;
using namespace sce::gcn;

namespace sce::Gnm
{

	// The compute shader using to clear color render target
	constexpr uint64_t ShaderHashClearRT = 0x8C25642DB09D8E59;

// Use this to break on a shader you want to debug.
#define SHADER_DEBUG_BREAK(mod, hash)  \
	if (mod->key().toUint64() == hash) \
	{                                  \
		__debugbreak();                \
	}

	GnmCommandBufferDraw::GnmCommandBufferDraw(vlt::VltDevice* device) :
		GnmCommandBuffer(device)
	{
	}

	GnmCommandBufferDraw::~GnmCommandBufferDraw()
	{
	}

	void GnmCommandBufferDraw::initializeDefaultHardwareState()
	{
	}

	void GnmCommandBufferDraw::setViewportTransformControl(ViewportTransformControl vportControl)
	{
		// TODO:
	}

	void GnmCommandBufferDraw::setPrimitiveSetup(PrimitiveSetup reg)
	{
	}

	void GnmCommandBufferDraw::setScreenScissor(int32_t left, int32_t top, int32_t right, int32_t bottom)
	{
		VkRect2D scissor;
		scissor.offset.x      = left;
		scissor.offset.y      = top;
		scissor.extent.width  = right - left;
		scissor.extent.height = bottom - top;
		m_context->setScissors(1, &scissor);
	}

	void GnmCommandBufferDraw::setViewport(uint32_t viewportId, float dmin, float dmax, const float scale[3], const float offset[3])
	{
		// The viewport's origin in Gnm is in the lower left of the screen,
		// with Y pointing up.
		// In Vulkan the origin is in the top left of the screen,
		// with Y pointing downwards.
		// We need to flip the viewport of gnm to adapt to vulkan.
		//
		// Note, this is going to work with VK_KHR_Maintenance1 extension enabled,
		// which is the default of Vulkan 1.1.
		// And we must use dynamic viewport state (vkCmdSetViewport), or negative viewport height won't work.

		float width  = scale[0] / 0.5f;
		float height = -scale[1] / 0.5f;
		float left   = offset[0] - scale[0];
		float top    = offset[1] + scale[1];

		VkViewport viewport;
		viewport.x        = left;
		viewport.y        = top + height;
		viewport.width    = width;
		viewport.height   = -height;
		viewport.minDepth = dmin;
		viewport.maxDepth = dmax;

		m_context->setViewports(1, &viewport);
	}

	void GnmCommandBufferDraw::setHardwareScreenOffset(uint32_t offsetX, uint32_t offsetY)
	{
		// TODO:
	}

	void GnmCommandBufferDraw::setGuardBands(float horzClip, float vertClip, float horzDiscard, float vertDiscard)
	{
		// TODO:
	}

	void GnmCommandBufferDraw::setPsShaderUsage(const uint32_t* inputTable, uint32_t numItems)
	{
	}

	void GnmCommandBufferDraw::setActiveShaderStages(ActiveShaderStages activeStages)
	{
	}

	void GnmCommandBufferDraw::setPsShader(const gcn::PsStageRegisters* psRegs)
	{
	}

	void GnmCommandBufferDraw::updatePsShader(const gcn::PsStageRegisters* psRegs)
	{
	}

	void GnmCommandBufferDraw::setVsShader(const gcn::VsStageRegisters* vsRegs, uint32_t shaderModifier)
	{
	}

	void GnmCommandBufferDraw::setEmbeddedVsShader(EmbeddedVsShader shaderId, uint32_t shaderModifier)
	{
	}

	void GnmCommandBufferDraw::updateVsShader(const gcn::VsStageRegisters* vsRegs, uint32_t shaderModifier)
	{
	}

	void GnmCommandBufferDraw::setVsharpInUserData(ShaderStage stage, uint32_t startUserDataSlot, const Buffer* buffer)
	{
	}

	void GnmCommandBufferDraw::setTsharpInUserData(ShaderStage stage, uint32_t startUserDataSlot, const Texture* tex)
	{
	}

	void GnmCommandBufferDraw::setSsharpInUserData(ShaderStage stage, uint32_t startUserDataSlot, const Sampler* sampler)
	{
	}

	void GnmCommandBufferDraw::setPointerInUserData(ShaderStage stage, uint32_t startUserDataSlot, void* gpuAddr)
	{
	}

	void GnmCommandBufferDraw::setUserDataRegion(ShaderStage stage, uint32_t startUserDataSlot, const uint32_t* userData, uint32_t numDwords)
	{
	}

	void GnmCommandBufferDraw::setRenderTarget(uint32_t rtSlot, RenderTarget const* target)
	{
		auto& tracker = GPU().resourceTracker();
		auto  resource = tracker.find(target->getBaseAddress());
		do 
		{
			if (!resource)
			{
				// TODO:
				// we should create new vulkan image for render target not found.
				// see setDepthRenderTarget
				LOG_ERR("can not find render target for slot %d", rtSlot);
				break;
			}

			// update render target
			SceRenderTarget rtRes = {};
			rtRes.image           = resource->renderTarget().image;
			rtRes.imageView       = resource->renderTarget().imageView;
			// replace the dummy target with real one
			rtRes.renderTarget = *target;
			resource->setRenderTarget(rtRes);

			// Currently, we use vulkan swapchain image directly
			// as the gnm render target's backend. 
			// (Maybe we should use a standalone vulkan image then bilt to swapchain in the future)
			// And swapchain images are in optimal tiling mode.
			// 
			// In Gnm, a common way to clear render target
			// is to treat it as a normal buffer and use compute shader to write the desired
			// values directly.
			// 
			// Hence comes the problem:
			// we can't write the swapchain images directly by the translated shader because it's not linear.
			// 
			// To support this, we use a linear staging buffer as the backend
			// of the render target buffer and copy the content to swapchain image
			// at binding time, converting the tiling mode implicitly.

			SceBuffer rtBuffer = {};
			uint32_t  bufferSize = target->getColorSizeAlign().m_size;
			uint32_t  numUints   = bufferSize / sizeof(uint32_t);
			rtBuffer.gnmBuffer.initAsDataBuffer(target->getBaseAddress(), Gnm::kDataFormatR32Uint, numUints);

			VltBufferCreateInfo info;
			info.size   = bufferSize;
			info.usage  = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
			info.stages = VK_PIPELINE_STAGE_TRANSFER_BIT;
			info.access = VK_ACCESS_TRANSFER_READ_BIT;

			rtBuffer.buffer = m_device->createBuffer(info,
													 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
													 VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
			resource->setBuffer(rtBuffer);

			VltAttachment attachment = 
			{
				rtRes.imageView,
				VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
			};
			m_context->bindRenderTarget(rtSlot, attachment);

		} while (false);
	}

	void GnmCommandBufferDraw::createDepthImage(
		const DepthRenderTarget* depthTarget,
		SceDepthRenderTarget&    depthImage)
	{
		VltImageCreateInfo imgInfo;
		imgInfo.type          = VK_IMAGE_TYPE_2D;
		imgInfo.format        = cvt::convertZFormat(depthTarget->getZFormat());
		imgInfo.flags         = 0;
		imgInfo.sampleCount   = cvt::convertNumFragments(depthTarget->getNumFragments());  // not really understand..
		imgInfo.extent.width  = depthTarget->getWidth();
		imgInfo.extent.height = depthTarget->getHeight();
		imgInfo.extent.depth  = 1;
		// NOTE: this slice count is only valid if the array view hasn't changed since initialization!
		imgInfo.numLayers = depthTarget->getLastArraySliceIndex() - depthTarget->getBaseArraySliceIndex() + 1;
		imgInfo.mipLevels = 1;
		imgInfo.usage     = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
		imgInfo.stages    = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
		imgInfo.access    = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
		imgInfo.tiling    = VK_IMAGE_TILING_OPTIMAL;
		imgInfo.layout    = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL_KHR;

		VltImageViewCreateInfo viewInfo;
		viewInfo.type      = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format    = imgInfo.format;
		viewInfo.usage     = imgInfo.usage;
		viewInfo.aspect    = VK_IMAGE_ASPECT_DEPTH_BIT;
		viewInfo.minLevel  = 0;
		viewInfo.numLevels = 1;
		viewInfo.minLayer  = 0;
		viewInfo.numLayers = 1;

		VltBufferCreateInfo info;
		info.size   = depthTarget->getZSizeAlign().m_size;
		info.usage  = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		info.stages = VK_PIPELINE_STAGE_TRANSFER_BIT;
		info.access = VK_ACCESS_TRANSFER_READ_BIT;

		depthImage.image     = m_device->createImage(
            imgInfo, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		depthImage.imageView = m_device->createImageView(
			depthImage.image, viewInfo);
		depthImage.depthRenderTarget = *depthTarget;
	}

	void GnmCommandBufferDraw::setDepthRenderTarget(DepthRenderTarget const* depthTarget)
	{
		auto& tracker  = GPU().resourceTracker();
		auto  resource = tracker.find(depthTarget->getZReadAddress());
		do 
		{
			if (!resource)
			{
				// create a new depth image and track it

				SceDepthRenderTarget depthResource = {};
				createDepthImage(depthTarget, depthResource);

				auto iter = tracker.track(depthResource).first;
				resource  = &iter->second;
			}
			
			VltAttachment attachment =
			{
				resource->depthRenderTarget().imageView,
				VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL
			};
			m_context->bindDepthRenderTarget(attachment);

		} while (false);
	}

	void GnmCommandBufferDraw::setDepthClearValue(float clearValue)
	{
	}

	void GnmCommandBufferDraw::setStencilClearValue(uint8_t clearValue)
	{
	}

	void GnmCommandBufferDraw::setRenderTargetMask(uint32_t mask)
	{
	}

	void GnmCommandBufferDraw::setBlendControl(uint32_t rtSlot, BlendControl blendControl)
	{
	}

	void GnmCommandBufferDraw::setDepthStencilControl(DepthStencilControl depthControl)
	{
	}

	void GnmCommandBufferDraw::setDbRenderControl(DbRenderControl reg)
	{
	}

	void GnmCommandBufferDraw::setVgtControl(uint8_t primGroupSizeMinusOne)
	{
	}

	void GnmCommandBufferDraw::setPrimitiveType(PrimitiveType primType)
	{
	}

	void GnmCommandBufferDraw::setIndexSize(IndexSize indexSize, CachePolicy cachePolicy)
	{
	}

	void GnmCommandBufferDraw::drawIndexAuto(uint32_t indexCount, DrawModifier modifier)
	{
	}

	void GnmCommandBufferDraw::drawIndexAuto(uint32_t indexCount)
	{
	}

	void GnmCommandBufferDraw::drawIndex(uint32_t indexCount, const void* indexAddr, DrawModifier modifier)
	{
	}

	void GnmCommandBufferDraw::drawIndex(uint32_t indexCount, const void* indexAddr)
	{
	}

	void GnmCommandBufferDraw::dispatch(uint32_t threadGroupX, uint32_t threadGroupY, uint32_t threadGroupZ)
	{
	}

	void GnmCommandBufferDraw::dispatchWithOrderedAppend(uint32_t threadGroupX, uint32_t threadGroupY, uint32_t threadGroupZ, DispatchOrderedAppendMode orderedAppendMode)
	{
	}

	void GnmCommandBufferDraw::writeDataInline(void* dstGpuAddr, const void* data, uint32_t sizeInDwords, WriteDataConfirmMode writeConfirm)
	{
	}

	void GnmCommandBufferDraw::writeDataInlineThroughL2(void* dstGpuAddr, const void* data, uint32_t sizeInDwords, CachePolicy cachePolicy, WriteDataConfirmMode writeConfirm)
	{
	}

	void GnmCommandBufferDraw::writeAtEndOfPipe(EndOfPipeEventType eventType, EventWriteDest dstSelector, void* dstGpuAddr, EventWriteSource srcSelector, uint64_t immValue, CacheAction cacheAction, CachePolicy cachePolicy)
	{
		emuWriteGpuLabel(srcSelector, dstGpuAddr, immValue);
	}

	void GnmCommandBufferDraw::writeAtEndOfPipeWithInterrupt(EndOfPipeEventType eventType, EventWriteDest dstSelector, void* dstGpuAddr, EventWriteSource srcSelector, uint64_t immValue, CacheAction cacheAction, CachePolicy cachePolicy)
	{
		emuWriteGpuLabel(srcSelector, dstGpuAddr, immValue);
	}

	void GnmCommandBufferDraw::writeAtEndOfShader(EndOfShaderEventType eventType, void* dstGpuAddr, uint32_t immValue)
	{
	}

	void GnmCommandBufferDraw::waitOnAddress(void* gpuAddr, uint32_t mask, WaitCompareFunc compareFunc, uint32_t refValue)
	{
	}

	void GnmCommandBufferDraw::waitOnAddressAndStallCommandBufferParser(void* gpuAddr, uint32_t mask, uint32_t refValue)
	{
	}

	void GnmCommandBufferDraw::waitForGraphicsWrites(uint32_t baseAddr256, uint32_t sizeIn256ByteBlocks, uint32_t targetMask, CacheAction cacheAction, uint32_t extendedCacheMask, StallCommandBufferParserMode commandBufferStallMode)
	{
	}

	void GnmCommandBufferDraw::setDepthStencilDisable()
	{
	}

	void GnmCommandBufferDraw::flushShaderCachesAndWait(CacheAction cacheAction, uint32_t extendedCacheMask, StallCommandBufferParserMode commandBufferStallMode)
	{
	}

	void GnmCommandBufferDraw::waitUntilSafeForRendering(uint32_t videoOutHandle, uint32_t displayBufferIndex)
	{
		// This cmd blocks command processor until the specified display buffer is no longer displayed.
		// should we call vkAcquireNextImageKHR here to implement it?
		// or should we create a new render target image and then bilt to swapchain like DXVK does?

		// get render target from swapchain
		auto& tracker    = GPU().resourceTracker();
		auto& videoOut   = GPU().videoOutGet(videoOutHandle);
		auto  dispBuffer = videoOut.getDisplayBuffer(displayBufferIndex);

		auto res = tracker.find(dispBuffer.address);
		if (res)
		{
			auto& image = res->renderTarget().image;
			auto  range = res->renderTarget().imageView->imageSubresources();
			m_context->transformImage(
				image,
				range,
				VK_IMAGE_LAYOUT_UNDEFINED,
				VK_PIPELINE_STAGE_2_TOP_OF_PIPE_BIT,
				0,
				VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL_KHR,
				VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
				VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT);
		}
	}

	void GnmCommandBufferDraw::prepareFlip()
	{
	}

	void GnmCommandBufferDraw::prepareFlip(void* labelAddr, uint32_t value)
	{
		*(uint32_t*)labelAddr = value;
	}

	void GnmCommandBufferDraw::prepareFlipWithEopInterrupt(EndOfPipeEventType eventType, CacheAction cacheAction)
	{
	}

	void GnmCommandBufferDraw::prepareFlipWithEopInterrupt(EndOfPipeEventType eventType, void* labelAddr, uint32_t value, CacheAction cacheAction)
	{
		*(uint32_t*)labelAddr = value;
	}

	void GnmCommandBufferDraw::setCsShader(const gcn::CsStageRegisters* computeData, uint32_t shaderModifier)
	{
	}

	void GnmCommandBufferDraw::writeReleaseMemEventWithInterrupt(ReleaseMemEventType eventType, EventWriteDest dstSelector, void* dstGpuAddr, EventWriteSource srcSelector, uint64_t immValue, CacheAction cacheAction, CachePolicy writePolicy)
	{
		emuWriteGpuLabel(srcSelector, dstGpuAddr, immValue);
	}

	void GnmCommandBufferDraw::writeReleaseMemEvent(ReleaseMemEventType eventType, EventWriteDest dstSelector, void* dstGpuAddr, EventWriteSource srcSelector, uint64_t immValue, CacheAction cacheAction, CachePolicy writePolicy)
	{
		emuWriteGpuLabel(srcSelector, dstGpuAddr, immValue);
	}

	void GnmCommandBufferDraw::setVgtControlForNeo(uint8_t primGroupSizeMinusOne, WdSwitchOnlyOnEopMode wdSwitchOnlyOnEopMode, VgtPartialVsWaveMode partialVsWaveMode)
	{
	}

}  // namespace sce::Gnm