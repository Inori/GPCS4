#include "GnmCommandBufferDraw.h"

#include "GnmBuffer.h"
#include "GnmConvertor.h"
#include "GnmSampler.h"
#include "GnmSharpBuffer.h"
#include "GnmTexture.h"
#include "GpuAddress/GnmGpuAddress.h"

#include "../Pssl/PsslShaderModule.h"
#include "../Violet/VltBuffer.h"
#include "../Violet/VltCmdList.h"
#include "../Violet/VltContext.h"
#include "../Violet/VltImage.h"
#include "../Violet/VltPresenter.h"
#include "../Violet/VltSampler.h"
#include "../Violet/VltShader.h"

#include "../Platform/UtilFile.h"

#include <algorithm>
#include <functional>

LOG_CHANNEL(Graphic.Gnm.GnmCommandBufferDraw);

using namespace sce;
using namespace vlt;
using namespace pssl;

// The compute shader using to clear color render target
constexpr uint64_t ShaderHashClearRT = 0x8C25642DB09D8E59;

// Use this to break on a shader you want to debug.
#define SHADER_DEBUG_BREAK(mod, hash)  \
	if (mod->key().toUint64() == hash) \
	{                                  \
		__debugbreak();                \
	}

GnmCommandBufferDraw::GnmCommandBufferDraw(
	const SceGpuQueueDevice& device,
	const RcPtr<VltContext>& context) :
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
		m_device->createCmdList(VltPipelineType::Graphics));

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

	auto rsInfo = VltRasterizationInfo(
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

	m_context->setViewports(1, &viewport, &scissor);
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
	shader::parseShaderRegPs(psRegs, m_shaders.ps.meta);
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

	// This is the original gnm embedded shader.
	// It outputs vertex:
	// 0  (-1.0, -1.0, 0.0, 1.0)
	// 1  (1.0, -1.0, 0.0, 1.0)
	// 2  (-1.0, 1.0, 0.0, 1.0)
	// And treated it as a rectangle list,
	// this will only cover the bottom-left triangle
	// of the screen, since vulkan doesn't
	// support rect list vertex data, and we
	// have to use triangle list.

	//const static uint8_t embeddedVsShaderFullScreen[] = {
	//	0xFF, 0x03, 0xEB, 0xBE, 0x07, 0x00, 0x00, 0x00, 0x81, 0x00, 0x02, 0x36, 0x81, 0x02, 0x02, 0x34,
	//	0xC2, 0x00, 0x00, 0x36, 0xC1, 0x02, 0x02, 0x4A, 0xC1, 0x00, 0x00, 0x4A, 0x01, 0x0B, 0x02, 0x7E,
	//	0x00, 0x0B, 0x00, 0x7E, 0x80, 0x02, 0x04, 0x7E, 0xF2, 0x02, 0x06, 0x7E, 0xCF, 0x08, 0x00, 0xF8,
	//	0x01, 0x00, 0x02, 0x03, 0x0F, 0x02, 0x00, 0xF8, 0x03, 0x03, 0x03, 0x03, 0x00, 0x00, 0x81, 0xBF,
	//	0x4F, 0x72, 0x62, 0x53, 0x68, 0x64, 0x72, 0x07, 0x47, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	//	0x9F, 0xC2, 0xF8, 0x47, 0xCF, 0xA5, 0x2D, 0x9B, 0x7D, 0x5B, 0x7C, 0xFF, 0x17, 0x00, 0x00, 0x00
	//};

	// This is our replaced version.
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

	VltAttachment colorTarget                 = {};
	colorTarget.view                          = image.view;
	colorTarget.layout                        = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	m_state.gp.om.renderTargets.color[rtSlot] = colorTarget;

	m_state.gp.om.colorTargets[rtSlot] = *target;
	m_flags.set(GnmContexFlag::GpDirtyRenderTarget);
}

void GnmCommandBufferDraw::setDepthRenderTarget(GnmDepthRenderTarget const* depthTarget)
{
	auto depthImage = m_factory.grabDepthRenderTarget(*depthTarget);

	VltAttachment depthAttachment     = {};
	depthAttachment.view              = depthImage.view;
	depthAttachment.layout            = depthImage.view->imageInfo().layout;
	m_state.gp.om.renderTargets.depth = depthAttachment;

	m_state.gp.om.depthTarget = *depthTarget;
	m_flags.set(GnmContexFlag::GpDirtyRenderTarget);
}

