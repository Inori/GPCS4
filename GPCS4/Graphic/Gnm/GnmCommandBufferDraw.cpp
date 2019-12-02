#include "GnmCommandBufferDraw.h"
#include "GnmSharpBuffer.h"
#include "GnmBuffer.h"
#include "GnmTexture.h"
#include "GnmSampler.h"

#include "../Gve/GveCmdList.h"
#include "../Gve/GveShader.h"
#include "../Gve/GveBuffer.h"
#include "../Gve/GveImage.h"
#include "../Gve/GveSampler.h"
#include "../Gve/GveContext.h"
#include "../Pssl/PsslShaderModule.h"
#include "Platform/PlatformUtils.h"

#include <algorithm>

using namespace gve;
using namespace pssl;

GnmCommandBufferDraw::GnmCommandBufferDraw(
	const RcPtr<GveDevice>& device,
	const RcPtr<gve::GveImageView>& defaultColorTarget):
	GnmCommandBuffer(device),
	m_defaultColorTarget(defaultColorTarget),
	m_depthTarget(nullptr)
{

}


GnmCommandBufferDraw::~GnmCommandBufferDraw()
{
}

// First call of a frame.
void GnmCommandBufferDraw::initializeDefaultHardwareState()
{
	clearUserDataSlots();
	m_context->beginRecording(m_cmd);
}

// Last call of a frame.
void GnmCommandBufferDraw::prepareFlip(void *labelAddr, uint32_t value)
{
	*(uint32_t*)labelAddr = value;
	m_context->endRecording();
}

// Last call of a frame, with interrupt.
void GnmCommandBufferDraw::prepareFlipWithEopInterrupt(EndOfPipeEventType eventType, void *labelAddr, uint32_t value, CacheAction cacheAction)
{
	*(uint32_t*)labelAddr = value;
	m_context->endRecording();
}

void GnmCommandBufferDraw::setPsShaderUsage(const uint32_t *inputTable, uint32_t numItems)
{

}

void GnmCommandBufferDraw::setViewport(uint32_t viewportId, float dmin, float dmax, const float scale[3], const float offset[3])
{
	float width = scale[0] / 0.5f;
	float height = -scale[1] / 0.5f;
	float left = offset[0] - scale[0];
	float top = offset[1] + scale[1];

	VkViewport viewport;
	viewport.x = left;
	viewport.y = top;
	viewport.width = width;
	viewport.height = height;
	viewport.minDepth = dmin;
	viewport.maxDepth = dmax;

	VkRect2D scissor;
	scissor.offset.x = viewport.x;
	scissor.offset.y = viewport.y;
	scissor.extent.width = viewport.width;
	scissor.extent.height = viewport.height;

	m_context->setViewport(viewport, scissor);
}

void GnmCommandBufferDraw::setPsShader(const pssl::PsStageRegisters *psRegs)
{
	m_psContext.code = psRegs->getCodeAddress();
}

void GnmCommandBufferDraw::setVsShader(const pssl::VsStageRegisters *vsRegs, uint32_t shaderModifier)
{
	m_vsContext.code = vsRegs->getCodeAddress();
}

void GnmCommandBufferDraw::setVgtControl(uint8_t primGroupSizeMinusOne, WdSwitchOnlyOnEopMode wdSwitchOnlyOnEopMode, VgtPartialVsWaveMode partialVsWaveMode)
{
	
}

void GnmCommandBufferDraw::setVsharpInUserData(ShaderStage stage, uint32_t startUserDataSlot, const VSharpBuffer *buffer)
{
	setUserDataSlots(stage, startUserDataSlot, (uint32_t*)buffer, sizeof(VSharpBuffer)/sizeof(uint32_t));
}

void GnmCommandBufferDraw::setTsharpInUserData(ShaderStage stage, uint32_t startUserDataSlot, const TSharpBuffer *tex)
{
	setUserDataSlots(stage, startUserDataSlot, (uint32_t*)tex, sizeof(TSharpBuffer) / sizeof(uint32_t));
}

