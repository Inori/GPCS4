#include "GnmCommandBufferDispatch.h"

#include "../Violet/VltDevice.h"
#include "../Violet/VltContext.h"

GnmCommandBufferDispatch::GnmCommandBufferDispatch(
	const sce::SceGpuQueueDevice& device,
	const RcPtr<vlt::VltContext>& context) :
	GnmCommandBuffer(device, context)
{
}

GnmCommandBufferDispatch::~GnmCommandBufferDispatch()
{
}

void GnmCommandBufferDispatch::initializeDefaultHardwareState()
{
	throw std::logic_error("The method or operation is not implemented.");
}

void GnmCommandBufferDispatch::setViewportTransformControl(ViewportTransformControl vportControl)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void GnmCommandBufferDispatch::setPrimitiveSetup(PrimitiveSetup reg)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void GnmCommandBufferDispatch::setScreenScissor(int32_t left, int32_t top, int32_t right, int32_t bottom)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void GnmCommandBufferDispatch::setViewport(uint32_t viewportId, float dmin, float dmax, const float scale[3], const float offset[3])
{
	throw std::logic_error("The method or operation is not implemented.");
}

void GnmCommandBufferDispatch::setHardwareScreenOffset(uint32_t offsetX, uint32_t offsetY)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void GnmCommandBufferDispatch::setGuardBands(float horzClip, float vertClip, float horzDiscard, float vertDiscard)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void GnmCommandBufferDispatch::setPsShaderUsage(const uint32_t* inputTable, uint32_t numItems)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void GnmCommandBufferDispatch::setActiveShaderStages(ActiveShaderStages activeStages)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void GnmCommandBufferDispatch::setPsShader(const pssl::PsStageRegisters* psRegs)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void GnmCommandBufferDispatch::updatePsShader(const pssl::PsStageRegisters* psRegs)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void GnmCommandBufferDispatch::setVsShader(const pssl::VsStageRegisters* vsRegs, uint32_t shaderModifier)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void GnmCommandBufferDispatch::setEmbeddedVsShader(EmbeddedVsShader shaderId, uint32_t shaderModifier)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void GnmCommandBufferDispatch::updateVsShader(const pssl::VsStageRegisters* vsRegs, uint32_t shaderModifier)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void GnmCommandBufferDispatch::setVsharpInUserData(ShaderStage stage, uint32_t startUserDataSlot, const GnmBuffer* buffer)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void GnmCommandBufferDispatch::setTsharpInUserData(ShaderStage stage, uint32_t startUserDataSlot, const GnmTexture* tex)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void GnmCommandBufferDispatch::setSsharpInUserData(ShaderStage stage, uint32_t startUserDataSlot, const GnmSampler* sampler)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void GnmCommandBufferDispatch::setPointerInUserData(ShaderStage stage, uint32_t startUserDataSlot, void* gpuAddr)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void GnmCommandBufferDispatch::setUserDataRegion(ShaderStage stage, uint32_t startUserDataSlot, const uint32_t* userData, uint32_t numDwords)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void GnmCommandBufferDispatch::setRenderTarget(uint32_t rtSlot, GnmRenderTarget const* target)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void GnmCommandBufferDispatch::setDepthRenderTarget(GnmDepthRenderTarget const* depthTarget)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void GnmCommandBufferDispatch::setDepthClearValue(float clearValue)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void GnmCommandBufferDispatch::setStencilClearValue(uint8_t clearValue)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void GnmCommandBufferDispatch::setRenderTargetMask(uint32_t mask)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void GnmCommandBufferDispatch::setBlendControl(uint32_t rtSlot, BlendControl blendControl)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void GnmCommandBufferDispatch::setDepthStencilControl(DepthStencilControl depthControl)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void GnmCommandBufferDispatch::setDbRenderControl(DbRenderControl reg)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void GnmCommandBufferDispatch::setVgtControl(uint8_t primGroupSizeMinusOne)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void GnmCommandBufferDispatch::setPrimitiveType(PrimitiveType primType)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void GnmCommandBufferDispatch::setIndexSize(IndexSize indexSize, CachePolicy cachePolicy)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void GnmCommandBufferDispatch::drawIndexAuto(uint32_t indexCount, DrawModifier modifier)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void GnmCommandBufferDispatch::drawIndexAuto(uint32_t indexCount)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void GnmCommandBufferDispatch::drawIndex(uint32_t indexCount, const void* indexAddr, DrawModifier modifier)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void GnmCommandBufferDispatch::drawIndex(uint32_t indexCount, const void* indexAddr)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void GnmCommandBufferDispatch::dispatch(uint32_t threadGroupX, uint32_t threadGroupY, uint32_t threadGroupZ)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void GnmCommandBufferDispatch::dispatchWithOrderedAppend(uint32_t threadGroupX, uint32_t threadGroupY, uint32_t threadGroupZ, DispatchOrderedAppendMode orderedAppendMode)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void GnmCommandBufferDispatch::writeDataInline(void* dstGpuAddr, const void* data, uint32_t sizeInDwords, WriteDataConfirmMode writeConfirm)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void GnmCommandBufferDispatch::writeDataInlineThroughL2(void* dstGpuAddr, const void* data, uint32_t sizeInDwords, CachePolicy cachePolicy, WriteDataConfirmMode writeConfirm)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void GnmCommandBufferDispatch::writeAtEndOfPipe(EndOfPipeEventType eventType, EventWriteDest dstSelector, void* dstGpuAddr, EventWriteSource srcSelector, uint64_t immValue, CacheAction cacheAction, CachePolicy cachePolicy)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void GnmCommandBufferDispatch::writeAtEndOfPipeWithInterrupt(EndOfPipeEventType eventType, EventWriteDest dstSelector, void* dstGpuAddr, EventWriteSource srcSelector, uint64_t immValue, CacheAction cacheAction, CachePolicy cachePolicy)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void GnmCommandBufferDispatch::writeAtEndOfShader(EndOfShaderEventType eventType, void* dstGpuAddr, uint32_t immValue)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void GnmCommandBufferDispatch::waitOnAddress(void* gpuAddr, uint32_t mask, WaitCompareFunc compareFunc, uint32_t refValue)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void GnmCommandBufferDispatch::waitOnAddressAndStallCommandBufferParser(void* gpuAddr, uint32_t mask, uint32_t refValue)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void GnmCommandBufferDispatch::waitForGraphicsWrites(uint32_t baseAddr256, uint32_t sizeIn256ByteBlocks, uint32_t targetMask, CacheAction cacheAction, uint32_t extendedCacheMask, StallCommandBufferParserMode commandBufferStallMode)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void GnmCommandBufferDispatch::setDepthStencilDisable()
{
	throw std::logic_error("The method or operation is not implemented.");
}

