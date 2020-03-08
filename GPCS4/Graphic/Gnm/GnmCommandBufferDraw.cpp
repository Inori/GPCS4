#include "GnmCommandBufferDraw.h"

#include "GnmSharpBuffer.h"
#include "GnmBuffer.h"
#include "GnmTexture.h"
#include "GnmSampler.h"
#include "GnmConvertor.h"

#include "GpuAddress/GnmGpuAddress.h"

#include "../Gve/GveCmdList.h"
#include "../Gve/GveShader.h"
#include "../Gve/GveContext.h"
#include "../Gve/GvePresenter.h"
#include "../Pssl/PsslShaderModule.h"

#include <algorithm>

LOG_CHANNEL(Graphic.Gnm.GnmCommandBufferDraw);

using namespace sce;
using namespace gve;
using namespace pssl;

GnmCommandBufferDraw::GnmCommandBufferDraw(
	const SceGpuQueueDevice& device,
	const RcPtr<GveContext>& context) :
	GnmCommandBuffer(device, context),
	m_factory(&device)
{
	
}

GnmCommandBufferDraw::~GnmCommandBufferDraw()
{
}

void GnmCommandBufferDraw::initializeDefaultHardwareState()
{
	m_context->beginRecording(
		m_device->createCmdList(GvePipelineType::Graphics));

	clearRenderState();
}

void GnmCommandBufferDraw::setViewportTransformControl(ViewportTransformControl vportControl)
{
	
}

void GnmCommandBufferDraw::setPrimitiveSetup(PrimitiveSetup reg)
{
	VkFrontFace     frontFace = reg.getFrontFace() == kPrimitiveSetupFrontFaceCcw ? 
		VK_FRONT_FACE_COUNTER_CLOCKWISE :
		VK_FRONT_FACE_CLOCKWISE;
	VkPolygonMode   polyMode  = cvt::convertPolygonMode(reg.getPolygonModeFront());
	VkCullModeFlags cullMode  = cvt::convertCullMode(reg.getCullFace());

	auto rsInfo = GveRasterizationInfo(
		VK_FALSE,
		VK_FALSE,
		polyMode,
		cullMode,
		frontFace,
		VK_SAMPLE_COUNT_1_BIT);

	m_context->setRasterizerState(rsInfo);
}

void GnmCommandBufferDraw::setScreenScissor(int32_t left, int32_t top, int32_t right, int32_t bottom)
{
	
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

	VkRect2D scissor;
	scissor.offset.x      = 0;
	scissor.offset.y      = 0;
	scissor.extent.width  = width;
	scissor.extent.height = height;

	m_context->setViewport(viewport, scissor);
}

void GnmCommandBufferDraw::setHardwareScreenOffset(uint32_t offsetX, uint32_t offsetY)
{
	
}

void GnmCommandBufferDraw::setGuardBands(float horzClip, float vertClip, float horzDiscard, float vertDiscard)
{
	
}

void GnmCommandBufferDraw::setPsShaderUsage(const uint32_t* inputTable, uint32_t numItems)
{
	
}

void GnmCommandBufferDraw::setActiveShaderStages(ActiveShaderStages activeStages)
{
	
}

void GnmCommandBufferDraw::setPsShader(const PsStageRegisters* psRegs)
{
	m_shaders.ps.code = psRegs->getCodeAddress();
}

void GnmCommandBufferDraw::updatePsShader(const PsStageRegisters* psRegs)
{
	m_shaders.ps.code = psRegs->getCodeAddress();
}

void GnmCommandBufferDraw::setVsShader(const VsStageRegisters* vsRegs, uint32_t shaderModifier)
{
	m_shaders.vs.code = vsRegs->getCodeAddress();
}