void GnmCommandBufferDraw::setSsharpInUserData(ShaderStage stage, uint32_t startUserDataSlot, const SSharpBuffer *sampler)
{
	setUserDataSlots(stage, startUserDataSlot, (uint32_t*)sampler, sizeof(SSharpBuffer) / sizeof(uint32_t));
}

void GnmCommandBufferDraw::setPointerInUserData(ShaderStage stage, uint32_t startUserDataSlot, void *gpuAddr)
{
	setUserDataSlots(stage, startUserDataSlot, (uint32_t*)gpuAddr, sizeof(void*) / sizeof(uint32_t));
}

void GnmCommandBufferDraw::setUserDataRegion(ShaderStage stage, uint32_t startUserDataSlot, const uint32_t *userData, uint32_t numDwords)
{
	setUserDataSlots(stage, startUserDataSlot, userData, numDwords);
}


void GnmCommandBufferDraw::writeAtEndOfPipe(EndOfPipeEventType eventType, 
	EventWriteDest dstSelector, void *dstGpuAddr, 
	EventWriteSource srcSelector, uint64_t immValue, 
	CacheAction cacheAction, CachePolicy cachePolicy)
{
	emuWriteGpuLabel(srcSelector, dstGpuAddr, immValue);
}

void GnmCommandBufferDraw::writeAtEndOfPipeWithInterrupt(EndOfPipeEventType eventType, 
	EventWriteDest dstSelector, void *dstGpuAddr, 
	EventWriteSource srcSelector, uint64_t immValue, 
	CacheAction cacheAction, CachePolicy cachePolicy)
{
	emuWriteGpuLabel(srcSelector, dstGpuAddr, immValue);
}


void GnmCommandBufferDraw::waitUntilSafeForRendering(uint32_t videoOutHandle, uint32_t displayBufferIndex)
{
	
}


void GnmCommandBufferDraw::setViewportTransformControl(ViewportTransformControl vportControl)
{

}

void GnmCommandBufferDraw::setScreenScissor(int32_t left, int32_t top, int32_t right, int32_t bottom)
{

}

void GnmCommandBufferDraw::setGuardBands(float horzClip, float vertClip, float horzDiscard, float vertDiscard)
{

}

void GnmCommandBufferDraw::setHardwareScreenOffset(uint32_t offsetX, uint32_t offsetY)
{

}

void GnmCommandBufferDraw::setRenderTarget(uint32_t rtSlot, RenderTarget const *target)
{
	do 
	{
		if (!target)
		{
			break;
		}

		LOG_ASSERT(rtSlot == 0, "only support one render target at 0");

		// TODO:
		// For future development, a game may use "render to texture" technique,
		// and set a render target which is not a swapchain image.
		// we should check whether "target" is the display buffer or not
		// and support extra render target,
		// but currently I just use the default one.
		GveAttachment colorTarget;
		colorTarget.view = m_defaultColorTarget;
		colorTarget.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		m_context->bindRenderTargets(&colorTarget, 1);
	} while (false);

}

void GnmCommandBufferDraw::setDepthRenderTarget(DepthRenderTarget const *depthTarget)
{
	do 
	{
		if (!depthTarget)
		{
			break;
		}

		if (!m_depthTarget)
		{
			m_depthTarget = getDepthTarget(depthTarget);
		}

		// TODO:
		// More checks
		if (m_depthTarget->getWidth() != depthTarget->getWidth() || 
			m_depthTarget->getHeight() != depthTarget->getHeight())
		{
			// Depth buffer changed, we need to create a new one.
			m_depthTarget = getDepthTarget(depthTarget);
		}


		GveAttachment depthAttach;
		depthAttach.view = m_depthTarget;
		depthAttach.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
		m_context->bindDepthRenderTarget(depthAttach);

	} while (false);
}

void GnmCommandBufferDraw::setRenderTargetMask(uint32_t mask)
{

}