void GnmCommandBufferDispatch::flushShaderCachesAndWait(CacheAction cacheAction, uint32_t extendedCacheMask, StallCommandBufferParserMode commandBufferStallMode)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void GnmCommandBufferDispatch::waitUntilSafeForRendering(uint32_t videoOutHandle, uint32_t displayBufferIndex)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void GnmCommandBufferDispatch::prepareFlip()
{
	throw std::logic_error("The method or operation is not implemented.");
}

void GnmCommandBufferDispatch::prepareFlip(void* labelAddr, uint32_t value)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void GnmCommandBufferDispatch::prepareFlipWithEopInterrupt(EndOfPipeEventType eventType, CacheAction cacheAction)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void GnmCommandBufferDispatch::prepareFlipWithEopInterrupt(EndOfPipeEventType eventType, void* labelAddr, uint32_t value, CacheAction cacheAction)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void GnmCommandBufferDispatch::setCsShader(const pssl::CsStageRegisters* computeData, uint32_t shaderModifier)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void GnmCommandBufferDispatch::writeReleaseMemEventWithInterrupt(ReleaseMemEventType eventType, EventWriteDest dstSelector, void* dstGpuAddr, EventWriteSource srcSelector, uint64_t immValue, CacheAction cacheAction, CachePolicy writePolicy)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void GnmCommandBufferDispatch::writeReleaseMemEvent(ReleaseMemEventType eventType, EventWriteDest dstSelector, void* dstGpuAddr, EventWriteSource srcSelector, uint64_t immValue, CacheAction cacheAction, CachePolicy writePolicy)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void GnmCommandBufferDispatch::setVgtControlForNeo(uint8_t primGroupSizeMinusOne, WdSwitchOnlyOnEopMode wdSwitchOnlyOnEopMode, VgtPartialVsWaveMode partialVsWaveMode)
{
	throw std::logic_error("The method or operation is not implemented.");
}

