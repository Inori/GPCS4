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

void GnmCommandBufferDraw::setPrimitiveType(PrimitiveType primType)
{

}
#include <set>
std::set<std::pair<uint64_t, uint64_t>> g_shaderKeys;
std::set<std::pair<uint32_t, uint32_t>> g_fmtSets;

void GnmCommandBufferDraw::drawIndex(uint32_t indexCount, const void *indexAddr, DrawModifier modifier)
{
	do
	{
		if (!m_vsCode)
		{
			break;
		}

		uint32_t* fsCode = getFetchShaderCode(m_vsCode);
		if (!fsCode)
		{
			break;
		}


		pssl::PsslShaderModule vsModule((const uint32_t*)m_vsCode, fsCode);
		pssl::PsslShaderModule psModule((const uint32_t*)m_psCode);
	

		uint64_t vsKey = vsModule.key().getKey();
		uint64_t psKey = psModule.key().getKey();
		g_shaderKeys.insert(std::make_pair(vsKey, psKey));


		// Index Buffer
		if (indexAddr)
		{
			m_videoOut->createIndexBuffer((void*)indexAddr, indexCount, indexCount * sizeof(uint16_t));
		}

		// Vertex Buffer
		uint32_t vbtStartReg = 0;
		auto vsInputSlots = vsModule.inputUsageSlots();
		for (auto& slot : vsInputSlots)
		{
			if (slot.usageType != pssl::kShaderInputUsagePtrVertexBufferTable)
			{
				continue;
			}

			vbtStartReg = slot.startRegister;
			break;
		}

		// Find VertexBufferTable
		GnmBuffer* vsharpBuffers = nullptr;
		for (auto& pair : m_vsUserDataSlotTable)
		{
			if (pair.first != vbtStartReg)
			{
				continue;
			}
			vsharpBuffers = (GnmBuffer*)pair.second;
			break;
		}

		if (!vsharpBuffers)
		{
			break;
		}

		auto inputSemantics = vsModule.vsInputSemantic();
		if (inputSemantics.empty())
		{
			break;
		}

		// All VertexElements should have the same stride
		uint32_t stride = vsharpBuffers[0].stride;
		m_videoOut->createVertexInputInfo(stride, vsharpBuffers, inputSemantics);
		uint32_t vertexBufferSize = vsharpBuffers[0].num_records * stride;
		m_videoOut->createVertexBuffer((void*)vsharpBuffers[0].base, vsharpBuffers[0].num_records, vertexBufferSize);

		// Texture
		uint32_t texBuffStartReg = 0xFF;
		auto psInputSlots = psModule.inputUsageSlots();
		for (auto& slot : psInputSlots)
		{
			if (slot.usageType != pssl::kShaderInputUsageImmResource)
			{
				continue;
			}
			texBuffStartReg = slot.startRegister;
			break;
		}

		GnmTexture* tsharpBuffer = nullptr;
		if (texBuffStartReg != 0xFF)
		{
			// Have texture, find it.
			for (auto& pair : m_psUserDataSlotTable)
			{
				if (pair.first != texBuffStartReg)
				{
					continue;
				}
				tsharpBuffer = (GnmTexture*)pair.second;
				break;
			}
		}

		if (tsharpBuffer)
		{
			GnmTexture& tex = tsharpBuffer[0];
			VkFormat format = VK_FORMAT_UNDEFINED;

			g_fmtSets.insert(std::make_pair<uint32_t, uint32_t>(tex.dfmt, tex.nfmt));

			uint32_t pixSize = 0;
			switch (tex.dfmt)
			{
			case kBufferFormat8:
				pixSize = 1;
				format = VK_FORMAT_R8_UNORM;
				break;
			case kBufferFormat32:
				pixSize = 4;
				format = VK_FORMAT_R32_SFLOAT;
				break;
			case kBufferFormat10_11_11:
				pixSize = 4;
				format = VK_FORMAT_B10G11R11_UFLOAT_PACK32;
				break;
			default:
				break;
			}

			uint32_t texSize = tex.width * tex.height * pixSize;
			void* texAddr = GNM_GPU_ABS_ADDR(indexAddr, tex.baseaddr256 << 8);
			m_videoOut->createTextureImage(texAddr,
				tex.width, tex.height, texSize, format);
		}

		// Shaders
		auto vsEmptyCode = UtilFile::LoadFile("empty.vert.spv");
		auto psEmptyCode = UtilFile::LoadFile("empty.frag.spv");
		m_videoOut->createShaderModules(vsEmptyCode, psEmptyCode);

		m_videoOut->createGraphicsPipeline();

		static uint32_t displayBufferIndex = 0;

		m_videoOut->createCommandBuffers(displayBufferIndex);

		++displayBufferIndex;
		displayBufferIndex %= 3;
		
	} while (false);

	m_videoOut->clearFrameResource();
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
