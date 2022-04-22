#include "GnmCommandBufferDraw.h"

#include "GnmBuffer.h"
#include "GnmConverter.h"
#include "GnmSampler.h"
#include "GnmSharpBuffer.h"
#include "GnmTexture.h"
#include "GpuAddress/GnmGpuAddress.h"

#include "Gcn/GcnUtil.h"
#include "Platform/PlatFile.h"
#include "Sce/SceResourceTracker.h"
#include "Sce/SceVideoOut.h"
#include "Violet/VltContext.h"
#include "Violet/VltDevice.h"
#include "Violet/VltImage.h"
#include "Violet/VltRenderTarget.h"

#include <algorithm>
#include <functional>

LOG_CHANNEL(Graphic.Gnm.GnmCommandBufferDraw);

using namespace sce::vlt;
using namespace sce::gcn;

namespace sce::Gnm
{

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
		VkFrontFace     frontFace = reg.getFrontFace() == kPrimitiveSetupFrontFaceCcw ? 
			VK_FRONT_FACE_COUNTER_CLOCKWISE : VK_FRONT_FACE_CLOCKWISE;
		VkPolygonMode   polyMode  = cvt::convertPolygonMode(reg.getPolygonModeFront());
		VkCullModeFlags cullMode  = cvt::convertCullMode(reg.getCullFace());

		VltRasterizerState rs = {
			polyMode,
			cullMode,
			frontFace,
			VK_FALSE,
			VK_FALSE,
			VK_SAMPLE_COUNT_1_BIT,
			VK_CONSERVATIVE_RASTERIZATION_MODE_DISABLED_EXT
		};

