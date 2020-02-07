#include "GnmCommandBufferDraw.h"

#include "GnmSharpBuffer.h"
#include "GnmBuffer.h"
#include "GnmTexture.h"
#include "GnmSampler.h"
#include "GnmConvertor.h"

#include "../Gve/GveCmdList.h"
#include "../Gve/GveShader.h"
#include "../Gve/GveBuffer.h"
#include "../Gve/GveImage.h"
#include "../Gve/GveSampler.h"
#include "../Gve/GveContext.h"
#include "../Pssl/PsslShaderModule.h"

#include <algorithm>

LOG_CHANNEL(Graphic.Gnm.GnmCommandBufferDraw);

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
	// m_context->beginRecording(m_cmd);
}

void GnmCommandBufferDraw::prepareFlip()
{
	// m_context->endRecording();
}

// Last call of a frame.
void GnmCommandBufferDraw::prepareFlip(void *labelAddr, uint32_t value)
{
	*(uint32_t*)labelAddr = value;
	// m_context->endRecording();
}

// Last call of a frame, with interrupt.
void GnmCommandBufferDraw::prepareFlipWithEopInterrupt(EndOfPipeEventType eventType, void *labelAddr, uint32_t value, CacheAction cacheAction)
{
	*(uint32_t*)labelAddr = value;
	// m_context->endRecording();
}

void GnmCommandBufferDraw::setPsShaderUsage(const uint32_t *inputTable, uint32_t numItems)
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

	float width = scale[0] / 0.5f;
	float height = -scale[1] / 0.5f;
	float left = offset[0] - scale[0];
	float top = offset[1] + scale[1];

	VkViewport viewport;
	viewport.x = left;
	viewport.y = top + height;
	viewport.width = width;
	viewport.height = -height;
	viewport.minDepth = dmin;
	viewport.maxDepth = dmax;

	VkRect2D scissor;
	scissor.offset.x = 0;
	scissor.offset.y = 0;
	scissor.extent.width = width;
	scissor.extent.height = height;

	// m_context->setViewport(viewport, scissor);
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
		// m_context->bindRenderTargets(&colorTarget, 1);
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
		depthAttach.layout = m_depthTarget->imageInfo().layout;
		// m_context->bindDepthRenderTarget(depthAttach);

	} while (false);
}

void GnmCommandBufferDraw::setRenderTargetMask(uint32_t mask)
{

}

void GnmCommandBufferDraw::setDepthStencilControl(DepthStencilControl depthStencilControl)
{
	do 
	{
		VkCompareOp depthCmpOp = cvt::convertCompareFunc(depthStencilControl.getDepthControlZCompareFunction());

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

		// m_context->setDepthStencilState(dsInfo);

	} while (false);
}

void GnmCommandBufferDraw::setBlendControl(uint32_t rtSlot, BlendControl blendControl)
{
	do 
	{
		// LOG_ASSERT(rtSlot == 0, "only support 0 rtSlot");

		auto cbInfo = GveColorBlendInfo(
			VK_FALSE,
			VK_LOGIC_OP_COPY
		);

		VkBlendFactor colorSrcFactor = cvt::convertBlendMultiplierToFactor(blendControl.getColorEquationSourceMultiplier());
		VkBlendFactor colorDstFactor = cvt::convertBlendMultiplierToFactor(blendControl.getColorEquationDestinationMultiplier());
		VkBlendOp colorBlendOp = cvt::convertBlendFuncToOp(blendControl.getColorEquationBlendFunction());

		VkBlendFactor alphaSrcFactor = cvt::convertBlendMultiplierToFactor(blendControl.getAlphaEquationSourceMultiplier());
		VkBlendFactor alphaDstFactor = cvt::convertBlendMultiplierToFactor(blendControl.getAlphaEquationDestinationMultiplier());
		VkBlendOp alphaBlendOp = cvt::convertBlendFuncToOp(blendControl.getAlphaEquationBlendFunction());

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

		// m_context->setColorBlendState(cbInfo);
	} while (false);
}

