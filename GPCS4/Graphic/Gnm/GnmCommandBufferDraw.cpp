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


void GnmCommandBufferDraw::prepareFlip(void *labelAddr, uint32_t value)
{
	*(uint32_t*)labelAddr = value;
}

void GnmCommandBufferDraw::prepareFlipWithEopInterrupt(EndOfPipeEventType eventType, void *labelAddr, uint32_t value, CacheAction cacheAction)
{
	*(uint32_t*)labelAddr = value;
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
	onSetUserDataRegister(stage, startUserDataSlot, (uint32_t*)buffer, sizeof(VSharpBuffer)/sizeof(uint32_t));
}

void GnmCommandBufferDraw::setTsharpInUserData(ShaderStage stage, uint32_t startUserDataSlot, const TSharpBuffer *tex)
{
	onSetUserDataRegister(stage, startUserDataSlot, (uint32_t*)tex, sizeof(TSharpBuffer) / sizeof(uint32_t));
}

void GnmCommandBufferDraw::setSsharpInUserData(ShaderStage stage, uint32_t startUserDataSlot, const SSharpBuffer *sampler)
{
	onSetUserDataRegister(stage, startUserDataSlot, (uint32_t*)sampler, sizeof(SSharpBuffer) / sizeof(uint32_t));
}

void GnmCommandBufferDraw::setPointerInUserData(ShaderStage stage, uint32_t startUserDataSlot, void *gpuAddr)
{
	onSetUserDataRegister(stage, startUserDataSlot, (uint32_t*)gpuAddr, sizeof(void*) / sizeof(uint32_t));
}

void GnmCommandBufferDraw::setUserDataRegion(ShaderStage stage, uint32_t startUserDataSlot, const uint32_t *userData, uint32_t numDwords)
{
	onSetUserDataRegister(stage, startUserDataSlot, userData, numDwords);
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
}

void GnmCommandBufferDraw::setDepthRenderTarget(DepthRenderTarget const *depthTarget)
{
	do 
	{
		// TODO:
		// Not very correct if the game's depth target changes.
		if (!m_depthTarget)
		{
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
	
}

void GnmCommandBufferDraw::setBlendControl(uint32_t rtSlot, BlendControl blendControl)
{

}

void GnmCommandBufferDraw::setPrimitiveSetup(PrimitiveSetup reg)
{

}

void GnmCommandBufferDraw::setActiveShaderStages(ActiveShaderStages activeStages)
{

}

void GnmCommandBufferDraw::setIndexSize(IndexSize indexSize, CachePolicy cachePolicy)
{
	
}

// Will be called on every frame start.
void GnmCommandBufferDraw::initializeDefaultHardwareState()
{
	m_context->beginRecording(m_cmd);
}

void GnmCommandBufferDraw::setPrimitiveType(PrimitiveType primType)
{

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

void GnmCommandBufferDraw::onSetUserDataRegister(ShaderStage stage, uint32_t startSlot, const uint32_t* data, uint32_t numDwords)
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
			insertUniqueShaderResource(m_vsContext.userDataSlotTable, startSlot, shaderRes);
			break;
		case kShaderStagePs:
			insertUniqueShaderResource(m_psContext.userDataSlotTable, startSlot, shaderRes);
			break;
		default:
			break;
		}

	} while (false);
}

void GnmCommandBufferDraw::insertUniqueShaderResource(GnmShaderContext::UDSTVector& container, uint32_t startSlot, pssl::PsslShaderResource& shaderRes)
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

		uint64_t key = reinterpret_cast<uint64_t>(depthTarget->getZReadAddress());
		auto depthImage = m_device->createOrGetImageTsharp(imgInfo, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, key);
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
		depthImgView = m_device->createOrGetImageViewTsharp(depthImage, viewInfo, key);
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
	case kZFormatInvalid:
		format = VK_FORMAT_UNDEFINED;
		break;
	case kZFormat16:
		format = VK_FORMAT_D16_UNORM;  // seems there's no half float point image format.
		break;
	case kZFormat32Float:
		format = VK_FORMAT_D32_SFLOAT;
		break;
	}
	return format;
}