void GnmCommandBufferDraw::setDepthClearValue(float clearValue)
{
	m_state.gp.om.depthClearValue.depthStencil.depth = clearValue;
}

void GnmCommandBufferDraw::setStencilClearValue(uint8_t clearValue)
{
	m_state.gp.om.depthClearValue.depthStencil.stencil = clearValue;
}

void GnmCommandBufferDraw::setRenderTargetMask(uint32_t mask)
{
	auto writeMasks = cvt::convertRrenderTargetMask(mask);
	for (uint32_t attachment = 0; attachment != writeMasks.size(); ++attachment)
	{
		m_context->setBlendMask(attachment, writeMasks[attachment]);
	}
}

void GnmCommandBufferDraw::setBlendControl(uint32_t rtSlot, BlendControl blendControl)
{
	VkBlendFactor colorSrcFactor = cvt::convertBlendMultiplierToVkFactor(blendControl.getColorEquationSourceMultiplier());
	VkBlendFactor colorDstFactor = cvt::convertBlendMultiplierToVkFactor(blendControl.getColorEquationDestinationMultiplier());
	VkBlendOp     colorBlendOp   = cvt::convertBlendFuncToVkOp(blendControl.getColorEquationBlendFunction());

	VkBlendFactor alphaSrcFactor = cvt::convertBlendMultiplierToVkFactor(blendControl.getAlphaEquationSourceMultiplier());
	VkBlendFactor alphaDstFactor = cvt::convertBlendMultiplierToVkFactor(blendControl.getAlphaEquationDestinationMultiplier());
	VkBlendOp     alphaBlendOp   = cvt::convertBlendFuncToVkOp(blendControl.getAlphaEquationBlendFunction());

	// Here we set color write mask to fullMask.
	// A correct mask value should be set through setRenderTargetMask call.
	auto colorBlendMode = VltColorBlendAttachment(
		blendControl.getBlendEnable(),
		colorSrcFactor,
		colorDstFactor,
		colorBlendOp,
		alphaSrcFactor,
		alphaDstFactor,
		alphaBlendOp);
		
	m_context->setBlendMode(rtSlot, colorBlendMode);
}

void GnmCommandBufferDraw::setDepthStencilControl(DepthStencilControl depthControl)
{
	VkCompareOp depthCmpOp = cvt::convertCompareFunc(depthControl.getDepthControlZCompareFunction());

	VltDepthStencilOp frontOp;
	VltDepthStencilOp backOp;

	LOG_ASSERT(depthControl.stencilEnable == false, "stencil test not supported yet.");

	auto dsInfo = VltDepthStencilInfo(
		depthControl.depthEnable,
		depthControl.zWrite,
		depthControl.depthBoundsEnable,
		depthControl.stencilEnable,
		depthCmpOp,
		frontOp,
		backOp);

	m_context->setDepthStencilState(dsInfo);
}

void GnmCommandBufferDraw::setDepthStencilDisable()
{
	auto dsInfo = VltDepthStencilInfo();

	m_context->setDepthStencilState(dsInfo);
}

void GnmCommandBufferDraw::setDbRenderControl(DbRenderControl reg)
{
	if (reg.getDepthClearEnable())
	{
		// Gnm provide a way to overwrite the depth value
		// outputted from pixel shader no matter what the original
		// value is.
		// That is enable depth clear using setDbRenderControl
		// and set the clear value using setDepthClearValue.
		//
		// While vulkan provide us a different way to achieve this,
		// when depth bounds test is enabled and the test fails,
		// the sample¡¯s coverage bit is cleared in the fragment.
		// So we set minDepthBounds to 1.0 and maxDepthBounds to 0.0
		// to ensure the depth bounds test must fail.
		// and then set the clear value using VltContext::clearRenderTarget
		VltDepthBounds depthBounds;
		depthBounds.enableDepthBounds = VK_TRUE;
		depthBounds.minDepthBounds    = 1.0;
		depthBounds.maxDepthBounds    = 0.0;
		m_context->setDepthBounds(depthBounds);

		m_flags.set(GnmContexFlag::GpClearDepthTarget);
	}
	else
	{
		VltDepthBounds depthBounds;
		depthBounds.enableDepthBounds = VK_FALSE;
		depthBounds.minDepthBounds    = 0.0;
		depthBounds.maxDepthBounds    = 1.0;
		m_context->setDepthBounds(depthBounds);

		m_flags.clr(GnmContexFlag::GpClearDepthTarget);
	}
}

