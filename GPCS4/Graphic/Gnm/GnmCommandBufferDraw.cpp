#include "GnmCommandBufferDraw.h"

GnmCommandBufferDraw::GnmCommandBufferDraw()
{
}

GnmCommandBufferDraw::~GnmCommandBufferDraw()
{
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

void GnmCommandBufferDraw::setUserData(ShaderStage stage, uint32_t userDataSlot, uint32_t data)
{

}

void GnmCommandBufferDraw::setUserDataRegion(ShaderStage stage, uint32_t startUserDataSlot, const uint32_t *userData, uint32_t numDwords)
{

}

void GnmCommandBufferDraw::waitForGraphicsWrites(uint32_t baseAddr256, uint32_t sizeIn256ByteBlocks, uint32_t targetMask, CacheAction cacheAction, uint32_t extendedCacheMask, StallCommandBufferParserMode commandBufferStallMode)
{

}