void GnmCommandBufferDraw::setDepthStencilControl(DepthStencilControl depthStencilControl)
{
	do 
	{
		VkCompareOp depthCmpOp = convertCompareFunc(depthStencilControl.getDepthControlZCompareFunction());

		GveDepthStencilOp frontOp;
		GveDepthStencilOp backOp;

		LOG_ASSERT(depthStencilControl.stencilEnable == false, "stencil test not supported yet.");

		auto dsInfo = GveDepthStencilInfo(
			depthStencilControl.depthEnable,
			depthStencilControl.zWrite,
			depthStencilControl.depthBoundsEnable,
			depthStencilControl.stencilEnable,
			depthCmpOp,
			frontOp,
			backOp);

		m_context->setDepthStencilState(dsInfo);

	} while (false);
}

void GnmCommandBufferDraw::setBlendControl(uint32_t rtSlot, BlendControl blendControl)
{
	do 
	{
		LOG_ASSERT(rtSlot == 0, "only support 0 rtSlot");

		auto cbInfo = GveColorBlendInfo(
			VK_FALSE,
			VK_LOGIC_OP_COPY
		);

		VkBlendFactor colorSrcFactor = convertBlendMultiplierToFactor(blendControl.getColorEquationSourceMultiplier());
		VkBlendFactor colorDstFactor = convertBlendMultiplierToFactor(blendControl.getColorEquationDestinationMultiplier());
		VkBlendOp colorBlendOp = convertBlendFuncToOp(blendControl.getColorEquationBlendFunction());

		VkBlendFactor alphaSrcFactor = convertBlendMultiplierToFactor(blendControl.getAlphaEquationSourceMultiplier());
		VkBlendFactor alphaDstFactor = convertBlendMultiplierToFactor(blendControl.getAlphaEquationDestinationMultiplier());
		VkBlendOp alphaBlendOp = convertBlendFuncToOp(blendControl.getAlphaEquationBlendFunction());

		VkColorComponentFlags colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

		auto colorAttach = GveColorBlendAttachment(
			blendControl.getBlendEnable(),
			colorSrcFactor,
			colorDstFactor,
			colorBlendOp,
			alphaSrcFactor,
			alphaDstFactor,
			alphaBlendOp,
			colorWriteMask
		);

		cbInfo.addAttachment(colorAttach);

		m_context->setColorBlendState(cbInfo);
	} while (false);
}

void GnmCommandBufferDraw::setPrimitiveSetup(PrimitiveSetup primSetup)
{
	do 
	{
		VkFrontFace frontFace = primSetup.getFrontFace() == kPrimitiveSetupFrontFaceCcw ? 
			VK_FRONT_FACE_COUNTER_CLOCKWISE : VK_FRONT_FACE_CLOCKWISE;
		VkPolygonMode polyMode = convertPolygonMode(primSetup.getPolygonModeFront());
		VkCullModeFlags cullMode = convertCullMode(primSetup.getCullFace());

		auto rsInfo = GveRasterizationInfo(
			VK_FALSE,
			VK_FALSE,
			polyMode,
			cullMode,
			frontFace
		);

		m_context->setRasterizerState(rsInfo);

	} while (false);
}

void GnmCommandBufferDraw::setActiveShaderStages(ActiveShaderStages activeStages)
{

}

void GnmCommandBufferDraw::setIndexSize(IndexSize indexSize, CachePolicy cachePolicy)
{
	m_indexSize = convertIndexSize(indexSize);
}

void GnmCommandBufferDraw::setPrimitiveType(PrimitiveType primType)
{
	do 
	{
		VkPrimitiveTopology topology = convertPrimitiveTypeToTopology(primType);

		auto isInfo = GveInputAssemblyInfo(
			topology,
			VK_FALSE,
			0
		);

		m_context->setInputAssemblyState(isInfo);
	} while (false);
}

void GnmCommandBufferDraw::drawIndex(uint32_t indexCount, const void *indexAddr, DrawModifier modifier)
{

}

void GnmCommandBufferDraw::drawIndex(uint32_t indexCount, const void *indexAddr)
{
	DrawModifier mod = { 0 };
	drawIndex(indexCount, indexAddr, mod);
}