void GnmCommandBufferDraw::setVgtControl(uint8_t primGroupSizeMinusOne)
{
}

void GnmCommandBufferDraw::setPrimitiveType(PrimitiveType primType)
{
	VkPrimitiveTopology topology = cvt::convertPrimitiveTypeToVkTopology(primType);

	// TODO:
	// This is a temporary solution, mainly for embedded vertex shader.
	// For a primitive type which is not supported by vulkan natively,
	// we need to find a workaround.
	if (topology == VK_PRIMITIVE_TOPOLOGY_MAX_ENUM)
	{
		topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	}

	auto isInfo = VltInputAssemblyInfo(
		topology,
		VK_FALSE,
		0);

	m_context->setInputAssemblyState(isInfo);
}

void GnmCommandBufferDraw::setIndexSize(IndexSize indexSize, CachePolicy cachePolicy)
{
	m_state.gp.ia.indexBuffer.type = cvt::convertIndexSize(indexSize);
}

void GnmCommandBufferDraw::drawIndexAuto(uint32_t indexCount, DrawModifier modifier)
{
	// Clear index buffer
	m_state.gp.ia.indexBuffer = GnmIndexBuffer();

	commitGraphicsStages<false, false>();

	// TODO:
	// Is indexCount == vertexCount ?
	uint32_t vertexCount = indexCount;

	m_context->draw(vertexCount, 1, 0, 0);
}

void GnmCommandBufferDraw::drawIndexAuto(uint32_t indexCount)
{
	DrawModifier modifier = {};
	drawIndexAuto(indexCount, modifier);
}

void GnmCommandBufferDraw::drawIndex(uint32_t indexCount, const void* indexAddr, DrawModifier modifier)
{
	m_state.gp.ia.indexBuffer.buffer = indexAddr;
	m_state.gp.ia.indexBuffer.count  = indexCount;
	uint32_t elementSize             = m_state.gp.ia.indexBuffer.type == VK_INDEX_TYPE_UINT16 ? sizeof(uint16_t) : sizeof(uint32_t);
	m_state.gp.ia.indexBuffer.size   = elementSize * indexCount;

	commitGraphicsStages<true, false>();

	m_context->drawIndexed(indexCount, 1, 0, 0, 0);
}

void GnmCommandBufferDraw::drawIndex(uint32_t indexCount, const void* indexAddr)
{
	DrawModifier modifier = {};
	drawIndex(indexCount, indexAddr, modifier);
}

void GnmCommandBufferDraw::dispatch(uint32_t threadGroupX, uint32_t threadGroupY, uint32_t threadGroupZ)
{
	dispatchWithOrderedAppend(threadGroupX, threadGroupY, threadGroupZ, kDispatchOrderedAppendModeDisabled);
}

