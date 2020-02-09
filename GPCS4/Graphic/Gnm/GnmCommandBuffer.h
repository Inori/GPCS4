#pragma once

#include "GnmCommon.h"
#include "GnmConstant.h"
#include "GnmStructure.h"
#include "GnmSharpBuffer.h"
#include "GnmRenderTarget.h"
#include "GnmDepthRenderTarget.h"


namespace gve
{;
class GveDevice;
class GveCmdList;
class GveContext;
}  // namespace gve

class GnmCommandBuffer
{
public:
	GnmCommandBuffer(const RcPtr<gve::GveDevice>& device);
	virtual ~GnmCommandBuffer();

	RcPtr<gve::GveCmdList> getCmdList();
	// Implement these one by one...

	// Note:
	// When implementing one of these virtual method,
	// Uncomment the method here,
	// then add the override(implementation) version
	// to either GnmCommandBufferDraw or GnmCommandBufferDispatch
	// and fill in param structures or constant in GnmContant.h or GnmStructure.h or other source files
	// at the same time.

	//virtual void *allocateFromCommandBuffer(uint32_t sizeInBytes, EmbeddedDataAlignment alignment) = 0;
	//virtual void chainCommandBufferAndResume(uint64_t holeAddr, void *nextIbBaseAddr, uint64_t nextIbSizeInDW) = 0;
	//virtual void chainCommandBuffer(void *cbBaseAddr, uint64_t cbSizeInDW) = 0;
	//virtual void disableGsMode() = 0;
	//virtual void disableOrderedAppendAllocationCounter(uint32_t oaCounterIndex) = 0;
	//virtual void dispatchDraw(PrimitiveType primType, uint32_t indexOffset, uint32_t primGroupIndexCount, uint32_t primGroupThreshold, uint32_t pollIntervalThreshold, DispatchDrawMode dispatchDrawMode, uint32_t sgprVrbLoc, DrawModifier modifier) = 0;
	virtual void dispatch(uint32_t threadGroupX, uint32_t threadGroupY, uint32_t threadGroupZ) = 0;
	//virtual void dispatchWithOrderedAppend(uint32_t threadGroupX, uint32_t threadGroupY, uint32_t threadGroupZ, DispatchOrderedAppendMode orderedAppendMode) = 0;
	//virtual void dmaData(DmaDataDst	dstSel, uint64_t dst, DmaDataSrc srcSel, uint64_t srcOrData, uint32_t numBytes, DmaDataBlockingMode isBlocking) = 0;
	virtual void drawIndexAuto(uint32_t indexCount, DrawModifier modifier) = 0;
	virtual void drawIndexAuto(uint32_t indexCount) = 0;
	//virtual void drawIndexInline(uint32_t indexCount, const void *indices, uint32_t indicesSizeInBytes, DrawModifier modifier) = 0;
	//virtual void drawIndexInline(uint32_t indexCount, const void *indices, uint32_t indicesSizeInBytes) = 0;
	virtual void drawIndex(uint32_t indexCount, const void *indexAddr, DrawModifier modifier) = 0;
	virtual void drawIndex(uint32_t indexCount, const void *indexAddr) = 0;
	//virtual void drawIndexMultiInstanced(uint32_t indexCount, uint32_t instanceCount, const void *indexAddr, const void *objectIdAddr, DrawModifier modifier) = 0;
	//virtual void drawIndexMultiInstanced(uint32_t indexCount, uint32_t instanceCount, const void *indexAddr, const void *objectIdAddr) = 0;
	//virtual void drawIndexOffset(uint32_t indexOffset, uint32_t indexCount, DrawModifier modifier) = 0;
	//virtual void drawIndexOffset(uint32_t indexOffset, uint32_t indexCount) = 0;
	//virtual void drawIndexIndirect(uint32_t dataOffsetInBytes, ShaderStage stage, uint8_t vertexOffsetUserSgpr, uint8_t instanceOffsetUserSgpr, DrawModifier modifier) = 0;
	//virtual void drawIndexIndirect(uint32_t dataOffsetInBytes, ShaderStage stage, uint8_t vertexOffsetUserSgpr, uint8_t instanceOffsetUserSgpr) = 0;
	//virtual void drawIndexIndirectCountMulti(uint32_t dataOffsetInBytes, uint32_t drawCount, void *countAddress, ShaderStage stage, uint8_t vertexOffsetUserSgpr, uint8_t instanceOffsetUserSgpr, DrawModifier modifier) = 0;
	//virtual void drawIndexIndirectCountMulti(uint32_t dataOffsetInBytes, uint32_t drawCount, void* countAddress, ShaderStage stage, uint8_t vertexOffsetUserSgpr, uint8_t instanceOffsetUserSgpr) = 0;
	//virtual void drawIndirect(uint32_t dataOffsetInBytes, ShaderStage stage, uint8_t vertexOffsetUserSgpr, uint8_t instanceOffsetUserSgpr, DrawModifier modifier) = 0;
	//virtual void drawIndirect(uint32_t dataOffsetInBytes, ShaderStage stage, uint8_t vertexOffsetUserSgpr, uint8_t instanceOffsetUserSgpr) = 0;
	//virtual void drawIndirectCountMulti(uint32_t dataOffsetInBytes, uint32_t count, void *countAddress, ShaderStage stage, uint8_t vertexOffsetUserSgpr, uint8_t instanceOffsetUserSgpr, DrawModifier modifier) = 0;
	//virtual void drawIndirectCountMulti(uint32_t dataOffsetInBytes, uint32_t count, void *countAddress, ShaderStage stage, uint8_t vertexOffsetUserSgpr, uint8_t instanceOffsetUserSgpr) = 0;
	//virtual void drawOpaqueAuto(DrawModifier modifier) = 0;
	//virtual void drawOpaqueAuto() = 0;
	//virtual void enableGsMode(GsMaxOutputPrimitiveDwordSize maxPrimDwordSize) = 0;
	//virtual void enableGsModeOnChip(GsMaxOutputPrimitiveDwordSize maxPrimDwordSize,
	//	uint32_t esVerticesPerSubGroup, uint32_t gsInputPrimitivesPerSubGroup) = 0;
	//virtual void enableOrderedAppendAllocationCounter(uint32_t oaCounterIndex, uint32_t gdsDwOffsetOfCounter, ShaderStage stage, uint32_t oaOpIndex, uint32_t spaceInAllocationUnits) = 0;
	//virtual void fillAndResume(uint64_t holeAddr, void *commandStream, uint32_t sizeInDW) = 0;
	//virtual void flushShaderCachesAndWait(CacheAction cacheAction, uint32_t extendedCacheMask, StallCommandBufferParserMode commandBufferStallMode) = 0;
	//virtual void flushStreamout() = 0;
	//virtual void incrementDeCounter() = 0;
	virtual void initializeDefaultHardwareState() = 0;
	//virtual void initializeToDefaultContextState() = 0;
	//virtual void insertNop(uint32_t numDwords) = 0;
	//virtual void markDispatchDrawAcbAddress(uint32_t const* addrAcb, uint32_t const* addrAcbBegin) = 0;
	//virtual uint64_t pause(uint32_t reservedDWs) = 0;
	//virtual void popMarker() = 0;
	//virtual void prefetchIntoL2(void *dataAddr, uint32_t sizeInBytes) = 0;
	virtual void prepareFlip() = 0;
	virtual void prepareFlip(void *labelAddr, uint32_t value) = 0;
	//virtual void prepareFlipWithEopInterrupt(EndOfPipeEventType eventType, CacheAction cacheAction) = 0;
	virtual void prepareFlipWithEopInterrupt(EndOfPipeEventType eventType, void *labelAddr, uint32_t value ,CacheAction cacheAction) = 0;
	//virtual void pushMarker(const char *debugString) = 0;
	//virtual void pushMarker(const char *debugString, uint32_t argbColor) = 0;
	//virtual void readCpcPerfCounter(uint32_t counterSlot, void *pgpuOutput) = 0;
	//virtual void readCpcPerfCounters(uint32_t counterSlotStart, uint32_t numCounterSlots, void *pgpuOutputStart) = 0;
	//virtual void readCpfPerfCounter(uint32_t counterSlot, void *pgpuOutput) = 0;
	//virtual void readCpfPerfCounters(uint32_t counterSlotStart, uint32_t numCounterSlots, void *pgpuOutputStart) = 0;
	//virtual void readCpPerfCounter(uint32_t counterSlot, void *pgpuOutput) = 0;
	//virtual void readCpPerfCounters(uint32_t counterSlotStart, uint32_t numCounterSlots, void *pgpuOutputStart) = 0;
	//virtual void readDataFromGds(EndOfShaderEventType eventType, void *dstGpuAddr, uint32_t gdsOffsetInDwords, uint32_t gdsSizeInDwords) = 0;
	//virtual void readGdsPerfCounter(uint32_t counterSlot, void *pgpuOutput) = 0;
	//virtual void readGdsPerfCounters(uint32_t counterSlotStart, uint32_t numCounterSlots, void *pgpuOutputStart) = 0;
	//virtual void readTcsPerfCounter(uint32_t counterSlot, void *pgpuOutput) = 0;
	//virtual void readTcsPerfCounters(uint32_t counterSlotStart, uint32_t numCounterSlots, void *pgpuOutputStart) = 0;
	//virtual void readWdPerfCounter(uint32_t counterSlot, void *pgpuOutput) = 0;
	//virtual void readWdPerfCounters(uint32_t counterSlotStart, uint32_t numCounterSlots, void *pgpuOutputStart) = 0;
	//virtual void requestMipStatsReportAndReset(void *outputBuffer, uint32_t sizeInByte) = 0;
	//virtual void resetFoveatedWindow() = 0;
	//virtual void resetVgtControl() = 0;
	//static void resume(uint64_t holeAddr);
	//virtual void selectCpcPerfCounter(uint32_t counterSlot, CpcPerfCounter counterSelect) = 0;
	//virtual void selectCpcPerfCounters(uint32_t counterSlotStart, uint32_t numCounterSlots, CpcPerfCounter const* aCounterSelect) = 0;
	//virtual void selectCpfPerfCounter(uint32_t counterSlot, CpfPerfCounter counterSelect) = 0;
	//virtual void selectCpfPerfCounters(uint32_t counterSlotStart, uint32_t numCounterSlots, CpfPerfCounter const* aCounterSelect) = 0;
	//virtual void selectCpPerfCounter(uint32_t counterSlot, CpPerfCounter counterSelect) = 0;
	//virtual void selectCpPerfCounters(uint32_t counterSlotStart, uint32_t numCounterSlots, CpPerfCounter const* aCounterSelect) = 0;
	//virtual void selectGdsPerfCounter(uint32_t counterSlot, GdsPerfCounter counterSelect) = 0;
	//virtual void selectGdsPerfCounters(uint32_t counterSlotStart, uint32_t numCounterSlots, GdsPerfCounter const* aCounterSelect) = 0;
	//virtual void selectIaPerfCounter(uint32_t iaIndex, uint32_t counterSlot, IaPerfCounterSelect counterSelect) = 0;
	//virtual void selectIaPerfCounters(uint32_t iaIndex, uint32_t counterSlotStart, uint32_t numCounterSlots, IaPerfCounterSelect const* aCounterSelect) = 0;
	//virtual void selectIaPerfCounter(ShaderEngineBroadcast broadcast, uint32_t counterSlot, IaPerfCounterSelect counterSelect) = 0;
	//virtual void selectIaPerfCounters(ShaderEngineBroadcast broadcast, uint32_t counterSlotStart, uint32_t numCounterSlots, IaPerfCounterSelect const* aCounterSelect) = 0;
	//virtual void selectTcaPerfCounter(uint32_t tcaIndex, uint32_t counterSlot, TcaPerfCounterSelect counterSelect) = 0;
	//virtual void selectTcaPerfCounters(uint32_t tcaIndex, uint32_t counterSlotStart, uint32_t numCounterSlots, TcaPerfCounterSelect const* aCounterSelect) = 0;
	//virtual void selectTcaPerfCounter(ShaderEngineBroadcast broadcast, uint32_t counterSlot, TcaPerfCounterSelect counterSelect) = 0;
	//virtual void selectTcaPerfCounters(ShaderEngineBroadcast broadcast, uint32_t counterSlotStart, uint32_t numCounterSlots, TcaPerfCounterSelect const* aCounterSelect) = 0;
	//virtual void selectTccPerfCounter(uint32_t tccIndex, uint32_t counterSlot, TccPerfCounterSelect counterSelect) = 0;
	//virtual void selectTccPerfCounters(uint32_t tccIndex, uint32_t counterSlotStart, uint32_t numCounterSlots, TccPerfCounterSelect const* aCounterSelect) = 0;
	//virtual void selectTccPerfCounter(ShaderEngineBroadcast broadcast, uint32_t counterSlot, TccPerfCounterSelect counterSelect) = 0;
	//virtual void selectTccPerfCounters(ShaderEngineBroadcast broadcast, uint32_t counterSlotStart, uint32_t numCounterSlots, TccPerfCounterSelect const* aCounterSelect) = 0;
	//virtual void selectTcsPerfCounter(uint32_t counterSlot, TcsPerfCounterSelect counterSelect) = 0;
	//virtual void selectTcsPerfCounters(uint32_t counterSlotStart, uint32_t numCounterSlots, TcsPerfCounterSelect const* aCounterSelect) = 0;
	//virtual void selectWdPerfCounter(uint32_t counterSlot, WdPerfCounterSelect counterSelect) = 0;
	//virtual void selectWdPerfCounters(uint32_t counterSlotStart, uint32_t numCounterSlots, WdPerfCounterSelect const* aCounterSelect) = 0;
	//virtual void setAaSampleCount(NumSamples logNumSamples, uint32_t maxSampleDistance) = 0;
	//virtual void setAaSampleLocationControl(const AaSampleLocationControl *control) = 0;
	//virtual void setAaSampleMask(uint64_t mask) = 0;
	virtual void setActiveShaderStages(ActiveShaderStages activeStages) = 0;
	//virtual void setAlphaToMaskControl(AlphaToMaskControl alphaToMaskControl) = 0;
	//virtual void setBaseIndirectArgs(ShaderType shaderType, void *indirectBaseAddr) = 0;
	//virtual void setBlendColor(float red, float green, float blue, float alpha) = 0;
	virtual void setBlendControl(uint32_t rtSlot, BlendControl blendControl) = 0;
	//virtual void setBorderColorTableAddr(void *tableAddr) = 0;
	//virtual void setCbControl(CbMode mode, RasterOp op) = 0;
	//virtual void setClipControl(ClipControl reg) = 0;
	//virtual void setClipRectangle(uint32_t rectId, uint32_t left, uint32_t top, uint32_t right, uint32_t bottom) = 0;
	//virtual void setClipRectangleRule(uint16_t clipRule) = 0;
	//virtual void setCmaskClearColor(uint32_t rtSlot, const uint32_t clearColor[2]) = 0;
	//virtual void setComputeResourceManagement(ShaderEngine engine, uint16_t mask) = 0;
	//virtual void setComputeScratchSize(uint32_t maxNumWaves, uint32_t num1KByteChunksPerWave) = 0;
	//virtual void setComputeShaderControl(uint32_t wavesPerSh, uint32_t threadgroupsPerCu, uint32_t lockThreshold) = 0;
	//virtual void setCsShader(const CsStageRegisters *csRegs, uint32_t shaderModifier) = 0;
	//virtual void setDbCountControl(DbCountControlPerfectZPassCounts perfectZPassCounts, uint32_t log2SampleRate) = 0;
	//virtual void setDbRenderControl(DbRenderControl reg) = 0;
	//virtual void setDepthBoundsRange(float depthBoundsMin, float depthBoundsMax) = 0;
	//virtual void setDepthClearValue(float clearValue) = 0;
	//virtual void setDepthEqaaControl(DepthEqaaControl depthEqaa) = 0;
	virtual void setDepthRenderTarget(DepthRenderTarget const *depthTarget) = 0;
	virtual void setDepthStencilControl(DepthStencilControl depthStencilControl) = 0;
	//virtual void setDepthStencilDisable() = 0;
	//virtual void setDispatchDrawIndexDeallocationMask(uint32_t indexMask) = 0;
	//virtual void setDrawPayloadControl(DrawPayloadControl cntrl) = 0;
	//virtual void setEmbeddedPsShader(EmbeddedPsShader shaderId) = 0;
	virtual void setEmbeddedVsShader(EmbeddedVsShader shaderId, uint32_t shaderModifier) = 0;
	//virtual void setEsShader(const EsStageRegisters *esRegs, uint32_t shaderModifier) = 0;
	//virtual void setFoveatedWindow(uint8_t xMinLeftEye, uint8_t xMaxLeftEye, uint8_t xMinRightEye, uint8_t xMaxRightEye, uint8_t yMinBothEyes, uint8_t yMaxBothEyes) = 0;
	//virtual void setGenericScissor(uint32_t left, uint32_t top, uint32_t right, uint32_t bottom, WindowOffsetMode windowOffsetEnable) = 0;
	//virtual void setGraphicsScratchSize(uint32_t maxNumWaves, uint32_t num1KByteChunksPerWave) = 0;
	//virtual void setGraphicsShaderControl(GraphicsShaderControl control) = 0;
	//virtual void setGsMode(GsMode mode, GsMaxOutputPrimitiveDwordSize maxPrimDwordSize) = 0;
	//virtual void setGsOnChipControl(uint32_t esVerticesPerSubGroup, uint32_t gsInputPrimitivesPerSubGroup) = 0;
	//virtual void setGsShader(const GsStageRegisters *gsRegs) = 0;
	virtual void setGuardBands(float horzClip, float vertClip, float horzDiscard, float vertDiscard) = 0;
	virtual void setHardwareScreenOffset(uint32_t offsetX, uint32_t offsetY) = 0;
	//virtual void setHsShader(const HsStageRegisters *hsRegs, const TessellationRegisters *tessRegs) = 0;
	//virtual void setHsShader(const HsStageRegisters *hsRegs, const TessellationRegisters *tessRegs, TessellationDistributionMode distributionMode) = 0;
	//virtual void setHtileStencil0(HtileStencilControl htileStencilControl) = 0;
	//virtual void setHtileStencil1(HtileStencilControl htileStencilControl) = 0;
	//virtual void setIndexBuffer(const void *indexAddr) = 0;
	//virtual void setIndexCount(uint32_t indexCount) = 0;
	//virtual void setIndexOffset(uint32_t offset) = 0;
	virtual void setIndexSize(IndexSize indexSize, CachePolicy cachePolicy) = 0;
	//virtual void setInstanceStepRate(uint32_t step0, uint32_t step1) = 0;
	//virtual void setLineWidth(uint16_t widthIn8ths) = 0;
	//virtual void setLsShader(const LsStageRegisters *lsRegs, uint32_t shaderModifier) = 0;
	//virtual void setMarker(const char *debugString) = 0;
	//virtual void setMarker(const char *debugString, uint32_t argbColor) = 0;
	//virtual void setNumInstances(uint32_t numInstances) = 0;
	//virtual void setObjectIdMode(ObjectIdSource objIdSource, AddPrimitiveId addPrimitiveId) = 0;
	//virtual void setObjectId(uint32_t id) = 0;
	//virtual void setPerfCounterControl(PerfCounterControl control) = 0;
	//virtual void setPerfmonEnable(PerfmonEnable enable) = 0;
	virtual void setPointerInUserData(ShaderStage stage, uint32_t startUserDataSlot, void *gpuAddr) = 0;
	//virtual void setPointMinMax(uint16_t minRadius, uint16_t maxRadius) = 0;
	//virtual void setPointSize(uint16_t halfWidth, uint16_t halfHeight) = 0;
	//virtual void setPolygonOffsetBack(float scale, float offset) = 0;
	//virtual void setPolygonOffsetClamp(float clamp) = 0;
	//virtual void setPolygonOffsetFront(float scale, float offset) = 0;
	//virtual void setPolygonOffsetZFormat(ZFormat format) = 0;
	//virtual void setPredication(void *condAddr, uint32_t predCountInDwords) = 0;
	//virtual void setPrimitiveIdEnable(bool enable) = 0;
	//virtual void setPrimitiveResetIndexEnable(bool enable) = 0;
	//virtual void setPrimitiveResetIndex(uint32_t resetIndex) = 0;
	virtual void setPrimitiveSetup(PrimitiveSetup primSetup) = 0;
	virtual void setPrimitiveType(PrimitiveType primType) = 0;
	//virtual void setPsShaderRate(PsShaderRate rate) = 0;
	//virtual void setPsShaderSampleExclusionMask(uint16_t mask) = 0;
	virtual void setPsShaderUsage(const uint32_t *inputTable, uint32_t numItems) = 0;
	virtual void setPsShader(const pssl::PsStageRegisters *psRegs) = 0;
	//virtual void setRenderOverride2Control(RenderOverride2Control renderOverride2Control) = 0;
	//virtual void setRenderOverrideControl(RenderOverrideControl renderOverrideControl) = 0;
	virtual void setRenderTarget(uint32_t rtSlot, RenderTarget const *target) = 0;
	virtual void setRenderTargetMask(uint32_t mask) = 0;
	//virtual void setScaledResolutionGrid(const ScaledResolutionGridAxis xAxisLeftEye, const ScaledResolutionGridAxis xAxisRightEye, const ScaledResolutionGridAxis yAxisBothEyes) = 0;
	//virtual void setScanModeControl(ScanModeControlAa msaa, ScanModeControlViewportScissor viewportScissor) = 0;
	virtual void setScreenScissor(int32_t left, int32_t top, int32_t right, int32_t bottom) = 0;
	virtual void setSsharpInUserData(ShaderStage stage, uint32_t startUserDataSlot, const SSharpBuffer *sampler) = 0;
	//virtual void setStencilClearValue(uint8_t clearValue) = 0;
	//virtual void setStencil(StencilControl stencilControl) = 0;
	//virtual void setStencilSeparate(StencilControl front, StencilControl back) = 0;
	//virtual void setStencilOpControl(StencilOpControl stencilControl) = 0;
	//virtual void setStreamoutBufferDimensions(StreamoutBufferId bufferId, uint32_t bufferSizeInDW, uint32_t bufferStrideInDW) = 0;
	//virtual void setStreamoutMapping(const StreamoutBufferMapping* mapping) = 0;
	//virtual void setTessellationDistributionThresholds(TessellationDistributionThresholds thresholds) = 0;
	//virtual void setTextureGradientFactors(uint8_t factor00, uint8_t factor01, uint8_t factor10, uint8_t factor11,
	//	TextureGradientFactor01SignNegationBehavior factor01sb, TextureGradientFactor10SignNegationBehavior factor10sb) = 0;
	virtual void setTsharpInUserData(ShaderStage stage, uint32_t startUserDataSlot, const TSharpBuffer *tex) = 0;
	//virtual void setupDrawOpaqueParameters(void *sizeLocation, uint32_t stride, uint32_t offset) = 0;
	//virtual void setupEsGsRingRegisters(uint32_t maxExportVertexSizeInDword) = 0;
	//virtual void setupGsVsRingRegisters(const uint32_t vertexSizePerStreamInDword[4], uint32_t maxOutputVertexCount) = 0;
	//virtual void setUserClipPlane(uint32_t clipPlane, float x, float y, float z, float w) = 0;
	//virtual void setUserData(ShaderStage stage, uint32_t userDataSlot, uint32_t data) = 0;
	virtual void setUserDataRegion(ShaderStage stage, uint32_t startUserDataSlot, const uint32_t *userData, uint32_t numDwords) = 0;
	//virtual void setVertexQuantization(VertexQuantizationMode quantizeMode, VertexQuantizationRoundMode roundMode, VertexQuantizationCenterMode centerMode) = 0;
	//virtual void setVertexReuseEnable(bool enable) = 0;
	virtual void setVgtControl(uint8_t primGroupSizeMinusOne, WdSwitchOnlyOnEopMode wdSwitchOnlyOnEopMode, VgtPartialVsWaveMode partialVsWaveMode) = 0;
	virtual void setViewport(uint32_t viewportId, float dmin, float dmax, const float scale[3], const float offset[3]) = 0;
	//virtual void setViewportScissor(uint32_t viewportId, uint32_t left, uint32_t top, uint32_t right, uint32_t bottom, WindowOffsetMode windowOffsetEnable) = 0;
	virtual void setViewportTransformControl(ViewportTransformControl vportControl) = 0;
	virtual void setVsharpInUserData(ShaderStage stage, uint32_t startUserDataSlot, const VSharpBuffer *buffer) = 0;
	//virtual void setVsShaderStreamoutEnable(bool enable) = 0;
	virtual void setVsShader(const pssl::VsStageRegisters *vsRegs, uint32_t shaderModifier) = 0;
	//virtual void setWindowOffset(int16_t offsetX, int16_t offsetY) = 0;
	//virtual void setWindowScissor(uint32_t left, uint32_t top, uint32_t right, uint32_t bottom, WindowOffsetMode windowOffsetEnable) = 0;
	//virtual void setZPassPredicationDisable(void) = 0;
	//virtual void setZPassPredicationEnable(OcclusionQueryResults *queryResults, PredicationZPassHint hint, PredicationZPassAction action) = 0;
	//virtual void signalSemaphore(uint64_t* semAddr, SemaphoreSignalBehavior behavior, SemaphoreUpdateConfirmMode updateConfirm) = 0;
	//virtual void stallCommandBufferParser() = 0;
	//virtual void triggerEndOfPipeInterrupt(EndOfPipeEventType eventType, CacheAction cacheAction) = 0;
	//virtual void triggerEvent(EventType eventType) = 0;
	//virtual void updateGsShader(const GsStageRegisters *gsRegs) = 0;
	//virtual void updateHsShader(const HsStageRegisters *hsRegs, const TessellationRegisters *tessRegs) = 0;
	virtual void updatePsShader(const pssl::PsStageRegisters *psRegs) = 0;
	virtual void updateVsShader(const pssl::VsStageRegisters *vsRegs, uint32_t shaderModifier) = 0;
	//virtual void waitForGraphicsWrites(uint32_t baseAddr256, uint32_t sizeIn256ByteBlocks, uint32_t targetMask, CacheAction cacheAction, uint32_t extendedCacheMask,
	//	StallCommandBufferParserMode commandBufferStallMode) = 0;
	//virtual void waitForSetupDispatchDrawKickRingBuffer(uint32_t krbCount, uint32_t gdsDwOffsetKrb, uint32_t gdsDwOffsetKrbCounters, void *addrIrb, uint32_t sizeofIrbInBytes) = 0;
	//virtual void waitOnAddress(void *gpuAddr, uint32_t mask, WaitCompareFunc compareFunc, uint32_t refValue) = 0;
	//virtual void waitOnAddressAndStallCommandBufferParser(void *gpuAddr, uint32_t mask, uint32_t refValue) = 0;
	//virtual void waitOnCe() = 0;
	//virtual void waitOnRegister(uint16_t gpuReg, uint32_t mask, WaitCompareFunc compareFunc, uint32_t refValue) = 0;
	//virtual void waitSemaphore(uint64_t* semAddr, SemaphoreWaitBehavior behavior) = 0;
	virtual void waitUntilSafeForRendering(uint32_t videoOutHandle, uint32_t displayBufferIndex) = 0;
	virtual void writeAtEndOfPipe(EndOfPipeEventType eventType,
		EventWriteDest dstSelector, void *dstGpuAddr,
		EventWriteSource srcSelector, uint64_t immValue,
		CacheAction cacheAction, CachePolicy cachePolicy) = 0;