void GnmCommandBufferDraw::setPrimitiveSetup(PrimitiveSetup primSetup)
{
	do 
	{
		VkFrontFace frontFace = primSetup.getFrontFace() == kPrimitiveSetupFrontFaceCcw ? 
			VK_FRONT_FACE_COUNTER_CLOCKWISE : VK_FRONT_FACE_CLOCKWISE;
		VkPolygonMode polyMode = cvt::convertPolygonMode(primSetup.getPolygonModeFront());
		VkCullModeFlags cullMode = cvt::convertCullMode(primSetup.getCullFace());

		auto rsInfo = GveRasterizationInfo(
			VK_FALSE,
			VK_FALSE,
			polyMode,
			cullMode,
			frontFace
		);

		// m_context->setRasterizerState(rsInfo);

	} while (false);
}

void GnmCommandBufferDraw::setActiveShaderStages(ActiveShaderStages activeStages)
{

}

void GnmCommandBufferDraw::setIndexSize(IndexSize indexSize, CachePolicy cachePolicy)
{
	m_indexType = cvt::convertIndexSize(indexSize);
}

void GnmCommandBufferDraw::dispatch(uint32_t threadGroupX, uint32_t threadGroupY, uint32_t threadGroupZ)
{
}

void GnmCommandBufferDraw::setPrimitiveType(PrimitiveType primType)
{
	do 
	{
		VkPrimitiveTopology topology = cvt::convertPrimitiveTypeToTopology(primType);

		auto isInfo = GveInputAssemblyInfo(
			topology,
			VK_FALSE,
			0
		);

		// m_context->setInputAssemblyState(isInfo);
	} while (false);
}