void GnmCommandBufferDraw::dispatchWithOrderedAppend(uint32_t threadGroupX, uint32_t threadGroupY, uint32_t threadGroupZ, DispatchOrderedAppendMode orderedAppendMode)
{
	commitComputeStages();

	if (m_flags.test(GnmContexFlag::CpPendingDispatch))
	{
		m_context->dispatch(threadGroupX, threadGroupY, threadGroupZ);
		m_flags.clr(GnmContexFlag::CpPendingDispatch);
	}
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
	shader::parseShaderRegCs(computeData, m_shaders.cs.meta);
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

void GnmCommandBufferDraw::bindRenderTargets()
{
	do
	{
		if (!m_flags.test(GnmContexFlag::GpDirtyRenderTarget))
		{
			break;
		}

		m_context->bindRenderTargets(m_state.gp.om.renderTargets);

		m_flags.clr(GnmContexFlag::GpDirtyRenderTarget);
	} while (false);
}

void GnmCommandBufferDraw::setVertexInputLayout(
	const pssl::GcnVertexInputAttributeTable& iat)
{
	// For some games, ie. Nier:Automata, vertex attributes are not stored
	// in a single vertex buffer area, so in such case we need to use multiple vertex
	// bindings. 
	// But for other games, all vertex attributes are within the same memory area,
	// in this case, we only need one vertex binding.

	uint32_t bindingCount   = 0;
	uint32_t attributeCount = 0;

	std::array<VltVertexBinding, MaxNumVertexBindings>     bindings;
	std::array<VltVertexAttribute, MaxNumVertexAttributes> attributes;

	attributeCount = iat.size();

	if (isSingleVertexBuffer(iat))
	{
		const GnmBuffer* firstVsharp = reinterpret_cast<const GnmBuffer*>(iat[0].vsharp);
		uint32_t         stride      = firstVsharp->getStride();

		bindingCount             = 1;
		VltVertexBinding binding = VltVertexBinding(0, stride, VK_VERTEX_INPUT_RATE_VERTEX, 0);
		bindings.at(0)           = binding;

		uint8_t* startAddress = reinterpret_cast<uint8_t*>(firstVsharp->getBaseAddress());

		for (uint32_t i = 0; i != attributeCount; ++i)
		{
			const GnmBuffer* vsharp = reinterpret_cast<const GnmBuffer*>(iat[i].vsharp);

			uint8_t* bufferAddress = reinterpret_cast<uint8_t*>(vsharp->getBaseAddress());
			uint32_t offset        = bufferAddress - startAddress;
			VkFormat format        = cvt::convertDataFormatToVkFormat(vsharp->getDataFormat());
			auto     attr          = VltVertexAttribute(i, 0, format, offset);
			attributes.at(i)       = attr;
		}
	}
	else
	{
		bindingCount = iat.size();
		for (uint32_t i = 0; i != attributeCount; ++i)
		{
			const GnmBuffer* vsharp = reinterpret_cast<const GnmBuffer*>(iat[i].vsharp);

			uint32_t stride  = vsharp->getStride();
			auto     binding = VltVertexBinding(i, stride, VK_VERTEX_INPUT_RATE_VERTEX, 0);
			bindings.at(i)   = binding;

			VkFormat vtxFmt  = cvt::convertDataFormatToVkFormat(vsharp->getDataFormat());
			auto     attr    = VltVertexAttribute(i, i, vtxFmt, 0);
			attributes.at(i) = attr;
		}
	}

	m_context->setInputLayout(
		bindingCount,
		bindings.data(),
		attributeCount,
		attributes.data());
}

void GnmCommandBufferDraw::bindVertexInput(
	const pssl::GcnVertexInputAttributeTable& iat)
{
	setVertexInputLayout(iat);

	uint32_t bindingCount = isSingleVertexBuffer(iat) ?
		1 : iat.size();

	for (uint32_t i = 0; i != bindingCount; ++i)
	{
		bindVertexBuffer(iat[i]);
	}
}

void GnmCommandBufferDraw::bindIndexBuffer()
{
	const auto& indexDesc   = m_state.gp.ia.indexBuffer;
	auto        indexBuffer = m_factory.grabIndex(indexDesc);

	m_context->updateBuffer(indexBuffer, 0, indexDesc.size, indexDesc.buffer);

	m_context->bindIndexBuffer(indexBuffer, indexDesc.type);
}

void GnmCommandBufferDraw::bindVertexBuffer(
	const GcnVertexInputAttribute& attr)
{
	// TODO:
	// There's a critical problem here, probably the most critical one for the whole GPCS4 project:
	// Because of the uniform memory architecture of PSS4 hardware,
	// We don't know when to update or release a GPU resource.
	// Because PS4 use the same memory chip for both CPU and GPU,
	// A PS4 game treat GPU buffers just as normal CPU memories, and manage them
	// using in-game memory pool, which doesn't export explicit interface for us.
	// That makes us impossible to detect buffer update and release.
	//
	// We may need to develop some heuristic strategies to deal with this problem.
	// Currently I just update GPU buffer every time it gets bound and don't release any of them.

	const GnmBuffer* vsharp  = reinterpret_cast<const GnmBuffer*>(attr.vsharp);
	void*            vtxData = vsharp->getBaseAddress();

	bool isSwizzled = vsharp->isSwizzled();
	LOG_ASSERT(isSwizzled == false, "do not support swizzled buffer currently.");

	VkDeviceSize bufferSize = vsharp->getSize();

	GnmBufferCreateInfo info = {};
	info.buffer              = vsharp;
	info.stages              = VK_PIPELINE_STAGE_VERTEX_INPUT_BIT;
	info.usageType           = kShaderInputUsageImmVertexBuffer;
	auto vertexBuffer        = m_factory.grabBuffer(info);

	m_context->updateBuffer(vertexBuffer, 0, bufferSize, vtxData);

	uint32_t stride = vsharp->getStride();
	m_context->bindVertexBuffer(attr.bindingId, 
		VltBufferSlice(vertexBuffer, 0, bufferSize), stride);
}

void GnmCommandBufferDraw::bindImmConstBuffer(pssl::PsslProgramType shaderType, const PsslShaderResource& res)
{
	const GnmBuffer* vsharp = reinterpret_cast<const GnmBuffer*>(res.resource);

	GnmBufferCreateInfo info = {};
	info.buffer              = vsharp;
	info.stages              = cvt::convertShaderStage(shaderType);
	info.usageType           = kShaderInputUsageImmConstBuffer;
	auto constBuffer         = m_factory.grabBuffer(info);

	VkDeviceSize bufferSize = vsharp->getSize();
	m_context->updateBuffer(constBuffer, 0, bufferSize, vsharp->getBaseAddress());

	uint32_t regSlot = computeConstantBufferBinding(shaderType, res.startRegister);
	m_context->bindResourceBuffer(regSlot, constBuffer);
}

void GnmCommandBufferDraw::bindImmBuffer(
	pssl::PsslProgramType            shaderType,
	const GcnShaderResourceInstance& res)
{
	const GnmBuffer* vsharp = reinterpret_cast<const GnmBuffer*>(res.res.resource);

	GnmBufferCreateInfo info = {};
	info.buffer              = vsharp;
	info.stages              = cvt::convertShaderStage(shaderType);
	info.usageType           = res.usageType;
	auto dataBuffer          = m_factory.grabBuffer(info);

	VkDeviceSize bufferSize = vsharp->getSize();
	
	bool updated = false;
	if (!updated)
	{
		m_context->updateBuffer(dataBuffer, 0, bufferSize, vsharp->getBaseAddress());
		updated = true;
	}

	uint32_t regSlot = computeResBinding(shaderType, res.res.startRegister);
	m_context->bindResourceBuffer(regSlot, dataBuffer);
}

void GnmCommandBufferDraw::bindImmTexture(
	pssl::PsslProgramType            shaderType,
	const GcnShaderResourceInstance& res)
{
	const GnmTexture* tsharp = reinterpret_cast<const GnmTexture*>(res.res.resource);

	GnmTextureCreateInfo info = {};
	info.texture              = tsharp;
	info.stages               = cvt::convertShaderStage(shaderType);
	info.usageType            = res.usageType;
	auto image                = m_factory.grabImage(info);

	auto     imgInfo       = image.image->info();
	uint32_t pitchPerRow   = tsharp->getPitch();
	uint32_t pitchPerLayer = pitchPerRow * tsharp->getHeight();

	VkDeviceSize imageBufferSize = tsharp->getSizeAlign().m_size;
	void*        data            = tsharp->getBaseAddress();

	auto tileMode = tsharp->getTileMode();
	if (tileMode == kTileModeDisplay_LinearAligned)
	{
		VkImageSubresourceLayers subRes = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1 };
		VkOffset3D               offset = { 0, 0, 0 };
		m_context->updateImage(
			image.image, subRes,
			offset, imgInfo.extent,
			data,
			pitchPerRow, pitchPerLayer);
	}
	else
	{
		// TODO:
		// Untiling textures on CPU is not effective, we should do this using compute shader.
		// But that would be a challenging job.
		void* untiledData = malloc(imageBufferSize);

		GpuAddress::TilingParameters tp;
		tp.initFromTexture(tsharp, 0, 0);
		GpuAddress::detileSurface(untiledData, data, &tp);

		VkImageSubresourceLayers subRes = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1 };
		VkOffset3D               offset = { 0, 0, 0 };
		m_context->updateImage(
			image.image, subRes,
			offset, imgInfo.extent,
			data,
			pitchPerRow, pitchPerLayer);

		free(untiledData);
	}

	uint32_t regSlot = computeResBinding(PsslProgramType::PixelShader, res.res.startRegister);
	m_context->bindResourceView(regSlot, image.view, nullptr);
}