	virtual void writeAtEndOfPipeWithInterrupt(EndOfPipeEventType eventType,
		EventWriteDest dstSelector, void *dstGpuAddr,
		EventWriteSource srcSelector, uint64_t immValue,
		CacheAction cacheAction, CachePolicy cachePolicy) = 0;
	//virtual void writeAtEndOfShader(EndOfShaderEventType eventType, void *dstGpuAddr, uint32_t immValue) = 0;
	//virtual void writeDataInline(void *dstGpuAddr, const void *data, uint32_t sizeInDwords, WriteDataConfirmMode writeConfirm) = 0;
	//virtual void writeDataInlineThroughL2(void *dstGpuAddr, const void *data, uint32_t sizeInDwords, CachePolicy cachePolicy, WriteDataConfirmMode writeConfirm) = 0;
	//virtual void writeEventStats(EventStats eventStats, void *dstGpuAddr) = 0;
	//virtual void writeOcclusionQuery(OcclusionQueryOp queryOp, OcclusionQueryResults *queryResults) = 0;
	//virtual void writeStreamoutBufferOffset(StreamoutBufferId buffer, uint32_t offset) = 0;
	//virtual void writeStreamoutBufferUpdate(StreamoutBufferId buffer, StreamoutBufferUpdateWrite sourceSelect,
	//	StreamoutBufferUpdateSaveFilledSize	updateMemory, void *dstAddr, uint64_t srcAddrOrImm) = 0;