void GnmCommandBufferDraw::setEmbeddedVsShader(EmbeddedVsShader shaderId, uint32_t shaderModifier)
{
	LOG_ASSERT(shaderId == kEmbeddedVsShaderFullScreen, "invalid shader id %d", shaderId);

	const static uint8_t embeddedVsShaderFullScreen[] = 
	{
		0xFF, 0x03, 0xEB, 0xBE, 0x07, 0x00, 0x00, 0x00, 0x81, 0x00, 0x02, 0x36, 0x81, 0x02, 0x02, 0x34,
		0xC2, 0x00, 0x00, 0x36, 0xC1, 0x02, 0x02, 0x4A, 0xC1, 0x00, 0x00, 0x4A, 0x01, 0x0B, 0x02, 0x7E,
		0x00, 0x0B, 0x00, 0x7E, 0x80, 0x02, 0x04, 0x7E, 0xF2, 0x02, 0x06, 0x7E, 0xCF, 0x08, 0x00, 0xF8,
		0x01, 0x00, 0x02, 0x03, 0x0F, 0x02, 0x00, 0xF8, 0x03, 0x03, 0x03, 0x03, 0x00, 0x00, 0x81, 0xBF,
		0x4F, 0x72, 0x62, 0x53, 0x68, 0x64, 0x72, 0x07, 0x47, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x9F, 0xC2, 0xF8, 0x47, 0xCF, 0xA5, 0x2D, 0x9B, 0x7D, 0x5B, 0x7C, 0xFF, 0x17, 0x00, 0x00, 0x00
	};
	
	m_shaders.vs.code = reinterpret_cast<const void*>(embeddedVsShaderFullScreen);
}

void GnmCommandBufferDraw::updateVsShader(const VsStageRegisters* vsRegs, uint32_t shaderModifier)
{
	m_shaders.vs.code = vsRegs->getCodeAddress();
}

void GnmCommandBufferDraw::setVsharpInUserData(ShaderStage stage, uint32_t startUserDataSlot, const GnmBuffer* buffer)
{
	setUserDataSlots(stage, startUserDataSlot, (uint32_t*)buffer, sizeof(GnmBuffer) / sizeof(uint32_t));
}

void GnmCommandBufferDraw::setTsharpInUserData(ShaderStage stage, uint32_t startUserDataSlot, const GnmTexture* tex)
{
	setUserDataSlots(stage, startUserDataSlot, (uint32_t*)tex, sizeof(GnmTexture) / sizeof(uint32_t));
}

void GnmCommandBufferDraw::setSsharpInUserData(ShaderStage stage, uint32_t startUserDataSlot, const GnmSampler* sampler)
{
	setUserDataSlots(stage, startUserDataSlot, (uint32_t*)sampler, sizeof(GnmSampler) / sizeof(uint32_t));
}

void GnmCommandBufferDraw::setPointerInUserData(ShaderStage stage, uint32_t startUserDataSlot, void* gpuAddr)
{
	setUserDataSlots(stage, startUserDataSlot, (uint32_t*)gpuAddr, sizeof(void*) / sizeof(uint32_t));
}

void GnmCommandBufferDraw::setUserDataRegion(ShaderStage stage, uint32_t startUserDataSlot, const uint32_t* userData, uint32_t numDwords)
{
	setUserDataSlots(stage, startUserDataSlot, userData, numDwords);
}

void GnmCommandBufferDraw::setRenderTarget(uint32_t rtSlot, GnmRenderTarget const* target)
{
	LOG_ASSERT(rtSlot == 0, "only support one render target at 0");

	auto image = m_factory.grabRenderTarget(*target);

	GveAttachment colorTarget                 = {};
	colorTarget.view                          = image.view;
	colorTarget.layout                        = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	m_state.gp.om.renderTargets.color[rtSlot] = colorTarget;
}

void GnmCommandBufferDraw::setDepthRenderTarget(GnmDepthRenderTarget const* depthTarget)
{
	auto depthImage = m_factory.grabDepthRenderTarget(*depthTarget);

	GveAttachment depthAttachment     = {};
	depthAttachment.view              = depthImage.view;
	depthAttachment.layout            = depthImage.view->imageInfo().layout;
	m_state.gp.om.renderTargets.depth = depthAttachment;
}