void GnmCommandBufferDraw::bindImmResource(
	PsslProgramType                  shaderType,
	const GcnShaderResourceInstance& res)
{
	if (res.sharpType == PsslSharpType::VSharp)
	{
		bindImmBuffer(shaderType, res);
	}
	else
	{
		bindImmTexture(shaderType, res);
	}
}

void GnmCommandBufferDraw::bindSampler(const PsslShaderResource& res)
{
	const GnmSampler* ssharp  = reinterpret_cast<const GnmSampler*>(res.resource);
	auto              sampler = m_factory.grabSampler(*ssharp);

	uint32_t regSlot = computeSamplerBinding(PsslProgramType::PixelShader, res.startRegister);
	m_context->bindSampler(regSlot, sampler);
}

void GnmCommandBufferDraw::bindShaderResources(
	PsslProgramType              shaderType,
	const GnmShaderResourceList& resources)
{
	for (const auto& res : resources)
	{
		ShaderInputUsageType type = res.usageType;
		switch (type)
		{
		case pssl::kShaderInputUsageImmSampler:
			bindSampler(res.res);
			break;
		case pssl::kShaderInputUsageImmRwResource:
		case pssl::kShaderInputUsageImmResource:
			bindImmResource(shaderType, res);
			break;
		case pssl::kShaderInputUsageImmConstBuffer:
			bindImmConstBuffer(shaderType, res.res);
			break;
		default:
			LOG_WARN("unsupported resource type %d", type);
			break;
		}
	}
}