		m_context->setRasterizerState(rs);
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
		// TODO:
		// Parse the input table
		m_state.shaderContext[kShaderStagePs].meta.ps.inputSemanticCount = numItems;
	}

	void GnmCommandBufferDraw::setActiveShaderStages(ActiveShaderStages activeStages)
	{
		// TODO:
	}

	void GnmCommandBufferDraw::setPsShader(const gcn::PsStageRegisters* psRegs)
	{
		auto& ctx = m_state.shaderContext[kShaderStagePs];
		ctx.code  = psRegs->getCodeAddress();
	}

	void GnmCommandBufferDraw::updatePsShader(const gcn::PsStageRegisters* psRegs)
	{
	}

	void GnmCommandBufferDraw::setVsShader(const gcn::VsStageRegisters* vsRegs, uint32_t shaderModifier)
	{
		auto& ctx = m_state.shaderContext[kShaderStageVs];
		ctx.code  = vsRegs->getCodeAddress();
	}

	void GnmCommandBufferDraw::setEmbeddedVsShader(EmbeddedVsShader shaderId, uint32_t shaderModifier)
	{
		LOG_ASSERT(shaderId == kEmbeddedVsShaderFullScreen, "invalid shader id %d", shaderId);

		// const static uint8_t embeddedVsShaderFullScreen[] = {
		//	0xFF, 0x03, 0xEB, 0xBE, 0x07, 0x00, 0x00, 0x00, 0x81, 0x00, 0x02, 0x36, 0x81, 0x02, 0x02, 0x34,
		//	0xC2, 0x00, 0x00, 0x36, 0xC1, 0x02, 0x02, 0x4A, 0xC1, 0x00, 0x00, 0x4A, 0x01, 0x0B, 0x02, 0x7E,
		//	0x00, 0x0B, 0x00, 0x7E, 0x80, 0x02, 0x04, 0x7E, 0xF2, 0x02, 0x06, 0x7E, 0xCF, 0x08, 0x00, 0xF8,
		//	0x01, 0x00, 0x02, 0x03, 0x0F, 0x02, 0x00, 0xF8, 0x03, 0x03, 0x03, 0x03, 0x00, 0x00, 0x81, 0xBF,
		//	0x4F, 0x72, 0x62, 0x53, 0x68, 0x64, 0x72, 0x07, 0x47, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		//	0x9F, 0xC2, 0xF8, 0x47, 0xCF, 0xA5, 0x2D, 0x9B, 0x7D, 0x5B, 0x7C, 0xFF, 0x17, 0x00, 0x00, 0x00
		// };

		// Above is the original Gnm embedded vs shader for kEmbeddedVsShaderFullScreen.
		// It outputs vertex:
		// 0  (-1.0, -1.0, 0.0, 1.0)
		// 1  (1.0, -1.0, 0.0, 1.0)
		// 2  (-1.0, 1.0, 0.0, 1.0)
		// And treated it as a rectangle list,
		// this will only cover the bottom-left triangle
		// of the screen, since vulkan doesn't
		// support rect list vertex format, so we
		// have to use triangle list.

		// Below is our replaced version.
		// It outputs vertex:
		// 0  (-1.0, -1.0, 0.0, 1.0)
		// 1  (-1.0, 3.0, 0.0, 1.0)
		// 2  (3.0, -1.0, 0.0, 1.0)
		// We treated it as triangle list,
		// and this way we cover the whole screen.

		// Note:
		// The generated vertex data is in clockwise,
		// thus we must make sure the front face is
		// VK_FRONT_FACE_CLOCKWISE. And if culling is enabled,
		// it must be VK_CULL_MODE_BACK_BIT.

		// Source code
		/*
		struct VS_OUTPUT
		{
			float4 vPosition  :  S_POSITION;
			float2 vTexcoord  :  TEXCOORD0;
		};

		VS_OUTPUT main(uint VertexId:S_VERTEX_ID)
		{
			VS_OUTPUT Output;

			Output.vTexcoord = float2(
			float(VertexId & 2),
			float(VertexId & 1) * 2.0);

			Output.vPosition = float4(-1.0 + 2.0 * Output.vTexcoord, 0.0, 1.0);
			return Output;
		}
		*/

		const static uint8_t embeddedVsShaderFullScreen[] = {
			0xFF, 0x03, 0xEB, 0xBE, 0x09, 0x00, 0x00, 0x00, 0x81, 0x00, 0x02, 0x36, 0x82, 0x00, 0x00, 0x36,
			0x00, 0x0D, 0x00, 0x7E, 0x01, 0x0D, 0x04, 0x7E, 0x03, 0x00, 0x82, 0xD2, 0xF4, 0x00, 0xCE, 0x03,
			0x04, 0x00, 0x82, 0xD2, 0xF6, 0x04, 0xCE, 0x03, 0x80, 0x02, 0x02, 0x7E, 0xF2, 0x02, 0x0A, 0x7E,
			0xCF, 0x08, 0x00, 0xF8, 0x03, 0x04, 0x01, 0x05, 0xF4, 0x04, 0x04, 0x10, 0x0F, 0x02, 0x00, 0xF8,
			0x00, 0x02, 0x01, 0x01, 0x00, 0x00, 0x81, 0xBF, 0x02, 0x03, 0x00, 0x00, 0x1C, 0x61, 0x6D, 0x04,
			0x4F, 0x72, 0x62, 0x53, 0x68, 0x64, 0x72, 0x07, 0x45, 0x48, 0x00, 0x00, 0x02, 0x00, 0x08, 0x05,
			0x61, 0xDE, 0xE7, 0xD1, 0x00, 0x00, 0x00, 0x00, 0x98, 0xE5, 0xCA, 0xB9
		};

		m_state.shaderContext[kShaderStageVs].code = reinterpret_cast<const void*>(embeddedVsShaderFullScreen);
	}

	void GnmCommandBufferDraw::updateVsShader(const gcn::VsStageRegisters* vsRegs, uint32_t shaderModifier)
	{
	}

	void GnmCommandBufferDraw::setVsharpInUserData(ShaderStage stage, uint32_t startUserDataSlot, const Buffer* buffer)
	{
		std::memcpy(&m_state.shaderContext[stage].userData[startUserDataSlot], buffer, sizeof(Buffer));
	}

	void GnmCommandBufferDraw::setTsharpInUserData(ShaderStage stage, uint32_t startUserDataSlot, const Texture* tex)
	{
		std::memcpy(&m_state.shaderContext[stage].userData[startUserDataSlot], tex, sizeof(Texture));
	}

	void GnmCommandBufferDraw::setSsharpInUserData(ShaderStage stage, uint32_t startUserDataSlot, const Sampler* sampler)
	{
		std::memcpy(&m_state.shaderContext[stage].userData[startUserDataSlot], sampler, sizeof(Sampler));
	}

	void GnmCommandBufferDraw::setPointerInUserData(ShaderStage stage, uint32_t startUserDataSlot, void* gpuAddr)
	{
		std::memcpy(&m_state.shaderContext[stage].userData[startUserDataSlot], gpuAddr, sizeof(void*));
	}

	void GnmCommandBufferDraw::setUserDataRegion(ShaderStage stage, uint32_t startUserDataSlot, const uint32_t* userData, uint32_t numDwords)
	{
		std::memcpy(&m_state.shaderContext[stage].userData[startUserDataSlot], userData, numDwords * sizeof(uint32_t));
	}

	void GnmCommandBufferDraw::setRenderTarget(uint32_t rtSlot, RenderTarget const* target)
	{
		auto  resource = m_tracker->find(target->getBaseAddress());
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

			// Record display buffer.
			m_state.om.displayRenderTarget = resource;

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

			SceBuffer rtBuffer   = {};
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

	void GnmCommandBufferDraw::setDepthRenderTarget(DepthRenderTarget const* depthTarget)
	{
		auto resource = m_tracker->find(depthTarget->getZReadAddress());
		do
		{
			if (!resource)
			{
				// create a new depth image and track it

				SceDepthRenderTarget depthResource = {};
				m_factory.createDepthImage(depthTarget, depthResource);

				auto iter = m_tracker->track(depthResource).first;
				resource  = &iter->second;
			}

			VltAttachment attachment = {
				resource->depthRenderTarget().imageView,
				VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL
			};
			m_context->bindDepthRenderTarget(attachment);

		} while (false);
	}

	void GnmCommandBufferDraw::setDepthClearValue(float clearValue)
	{
		VkClearValue value;
		value.depthStencil.depth = clearValue;
		m_context->setDepthClearValue(value);
	}

	void GnmCommandBufferDraw::setStencilClearValue(uint8_t clearValue)
	{
		VkClearValue value;
		value.depthStencil.stencil = clearValue;
		m_context->setStencilClearValue(value);
	}

	void GnmCommandBufferDraw::setRenderTargetMask(uint32_t mask)
	{
		auto writeMasks = cvt::convertRenderTargetMask(mask);
		for (uint32_t attachment = 0; attachment != writeMasks.size(); ++attachment)
		{
			m_context->setBlendMask(
				attachment, writeMasks[attachment]);
		}
	}

	void GnmCommandBufferDraw::setBlendControl(uint32_t rtSlot, BlendControl blendControl)
	{
		VkBlendFactor colorSrcFactor = cvt::convertBlendMultiplier(blendControl.getColorEquationSourceMultiplier());
		VkBlendFactor colorDstFactor = cvt::convertBlendMultiplier(blendControl.getColorEquationDestinationMultiplier());
		VkBlendOp     colorBlendOp   = cvt::convertBlendFunc(blendControl.getColorEquationBlendFunction());

		VkBlendFactor alphaSrcFactor = cvt::convertBlendMultiplier(blendControl.getAlphaEquationSourceMultiplier());
		VkBlendFactor alphaDstFactor = cvt::convertBlendMultiplier(blendControl.getAlphaEquationDestinationMultiplier());
		VkBlendOp     alphaBlendOp   = cvt::convertBlendFunc(blendControl.getAlphaEquationBlendFunction());

		VkColorComponentFlags fullMask = 
			VK_COLOR_COMPONENT_R_BIT | 
			VK_COLOR_COMPONENT_G_BIT | 
			VK_COLOR_COMPONENT_B_BIT | 
			VK_COLOR_COMPONENT_A_BIT;  

		// Here we set color write mask to fullMask.
		// The real mask value should be set through setRenderTargetMask call.

		VltBlendMode blend = {
			(VkBool32)blendControl.getBlendEnable(),
			colorSrcFactor,
			colorDstFactor,
			colorBlendOp,
			alphaSrcFactor,
			alphaDstFactor,
			alphaBlendOp,
			fullMask
		};

		m_context->setBlendMode(rtSlot, blend);
	}

	void GnmCommandBufferDraw::setDepthStencilControl(DepthStencilControl depthControl)
	{
		LOG_ASSERT(depthControl.stencilEnable == false, "stencil test not supported yet.");

		VkCompareOp depthCmpOp = cvt::convertCompareFunc(depthControl.getDepthControlZCompareFunction());
		VkCompareOp stencilFront = cvt::convertCompareFunc(depthControl.getStencilFunction());
		VkCompareOp stencilBack = cvt::convertCompareFunc(depthControl.getStencilFunctionBack());

		VkStencilOpState frontOp = {};
		frontOp.compareOp        = stencilFront;
		VkStencilOpState backOp  = {};
		backOp.compareOp         = stencilBack;

		VltDepthStencilState ds = {
			(VkBool32)depthControl.depthEnable,
			(VkBool32)depthControl.zWrite,
			(VkBool32)depthControl.stencilEnable,
			depthCmpOp,
			frontOp,
			backOp
		};

		m_context->setDepthBoundsTestEnable(depthControl.depthBoundsEnable);
		m_context->setDepthStencilState(ds);
	}

	void GnmCommandBufferDraw::setDbRenderControl(DbRenderControl reg)
	{
		if (reg.getDepthClearEnable() && !reg.getHtileResummarizeEnable())
		{
			// In Gnm, when depth clear enable and HTILE compress disable
			// all writes to the depth buffer will use the depth clear value set by
			// DrawCommandBuffer::setDepthClearValue() instead of the fragment's depth value.
			// 
			// For vulkan, we use depth bound test to emulate this somehow.
			// We first set the depth clear value to clear depth buffer once render pass begin.
			// Then force depth bound test failed to leave depth buffer untouched.
			// This way the depth buffer remains the clear value.

			// TODO:
			// This approach is not accurate, fix it in the future.

			m_context->setDepthBoundsTestEnable(VK_TRUE);

			VltDepthBoundsRange depthBounds;
			depthBounds.minDepthBounds    = 1.0;
			depthBounds.maxDepthBounds    = 0.0;
			m_context->setDepthBoundsRange(depthBounds);
		}
		else
		{
			m_context->setDepthBoundsTestEnable(VK_FALSE);
		}
	}

	void GnmCommandBufferDraw::setVgtControl(uint8_t primGroupSizeMinusOne)
	{
	}

	void GnmCommandBufferDraw::setPrimitiveType(PrimitiveType primType)
	{
		VkPrimitiveTopology topology = cvt::convertPrimitiveType(primType);
		
		// TODO:
		// This is a temporary solution, mainly for embedded vertex shader.
		// For a primitive type which is not supported by vulkan natively,
		// we need to find a workaround.
		if (primType == kPrimitiveTypeRectList)
		{
			topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		}

		LOG_ASSERT(topology != VK_PRIMITIVE_TOPOLOGY_MAX_ENUM, "primType not supported.");
		m_state.ia.topology = topology;

		VltInputAssemblyState ia = {
			topology,
			VK_FALSE,
			0
		};
		m_context->setInputAssemblyState(ia);
	}

	void GnmCommandBufferDraw::setIndexSize(IndexSize indexSize, CachePolicy cachePolicy)
	{
		m_state.ia.indexType = cvt::convertIndexSize(indexSize);
	}

	void GnmCommandBufferDraw::drawIndexAuto(uint32_t indexCount, DrawModifier modifier)
	{
		m_state.ia.indexBuffer = generateIndexBuffer(indexCount);
		// If the index size is currently 32 bits, this command will partially set it to 16 bits
		m_state.ia.indexType = VK_INDEX_TYPE_UINT16;

		commitGraphicsState();

		m_context->drawIndexed(indexCount, 1, 0, 0, 0);
	}

	void GnmCommandBufferDraw::drawIndexAuto(uint32_t indexCount)
	{
		DrawModifier modifier;
		modifier.renderTargetSliceOffset = 0;
		drawIndexAuto(indexCount, modifier);
	}

	void GnmCommandBufferDraw::drawIndex(uint32_t indexCount, const void* indexAddr, DrawModifier modifier)
	{
	}

	void GnmCommandBufferDraw::drawIndex(uint32_t indexCount, const void* indexAddr)
	{
	}

	void GnmCommandBufferDraw::dispatch(uint32_t threadGroupX, uint32_t threadGroupY, uint32_t threadGroupZ)
	{
		commitComputeState();

		m_context->dispatch(threadGroupX, threadGroupY, threadGroupZ);
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
				VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
				0,
				VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
				VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
				VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT);
		}
	}

	void GnmCommandBufferDraw::prepareFlip()
	{
		onPrepareFlip();
	}

	void GnmCommandBufferDraw::prepareFlip(void* labelAddr, uint32_t value)
	{
		*(uint32_t*)labelAddr = value;
		onPrepareFlip();
	}

	void GnmCommandBufferDraw::prepareFlipWithEopInterrupt(EndOfPipeEventType eventType, CacheAction cacheAction)
	{
		onPrepareFlip();
	}

	void GnmCommandBufferDraw::prepareFlipWithEopInterrupt(EndOfPipeEventType eventType, void* labelAddr, uint32_t value, CacheAction cacheAction)
	{
		*(uint32_t*)labelAddr = value;
		onPrepareFlip();
	}

	void GnmCommandBufferDraw::setCsShader(const gcn::CsStageRegisters* computeData, uint32_t shaderModifier)
	{
		auto& ctx = m_state.shaderContext[kShaderStageCs];
		ctx.code  = computeData->getCodeAddress();

		ctx.meta.cs.computeNumThreadX = computeData->computeNumThreadX;
		ctx.meta.cs.computeNumThreadY = computeData->computeNumThreadY;
		ctx.meta.cs.computeNumThreadZ = computeData->computeNumThreadZ;
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

	Rc<VltBuffer> GnmCommandBufferDraw::generateIndexBuffer(uint32_t indexCount)
	{
		Rc<VltBuffer> buffer = nullptr;

		// Auto-generated indexes are forced in 16 bits width.
		std::vector<uint16_t> indexes;

		switch (m_state.ia.topology)
		{
		case VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST:
		{
			indexes.resize(indexCount);
			std::generate(indexes.begin(), indexes.end(),
				[n = 0]() mutable -> uint16_t { return n++; });
		}
			break;
		default:
			LOG_ASSERT(false, "topology type not supported.");
			break;
		}

		VltBufferCreateInfo info = {};
		info.size                = sizeof(uint16_t) * indexes.size();
		info.usage               = VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		info.stages              = VK_PIPELINE_STAGE_VERTEX_INPUT_BIT;
		info.access              = VK_ACCESS_INDEX_READ_BIT;

		buffer = m_device->createBuffer(info, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		m_context->uploadBuffer(buffer, indexes.data());

		return buffer;
	}

	bool GnmCommandBufferDraw::isSingleVertexBinding(
		const uint32_t*                 vtxTable,
		const VertexInputSemanticTable& semanticTable)
	{
		struct VertexElement
		{
			void*    data;
			uint32_t stride;
		};

		std::array<VertexElement, kMaxVertexBufferCount> vtxData;

		uint32_t semanticCount = semanticTable.size();
		for (uint32_t i = 0; i != semanticCount; ++i)
		{
			auto&    sema           = semanticTable[i];
			uint32_t offsetInDwords = sema.m_semantic * ShaderConstantDwordSize::kDwordSizeVertexBuffer;
			const Buffer* vtxBuffer = reinterpret_cast<const Buffer*>(vtxTable + offsetInDwords);

			vtxData[i].data   = vtxBuffer->getBaseAddress();
			vtxData[i].stride = vtxBuffer->getStride();
		}

		void* firstVertextStart = vtxData[0].data;
		void* firstVertextEnd   = reinterpret_cast<uint8_t*>(firstVertextStart) + vtxData[0].stride;

		bool isSingleBinding = true;
		// If all left vertex attribute data start address is within the first and second
		// vertex address of the first attribute data, 
		// we think the game uses a single vertex buffer binding.
		// Otherwise we use multiple bindings.
		for (uint32_t i = 1; i != semanticCount; ++i)
		{
			void* vertex = vtxData[i].data;
			isSingleBinding &= (vertex > firstVertextStart && vertex < firstVertextEnd);
		}
		return isSingleBinding;
	}

	inline void GnmCommandBufferDraw::bindVertexBuffer(
		const Buffer* vsharp, uint32_t binding)
	{
		SceBuffer buffer;

		GnmBufferCreateInfo info;
		info.vsharp     = vsharp;
		info.usage      = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		info.stage      = VK_PIPELINE_STAGE_VERTEX_INPUT_BIT | VK_PIPELINE_STAGE_TRANSFER_BIT;
		info.access     = VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT | VK_ACCESS_TRANSFER_WRITE_BIT;
		info.memoryType = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

		m_factory.createBuffer(info, buffer);
		m_tracker->track(buffer);

		m_context->uploadBuffer(buffer.buffer, vsharp->getBaseAddress());
		m_context->bindVertexBuffer(
			binding,
			VltBufferSlice(buffer.buffer, 0, buffer.buffer->info().size),
			vsharp->getStride());
	}

	void GnmCommandBufferDraw::updateVertexBinding(GcnModule& vsModule)
	{
		auto& ctx      = m_state.shaderContext[kShaderStageVs];
		auto& resTable = vsModule.getResourceTable();

		// Find fetch shader
		VertexInputSemanticTable semaTable;
		auto fsCode = findFetchShader(resTable, ctx.userData);
		if (fsCode != nullptr)
		{
			GcnFetchShader fs(fsCode);
			semaTable = fs.getVertexInputSemanticTable();
		}

		// Update input layout
		if (!semaTable.empty())
		{
			int32_t vertexTableReg = findUsageRegister(resTable, kShaderInputUsagePtrVertexBufferTable);
			LOG_ASSERT(vertexTableReg >= 0, "vertex table not found while input semantic exist.");
			const uint32_t* vertexTable = &ctx.userData[vertexTableReg];

			bool singleBinding = isSingleVertexBinding(vertexTable, semaTable);

			std::array<VltVertexAttribute, kMaxVertexBufferCount> attributes;
			std::array<VltVertexBinding, kMaxVertexBufferCount>   bindings;

			size_t   firstAttributeOffset = 0;
			uint32_t semanticCount        = semaTable.size();
			for (uint32_t i = 0; i != semanticCount; ++i)
			{
				auto&    sema           = semaTable[i];
				uint32_t offsetInDwords = sema.m_semantic * ShaderConstantDwordSize::kDwordSizeVertexBuffer;
				// We need to trust format info in V#, not instructions in fetch shader.
				// From GPU ISA:
				// The number of bytes loaded is determined solely by sV#.dfmt,
				// even if the instruction op count does not match.
				const Buffer* vsharp = reinterpret_cast<const Buffer*>(vertexTable + offsetInDwords);

				if (firstAttributeOffset == 0)
				{
					firstAttributeOffset = reinterpret_cast<size_t>(vsharp->getBaseAddress());
				}

				LOG_ASSERT(sema.m_semantic == i, "semantic index is not equal to table index.");

				// Attributes
				attributes[i].location = sema.m_semantic;
				attributes[i].binding  = singleBinding ? 
					0 : sema.m_semantic;
				attributes[i].format   = cvt::convertDataFormat(vsharp->getDataFormat());
				attributes[i].offset   = singleBinding ?
					reinterpret_cast<size_t>(vsharp->getBaseAddress()) - firstAttributeOffset : 0;

				// Bindings
				bindings[i].binding   = sema.m_semantic;
				bindings[i].fetchRate = 0;
				bindings[i].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

				// Fix element count
				sema.m_sizeInElements = 
					std::min(static_cast<uint32_t>(sema.m_sizeInElements), vsharp->getDataFormat().getNumComponents());
			}

			m_context->setInputLayout(
				semanticCount,
				attributes.data(),
				singleBinding ? 1 : semanticCount,
				bindings.data());

			// Create, upload and bind vertex buffer
			for (uint32_t i = 0; i != semanticCount; ++i)
			{
				auto&         sema           = semaTable[i];
				uint32_t      offsetInDwords = sema.m_semantic * ShaderConstantDwordSize::kDwordSizeVertexBuffer;
				const Buffer* vsharp         = reinterpret_cast<const Buffer*>(vertexTable + offsetInDwords);

				bindVertexBuffer(vsharp, sema.m_semantic);

				if (singleBinding)
				{
					break;
				}
			}

			// Record shader meta info
			ctx.meta.vs.inputSemanticCount = semanticCount;
			std::memcpy(
				ctx.meta.vs.inputSemanticTable,
				semaTable.data(),
				sizeof(VertexInputSemantic) * semanticCount);
		}
		else
		{
			// No vertex buffer bind to the pipeline.
			m_context->setInputLayout(
				0, nullptr,
				0, nullptr);
		}
	}

	void GnmCommandBufferDraw::updateVertexShaderStage()
	{
		// Update index
		// All draw calls in Gnm need index buffer.
		auto indexBuffer = m_state.ia.indexBuffer;
		m_context->bindIndexBuffer(
			VltBufferSlice(indexBuffer, 0, indexBuffer->info().size),
			m_state.ia.indexType);

		// Update vertex input
		auto& ctx = m_state.shaderContext[kShaderStageVs];

		GcnModule vsModule(
			GcnProgramType::VertexShader,
			reinterpret_cast<const uint8_t*>(ctx.code));

		auto& resTable = vsModule.getResourceTable();

		// Update input layout and bind vertex buffer
		updateVertexBinding(vsModule);

		// create and bind shader resources
		bindResource(VK_PIPELINE_STAGE_VERTEX_SHADER_BIT, resTable, ctx.userData);

		// bind the shader
		m_context->bindShader(
			VK_SHADER_STAGE_VERTEX_BIT,
			vsModule.compile(ctx.meta));
	}

	void GnmCommandBufferDraw::updatePixelShaderStage()
	{
		auto& ctx = m_state.shaderContext[kShaderStagePs];

		GcnModule psModule(
			GcnProgramType::PixelShader,
			reinterpret_cast<const uint8_t*>(ctx.code));

		auto& resTable = psModule.getResourceTable();

		// create and bind shader resources
		bindResource(VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, resTable, ctx.userData);

		// bind the shader
		m_context->bindShader(
			VK_SHADER_STAGE_COMPUTE_BIT,
			psModule.compile(ctx.meta));
	}

	void GnmCommandBufferDraw::commitGraphicsState()
	{
		updateVertexShaderStage();

		updatePixelShaderStage();
	}

	void GnmCommandBufferDraw::commitComputeState()
	{
		auto& ctx = m_state.shaderContext[kShaderStageCs];

		GcnModule csModule(
			GcnProgramType::ComputeShader,
			reinterpret_cast<const uint8_t*>(ctx.code));

		auto& resTable = csModule.getResourceTable();

		// create and bind shader resources
		bindResource(VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, resTable, ctx.userData);

		// bind the shader
		m_context->bindShader(
			VK_SHADER_STAGE_COMPUTE_BIT,
			csModule.compile(ctx.meta));
	}

	void GnmCommandBufferDraw::bindResourceBuffer(
		const Buffer*         vsharp,
		uint32_t              startRegister,
		VkBufferUsageFlags    usage,
		VkPipelineStageFlags2 stage,
		VkAccessFlagBits2     access)
	{
		SceBuffer buffer;

		GnmBufferCreateInfo info;
		info.vsharp = vsharp;
		info.usage  = usage;
		info.stage  = stage;
		info.access = access;

		uint32_t slot = 0;
		if (usage == VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT)
		{
			info.memoryType = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
							  VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

			m_factory.createBuffer(info, buffer);

			void* bufferMem = buffer.buffer->mapPtr(0);
			std::memcpy(bufferMem,
						buffer.gnmBuffer.getBaseAddress(),
						buffer.gnmBuffer.getSize());

			slot = computeConstantBufferBinding(
				gcnProgramTypeFromVkStage(stage), startRegister);
		}
		else
		{
			info.memoryType = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

			m_factory.createBuffer(info, buffer);

			m_context->uploadBuffer(buffer.buffer,
									buffer.gnmBuffer.getBaseAddress());

			slot = computeResourceBinding(
				gcnProgramTypeFromVkStage(stage), startRegister);
		}

		m_tracker->track(buffer);

		m_context->bindResourceBuffer(slot, VltBufferSlice(buffer.buffer));
	}

	void GnmCommandBufferDraw::bindResourceImage(
		const Texture*        tsharp,
		uint32_t              startRegister,
		VkImageUsageFlags     usage,
		VkPipelineStageFlags2 stage,
		VkAccessFlagBits2     access,
		VkImageTiling         tiling,
		VkImageLayout         layout)
	{
		SceTexture texture;

		GnmImageCreateInfo info;
		info.tsharp = tsharp;
		info.usage  = usage;
		info.stage  = stage;
		info.access = access;
		info.tiling = tiling;
		info.layout = layout;

		m_factory.createImage(info, texture);
		m_tracker->track(texture);

		auto& image = texture.image;

		GpuAddress::TilingParameters params;
		params.initFromTexture(tsharp, 0, 0);
		GpuAddress::SurfaceInfo surfaceInfo;
		GpuAddress::computeSurfaceInfo(&surfaceInfo, &params);

		// TODO:
		// Support multiple miplevels and layers initialize.
		// reference:
		// https://github.com/doitsujin/dxvk/blob/v1.4.6/src/d3d11/d3d11_initializer.cpp#L117

		VkImageSubresourceLayers subresourceLayers;
		subresourceLayers.aspectMask     = image->formatInfo()->aspectMask;
		subresourceLayers.mipLevel       = 0;
		subresourceLayers.baseArrayLayer = 0;
		subresourceLayers.layerCount     = 1;

		m_context->transformImage(
			image,
			image->getAvailableSubresources(),
			VK_IMAGE_LAYOUT_UNDEFINED,
			image->info().layout);

		m_context->uploadImage(
			image,
			subresourceLayers,
			tsharp->getBaseAddress(),
			tsharp->getPitch(),
			surfaceInfo.m_surfaceSize);

		uint32_t slot = computeResourceBinding(
			gcnProgramTypeFromVkStage(stage), startRegister);

		m_context->bindResourceView(slot, texture.imageView, nullptr);
	}

	void GnmCommandBufferDraw::bindResourceSampler(
		const Sampler*        ssharp,
		uint32_t              startRegister,
		VkPipelineStageFlags2 stage)
	{
		SceSampler sampler;
		m_factory.createSampler(ssharp, sampler);

		uint32_t slot = computeSamplerBinding(
			gcnProgramTypeFromVkStage(stage), startRegister);

		m_context->bindResourceSampler(slot, sampler.sampler);
	}

	void GnmCommandBufferDraw::bindResource(
		VkPipelineStageFlags          stage,
		const GcnShaderResourceTable& table,
		const UserDataArray&          userData)
	{
		// Find EUD
		uint32_t eudIndex = findUsageRegister(table, kShaderInputUsagePtrExtendedUserData);
		for (const auto& res : table)
		{
			switch (res.type)
			{
			case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
			{
				const Buffer* vsharp = reinterpret_cast<const Buffer*>(findUserData(res, eudIndex, userData));

				bindResourceBuffer(
					vsharp,
					res.startRegister,
					VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
					stage,
					VK_ACCESS_UNIFORM_READ_BIT | VK_ACCESS_TRANSFER_WRITE_BIT);
			}
				break;
			case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER:
			{
				const Buffer* vsharp = reinterpret_cast<const Buffer*>(findUserData(res, eudIndex, userData));

				bindResourceBuffer(
					vsharp,
					res.startRegister,
					VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
					stage,
					VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_SHADER_WRITE_BIT | VK_ACCESS_TRANSFER_WRITE_BIT);
			}
				break;
			case VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE:
			{
				const Texture* tsharp = reinterpret_cast<const Texture*>(findUserData(res, eudIndex, userData));

				bindResourceImage(
					tsharp,
					res.startRegister,
					VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
					stage,
					VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_TRANSFER_WRITE_BIT,
					VK_IMAGE_TILING_OPTIMAL,
					VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

				updateMetaTextureInfo(stage, res.startRegister, false, tsharp);

			}
				break;
			case VK_DESCRIPTOR_TYPE_STORAGE_IMAGE:
			{
				const Texture* tsharp = reinterpret_cast<const Texture*>(findUserData(res, eudIndex, userData));

				bindResourceImage(
					tsharp,
					res.startRegister,
					VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
					stage,
					VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_SHADER_WRITE_BIT | VK_ACCESS_TRANSFER_WRITE_BIT,
					VK_IMAGE_TILING_OPTIMAL,
					VK_IMAGE_LAYOUT_GENERAL);

				updateMetaTextureInfo(stage, res.startRegister, false, tsharp);
			}
				break;
			case VK_DESCRIPTOR_TYPE_SAMPLER:
			{
				const Sampler* ssharp = reinterpret_cast<const Sampler*>(findUserData(res, eudIndex, userData));

				bindResourceSampler(
					ssharp,
					res.startRegister,
					stage);
			}
				break;
			default:
				LOG_ASSERT(false, "resource type not supported.");
				break;
			}
		}
	}

	const uint8_t* GnmCommandBufferDraw::findFetchShader(
		const gcn::GcnShaderResourceTable& table,
		const UserDataArray&               userData)
	{
		const uint8_t* fsCode = nullptr;

		int32_t fsReg = findUsageRegister(table, kShaderInputUsageSubPtrFetchShader);
		if (fsReg >= 0)
		{
			fsCode = reinterpret_cast<const uint8_t*>(&userData[fsReg]);
		}
		return fsCode;
	}


	const uint32_t* GnmCommandBufferDraw::findUserData(
		const gcn::GcnShaderResource& res,
		uint32_t                      eudIndex,
		const UserDataArray&          userData)
	{
		const uint32_t* registerData = nullptr;
		if (!res.inEud)
		{
			registerData = &userData[res.startRegister];
		}
		else
		{
			const uint32_t* eudTable = &userData[eudIndex];
			registerData             = &eudTable[res.eudOffsetInDwords];
		}
		return registerData;
	}

	int32_t GnmCommandBufferDraw::findUsageRegister(
		const GcnShaderResourceTable& table,
		uint32_t                      usage)
	{
		int32_t regIndex = -1;
		auto    iter     = std::find_if(table.begin(), table.end(),
										[usage](const GcnShaderResource& res)
										{
											return res.usage == usage;
										});
		if (iter != table.end())
		{
			regIndex = static_cast<uint32_t>(iter->startRegister);
		}

		return regIndex;
	}

	void GnmCommandBufferDraw::onPrepareFlip()
	{
		// This is the last cmd for a command buffer submission,
		// we can do some finish works before submit and present.

		if (m_state.om.displayRenderTarget)
		{
			auto& image = m_state.om.displayRenderTarget->renderTarget().image;
			// Transform render target to SHADER_READ layout
			// so that we can copy it to swapchain.
			// Note that the content must be preserved.
			m_context->transformImage(
				image,
				image->getAvailableSubresources(),
				VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
				VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		}
	}

	ShaderStage GnmCommandBufferDraw::getShaderStage(
		VkPipelineStageFlags pipeStage)
	{
		ShaderStage shaderStage = kShaderStageCount;
		// clang-format off
		switch (pipeStage)
		{
		case VK_PIPELINE_STAGE_VERTEX_SHADER_BIT: shaderStage = kShaderStageVs; break;
		case VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT: shaderStage = kShaderStagePs; break;
		case VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT: shaderStage = kShaderStageCs; break;
		case VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT: shaderStage = kShaderStageGs; break;
		case VK_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT: shaderStage = kShaderStageHs; break;
		// Don't know which stage equals domain, and what the fuck is ES LS?
		//case VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT: shaderStage = kShaderStageEs; break;
		default:
			LOG_ASSERT(false, "pipeline stage can not be converted.");
			break;
		}
		// clang-format on
		return shaderStage;
	}

	void GnmCommandBufferDraw::updateMetaTextureInfo(
		VkPipelineStageFlags stage,
		uint32_t             startRegister,
		bool                 isDepth,
		const Texture*       tsharp)
	{
		// T# information is ripped upon uploading shader binary to GPU,
		// yet we need these information to proper declare image resource
		// when recompiling shaders.

		GcnTextureInfo info;
		info.textureType = tsharp->getTextureType();
		info.channelType = tsharp->getTextureChannelType();
		info.isDepth     = isDepth;

		switch (stage)
		{
		case VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT:
		{
			auto& ctx = m_state.shaderContext[kShaderStagePs];
			ctx.meta.ps.textureInfos[startRegister] = info;
		}
			break;
		case VK_PIPELINE_STAGE_VERTEX_SHADER_BIT:
		case VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT:
		case VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT:
		case VK_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT:
		case VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT:
		default:
			LOG_ASSERT(false, "TODO: stage %d is not supported yet, please support it.", stage);
			break;
		}
	}

}  // namespace sce::Gnm