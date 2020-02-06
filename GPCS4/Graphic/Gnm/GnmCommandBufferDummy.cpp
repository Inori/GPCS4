#include "GnmCommandBufferDummy.h"

#include "../Gve/GveDevice.h"

GnmCommandBufferDummy::GnmCommandBufferDummy():
	GnmCommandBuffer(nullptr)
{
}

GnmCommandBufferDummy::~GnmCommandBufferDummy()
{
}

void GnmCommandBufferDummy::drawIndexAuto(uint32_t indexCount, DrawModifier modifier)
{
	
}

void GnmCommandBufferDummy::drawIndexAuto(uint32_t indexCount)
{
	
}

void GnmCommandBufferDummy::drawIndex(uint32_t indexCount, const void *indexAddr, DrawModifier modifier)
{
	
}

void GnmCommandBufferDummy::drawIndex(uint32_t indexCount, const void *indexAddr)
{
	
}

void GnmCommandBufferDummy::initializeDefaultHardwareState()
{
	
}

void GnmCommandBufferDummy::prepareFlip(void *labelAddr, uint32_t value)
{
	*(uint32_t*)labelAddr = value;
}

void GnmCommandBufferDummy::prepareFlip()
{
	
}

void GnmCommandBufferDummy::prepareFlipWithEopInterrupt(EndOfPipeEventType eventType, void *labelAddr, uint32_t value, CacheAction cacheAction)
{
	*(uint32_t*)labelAddr = value;
}

void GnmCommandBufferDummy::setActiveShaderStages(ActiveShaderStages activeStages)
{
	
}

void GnmCommandBufferDummy::setBlendControl(uint32_t rtSlot, BlendControl blendControl)
{
	
}

void GnmCommandBufferDummy::setDepthRenderTarget(DepthRenderTarget const *depthTarget)
{
	
}

void GnmCommandBufferDummy::setDepthStencilControl(DepthStencilControl depthStencilControl)
{
	
}

void GnmCommandBufferDummy::setEmbeddedVsShader(EmbeddedVsShader shaderId, uint32_t shaderModifier)
{
	
}

void GnmCommandBufferDummy::setGuardBands(float horzClip, float vertClip, float horzDiscard, float vertDiscard)
{
	
}

void GnmCommandBufferDummy::setHardwareScreenOffset(uint32_t offsetX, uint32_t offsetY)
{
	
}

void GnmCommandBufferDummy::setIndexSize(IndexSize indexSize, CachePolicy cachePolicy)
{
	
}

void GnmCommandBufferDummy::setPointerInUserData(ShaderStage stage, uint32_t startUserDataSlot, void *gpuAddr)
{
	
}

void GnmCommandBufferDummy::setPrimitiveSetup(PrimitiveSetup primSetup)
{
	
}

void GnmCommandBufferDummy::setPrimitiveType(PrimitiveType primType)
{
	
}

void GnmCommandBufferDummy::setPsShaderUsage(const uint32_t *inputTable, uint32_t numItems)
{
	
}

void GnmCommandBufferDummy::setPsShader(const pssl::PsStageRegisters *psRegs)
{
	
}

void GnmCommandBufferDummy::setRenderTarget(uint32_t rtSlot, RenderTarget const *target)
{
	
}

void GnmCommandBufferDummy::setRenderTargetMask(uint32_t mask)
{
	
}

void GnmCommandBufferDummy::setScreenScissor(int32_t left, int32_t top, int32_t right, int32_t bottom)
{
	
}

void GnmCommandBufferDummy::setSsharpInUserData(ShaderStage stage, uint32_t startUserDataSlot, const SSharpBuffer *sampler)
{
	
}

void GnmCommandBufferDummy::setTsharpInUserData(ShaderStage stage, uint32_t startUserDataSlot, const TSharpBuffer *tex)
{
	
}

void GnmCommandBufferDummy::setUserDataRegion(ShaderStage stage, uint32_t startUserDataSlot, const uint32_t *userData, uint32_t numDwords)
{
	
}

void GnmCommandBufferDummy::setVgtControl(uint8_t primGroupSizeMinusOne, WdSwitchOnlyOnEopMode wdSwitchOnlyOnEopMode, VgtPartialVsWaveMode partialVsWaveMode)
{
	
}

void GnmCommandBufferDummy::setViewport(uint32_t viewportId, float dmin, float dmax, const float scale[3], const float offset[3])
{
	
}

void GnmCommandBufferDummy::setViewportTransformControl(ViewportTransformControl vportControl)
{
	
}

void GnmCommandBufferDummy::setVsharpInUserData(ShaderStage stage, uint32_t startUserDataSlot, const VSharpBuffer *buffer)
{
	
}

void GnmCommandBufferDummy::setVsShader(const pssl::VsStageRegisters *vsRegs, uint32_t shaderModifier)
{
	
}

void GnmCommandBufferDummy::updatePsShader(const pssl::PsStageRegisters *psRegs)
{
	
}

void GnmCommandBufferDummy::updateVsShader(const pssl::VsStageRegisters *vsRegs, uint32_t shaderModifier)
{
	
}

void GnmCommandBufferDummy::waitUntilSafeForRendering(uint32_t videoOutHandle, uint32_t displayBufferIndex)
{
	
}

void GnmCommandBufferDummy::writeAtEndOfPipe(EndOfPipeEventType eventType, EventWriteDest dstSelector, void *dstGpuAddr, EventWriteSource srcSelector, uint64_t immValue, CacheAction cacheAction, CachePolicy cachePolicy)
{
	emuWriteGpuLabel(srcSelector, dstGpuAddr, immValue);
}

void GnmCommandBufferDummy::writeAtEndOfPipeWithInterrupt(EndOfPipeEventType eventType, EventWriteDest dstSelector, void *dstGpuAddr, EventWriteSource srcSelector, uint64_t immValue, CacheAction cacheAction, CachePolicy cachePolicy)
{
	emuWriteGpuLabel(srcSelector, dstGpuAddr, immValue);
}

void GnmCommandBufferDummy::dispatch(uint32_t threadGroupX, uint32_t threadGroupY, uint32_t threadGroupZ)
{
	
}
