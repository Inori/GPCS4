#include "GnmCommandBufferDraw.h"
#include "Platform/PlatformUtils.h"
#include "GnmSharpBuffer.h"
#include "../Pssl/PsslShaderModule.h"

GnmCommandBufferDraw::GnmCommandBufferDraw():
	m_vsCode(nullptr),
	m_psCode(nullptr)
{
}

GnmCommandBufferDraw::~GnmCommandBufferDraw()
{
}

void GnmCommandBufferDraw::drawIndex(uint32_t indexCount, const void *indexAddr, DrawModifier modifier)
{
	do 
	{
		if (!m_vsCode)
		{
			break;
		}

		uint32_t* fsCode = getFetchShaderCode(m_vsCode);
		if (fsCode)
		{
			pssl::PsslShaderModule module((const uint32_t*)m_vsCode, fsCode);
			auto vsInputSlots = module.inputUsageSlots();
			//m_vsShader = module.compile();
		}
		else
		{
			pssl::PsslShaderModule module((const uint32_t*)m_vsCode);
			//m_vsShader = module.compile();
		}

		pssl::PsslShaderModule module((const uint32_t*)m_psCode);
		
		auto psInputSlots = module.inputUsageSlots();
	

		//m_psShader = module.compile();

	} while (false);

	clearRenderState();
}

void GnmCommandBufferDraw::drawIndex(uint32_t indexCount, const void *indexAddr)
{
	DrawModifier mod = { 0 };
	drawIndex(indexCount, indexAddr, mod);
}

void GnmCommandBufferDraw::prepareFlip(void *labelAddr, uint32_t value)
{
	*(uint32_t*)labelAddr = value;
}


void GnmCommandBufferDraw::setPsShaderUsage(const uint32_t *inputTable, uint32_t numItems)
{

}

void GnmCommandBufferDraw::setPsShader(const pssl::PsStageRegisters *psRegs)
{
	m_psCode = psRegs->getCodeAddress();
}

void GnmCommandBufferDraw::setVsShader(const pssl::VsStageRegisters *vsRegs, uint32_t shaderModifier)
{
	m_vsCode = vsRegs->getCodeAddress();
}

void GnmCommandBufferDraw::setVsharpInUserData(ShaderStage stage, uint32_t startUserDataSlot, const GnmBuffer *buffer)
{

}

void GnmCommandBufferDraw::setTsharpInUserData(ShaderStage stage, uint32_t startUserDataSlot, const GnmTexture *tex)
{

}

void GnmCommandBufferDraw::setSsharpInUserData(ShaderStage stage, uint32_t startUserDataSlot, const GnmSampler *sampler)
{

}

void GnmCommandBufferDraw::setPointerInUserData(ShaderStage stage, uint32_t startUserDataSlot, void *gpuAddr)
{
	do
	{
		if (stage == kShaderStageVs)
		{
			m_vsUserDataSlotTable.push_back(std::make_pair(startUserDataSlot, gpuAddr));
		}
		else if (stage == kShaderStagePs)
		{
			m_psUserDataSlotTable.push_back(std::make_pair(startUserDataSlot, gpuAddr));
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
			m_vsUserDataSlotTable.push_back(std::make_pair(startUserDataSlot, (void*)userData));
		}
		else if (stage == kShaderStagePs)
		{
			m_psUserDataSlotTable.push_back(std::make_pair(startUserDataSlot, (void*)userData));
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
	throw std::logic_error("The method or operation is not implemented.");
}

void GnmCommandBufferDraw::initializeDefaultHardwareState()
{
	throw std::logic_error("The method or operation is not implemented.");
}

void GnmCommandBufferDraw::drawIndexAuto(uint32_t indexCount, DrawModifier modifier)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void GnmCommandBufferDraw::drawIndexAuto(uint32_t indexCount)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void GnmCommandBufferDraw::setEmbeddedVsShader(EmbeddedVsShader shaderId, uint32_t shaderModifier)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void GnmCommandBufferDraw::setVgtControl(uint8_t primGroupSizeMinusOne)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void GnmCommandBufferDraw::setVgtControl(uint8_t primGroupSizeMinusOne, VgtPartialVsWaveMode partialVsWaveMode)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void GnmCommandBufferDraw::updatePsShader(const pssl::PsStageRegisters *psRegs)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void GnmCommandBufferDraw::updateVsShader(const pssl::VsStageRegisters *vsRegs, uint32_t shaderModifier)
{
	throw std::logic_error("The method or operation is not implemented.");
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

		for (auto& pair : m_vsUserDataSlotTable)
		{
			if (pair.first != fsStartReg)
			{
				continue;
			}

			fsCode = (uint32_t*)pair.second;
			break;
		}

	} while (false);
	return fsCode;
}

void GnmCommandBufferDraw::clearRenderState()
{
	m_vsCode = nullptr;
	m_psCode = nullptr;
	m_vsUserDataSlotTable.clear();
	m_psUserDataSlotTable.clear();
}
