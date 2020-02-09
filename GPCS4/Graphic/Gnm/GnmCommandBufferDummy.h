#pragma once

#include "GnmCommon.h"
#include "GnmCommandBuffer.h"

// This class only emulate GPU write labels,
// preventing program from trapping into infinite loop waiting for labels been write.
// Useful when developing non-graphics parts.
// Define GPCS4_NO_GRAPHICS in GPCS4Config.h to enable this class.

class GnmCommandBufferDummy : public GnmCommandBuffer
{
public:
	GnmCommandBufferDummy();
	virtual ~GnmCommandBufferDummy();


	virtual void drawIndexAuto(uint32_t indexCount, DrawModifier modifier) override;


	virtual void drawIndexAuto(uint32_t indexCount) override;


	virtual void drawIndex(uint32_t indexCount, const void *indexAddr, DrawModifier modifier) override;


	virtual void drawIndex(uint32_t indexCount, const void *indexAddr) override;


	virtual void initializeDefaultHardwareState() override;

	
	virtual void prepareFlip() override;


	virtual void prepareFlip(void *labelAddr, uint32_t value) override;


	virtual void prepareFlipWithEopInterrupt(EndOfPipeEventType eventType, void *labelAddr, uint32_t value, CacheAction cacheAction) override;


	virtual void setActiveShaderStages(ActiveShaderStages activeStages) override;


	virtual void setBlendControl(uint32_t rtSlot, BlendControl blendControl) override;


	virtual void setDepthRenderTarget(DepthRenderTarget const *depthTarget) override;


	virtual void setDepthStencilControl(DepthStencilControl depthStencilControl) override;


	virtual void setEmbeddedVsShader(EmbeddedVsShader shaderId, uint32_t shaderModifier) override;


	virtual void setGuardBands(float horzClip, float vertClip, float horzDiscard, float vertDiscard) override;


	virtual void setHardwareScreenOffset(uint32_t offsetX, uint32_t offsetY) override;


	virtual void setIndexSize(IndexSize indexSize, CachePolicy cachePolicy) override;


	virtual void setPointerInUserData(ShaderStage stage, uint32_t startUserDataSlot, void *gpuAddr) override;


	virtual void setPrimitiveSetup(PrimitiveSetup primSetup) override;


	virtual void setPrimitiveType(PrimitiveType primType) override;


	virtual void setPsShaderUsage(const uint32_t *inputTable, uint32_t numItems) override;


	virtual void setPsShader(const pssl::PsStageRegisters *psRegs) override;


	virtual void setRenderTarget(uint32_t rtSlot, RenderTarget const *target) override;


	virtual void setRenderTargetMask(uint32_t mask) override;


	virtual void setScreenScissor(int32_t left, int32_t top, int32_t right, int32_t bottom) override;


	virtual void setSsharpInUserData(ShaderStage stage, uint32_t startUserDataSlot, const SSharpBuffer *sampler) override;


	virtual void setTsharpInUserData(ShaderStage stage, uint32_t startUserDataSlot, const TSharpBuffer *tex) override;


	virtual void setUserDataRegion(ShaderStage stage, uint32_t startUserDataSlot, const uint32_t *userData, uint32_t numDwords) override;


	virtual void setVgtControl(uint8_t primGroupSizeMinusOne, WdSwitchOnlyOnEopMode wdSwitchOnlyOnEopMode, VgtPartialVsWaveMode partialVsWaveMode) override;


	virtual void setViewport(uint32_t viewportId, float dmin, float dmax, const float scale[3], const float offset[3]) override;


	virtual void setViewportTransformControl(ViewportTransformControl vportControl) override;


	virtual void setVsharpInUserData(ShaderStage stage, uint32_t startUserDataSlot, const VSharpBuffer *buffer) override;


	virtual void setVsShader(const pssl::VsStageRegisters *vsRegs, uint32_t shaderModifier) override;


	virtual void updatePsShader(const pssl::PsStageRegisters *psRegs) override;


	virtual void updateVsShader(const pssl::VsStageRegisters *vsRegs, uint32_t shaderModifier) override;


	virtual void waitUntilSafeForRendering(uint32_t videoOutHandle, uint32_t displayBufferIndex) override;


	virtual void writeAtEndOfPipe(EndOfPipeEventType eventType, EventWriteDest dstSelector, void *dstGpuAddr, EventWriteSource srcSelector, uint64_t immValue, CacheAction cacheAction, CachePolicy cachePolicy) override;


	virtual void writeAtEndOfPipeWithInterrupt(EndOfPipeEventType eventType, EventWriteDest dstSelector, void *dstGpuAddr, EventWriteSource srcSelector, uint64_t immValue, CacheAction cacheAction, CachePolicy cachePolicy) override;


	virtual void dispatch(uint32_t threadGroupX, uint32_t threadGroupY, uint32_t threadGroupZ) override;

private:

};

