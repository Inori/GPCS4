#include "GnmCommandBufferDraw.h"

GnmCommandBufferDraw::GnmCommandBufferDraw()
{
}

GnmCommandBufferDraw::~GnmCommandBufferDraw()
{
}

void GnmCommandBufferDraw::prepareFlip(void *labelAddr, uint32_t value)
{
	*(uint32_t*)labelAddr = value;
}

void GnmCommandBufferDraw::setComputeResourceManagement(ShaderEngine engine, uint16_t mask)
{

}

void GnmCommandBufferDraw::setComputeScratchSize(uint32_t maxNumWaves, uint32_t num1KByteChunksPerWave)
{

}

void GnmCommandBufferDraw::setComputeShaderControl(uint32_t wavesPerSh, uint32_t threadgroupsPerCu, uint32_t lockThreshold)
{

}

void GnmCommandBufferDraw::setPointerInUserData(ShaderStage stage, uint32_t startUserDataSlot, void *gpuAddr)
{

}

void GnmCommandBufferDraw::setPsShader(const PsStageRegisters *psRegs)
{

}

void GnmCommandBufferDraw::setUserData(ShaderStage stage, uint32_t userDataSlot, uint32_t data)
{

}

void GnmCommandBufferDraw::setUserDataRegion(ShaderStage stage, uint32_t startUserDataSlot, const uint32_t *userData, uint32_t numDwords)
{

}

struct ShaderBinaryInfo
{
	uint8_t			m_signature[7];				// 'OrbShdr'
	uint8_t			m_version;					// ShaderBinaryInfoVersion

	unsigned int	m_pssl_or_cg : 1;	// 1 = PSSL / Cg, 0 = IL / shtb
	unsigned int	m_cached : 1;	// 1 = when compile, debugging source was cached.  May only make sense for PSSL=1
	uint32_t		m_type : 4;	// See enum ShaderBinaryType
	uint32_t		m_source_type : 2;	// See enum ShaderSourceType
	unsigned int	m_length : 24;	// Binary code length (does not include this structure or any of its preceding associated tables)

	uint8_t			m_chunkUsageBaseOffsetInDW;			// in DW, which starts at ((uint32_t*)&ShaderBinaryInfo) - m_chunkUsageBaseOffsetInDW; max is currently 7 dwords (128 T# + 32 V# + 20 CB V# + 16 UAV T#/V#)
	uint8_t			m_numInputUsageSlots;				// Up to 16 user data reg slots + 128 extended user data dwords supported by CUE; up to 16 user data reg slots + 240 extended user data dwords supported by InputUsageSlot
	uint8_t         m_isSrt : 1;	// 1 if this shader uses shader resource tables and has an SrtDef table embedded below the input usage table and any extended usage info
	uint8_t         m_isSrtUsedInfoValid : 1;	// 1 if SrtDef::m_isUsed=0 indicates an element is definitely unused; 0 if SrtDef::m_isUsed=0 indicates only that the element is not known to be used (m_isUsed=1 always indicates a resource is known to be used)
	uint8_t         m_isExtendedUsageInfo : 1;	// 1 if this shader has extended usage info for the InputUsage table embedded below the input usage table
	uint8_t         m_reserved2 : 5;	// For future use
	uint8_t         m_reserved3;						// For future use

	uint32_t		m_shaderHash0;				// Association hash first 4 bytes
	uint32_t		m_shaderHash1;				// Association hash second 4 bytes
	uint32_t		m_crc32;					// crc32 of shader + this struct, just up till this field
};


ShaderBinaryInfo* findShaderBinInfo(uint8_t* code)
{
	for (size_t i = 0; i != 0xFFFFFFFF; i++)
	{
		if (!std::memcmp(&code[i], "OrbShdr", 7))
		{
			return (ShaderBinaryInfo*)&code[i];
		}
	}
	return nullptr;
}


void GnmCommandBufferDraw::setVsShader(const VsStageRegisters *vsRegs, uint32_t shaderModifier)
{
	void* gpuAddress = (void*)(uintptr_t(vsRegs->spiShaderPgmHiVs) << 40 | uintptr_t(vsRegs->spiShaderPgmLoVs) << 8);
	ShaderBinaryInfo* sbInfo = findShaderBinInfo((uint8_t*)gpuAddress);
	LOG_DEBUG("%d", sbInfo->m_crc32);
}

void GnmCommandBufferDraw::waitForGraphicsWrites(uint32_t baseAddr256, uint32_t sizeIn256ByteBlocks, uint32_t targetMask, CacheAction cacheAction, uint32_t extendedCacheMask, StallCommandBufferParserMode commandBufferStallMode)
{

}

void GnmCommandBufferDraw::writeAtEndOfPipe(EndOfPipeEventType eventType, EventWriteDest dstSelector, void *dstGpuAddr, EventWriteSource srcSelector, uint64_t immValue, CacheAction cacheAction, CachePolicy cachePolicy)
{

}

void GnmCommandBufferDraw::writeAtEndOfPipeWithInterrupt(EndOfPipeEventType eventType, EventWriteDest dstSelector, void *dstGpuAddr, EventWriteSource srcSelector, uint64_t immValue, CacheAction cacheAction, CachePolicy cachePolicy)
{

}