void GnmCommandBufferDraw::setDepthClearValue(float clearValue)
{
	m_state.gp.om.depthClearValue.depth = clearValue;
}

void GnmCommandBufferDraw::setStencilClearValue(uint8_t clearValue)
{
	m_state.gp.om.depthClearValue.stencil = clearValue;
}

void GnmCommandBufferDraw::setRenderTargetMask(uint32_t mask)
{
	
}

void GnmCommandBufferDraw::setBlendControl(uint32_t rtSlot, BlendControl blendControl)
{
	auto cbInfo = GveColorBlendInfo(
		VK_FALSE,
		VK_LOGIC_OP_COPY);

	VkBlendFactor colorSrcFactor = cvt::convertBlendMultiplierToVkFactor(blendControl.getColorEquationSourceMultiplier());
	VkBlendFactor colorDstFactor = cvt::convertBlendMultiplierToVkFactor(blendControl.getColorEquationDestinationMultiplier());
	VkBlendOp     colorBlendOp   = cvt::convertBlendFuncToVkOp(blendControl.getColorEquationBlendFunction());

	VkBlendFactor alphaSrcFactor = cvt::convertBlendMultiplierToVkFactor(blendControl.getAlphaEquationSourceMultiplier());
	VkBlendFactor alphaDstFactor = cvt::convertBlendMultiplierToVkFactor(blendControl.getAlphaEquationDestinationMultiplier());
	VkBlendOp     alphaBlendOp   = cvt::convertBlendFuncToVkOp(blendControl.getAlphaEquationBlendFunction());

	VkColorComponentFlags colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

	auto colorAttach = GveColorBlendAttachment(
		blendControl.getBlendEnable(),
		colorSrcFactor,
		colorDstFactor,
		colorBlendOp,
		alphaSrcFactor,
		alphaDstFactor,
		alphaBlendOp,
		colorWriteMask);

	cbInfo.addAttachment(colorAttach);

	m_context->setColorBlendState(cbInfo);
}

void GnmCommandBufferDraw::setDepthStencilControl(DepthStencilControl depthControl)
{
	VkCompareOp depthCmpOp = cvt::convertCompareFunc(depthControl.getDepthControlZCompareFunction());

	GveDepthStencilOp frontOp;
	GveDepthStencilOp backOp;

	LOG_ASSERT(depthControl.stencilEnable == false, "stencil test not supported yet.");

	auto dsInfo = GveDepthStencilInfo(
		depthControl.depthEnable,
		depthControl.zWrite,
		depthControl.depthBoundsEnable,
		depthControl.stencilEnable,
		depthCmpOp,
		frontOp,
		backOp);

	m_context->setDepthStencilState(dsInfo);
}

void GnmCommandBufferDraw::setDbRenderControl(DbRenderControl reg)
{
	
}

void GnmCommandBufferDraw::setVgtControl(uint8_t primGroupSizeMinusOne)
{
	
}

void GnmCommandBufferDraw::setPrimitiveType(PrimitiveType primType)
{
	VkPrimitiveTopology topology = cvt::convertPrimitiveTypeToVkTopology(primType);

	auto isInfo = GveInputAssemblyInfo(
		topology,
		VK_FALSE,
		0);

	m_context->setInputAssemblyState(isInfo);
}