void GnmCommandBufferDraw::commitVsStage()
{
	do 
	{
		if (!m_shaders.vs.code)
		{
			break;
		}

		m_shaders.vs.shader = new PsslShaderModule(
			{.vs = m_shaders.vs.meta},
			(const uint32_t*)m_shaders.vs.code);

		const uint32_t* fsCode = findFetchShaderCode(m_shaders.vs);
		// Some vs shaders doesn't have fetch shader, we need to check.
		if (fsCode)
		{
			m_shaders.vs.shader->defineFetchShader(fsCode);
		}

		LOG_DEBUG("vertex shader hash %llX", m_shaders.vs.shader->key().toUint64());
		m_shaders.vs.shader->defineShaderInput(m_shaders.vs.userDataSlotTable);

		auto shaderResourcesDcl = m_shaders.vs.shader->getShaderResourceDeclaration();
		auto bindingResources = PsslShaderModule::flattenShaderResources(shaderResourcesDcl);

		// Bind Vertex input
		if (shaderResourcesDcl.iat.has_value())
		{
			bindVertexInput(shaderResourcesDcl.iat.value());
		}

		// Bind all resources which the shader uses.
		bindShaderResources(PsslProgramType::VertexShader, bindingResources);

		m_context->bindShader(
			VK_SHADER_STAGE_VERTEX_BIT,
			m_shaders.vs.shader->compile());
	} while (false);
}

void GnmCommandBufferDraw::commitPsStage()
{
	do 
	{
		if (!m_shaders.ps.code)
		{
			break;
		}

		m_shaders.ps.shader = new PsslShaderModule(
			{ .ps = m_shaders.ps.meta },
			(const uint32_t*)m_shaders.ps.code);

		LOG_DEBUG("pixel shader hash %llX", m_shaders.ps.shader->key().toUint64());
		m_shaders.ps.shader->defineShaderInput(m_shaders.ps.userDataSlotTable);

		// SHADER_DEBUG_BREAK(m_shaders.ps.shader, 0xAB97D172C647AFE9);

		auto shaderResourcesDcl = m_shaders.ps.shader->getShaderResourceDeclaration();
		auto bindingResources   = PsslShaderModule::flattenShaderResources(shaderResourcesDcl);

		// Bind all resources which the shader uses.
		bindShaderResources(PsslProgramType::PixelShader, bindingResources);

		m_context->bindShader(
			VK_SHADER_STAGE_FRAGMENT_BIT,
			m_shaders.ps.shader->compile());
	} while (false);
}

