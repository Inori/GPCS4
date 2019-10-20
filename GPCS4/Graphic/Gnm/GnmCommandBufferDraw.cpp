#include "GnmCommandBufferDraw.h"
#include "GnmSharpBuffer.h"



#include "Platform/PlatformUtils.h"

//// For test
//#define STB_IMAGE_WRITE_IMPLEMENTATION
//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image/stb_image.h"
//#include "stb_image/stb_image_write.h"

GnmCommandBufferDraw::GnmCommandBufferDraw(RcPtr<gve::GveContex>& context, const gve::GveRenderTarget& renderTarget):
	GnmCommandBuffer(context),
	m_renderTarget(renderTarget),
	m_vsCode(nullptr),
	m_psCode(nullptr)
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
}

void GnmCommandBufferDraw::setPsShader(const pssl::PsStageRegisters *psRegs)
{
	m_psCode = psRegs->getCodeAddress();
}

void GnmCommandBufferDraw::setVsShader(const pssl::VsStageRegisters *vsRegs, uint32_t shaderModifier)
{
	m_vsCode = vsRegs->getCodeAddress();

	// Fetch shader slot can be set after vs shader set and before draw call
}

void GnmCommandBufferDraw::setVgtControl(uint8_t primGroupSizeMinusOne, WdSwitchOnlyOnEopMode wdSwitchOnlyOnEopMode, VgtPartialVsWaveMode partialVsWaveMode)
{
	
}

void GnmCommandBufferDraw::setVsharpInUserData(ShaderStage stage, uint32_t startUserDataSlot, const VSharpBuffer *buffer)
{

}

void GnmCommandBufferDraw::setTsharpInUserData(ShaderStage stage, uint32_t startUserDataSlot, const TSharpBuffer *tex)
{

}

void GnmCommandBufferDraw::setSsharpInUserData(ShaderStage stage, uint32_t startUserDataSlot, const SSharpBuffer *sampler)
{

}

void GnmCommandBufferDraw::setPointerInUserData(ShaderStage stage, uint32_t startUserDataSlot, void *gpuAddr)
{
	do
	{
		if (stage == kShaderStageVs)
		{
			m_vsUserDataSlotTable.push_back({ startUserDataSlot, gpuAddr });
		}
		else if (stage == kShaderStagePs)
		{
			//m_psUserDataSlotTable.push_back({ startUserDataSlot, gpuAddr });
		}
		else
		{
			LOG_FIXME("other stage %d", stage);
		}
	} while (false);
}

void GnmCommandBufferDraw::setUserDataRegion(ShaderStage stage, uint32_t startUserDataSlot, const uint32_t *userData, uint32_t numDwords)
{
	do
	{
		if (stage == kShaderStageVs)
		{
			m_vsUserDataSlotTable.push_back({ startUserDataSlot, (void*)userData });
		}
		else if (stage == kShaderStagePs)
		{
			m_psUserDataSlotTable.push_back({ startUserDataSlot, (void*)userData });
		}
		else
		{
			LOG_FIXME("other stage %d", stage);
		}
	} while (false);
}


void GnmCommandBufferDraw::writeAtEndOfPipe(EndOfPipeEventType eventType, 
	EventWriteDest dstSelector, void *dstGpuAddr, 
	EventWriteSource srcSelector, uint64_t immValue, 
	CacheAction cacheAction, CachePolicy cachePolicy)
{
	if (srcSelector == kEventWriteSource32BitsImmediate)
	{
		*(uint32_t*)dstGpuAddr = immValue;
	}
	else if (srcSelector == kEventWriteSource64BitsImmediate)
	{
		*(uint64_t*)dstGpuAddr = immValue;
	}
	else
	{
		*(uint64_t*)dstGpuAddr = UtilProcess::GetProcessTimeCounter();
	}
}

void GnmCommandBufferDraw::writeAtEndOfPipeWithInterrupt(EndOfPipeEventType eventType, 
	EventWriteDest dstSelector, void *dstGpuAddr, 
	EventWriteSource srcSelector, uint64_t immValue, 
	CacheAction cacheAction, CachePolicy cachePolicy)
{
	if (srcSelector == kEventWriteSource32BitsImmediate)
	{
		*(uint32_t*)dstGpuAddr = immValue;
	}
	else if (srcSelector == kEventWriteSource64BitsImmediate)
	{
		*(uint64_t*)dstGpuAddr = immValue;
	}
	else
	{
		*(uint64_t*)dstGpuAddr = UtilProcess::GetProcessTimeCounter();
	}
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

}

void GnmCommandBufferDraw::setDepthRenderTarget(DepthRenderTarget const *depthTarget)
{

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
	do
	{
		uint32_t* fsCode = getFetchShaderCode(m_vsCode);

		RcPtr<gve::GveShader> vsShader;
		if (fsCode)
		{
			pssl::PsslShaderModule vsModule((const uint32_t*)m_vsCode, fsCode, m_vsUserDataSlotTable);
			vsShader = vsModule.compile();
		}
		else
		{
			pssl::PsslShaderModule vsModule((const uint32_t*)m_vsCode, m_vsUserDataSlotTable);
			vsShader = vsModule.compile();
		}

		pssl::PsslShaderModule psModule((const uint32_t*)m_psCode, m_psUserDataSlotTable);
		RcPtr<gve::GveShader> psShader = psModule.compile();

		m_context->bindShader(VK_SHADER_STAGE_VERTEX_BIT, vsShader);
		m_context->bindShader(VK_SHADER_STAGE_FRAGMENT_BIT, psShader);
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


uint32_t* GnmCommandBufferDraw::getFetchShaderCode(void* vsCode)
{
	uint32_t* fsCode = nullptr;
	do 
	{
		uint32_t fsStartReg = pssl::getFetchShaderStartRegister((const uint8_t*)vsCode);
		if (fsStartReg == UINT_MAX)
		{
			break;
		}

		for (const auto& res : m_vsUserDataSlotTable)
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