void GnmCommandBufferDraw::drawIndex(uint32_t indexCount, const void *indexAddr, DrawModifier modifier)
{
	do 
	{
		bindIndexBuffer(indexAddr, indexCount);

		commitVsStage();
		commitPsStage();

		// TODO:
		// This is a dummy state.
		auto msInfo = GveMultisampleInfo(VK_SAMPLE_COUNT_1_BIT, VK_FALSE, 0.0, 0, VK_FALSE, VK_FALSE);
		// m_context->setMultiSampleState(msInfo);

		// m_context->drawIndex(indexCount, 1, 0, 0, 0);

	} while (false);
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

#define SHADER_DEBUG_BREAK(mod, hash) \
if (mod.key().toUint64() == hash) \
{\
	__debugbreak();\
}

void GnmCommandBufferDraw::commitVsStage()
{
	do
	{
		uint32_t* fsCode = getFetchShaderCode(m_vsContext);
		LOG_ASSERT(fsCode != nullptr, "can not find fetch shader code.");

		PsslShaderModule vsModule((const uint32_t*)m_vsContext.code, fsCode);
		LOG_DEBUG("vertex shader hash %llX", vsModule.key().toUint64());
		vsModule.defineShaderInput(m_vsContext.userDataSlotTable);

		m_vsContext.shader = vsModule.compile();

		auto vsInputUsageSlots = vsModule.inputUsageSlots();
		for (const auto& inputSlot : vsInputUsageSlots)
		{
			// For Debug
			continue;

			// Find shader res in current slot
			auto pred = [&inputSlot](const auto& item)
			{
				return inputSlot.startRegister == item.startRegister;
			};
			auto iter = std::find_if(m_vsContext.userDataSlotTable.begin(), m_vsContext.userDataSlotTable.end(), pred);

			// Bind shader resources
			switch (inputSlot.usageType)
			{
			case kShaderInputUsageImmConstBuffer:
				bindImmConstBuffer(*iter);
				break;
			case kShaderInputUsagePtrVertexBufferTable:
			{
				setVertexInputLayout(*iter, vsModule.vsInputSemantic());
				bindVertexBuffers(*iter, vsModule.vsInputSemantic());
			}
				break;
			default:
				LOG_WARN("unsupported input usage %d", inputSlot.usageType);
				break;
			}
		}

		// m_context->bindShader(VK_SHADER_STAGE_VERTEX_BIT, m_vsContext.shader);
	} while (false);
}

void GnmCommandBufferDraw::commitPsStage()
{
	do 
	{
		PsslShaderModule psModule((const uint32_t*)m_psContext.code);
		LOG_DEBUG("pixel shader hash %llX", psModule.key().toUint64());
		psModule.defineShaderInput(m_psContext.userDataSlotTable);

		SHADER_DEBUG_BREAK(psModule, 0x1C3E9BEC02B89E0E);

		m_psContext.shader = psModule.compile();

		auto psInputUsageSlots = psModule.inputUsageSlots();
		for (const auto& inputSlot : psInputUsageSlots)
		{
			// For Debug
			continue;

			// Find shader res in current slot
			auto pred = [&inputSlot](const auto& item)
			{
				return inputSlot.startRegister == item.startRegister;
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

		// m_context->bindShader(VK_SHADER_STAGE_FRAGMENT_BIT, m_psContext.shader);
	} while (false);
}

void GnmCommandBufferDraw::bindIndexBuffer(const void* indexAddr, uint32_t indexCount)
{
	do 
	{
		if (!indexAddr || !indexCount)
		{
			break;
		}

		uint32_t perIndexSize = m_indexType == VK_INDEX_TYPE_UINT16 ? sizeof(uint16_t) : sizeof(uint32_t);
		VkDeviceSize indexBufferSize = perIndexSize * indexCount;

		GveBufferCreateInfo info = {};
		info.size = indexBufferSize;
		info.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;

		uint64_t key = reinterpret_cast<uint64_t>(indexAddr);
		auto indexBuffer = m_device->createOrGetBufferVsharp(info, 
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, key);

		// m_context->updateBuffer(indexBuffer, 0, indexBufferSize, indexAddr);

		// m_context->bindIndexBuffer(indexBuffer, m_indexType);

	} while (false);
}

void GnmCommandBufferDraw::bindImmConstBuffer(const PsslShaderResource& res)
{
	// TODO:
	// For buffers allocated directly from gnm command buffer,
	// we should implement "allocateFromCommandBuffer",
	// and use push constants instead of UBOs.
	do 
	{
		const GnmBuffer* buffer = reinterpret_cast<const GnmBuffer*>(res.resource);
		if (!buffer)
		{
			break;
		}

		VkDeviceSize bufferSize = buffer->getSize();
		GveBufferCreateInfo info = {};
		info.size = bufferSize;
		info.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;

		uint64_t key = reinterpret_cast<uint64_t>(buffer->getBaseAddress());

		auto uniformBuffer = m_device->createOrGetBufferVsharp(info, 
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, key);
		
		// m_context->updateBuffer(uniformBuffer, 0, bufferSize, buffer->getBaseAddress());

		uint32_t regSlot = computeConstantBufferBinding(VertexShader, res.startRegister);
		// m_context->bindResourceBuffer(regSlot, uniformBuffer);

	} while (false);

}

void GnmCommandBufferDraw::setVertexInputLayout(const PsslShaderResource& res, const std::vector<VertexInputSemantic>& inputSemantics)
{
	do
	{
		const GnmBuffer* vertexTable = reinterpret_cast<const GnmBuffer*>(res.resource);
		if (!vertexTable)
		{
			break;
		}

		// TODO:
		// For some games, ie. Nier:Automata, vertex attributes are not stored
		// in a single vertex buffer area, so in this case we need to use multiple vertex
		// bindings. But for other games, all vertex attributes are within the same memory area,
		// in this case, we only need one vertex binding.
		// Currently I only support the first case, we need to check whether these attributes 
		// are in same memory area or not.

		uint32_t bindingCount = inputSemantics.size();
		GveVertexInputInfo viInfo = {};

		uint32_t location = 0;
		for (uint32_t i = 0; i != bindingCount; ++i)
		{
			const GnmBuffer& vsharp = vertexTable[i];

			uint32_t stride = vsharp.getStride();
			auto binding = GveVertexBinding(i, stride, VK_VERTEX_INPUT_RATE_VERTEX, 0);
			viInfo.addBinding(binding);

			VkFormat vtxFmt = cvt::convertDataFormatToVkFormat(vsharp.getDataFormat());
			auto attr = GveVertexAttribute(location++, i, vtxFmt, 0);
			viInfo.addAttribute(attr);
		}

		// m_context->setVertexInputLayout(viInfo);
	} while (false);
}

bool GnmCommandBufferDraw::bindVertexBuffer(uint32_t bindingId, const GnmBuffer& vsharp)
{
	bool ret = false;
	do
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

		void* vtxData = vsharp.getBaseAddress();
		if (!vtxData)
		{
			LOG_WARN("empty vertex data");
			break;
		}

		VkDeviceSize bufferSize = vsharp.getSize();

		GveBufferCreateInfo buffInfo = {};
		buffInfo.size = bufferSize;
		buffInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

		uint64_t key = reinterpret_cast<uint64_t>(vtxData);

		auto vertexBuffer = m_device->createOrGetBufferVsharp(buffInfo, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, key);
		// m_context->updateBuffer(vertexBuffer, 0, bufferSize, vtxData);

		uint32_t stride = vsharp.getStride();
		// m_context->bindVertexBuffer(bindingId, GveBufferSlice(vertexBuffer, 0, bufferSize), stride);
	} while (false);
	return ret;
}

void GnmCommandBufferDraw::bindVertexBuffers(const PsslShaderResource& res, const std::vector<VertexInputSemantic>& inputSemantics)
{
	do 
	{
		const GnmBuffer* vertexTable = reinterpret_cast<const GnmBuffer*>(res.resource);
		if (!vertexTable)
		{
			break;
		}

		uint32_t bindingCount = inputSemantics.size();
		for (uint32_t i = 0; i != bindingCount; ++i)
		{
			const GnmBuffer& vsharp = vertexTable[i];
			bindVertexBuffer(i, vsharp);
		}
	} while (false);
}

void GnmCommandBufferDraw::bindImmResource(const PsslShaderResource& res)
{
	do 
	{
		const GnmTexture* tsharp = reinterpret_cast<const GnmTexture*>(res.resource);
		if (!tsharp)
		{
			break;
		}

		GveImageCreateInfo imgInfo = {};
		imgInfo.type = VK_IMAGE_TYPE_2D;
		imgInfo.format = cvt::convertDataFormatToVkFormat(tsharp->getDataFormat());
		imgInfo.flags = 0;
		imgInfo.sampleCount = VK_SAMPLE_COUNT_1_BIT;
		imgInfo.extent.width = tsharp->getWidth();
		imgInfo.extent.height = tsharp->getHeight();
		imgInfo.extent.depth = tsharp->getDepth();
		imgInfo.numLayers = 1;
		imgInfo.mipLevels = 1;
		imgInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
		imgInfo.stages = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		imgInfo.access = VK_ACCESS_SHADER_READ_BIT;
		imgInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imgInfo.layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imgInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

		uint64_t key = reinterpret_cast<uint64_t>(tsharp->getBaseAddress());
		auto texture = m_device->createOrGetImageTsharp(imgInfo, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, key);

		VkDeviceSize imageBufferSize = tsharp->getSizeAlign().m_size;
		void* data                   = util::gnmGpuAbsAddr((void*)res.resource, tsharp->getBaseAddress());
		m_context->updateImage(texture, 0, imageBufferSize, data);

		GveImageViewCreateInfo viewInfo;
		viewInfo.type = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format = imgInfo.format;
		viewInfo.usage = imgInfo.usage;
		viewInfo.aspect = VK_IMAGE_ASPECT_COLOR_BIT;
		auto texView = m_device->createOrGetImageViewTsharp(texture, viewInfo, key);

		uint32_t regSlot = computeResBinding(PixelShader, res.startRegister);
		// m_context->bindResourceView(regSlot, texView, nullptr);

	} while (false);
}

void GnmCommandBufferDraw::bindSampler(const PsslShaderResource& res)
{
	do 
	{
		const GnmSampler* ssharp = reinterpret_cast<const GnmSampler*>(res.resource);
		if (!ssharp)
		{
			break;
		}

		uint64_t key = (uint64_t)ssharp->m_regs[0] | (uint64_t)ssharp->m_regs[1] << 32;

		// TODO:
		// Fill info
		GveSamplerCreateInfo info;
		auto sampler = m_device->createOrGetSamplerSsharp(info, key);

		uint32_t regSlot = computeSamplerBinding(PixelShader, res.startRegister);
		// m_context->bindSampler(regSlot, sampler);

	} while (false);
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
			if (res.startRegister != fsStartReg)
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
		imgInfo.format = cvt::convertZFormatToVkFormat(depthTarget->getZFormat());  // TODO: Should check format support
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
