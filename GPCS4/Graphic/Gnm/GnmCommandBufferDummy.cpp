#include "GnmCommandBufferDummy.h"

#include "../Violet/VltDevice.h"
#include "../Violet/VltContext.h"
#include "../Violet/VltPresenter.h"
#include "../Sce/SceGpuQueue.h"


GnmCommandBufferDummy::GnmCommandBufferDummy():
	GnmCommandBuffer( {nullptr, nullptr, nullptr}, nullptr)
{
}

GnmCommandBufferDummy::~GnmCommandBufferDummy()
{
}

void GnmCommandBufferDummy::initializeDefaultHardwareState()
{
	
}

void GnmCommandBufferDummy::setViewportTransformControl(ViewportTransformControl vportControl)
{
	
}

void GnmCommandBufferDummy::setPrimitiveSetup(PrimitiveSetup reg)
{
	
}

void GnmCommandBufferDummy::setScreenScissor(int32_t left, int32_t top, int32_t right, int32_t bottom)
{
	
}

void GnmCommandBufferDummy::setViewport(uint32_t viewportId, float dmin, float dmax, const float scale[3], const float offset[3])
{
	
}

void GnmCommandBufferDummy::setHardwareScreenOffset(uint32_t offsetX, uint32_t offsetY)
{
	
}

void GnmCommandBufferDummy::setGuardBands(float horzClip, float vertClip, float horzDiscard, float vertDiscard)
{
	
}

void GnmCommandBufferDummy::setPsShaderUsage(const uint32_t* inputTable, uint32_t numItems)
{
	
}

void GnmCommandBufferDummy::setActiveShaderStages(ActiveShaderStages activeStages)
{
	
}

void GnmCommandBufferDummy::setPsShader(const pssl::PsStageRegisters* psRegs)
{
	
}

void GnmCommandBufferDummy::updatePsShader(const pssl::PsStageRegisters* psRegs)
{
	
}

void GnmCommandBufferDummy::setVsShader(const pssl::VsStageRegisters* vsRegs, uint32_t shaderModifier)
{
	
}

void GnmCommandBufferDummy::setEmbeddedVsShader(EmbeddedVsShader shaderId, uint32_t shaderModifier)
{
	
}

void GnmCommandBufferDummy::updateVsShader(const pssl::VsStageRegisters* vsRegs, uint32_t shaderModifier)
{
	
}

void GnmCommandBufferDummy::setVsharpInUserData(ShaderStage stage, uint32_t startUserDataSlot, const GnmBuffer* buffer)
{
	
}

void GnmCommandBufferDummy::setTsharpInUserData(ShaderStage stage, uint32_t startUserDataSlot, const GnmTexture* tex)
{
	
}

void GnmCommandBufferDummy::setSsharpInUserData(ShaderStage stage, uint32_t startUserDataSlot, const GnmSampler* sampler)
{
	
}

void GnmCommandBufferDummy::setPointerInUserData(ShaderStage stage, uint32_t startUserDataSlot, void* gpuAddr)
{
	
}

void GnmCommandBufferDummy::setUserDataRegion(ShaderStage stage, uint32_t startUserDataSlot, const uint32_t* userData, uint32_t numDwords)
{
	
}

void GnmCommandBufferDummy::setRenderTarget(uint32_t rtSlot, GnmRenderTarget const* target)
{
	
}

void GnmCommandBufferDummy::setDepthRenderTarget(GnmDepthRenderTarget const* depthTarget)
{
	
}

void GnmCommandBufferDummy::setDepthClearValue(float clearValue)
{
	
}

void GnmCommandBufferDummy::setStencilClearValue(uint8_t clearValue)
{
	
}

void GnmCommandBufferDummy::setRenderTargetMask(uint32_t mask)
{
	
}

void GnmCommandBufferDummy::setBlendControl(uint32_t rtSlot, BlendControl blendControl)
{
	
}

void GnmCommandBufferDummy::setDepthStencilControl(DepthStencilControl depthControl)
{
	
}

void GnmCommandBufferDummy::setDbRenderControl(DbRenderControl reg)
{
	
}

void GnmCommandBufferDummy::setVgtControl(uint8_t primGroupSizeMinusOne)
{
	
}

void GnmCommandBufferDummy::setPrimitiveType(PrimitiveType primType)
{
	
}

void GnmCommandBufferDummy::setIndexSize(IndexSize indexSize, CachePolicy cachePolicy)
{
	
}

void GnmCommandBufferDummy::drawIndexAuto(uint32_t indexCount, DrawModifier modifier)
{
	
}

void GnmCommandBufferDummy::drawIndexAuto(uint32_t indexCount)
{
	
}

void GnmCommandBufferDummy::drawIndex(uint32_t indexCount, const void* indexAddr, DrawModifier modifier)
{
	
}