void GnmCommandBufferDraw::drawIndexAuto(uint32_t indexCount, DrawModifier modifier)
{
}

void GnmCommandBufferDraw::drawIndexAuto(uint32_t indexCount)
{

}

void GnmCommandBufferDraw::setEmbeddedVsShader(EmbeddedVsShader shaderId, uint32_t shaderModifier)
{
	
}

void GnmCommandBufferDraw::updatePsShader(const pssl::PsStageRegisters *psRegs)
{
	
}

void GnmCommandBufferDraw::updateVsShader(const pssl::VsStageRegisters *vsRegs, uint32_t shaderModifier)
{
	
}

void GnmCommandBufferDraw::commitVsStage()
{
	do
	{
		uint32_t* fsCode = getFetchShaderCode(m_vsContext);
		LOG_ASSERT(fsCode != nullptr, "can not find fetch shader code.");

		PsslShaderModule vsModule((const uint32_t*)m_vsContext.code, fsCode, m_vsContext.userDataSlotTable);
		m_vsContext.shader = vsModule.compile();

		auto vsInputUsageSlots = vsModule.inputUsageSlots();
		for (const auto& inputSlot : vsInputUsageSlots)
		{
			// Find shader res in current slot
			auto pred = [&inputSlot](const auto& item)
			{
				return inputSlot.startRegister == item.startSlot;
			};
			auto iter = std::find_if(m_vsContext.userDataSlotTable.begin(), m_vsContext.userDataSlotTable.end(), pred);

			// Bind shader resources
			switch (inputSlot.usageType)
			{
			case kShaderInputUsageImmConstBuffer:
				bindImmConstBuffer(*iter);
				break;
			case kShaderInputUsagePtrVertexBufferTable:
				bindVertexBuffers(*iter, vsModule.vsInputSemantic());
				break;
			default:
				LOG_WARN("unsupported input usage %d", inputSlot.usageType);
				break;
			}
		}

		m_context->bindShader(VK_SHADER_STAGE_VERTEX_BIT, m_vsContext.shader);
	} while (false);
}

void GnmCommandBufferDraw::commitPsStage()
{
	do 
	{
		PsslShaderModule psModule((const uint32_t*)m_psContext.code, m_psContext.userDataSlotTable);
		m_psContext.shader = psModule.compile();

		auto psInputUsageSlots = psModule.inputUsageSlots();
		for (const auto& inputSlot : psInputUsageSlots)
		{
			// Find shader res in current slot
			auto pred = [&inputSlot](const auto& item)
			{
				return inputSlot.startRegister == item.startSlot;
			};
			auto iter = std::find_if(m_psContext.userDataSlotTable.begin(), m_psContext.userDataSlotTable.end(), pred);

			// Bind shader resources
			switch (inputSlot.usageType)
			{
			case kShaderInputUsageImmResource:
				bindImmResource(*iter);
				break;
			case kShaderInputUsageImmSampler:
				bindSampler(*iter);
				break;
			default:
				LOG_WARN("unsupported input usage %d", inputSlot.usageType);
				break;
			}
		}

		m_context->bindShader(VK_SHADER_STAGE_FRAGMENT_BIT, m_psContext.shader);
	} while (false);
}

void GnmCommandBufferDraw::bindIndexBuffer(void* indexAddr, uint32_t indexCount)
{

}

void GnmCommandBufferDraw::bindImmConstBuffer(const PsslShaderResource& res)
{
	// TODO:
	// For buffers allocated directly from gnm command buffer,
	// we should implement "allocateFromCommandBuffer",
	// and use push constants instead of UBOs.

}

void GnmCommandBufferDraw::bindVertexBuffers(const PsslShaderResource& res, const std::vector<VertexInputSemantic>& inputSemantics)
{

}

void GnmCommandBufferDraw::bindImmResource(const PsslShaderResource& res)
{

}

void GnmCommandBufferDraw::bindSampler(const PsslShaderResource& res)
{

}