	// Dispatch only
	//virtual void setComputeResourceManagement(ShaderEngine engine, uint16_t mask) = 0;
	//virtual void setComputeResourceManagementForBase(ShaderEngine engine, uint16_t mask) = 0;
	//virtual void setComputeResourceManagementForNeo(ShaderEngine engine, uint16_t mask) = 0;
	//virtual void setBulkyCsShader(const CsStageRegisters *computeData) = 0;
	//virtual void setScratchSize(uint32_t maxNumWaves, uint32_t num1KByteChunksPerWave) = 0;
	//virtual void dispatchIndirect(DispatchIndirectArgs *args) = 0;
	//virtual void dispatchIndirectWithOrderedAppend(DispatchIndirectArgs *args, DispatchOrderedAppendMode orderedAppendMode) = 0;
	//virtual void setupDispatchDrawKickRingBuffer(uint32_t krbCount, uint32_t gdsDwOffsetKrb, uint32_t gdsDwOffsetKrbCounters) = 0;
	//virtual void dispatchDraw(uint32_t threadGroupX, uint32_t threadGroupY, uint32_t threadGroupZ, DispatchOrderedAppendMode orderedAppendMode, uint32_t sgprKrbLoc) = 0;
	//virtual void triggerReleaseMemEventInterrupt(ReleaseMemEventType eventType, CacheAction cacheAction) = 0;
	//virtual void writeReleaseMemEventWithInterrupt(ReleaseMemEventType eventType,
	//	EventWriteDest dstSelector, void *dstGpuAddr,
	//	EventWriteSource srcSelector, uint64_t immValue,
	//	CacheAction cacheAction, CachePolicy writePolicy) = 0;
	//virtual void writeReleaseMemEvent(ReleaseMemEventType eventType,
	//	EventWriteDest dstSelector, void *dstGpuAddr,
	//	EventWriteSource srcSelector, uint64_t immValue,
	//	CacheAction cacheAction, CachePolicy writePolicy) = 0;
	//virtual void insertDingDongMarker() = 0;
	//virtual void pushDispatchDrawAcbSubmitMarker() = 0;
	//virtual void setQueuePriority(uint32_t queueId, uint32_t priority) = 0;
	//virtual void enableQueueQuantumTimer(uint32_t queueId, QuantumScale quantumScale, uint32_t quantumDuration) = 0;
	//virtual void disableQueueQuantumTimer(uint32_t queueId) = 0;
	//virtual void writeResumeEvent(ReleaseMemEventType eventType, uint64_t holeAddr, CacheAction cacheAction, CachePolicy writePolicy) = 0;
	//virtual void writeResumeEventWithInterrupt(ReleaseMemEventType eventType, uint64_t holeAddr, CacheAction cacheAction, CachePolicy writePolicy) = 0;
	//virtual void waitForResume(uint64_t holeAddr) = 0;
	//virtual void writeResume(uint64_t holeAddr) = 0;
	//virtual void callCommandBuffer(void *cbBaseAddr, uint64_t cbSizeInDW) = 0;