void GnmCommandBufferDummy::drawIndex(uint32_t indexCount, const void* indexAddr)
{
	
}

void GnmCommandBufferDummy::dispatch(uint32_t threadGroupX, uint32_t threadGroupY, uint32_t threadGroupZ)
{
	
}

void GnmCommandBufferDummy::dispatchWithOrderedAppend(uint32_t threadGroupX, uint32_t threadGroupY, uint32_t threadGroupZ, DispatchOrderedAppendMode orderedAppendMode)
{
	
}

void GnmCommandBufferDummy::writeDataInline(void* dstGpuAddr, const void* data, uint32_t sizeInDwords, WriteDataConfirmMode writeConfirm)
{
	
}

void GnmCommandBufferDummy::writeDataInlineThroughL2(void* dstGpuAddr, const void* data, uint32_t sizeInDwords, CachePolicy cachePolicy, WriteDataConfirmMode writeConfirm)
{
	
}

void GnmCommandBufferDummy::writeAtEndOfPipe(EndOfPipeEventType eventType, EventWriteDest dstSelector, void* dstGpuAddr, EventWriteSource srcSelector, uint64_t immValue, CacheAction cacheAction, CachePolicy cachePolicy)
{
	emuWriteGpuLabel(srcSelector, dstGpuAddr, immValue);
}

void GnmCommandBufferDummy::writeAtEndOfPipeWithInterrupt(EndOfPipeEventType eventType, EventWriteDest dstSelector, void* dstGpuAddr, EventWriteSource srcSelector, uint64_t immValue, CacheAction cacheAction, CachePolicy cachePolicy)
{
	emuWriteGpuLabel(srcSelector, dstGpuAddr, immValue);
}

void GnmCommandBufferDummy::writeAtEndOfShader(EndOfShaderEventType eventType, void* dstGpuAddr, uint32_t immValue)
{
	
}

void GnmCommandBufferDummy::waitOnAddress(void* gpuAddr, uint32_t mask, WaitCompareFunc compareFunc, uint32_t refValue)
{
	
}

void GnmCommandBufferDummy::waitOnAddressAndStallCommandBufferParser(void* gpuAddr, uint32_t mask, uint32_t refValue)
{
	
}

void GnmCommandBufferDummy::waitForGraphicsWrites(uint32_t baseAddr256, uint32_t sizeIn256ByteBlocks, uint32_t targetMask, CacheAction cacheAction, uint32_t extendedCacheMask, StallCommandBufferParserMode commandBufferStallMode)
{
	
}

void GnmCommandBufferDummy::setDepthStencilDisable()
{
	
}

void GnmCommandBufferDummy::flushShaderCachesAndWait(CacheAction cacheAction, uint32_t extendedCacheMask, StallCommandBufferParserMode commandBufferStallMode)
{
	
}

void GnmCommandBufferDummy::waitUntilSafeForRendering(uint32_t videoOutHandle, uint32_t displayBufferIndex)
{
	
}

void GnmCommandBufferDummy::prepareFlip()
{
	
}

void GnmCommandBufferDummy::prepareFlip(void* labelAddr, uint32_t value)
{
	*(uint32_t*)labelAddr = value;
}

void GnmCommandBufferDummy::prepareFlipWithEopInterrupt(EndOfPipeEventType eventType, CacheAction cacheAction)
{
	
}

void GnmCommandBufferDummy::prepareFlipWithEopInterrupt(EndOfPipeEventType eventType, void* labelAddr, uint32_t value, CacheAction cacheAction)
{
	*(uint32_t*)labelAddr = value;
}

void GnmCommandBufferDummy::setCsShader(const pssl::CsStageRegisters* computeData, uint32_t shaderModifier)
{
	
}

void GnmCommandBufferDummy::writeReleaseMemEventWithInterrupt(ReleaseMemEventType eventType, EventWriteDest dstSelector, void* dstGpuAddr, EventWriteSource srcSelector, uint64_t immValue, CacheAction cacheAction, CachePolicy writePolicy)
{
	emuWriteGpuLabel(srcSelector, dstGpuAddr, immValue);
}

void GnmCommandBufferDummy::writeReleaseMemEvent(ReleaseMemEventType eventType, EventWriteDest dstSelector, void* dstGpuAddr, EventWriteSource srcSelector, uint64_t immValue, CacheAction cacheAction, CachePolicy writePolicy)
{
	emuWriteGpuLabel(srcSelector, dstGpuAddr, immValue);
}

void GnmCommandBufferDummy::setVgtControlForNeo(uint8_t primGroupSizeMinusOne, WdSwitchOnlyOnEopMode wdSwitchOnlyOnEopMode, VgtPartialVsWaveMode partialVsWaveMode)
{
	
}