void GnmCommandBufferDraw::emuWriteGpuLabel(EventWriteSource selector, void* label, uint64_t value)
{
	if (selector == kEventWriteSource32BitsImmediate)
	{
		*(uint32_t*)label = value;
	}
	else if (selector == kEventWriteSource64BitsImmediate)
	{
		*(uint64_t*)label = value;
	}
	else
	{
		*(uint64_t*)label = UtilProcess::GetProcessTimeCounter();
	}
}

uint32_t* GnmCommandBufferDraw::getFetchShaderCode(const GnmShaderContext& vsCtx)
{
	uint32_t* fsCode = nullptr;
	do 
	{
		uint32_t fsStartReg = pssl::getFetchShaderStartRegister((const uint8_t*)vsCtx.code);
		if (fsStartReg == UINT_MAX)
		{
			break;
		}

		for (const auto& res : vsCtx.userDataSlotTable)
		{
			if (res.startSlot != fsStartReg)
			{
				continue;
			}

			fsCode = (uint32_t*)res.resource;
			break;
		}

	} while (false);
	return fsCode;
}

void GnmCommandBufferDraw::setUserDataSlots(ShaderStage stage, uint32_t startSlot, const uint32_t* data, uint32_t numDwords)
{
	do 
	{
		if (!data || !numDwords)
		{
			break;
		}

		pssl::PsslShaderResource shaderRes = { startSlot, data, numDwords };

		switch (stage)
		{
		case kShaderStageVs:
			insertUniqueUserDataSlot(m_vsContext.userDataSlotTable, startSlot, shaderRes);
			break;
		case kShaderStagePs:
			insertUniqueUserDataSlot(m_psContext.userDataSlotTable, startSlot, shaderRes);
			break;
		default:
			break;
		}

	} while (false);
}

void GnmCommandBufferDraw::clearUserDataSlots()
{
	m_vsContext.userDataSlotTable.clear();
	m_psContext.userDataSlotTable.clear();
}

