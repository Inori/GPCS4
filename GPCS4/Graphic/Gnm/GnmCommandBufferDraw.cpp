#include "GnmCommandBufferDraw.h"
#include "Platform/PlatformUtils.h"
#include "GnmSharpBuffer.h"
#include "../Pssl/PsslShaderModule.h"

// For test
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.h"

GnmCommandBufferDraw::GnmCommandBufferDraw(std::shared_ptr<sce::SceVideoOut> videoOut):
	m_videoOut(videoOut),
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

void GnmCommandBufferDraw::setVgtControl(uint8_t primGroupSizeMinusOne, WdSwitchOnlyOnEopMode wdSwitchOnlyOnEopMode, VgtPartialVsWaveMode partialVsWaveMode)
{
	
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
	
}


// We be called on every frame start.
void GnmCommandBufferDraw::initializeDefaultHardwareState()
{
	
}


#include <set>
std::set<std::pair<uint64_t, uint64_t>> g_shaderKeys;

void GnmCommandBufferDraw::drawIndex(uint32_t indexCount, const void *indexAddr, DrawModifier modifier)
{
	do
	{
		if (!m_vsCode)
		{
			break;
		}

		uint32_t* fsCode = getFetchShaderCode(m_vsCode);
		uint64_t vsKey = 0;
		if (fsCode)
		{
			pssl::PsslShaderModule module((const uint32_t*)m_vsCode, fsCode);
			auto vsInputSlots = module.inputUsageSlots();
			vsKey = module.key().getKey();
			//m_vsShader = module.compile();
		}
		else
		{
			pssl::PsslShaderModule module((const uint32_t*)m_vsCode);
			vsKey = module.key().getKey();
			//m_vsShader = module.compile();
		}

		pssl::PsslShaderModule module((const uint32_t*)m_psCode);
		uint64_t psKey = module.key().getKey();
		g_shaderKeys.insert(std::make_pair(vsKey, psKey));

		auto psInputSlots = module.inputUsageSlots();
		//m_psShader = module.compile();

		if (m_vsUserDataSlotTable.size())
		{
			for (auto& pair : m_vsUserDataSlotTable)
			{
				GnmBuffer* vsBuffer = (GnmBuffer*)pair.second;
				uint64_t gpuAddr = vsBuffer->base;
				uint32_t stride = vsBuffer->stride;
			}
		}
		//debugDumpTexture();

	} while (false);

	clearRenderState();
}

void GnmCommandBufferDraw::drawIndex(uint32_t indexCount, const void *indexAddr)
{
	DrawModifier mod = { 0 };
	drawIndex(indexCount, indexAddr, mod);
}

void GnmCommandBufferDraw::drawIndexAuto(uint32_t indexCount, DrawModifier modifier)
{
	drawIndex(indexCount, NULL);
}

void GnmCommandBufferDraw::drawIndexAuto(uint32_t indexCount)
{
	drawIndex(indexCount, NULL);
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

void GnmCommandBufferDraw::debugDumpTexture()
{
	if (m_psUserDataSlotTable.size())
	{
		if (m_psUserDataSlotTable[0].first == 0)
		{
			TSharpBuffer* tsBuffer = (TSharpBuffer*)m_psUserDataSlotTable[0].second;
			uint64_t relaAddr = tsBuffer->baseaddr256 << 8;
			void* texAddr = GNM_GPU_ABS_ADDR(m_vsCode, relaAddr);
			uint32_t width = tsBuffer->width;
			uint32_t height = tsBuffer->height;
			uint32_t dfmd = tsBuffer->dfmt;
			uint32_t nfmd = tsBuffer->nfmt;

			static uint32_t count = 0;
			char filename[64] = { 0 };
			sprintf(filename, "tex%d.bmp", count++);
			stbi_write_bmp(filename, width, height, 4, texAddr);
		}
	}
}