void GnmCommandBufferDraw::setIndexSize(IndexSize indexSize, CachePolicy cachePolicy)
{
	m_state.gp.ia.indexType = cvt::convertIndexSize(indexSize);
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

void GnmCommandBufferDraw::flushShaderCachesAndWait(CacheAction cacheAction, uint32_t extendedCacheMask, StallCommandBufferParserMode commandBufferStallMode)
{
	
}

void GnmCommandBufferDraw::waitUntilSafeForRendering(uint32_t videoOutHandle, uint32_t displayBufferIndex)
{
	
}

void GnmCommandBufferDraw::prepareFlip()
{
	m_cmdList = m_context->endRecording();
}

void GnmCommandBufferDraw::prepareFlip(void* labelAddr, uint32_t value)
{
	m_cmdList             = m_context->endRecording();
	*(uint32_t*)labelAddr = value;
}

void GnmCommandBufferDraw::prepareFlipWithEopInterrupt(EndOfPipeEventType eventType, CacheAction cacheAction)
{
	m_cmdList = m_context->endRecording();
}

void GnmCommandBufferDraw::prepareFlipWithEopInterrupt(EndOfPipeEventType eventType, void* labelAddr, uint32_t value, CacheAction cacheAction)
{
	m_cmdList             = m_context->endRecording();
	*(uint32_t*)labelAddr = value;
}

void GnmCommandBufferDraw::setCsShader(const CsStageRegisters* computeData, uint32_t shaderModifier)
{
	m_shaders.cs.code = computeData->getCodeAddress();
	shader::parseShaderRegCs(computeData, m_shaders.cs.meta.cs);
}

void GnmCommandBufferDraw::writeReleaseMemEventWithInterrupt(ReleaseMemEventType eventType, EventWriteDest dstSelector, void* dstGpuAddr, EventWriteSource srcSelector, uint64_t immValue, CacheAction cacheAction, CachePolicy writePolicy)
{
	
}

void GnmCommandBufferDraw::writeReleaseMemEvent(ReleaseMemEventType eventType, EventWriteDest dstSelector, void* dstGpuAddr, EventWriteSource srcSelector, uint64_t immValue, CacheAction cacheAction, CachePolicy writePolicy)
{
	
}

void GnmCommandBufferDraw::setVgtControlForNeo(uint8_t primGroupSizeMinusOne, WdSwitchOnlyOnEopMode wdSwitchOnlyOnEopMode, VgtPartialVsWaveMode partialVsWaveMode)
{
	
}

void GnmCommandBufferDraw::commitVsStage()
{
}

void GnmCommandBufferDraw::commitPsStage()
{
}

void GnmCommandBufferDraw::commitGraphicsStages()
{
}

void GnmCommandBufferDraw::commitCsStage()
{
}

void GnmCommandBufferDraw::commitComputeStages()
{
}

void GnmCommandBufferDraw::clearRenderState()
{
	m_state = GnmContextState();
	m_shaders = GnmShaderContextGroup();
}

void GnmCommandBufferDraw::setUserDataSlots(
	ShaderStage     stage,
	uint32_t        startSlot,
	const uint32_t* data,
	uint32_t        numDwords)
{
	do
	{
		if (!data || !numDwords)
		{
			break;
		}

		PsslShaderResource shaderRes = { startSlot, data, numDwords };

		switch (stage)
		{
		case kShaderStageVs:
			insertUniqueUserDataSlot(m_shaders.vs.userDataSlotTable, startSlot, shaderRes);
			break;
		case kShaderStagePs:
			insertUniqueUserDataSlot(m_shaders.ps.userDataSlotTable, startSlot, shaderRes);
			break;
		case kShaderStageCs:
			insertUniqueUserDataSlot(m_shaders.cs.userDataSlotTable, startSlot, shaderRes);
			break;
		default:
			LOG_FIXME("unsupported user data for stage %d", stage);
			break;
		}

	} while (false);
}

void GnmCommandBufferDraw::insertUniqueUserDataSlot(
	std::vector<pssl::PsslShaderResource>& container,
	uint32_t                               startSlot,
	pssl::PsslShaderResource&              shaderRes)
{
	// Sometimes the game tries to set user data repeatedly at the same slot
	// we need to ensure a slot contains the latest data.

	auto pred = [startSlot](const pssl::PsslShaderResource& item) 
	{
		return item.startRegister == startSlot;
	};

	auto iter = std::find_if(container.begin(), container.end(), pred);
	if (iter == container.end())
	{
		container.push_back(shaderRes);
	}
	else
	{
		*iter = shaderRes;
	}
}