void GnmCommandBufferDraw::insertUniqueUserDataSlot(GnmShaderContext::UDSTVector& container, uint32_t startSlot, pssl::PsslShaderResource& shaderRes)
{
	auto pred = [startSlot](const pssl::PsslShaderResource& item)
	{
		return item.startSlot == startSlot;
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

RcPtr<gve::GveImageView> GnmCommandBufferDraw::getDepthTarget(const DepthRenderTarget* depthTarget)
{
	RcPtr<gve::GveImageView> depthImgView = nullptr;
	do 
	{
		if (!depthTarget)
		{
			break;
		}

		GveImageCreateInfo imgInfo = {};
		imgInfo.type = VK_IMAGE_TYPE_2D;
		imgInfo.format = convertZFormatToVkFormat(depthTarget->getZFormat());  // TODO: Should check format support
		imgInfo.flags = 0;
		imgInfo.sampleCount = VK_SAMPLE_COUNT_1_BIT;
		imgInfo.extent.width = depthTarget->getWidth();
		imgInfo.extent.height = depthTarget->getHeight();
		imgInfo.extent.depth = 1;
		imgInfo.numLayers = 1;
		imgInfo.mipLevels = 1;
		imgInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
		imgInfo.stages = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
		imgInfo.access = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
		imgInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imgInfo.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
		imgInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

		//uint64_t key = reinterpret_cast<uint64_t>(depthTarget->getZReadAddress());
		//auto depthImage = m_device->createOrGetImageTsharp(imgInfo, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, key);
		auto depthImage = m_device->createImage(imgInfo, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		if (!depthImage)
		{
			LOG_ERR("create depth image failed.");
			break;
		}

		GveImageViewCreateInfo viewInfo = {};
		viewInfo.type = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format = imgInfo.format;
		viewInfo.usage = imgInfo.usage;
		viewInfo.aspect = VK_IMAGE_ASPECT_DEPTH_BIT;
		//depthImgView = m_device->createOrGetImageViewTsharp(depthImage, viewInfo, key);
		depthImgView = m_device->createImageView(depthImage, viewInfo);
		if (!depthImgView)
		{
			LOG_ERR("create depth image view failed.");
			break;
		}

	} while (false);
	return depthImgView;
}


VkFormat GnmCommandBufferDraw::convertZFormatToVkFormat(ZFormat zfmt)
{
	VkFormat format = VK_FORMAT_UNDEFINED;
	switch (zfmt)
	{
	case kZFormatInvalid: format = VK_FORMAT_UNDEFINED; break;
	// seems there's no half float point image format.
	case kZFormat16: format = VK_FORMAT_D16_UNORM; break;
	case kZFormat32Float: format = VK_FORMAT_D32_SFLOAT; break;
	}
	return format;
}

VkCompareOp GnmCommandBufferDraw::convertCompareFunc(CompareFunc cmpFunc)
{
	VkCompareOp op;
	switch (cmpFunc)
	{
	case kCompareFuncNever:        op = VK_COMPARE_OP_NEVER; break;
	case kCompareFuncLess:         op = VK_COMPARE_OP_LESS; break;
	case kCompareFuncEqual:        op = VK_COMPARE_OP_EQUAL; break;
	case kCompareFuncLessEqual:    op = VK_COMPARE_OP_LESS_OR_EQUAL; break;
	case kCompareFuncGreater:      op = VK_COMPARE_OP_GREATER; break;
	case kCompareFuncNotEqual:     op = VK_COMPARE_OP_NOT_EQUAL; break;
	case kCompareFuncGreaterEqual: op = VK_COMPARE_OP_GREATER_OR_EQUAL; break;
	case kCompareFuncAlways:       op = VK_COMPARE_OP_ALWAYS;break;
	}
	return op;
}

VkPolygonMode GnmCommandBufferDraw::convertPolygonMode(PrimitiveSetupPolygonMode polyMode)
{
	VkPolygonMode mode;
	switch (polyMode)
	{
	case kPrimitiveSetupPolygonModePoint: mode = VK_POLYGON_MODE_POINT; break;
	case kPrimitiveSetupPolygonModeLine: mode = VK_POLYGON_MODE_LINE; break;
	case kPrimitiveSetupPolygonModeFill: mode = VK_POLYGON_MODE_FILL; break;
	}
	return mode;
}

VkCullModeFlags GnmCommandBufferDraw::convertCullMode(PrimitiveSetupCullFaceMode cullMode)
{
	VkCullModeFlags mode;
	switch (cullMode)
	{
	case kPrimitiveSetupCullFaceNone:  mode = VK_CULL_MODE_NONE; break;
	case kPrimitiveSetupCullFaceFront: mode = VK_CULL_MODE_FRONT_BIT; break;
	case kPrimitiveSetupCullFaceBack:  mode = VK_CULL_MODE_BACK_BIT; break;
	case kPrimitiveSetupCullFaceFrontAndBack: mode = VK_CULL_MODE_FRONT_AND_BACK; break;
	}
	return mode;
}

VkBlendFactor GnmCommandBufferDraw::convertBlendMultiplierToFactor(BlendMultiplier blendMul)
{
	VkBlendFactor factor;
	switch (blendMul)
	{
	case kBlendMultiplierZero: factor = VK_BLEND_FACTOR_ZERO; break;
	case kBlendMultiplierOne: factor = VK_BLEND_FACTOR_ONE; break;
	case kBlendMultiplierSrcColor: factor = VK_BLEND_FACTOR_SRC_COLOR; break;
	case kBlendMultiplierOneMinusSrcColor: factor = VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR; break;
	case kBlendMultiplierSrcAlpha: factor = VK_BLEND_FACTOR_SRC_ALPHA; break;
	case kBlendMultiplierOneMinusSrcAlpha: factor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA; break;
	case kBlendMultiplierDestAlpha: factor = VK_BLEND_FACTOR_DST_ALPHA; break;
	case kBlendMultiplierOneMinusDestAlpha: factor = VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA; break;
	case kBlendMultiplierDestColor: factor = VK_BLEND_FACTOR_DST_COLOR; break;
	case kBlendMultiplierOneMinusDestColor: factor = VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR; break;
	case kBlendMultiplierSrcAlphaSaturate: factor = VK_BLEND_FACTOR_SRC_ALPHA_SATURATE; break;
	case kBlendMultiplierConstantColor: factor = VK_BLEND_FACTOR_CONSTANT_COLOR; break;
	case kBlendMultiplierOneMinusConstantColor: factor = VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR; break;
	case kBlendMultiplierSrc1Color: factor = VK_BLEND_FACTOR_SRC1_COLOR; break;
	case kBlendMultiplierInverseSrc1Color: factor = VK_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR; break;
	case kBlendMultiplierSrc1Alpha: factor = VK_BLEND_FACTOR_SRC1_ALPHA; break;
	case kBlendMultiplierInverseSrc1Alpha: factor = VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA; break;
	case kBlendMultiplierConstantAlpha: factor = VK_BLEND_FACTOR_CONSTANT_ALPHA; break;
	case kBlendMultiplierOneMinusConstantAlpha: factor = VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA; break;
	}
	return factor;
}

VkBlendOp GnmCommandBufferDraw::convertBlendFuncToOp(BlendFunc func)
{
	VkBlendOp op;
	switch (func)
	{
	case kBlendFuncAdd: op = VK_BLEND_OP_ADD; break;
	case kBlendFuncSubtract: op = VK_BLEND_OP_SUBTRACT; break;
	case kBlendFuncMin: op = VK_BLEND_OP_MIN; break;
	case kBlendFuncMax: op = VK_BLEND_OP_MAX; break;
	case kBlendFuncReverseSubtract: op = VK_BLEND_OP_REVERSE_SUBTRACT; break;
	}
	return op;
}

VkPrimitiveTopology GnmCommandBufferDraw::convertPrimitiveTypeToTopology(PrimitiveType primType)
{
	VkPrimitiveTopology topology;
	switch (primType)
	{
	case kPrimitiveTypeNone: topology = VK_PRIMITIVE_TOPOLOGY_MAX_ENUM; break;
	case kPrimitiveTypePointList: topology = VK_PRIMITIVE_TOPOLOGY_POINT_LIST; break;
	case kPrimitiveTypeLineList: topology = VK_PRIMITIVE_TOPOLOGY_LINE_LIST; break;
	case kPrimitiveTypeLineStrip: topology = VK_PRIMITIVE_TOPOLOGY_LINE_STRIP; break;
	case kPrimitiveTypeTriList: topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST; break;
	case kPrimitiveTypeTriFan: topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN; break;
	case kPrimitiveTypeTriStrip: topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP; break;
	case kPrimitiveTypePatch: topology = VK_PRIMITIVE_TOPOLOGY_PATCH_LIST; break;
	case kPrimitiveTypeLineListAdjacency: topology = VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY; break;
	case kPrimitiveTypeLineStripAdjacency: topology = VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY; break;
	case kPrimitiveTypeTriListAdjacency: topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY; break;
	case kPrimitiveTypeTriStripAdjacency: topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY; break;
	// Not supported by vulkan
	//case kPrimitiveTypeRectList: topology = ; break;
	//case kPrimitiveTypeLineLoop: topology = ; break;
	//case kPrimitiveTypeQuadList: topology = ; break;
	//case kPrimitiveTypeQuadStrip: topology = ; break;
	//case kPrimitiveTypePolygon: topology = ; break;
	}
	return topology;
}

VkIndexType GnmCommandBufferDraw::convertIndexSize(IndexSize indexSize)
{
	VkIndexType indexType;
	switch (indexSize)
	{
	case kIndexSize16: indexType = VK_INDEX_TYPE_UINT16; break;
	case kIndexSize32: indexType = VK_INDEX_TYPE_UINT32; break;
	// Not sure for dispatch draw
	case kIndexSize16ForDispatchDraw: indexType = VK_INDEX_TYPE_UINT16; break;
	case kIndexSize32ForDispatchDraw: indexType = VK_INDEX_TYPE_UINT32; break;
	}
	return indexType;
}