	// Internal functions
	// Dispatch Only
	//virtual void setQueueRegister(uint32_t regAddr, uint32_t vqid, uint32_t regValue) = 0;
	// Shared internals
	//virtual void setContextRegisterWithIndex(uint32_t regAddr, uint32_t regValue, uint32_t index) = 0;
	//virtual void setContextRegisterRange(uint32_t regAddr, const uint32_t *pRegValues, uint32_t numValues) = 0;
	//virtual void setConfigRegister(uint32_t regAddr, uint32_t regValue) = 0;
	//virtual void setConfigRegisterRange(uint32_t regAddr, const uint32_t *pRegValues, uint32_t numValues) = 0;
	//virtual void setUserConfigRegister(uint32_t regAddr, uint32_t regValue) = 0;
	//virtual void setUserConfigRegisterRange(uint32_t regAddr, const uint32_t *pRegValues, uint32_t numValues) = 0;
	//virtual void setUserConfigRegisterWithIndex(uint32_t regAddr, uint32_t regValue, uint32_t index) = 0;
	//virtual void setPersistentRegister(ShaderType shaderType, uint32_t regAddr, uint32_t regValue) = 0;
	//virtual void setPersistentRegisterRange(ShaderType shaderType, uint32_t regAddr, const uint32_t *pRegValues, uint32_t numValues) = 0;
	//virtual void setRegisterRange(uint32_t header,
	//	uint32_t regOffset,
	//	const uint32_t *pData,
	//	uint32_t numDword) = 0;
	//virtual void writeWaitMemCmd(
	//	uint32_t function,
	//	uint32_t engine,
	//	uint64_t pollRegAddrOrMemOffset,
	//	uint32_t reference,
	//	uint32_t mask,
	//	uint32_t interval) = 0;


	//static bool patchDispatchDrawAcbSubmitMarker(uint32_t *pAcbSubmitMarker, uint32_t sizeofAcbSegment, uint32_t pipe, uint32_t queue, void const* pAcbSegmentNext);
	//static bool isDispatchDrawAcbSubmitMarker(uint32_t const *pAcbSubmitMarker);
	//static bool readDispatchDrawAcbSubmitMarker(uint32_t const *pAcbSubmitMarker, uint32_t *out_pSizeofAcbSegment, uint32_t *out_pPipe, uint32_t *out_pQueue, void const** out_ppAcbSegmentNext);

protected:
	void emuWriteGpuLabel(EventWriteSource selector, void* label, uint64_t value);

protected:
	RcPtr<gve::GveDevice> m_device;
	RcPtr<gve::GveCmdList> m_cmd;
	RcPtr<gve::GveContext> m_context;
	
private:

};