template <bool Indexed, bool Indirect>
void GnmCommandBufferDraw::commitGraphicsStages()
{
	if (m_flags.test(GnmContexFlag::GpDirtyRenderTarget))
	{
		bindRenderTargets();
	}

	if (m_flags.test(GnmContexFlag::GpClearDepthTarget))
	{
		clearDepthTarget();
	}

	if constexpr (Indexed)
	{
		bindIndexBuffer();
	}

	commitVsStage();
	commitPsStage();

	clearShaderContext();
}

void GnmCommandBufferDraw::clearColorTargetHack(GnmShaderResourceList& shaderResources)
{
	// HACK:
	// This special compute shader is used by Gnmx Toolkit to clear render target.
	// It simply treats the render target image as an uint32 array, and fill values in the array.
	//
	// But why we need a hack?
	// Because like I said above, the memory content of the render target image will be
	// treated as a normal uint32 array, and descripted by V# (GnmBuffer),
	// not by T# (GnmTexture) anymore, thus we lost all the image format information.
	// And even worse, the shader will use a constant buffer generated using the
	// original render target image, and we don't know the structure definition of
	// the constant buffer obviously. (well, we do if we hard-code it.)
	// So as a conclusion, this compute shader is useless for us.

	// Extract the resource which hold the encoded color value using to clear the target.
	auto             sourceRes    = findShaderResource(shaderResources, kShaderInputUsageImmResource);
	const GnmBuffer* sourceBuffer = reinterpret_cast<const GnmBuffer*>(sourceRes.resource);
	void*            sourceMemory = sourceBuffer->getBaseAddress();
	uint32_t         sourceSize   = sourceBuffer->getSize();
	// Extract the resource which re-represents the color target.
	auto             destRes    = findShaderResource(shaderResources, kShaderInputUsageImmRwResource);
	const GnmBuffer* destBuffer = reinterpret_cast<const GnmBuffer*>(destRes.resource);
	// The buffer to clear should hold the same memory block as the render target image,
	// so we find the actual GnmRenderTarget by memory address
	void*                  destMemory = destBuffer->getBaseAddress();
	const GnmRenderTarget* target     = findRenderTarget(destMemory);

	LOG_ASSERT(target != nullptr, "the render target to clear is not bound previously.");

	// Gnmx Toolkit encodes the RGBA color value which the game provides
	// to a encoded value according to the render target's data format.
	// It's likely a tiled value, so we have to decode it to recover the
	// original RGBA value.
	uint32_t encodeValues[4] = { 0 };
	std::memcpy(encodeValues, sourceMemory, sourceSize);

	// Note:
	// The decoded float value is not exactly equal to the original value
	// the game set, e.g. 0.5 will become 0.496093750
	GpuAddress::Reg32 reg[4] = {};
	GpuAddress::dataFormatDecoder(reg, encodeValues, target->getDataFormat());

	// Do the clear
	auto targetImage = m_factory.grabRenderTarget(*target);
	m_context->clearRenderTarget(
		targetImage.view,
		VK_IMAGE_ASPECT_COLOR_BIT,
		*reinterpret_cast<VkClearValue*>(reg));
}

void GnmCommandBufferDraw::commitCsStage()
{
	do 
	{
		if (!m_shaders.cs.code)
		{
			break;
		}

		m_shaders.cs.shader = new PsslShaderModule(
			{ .cs = m_shaders.cs.meta }, 
			(const uint32_t*)m_shaders.cs.code);
		LOG_DEBUG("compute shader hash %llX", m_shaders.cs.shader->key().toUint64());

		// SHADER_DEBUG_BREAK(m_shaders.cs.shader, 0xAF20AC1F702451D8);

		m_shaders.cs.shader->defineShaderInput(m_shaders.cs.userDataSlotTable);
		auto nestedResources = m_shaders.cs.shader->getShaderResourceDeclaration();
		auto shaderResources = PsslShaderModule::flattenShaderResources(nestedResources);

		// Hack
		if (m_shaders.cs.shader->key().toUint64() == ShaderHashClearRT)
		{
			clearColorTargetHack(shaderResources);
		}
		else
		{
			// Bind all resources which the shader uses.
			bindShaderResources(PsslProgramType::ComputeShader, shaderResources);

			RcPtr<VltShader> computeShader = m_shaders.cs.shader->compile();
			auto newShader = UtilFile::LoadFile("comp.spv");
			computeShader->replaceCode(newShader);

			m_context->bindShader(
				VK_SHADER_STAGE_COMPUTE_BIT,
				computeShader);

			m_flags.set(GnmContexFlag::CpPendingDispatch);
		}
	} while (false);
}

