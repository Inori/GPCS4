#pragma once

#include "GnmCommandBuffer.h"


class GnmCommandBufferDispatch : public GnmCommandBuffer
{
public:
	GnmCommandBufferDispatch(
		const sce::SceGpuQueueDevice& device,
		const RcPtr<vlt::VltContext>& context
	);

	virtual ~GnmCommandBufferDispatch();

	virtual void initializeDefaultHardwareState() override;

	virtual void setViewportTransformControl(ViewportTransformControl vportControl) override;

	virtual void setPrimitiveSetup(PrimitiveSetup reg) override;

	virtual void setScreenScissor(int32_t left, int32_t top, int32_t right, int32_t bottom) override;

	virtual void setViewport(uint32_t viewportId, float dmin, float dmax, const float scale[3], const float offset[3]) override;

	virtual void setHardwareScreenOffset(uint32_t offsetX, uint32_t offsetY) override;

	virtual void setGuardBands(float horzClip, float vertClip, float horzDiscard, float vertDiscard) override;

	virtual void setPsShaderUsage(const uint32_t* inputTable, uint32_t numItems) override;

	virtual void setActiveShaderStages(ActiveShaderStages activeStages) override;

	virtual void setPsShader(const pssl::PsStageRegisters* psRegs) override;

	virtual void updatePsShader(const pssl::PsStageRegisters* psRegs) override;

	virtual void setVsShader(const pssl::VsStageRegisters* vsRegs, uint32_t shaderModifier) override;

	virtual void setEmbeddedVsShader(EmbeddedVsShader shaderId, uint32_t shaderModifier) override;

	virtual void updateVsShader(const pssl::VsStageRegisters* vsRegs, uint32_t shaderModifier) override;

	virtual void setVsharpInUserData(ShaderStage stage, uint32_t startUserDataSlot, const GnmBuffer* buffer) override;

	virtual void setTsharpInUserData(ShaderStage stage, uint32_t startUserDataSlot, const GnmTexture* tex) override;

	virtual void setSsharpInUserData(ShaderStage stage, uint32_t startUserDataSlot, const GnmSampler* sampler) override;

	virtual void setPointerInUserData(ShaderStage stage, uint32_t startUserDataSlot, void* gpuAddr) override;

	virtual void setUserDataRegion(ShaderStage stage, uint32_t startUserDataSlot, const uint32_t* userData, uint32_t numDwords) override;

	virtual void setRenderTarget(uint32_t rtSlot, GnmRenderTarget const* target) override;

	virtual void setDepthRenderTarget(GnmDepthRenderTarget const* depthTarget) override;

	virtual void setDepthClearValue(float clearValue) override;

	virtual void setStencilClearValue(uint8_t clearValue) override;

	virtual void setRenderTargetMask(uint32_t mask) override;

	virtual void setBlendControl(uint32_t rtSlot, BlendControl blendControl) override;

	virtual void setDepthStencilControl(DepthStencilControl depthControl) override;

	virtual void setDbRenderControl(DbRenderControl reg) override;

	virtual void setVgtControl(uint8_t primGroupSizeMinusOne) override;

	virtual void setPrimitiveType(PrimitiveType primType) override;

	virtual void setIndexSize(IndexSize indexSize, CachePolicy cachePolicy) override;

	virtual void drawIndexAuto(uint32_t indexCount, DrawModifier modifier) override;

	virtual void drawIndexAuto(uint32_t indexCount) override;

	virtual void drawIndex(uint32_t indexCount, const void* indexAddr, DrawModifier modifier) override;

	virtual void drawIndex(uint32_t indexCount, const void* indexAddr) override;

	virtual void dispatch(uint32_t threadGroupX, uint32_t threadGroupY, uint32_t threadGroupZ) override;

	virtual void dispatchWithOrderedAppend(uint32_t threadGroupX, uint32_t threadGroupY, uint32_t threadGroupZ, DispatchOrderedAppendMode orderedAppendMode) override;

	virtual void writeDataInline(void* dstGpuAddr, const void* data, uint32_t sizeInDwords, WriteDataConfirmMode writeConfirm) override;

	virtual void writeDataInlineThroughL2(void* dstGpuAddr, const void* data, uint32_t sizeInDwords, CachePolicy cachePolicy, WriteDataConfirmMode writeConfirm) override;

	virtual void writeAtEndOfPipe(EndOfPipeEventType eventType, EventWriteDest dstSelector, void* dstGpuAddr, EventWriteSource srcSelector, uint64_t immValue, CacheAction cacheAction, CachePolicy cachePolicy) override;

	virtual void writeAtEndOfPipeWithInterrupt(EndOfPipeEventType eventType, EventWriteDest dstSelector, void* dstGpuAddr, EventWriteSource srcSelector, uint64_t immValue, CacheAction cacheAction, CachePolicy cachePolicy) override;

	virtual void writeAtEndOfShader(EndOfShaderEventType eventType, void* dstGpuAddr, uint32_t immValue) override;

	virtual void waitOnAddress(void* gpuAddr, uint32_t mask, WaitCompareFunc compareFunc, uint32_t refValue) override;

	virtual void waitOnAddressAndStallCommandBufferParser(void* gpuAddr, uint32_t mask, uint32_t refValue) override;

	virtual void flushShaderCachesAndWait(CacheAction cacheAction, uint32_t extendedCacheMask, StallCommandBufferParserMode commandBufferStallMode) override;

	virtual void waitUntilSafeForRendering(uint32_t videoOutHandle, uint32_t displayBufferIndex) override;

	virtual void prepareFlip() override;

	virtual void prepareFlip(void* labelAddr, uint32_t value) override;

	virtual void prepareFlipWithEopInterrupt(EndOfPipeEventType eventType, CacheAction cacheAction) override;

	virtual void prepareFlipWithEopInterrupt(EndOfPipeEventType eventType, void* labelAddr, uint32_t value, CacheAction cacheAction) override;

	virtual void setCsShader(const pssl::CsStageRegisters* computeData, uint32_t shaderModifier) override;

	virtual void writeReleaseMemEventWithInterrupt(ReleaseMemEventType eventType, EventWriteDest dstSelector, void* dstGpuAddr, EventWriteSource srcSelector, uint64_t immValue, CacheAction cacheAction, CachePolicy writePolicy) override;

	virtual void writeReleaseMemEvent(ReleaseMemEventType eventType, EventWriteDest dstSelector, void* dstGpuAddr, EventWriteSource srcSelector, uint64_t immValue, CacheAction cacheAction, CachePolicy writePolicy) override;

	virtual void setVgtControlForNeo(uint8_t primGroupSizeMinusOne, WdSwitchOnlyOnEopMode wdSwitchOnlyOnEopMode, VgtPartialVsWaveMode partialVsWaveMode) override;

	virtual void waitForGraphicsWrites(uint32_t baseAddr256, uint32_t sizeIn256ByteBlocks, uint32_t targetMask, CacheAction cacheAction, uint32_t extendedCacheMask, StallCommandBufferParserMode commandBufferStallMode) override;

	virtual void setDepthStencilDisable() override;

private:
	
};