void GnmCommandBufferDraw::commitComputeStages()
{
	commitCsStage();
	clearShaderContext();
}

void GnmCommandBufferDraw::clearShaderContext()
{
	m_shaders = GnmShaderContextGroup();
}

void GnmCommandBufferDraw::clearDepthTarget()
{
	auto depthImage = m_factory.grabDepthRenderTarget(m_state.gp.om.depthTarget);
	m_context->clearRenderTarget(depthImage.view, VK_IMAGE_ASPECT_DEPTH_BIT, m_state.gp.om.depthClearValue);
	m_flags.clr(GnmContexFlag::GpClearDepthTarget);
}

void GnmCommandBufferDraw::clearRenderState()
{
	m_flags.clr(
		GnmContexFlag::GpClearDepthTarget,
		GnmContexFlag::CpPendingDispatch);

	m_flags.set(
		GnmContexFlag::GpDirtyRenderTarget);

	m_state = GnmContextState();

	clearShaderContext();
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

	auto pred = [startSlot](const pssl::PsslShaderResource& item) {
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

const uint32_t* GnmCommandBufferDraw::findFetchShaderCode(const GnmShaderContext& shdrCtx)
{
	const uint32_t* fsCode = nullptr;
	do
	{
		uint32_t fsStartReg = pssl::getFetchShaderStartRegister((const uint8_t*)shdrCtx.code);
		if (fsStartReg == UINT_MAX)
		{
			break;
		}

		auto matchStartRegister = [fsStartReg](const auto& res) {
			return res.startRegister == fsStartReg;
		};

		auto iter = std::find_if(
			shdrCtx.userDataSlotTable.begin(),
			shdrCtx.userDataSlotTable.end(),
			matchStartRegister);

		if (iter == shdrCtx.userDataSlotTable.end())
		{
			break;
		}

		fsCode = reinterpret_cast<const uint32_t*>(iter->resource);

	} while (false);
	return fsCode;
}

const PsslShaderResource GnmCommandBufferDraw::findShaderResource(
	const GnmShaderResourceList& resources,
	ShaderInputUsageType         type)
{
	PsslShaderResource result = {};

	for (const auto& res : resources)
	{
		if (res.usageType == type)
		{
			result = res.res;
			break;
		}
	}

	return result;
}

const GnmRenderTarget* GnmCommandBufferDraw::findRenderTarget(void* address)
{
	auto matchAddress = [address](const GnmRenderTarget& target) {
		return address == target.getBaseAddress();
	};

	auto iter = std::find_if(
		m_state.gp.om.colorTargets.begin(),
		m_state.gp.om.colorTargets.end(),
		matchAddress);

	auto target = iter != m_state.gp.om.colorTargets.end() ? 
		&*iter : nullptr;
	return target;
}

bool GnmCommandBufferDraw::isSingleVertexBuffer(const pssl::GcnVertexInputAttributeTable& iat)
{
	const GnmBuffer* firstVsharp = reinterpret_cast<const GnmBuffer*>(iat[0].vsharp);
	uint32_t         stride      = firstVsharp->getStride();
	
	uintptr_t firstStart = reinterpret_cast<uintptr_t>(firstVsharp->getBaseAddress());
	uintptr_t firstEnd   = firstStart + stride;

	bool single = true;
	// If the first record start address of all the left attributes are within
	// the first attribute record, we think the vertex input uses a single
	// vertex buffer.
	uint32_t attrCount = iat.size();
	for (uint32_t i = 1; i != attrCount; ++i)
	{
		const GnmBuffer* vsharp = reinterpret_cast<const GnmBuffer*>(iat[i].vsharp);
		uintptr_t        start  = reinterpret_cast<uintptr_t>(vsharp->getBaseAddress());
		if (start < firstStart || start > firstEnd)
		{
			single = false;
			break;
		}
	}

	return single;
}

