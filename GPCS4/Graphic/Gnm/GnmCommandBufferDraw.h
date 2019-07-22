#pragma once

#include "GPCS4Common.h"
#include "GnmCommandBuffer.h"
#include "GnmConstant.h"
#include "GnmStructure.h"

class GnmCommandBufferDraw : public GnmCommandBuffer
{
public:
	GnmCommandBufferDraw();
	virtual ~GnmCommandBufferDraw();

	// Implement these one by one...

	//void *allocateFromCommandBuffer(uint32_t sizeInBytes, EmbeddedDataAlignment alignment);
	//void chainCommandBufferAndResume(uint64_t holeAddr, void *nextIbBaseAddr, uint64_t nextIbSizeInDW);
	//void chainCommandBuffer(void *cbBaseAddr, uint64_t cbSizeInDW);
	//void chainCommandBufferAndResume(uint64_t holeAddr, void *nextIbBaseAddr, uint64_t nextIbSizeInDW);
	//void disableGsMode();
	//void disableOrderedAppendAllocationCounter(uint32_t oaCounterIndex);
	//void dispatchDraw(PrimitiveType primType, uint32_t indexOffset, uint32_t primGroupIndexCount, uint32_t primGroupThreshold, uint32_t pollIntervalThreshold, DispatchDrawMode dispatchDrawMode, uint32_t sgprVrbLoc, DrawModifier modifier);
	//void dispatch(uint32_t threadGroupX, uint32_t threadGroupY, uint32_t threadGroupZ);
	//void dispatchWithOrderedAppend(uint32_t threadGroupX, uint32_t threadGroupY, uint32_t threadGroupZ, DispatchOrderedAppendMode orderedAppendMode);
	//void dispatchIndirect(uint32_t dataOffsetInBytes);
	//void dispatchIndirectWithOrderedAppend(uint32_t dataOffsetInBytes, DispatchOrderedAppendMode orderedAppendMode);
	//void dispatchDraw(PrimitiveType primType, uint32_t indexOffset, uint32_t primGroupIndexCount, uint32_t primGroupThreshold, uint32_t pollIntervalThreshold, DispatchDrawMode dispatchDrawMode, uint32_t sgprVrbLoc, DrawModifier modifier);
	//void dispatchIndirect(uint32_t dataOffsetInBytes);
	//void dispatchIndirectWithOrderedAppend(uint32_t dataOffsetInBytes, DispatchOrderedAppendMode orderedAppendMode);
	//void dispatchIndirectWithOrderedAppend(uint32_t dataOffsetInBytes, DispatchOrderedAppendMode orderedAppendMode);
	//void dispatchWithOrderedAppend(uint32_t threadGroupX, uint32_t threadGroupY, uint32_t threadGroupZ, DispatchOrderedAppendMode orderedAppendMode);
	//void dmaData(DmaDataDst	dstSel, uint64_t dst, DmaDataSrc srcSel, uint64_t srcOrData, uint32_t numBytes, DmaDataBlockingMode isBlocking);
	//void drawIndexAuto(uint32_t indexCount, DrawModifier modifier);
	//void drawIndexAuto(uint32_t indexCount);
	//void drawIndexInline(uint32_t indexCount, const void *indices, uint32_t indicesSizeInBytes, DrawModifier modifier);
	//void drawIndexInline(uint32_t indexCount, const void *indices, uint32_t indicesSizeInBytes);
	//void drawIndex(uint32_t indexCount, const void *indexAddr, DrawModifier modifier);
	//void drawIndex(uint32_t indexCount, const void *indexAddr);
	//void drawIndexMultiInstanced(uint32_t indexCount, uint32_t instanceCount, const void *indexAddr, const void *objectIdAddr, DrawModifier modifier);
	//void drawIndexMultiInstanced(uint32_t indexCount, uint32_t instanceCount, const void *indexAddr, const void *objectIdAddr);
	//void drawIndexOffset(uint32_t indexOffset, uint32_t indexCount, DrawModifier modifier);
	//void drawIndexOffset(uint32_t indexOffset, uint32_t indexCount);
	//void drawIndexIndirect(uint32_t dataOffsetInBytes, ShaderStage stage, uint8_t vertexOffsetUserSgpr, uint8_t instanceOffsetUserSgpr, DrawModifier modifier);
	//void drawIndexIndirect(uint32_t dataOffsetInBytes, ShaderStage stage, uint8_t vertexOffsetUserSgpr, uint8_t instanceOffsetUserSgpr);
	//void drawIndexIndirectCountMulti(uint32_t dataOffsetInBytes, uint32_t drawCount, void *countAddress, ShaderStage stage, uint8_t vertexOffsetUserSgpr, uint8_t instanceOffsetUserSgpr, DrawModifier modifier);
	//void drawIndexIndirectCountMulti(uint32_t dataOffsetInBytes, uint32_t drawCount, void* countAddress, ShaderStage stage, uint8_t vertexOffsetUserSgpr, uint8_t instanceOffsetUserSgpr);
	//void drawIndexAuto(uint32_t indexCount, DrawModifier modifier);
	//void drawIndexAuto(uint32_t indexCount);
	//void drawIndexInline(uint32_t indexCount, const void *indices, uint32_t indicesSizeInBytes, DrawModifier modifier);
	//void drawIndexInline(uint32_t indexCount, const void *indices, uint32_t indicesSizeInBytes);
	//void drawIndex(uint32_t indexCount, const void *indexAddr, DrawModifier modifier);
	//void drawIndex(uint32_t indexCount, const void *indexAddr);
	//void drawIndexMultiInstanced(uint32_t indexCount, uint32_t instanceCount, const void *indexAddr, const void *objectIdAddr, DrawModifier modifier);
	//void drawIndexMultiInstanced(uint32_t indexCount, uint32_t instanceCount, const void *indexAddr, const void *objectIdAddr);
	//void drawIndexOffset(uint32_t indexOffset, uint32_t indexCount, DrawModifier modifier);
	//void drawIndexOffset(uint32_t indexOffset, uint32_t indexCount);
	//void drawIndexIndirect(uint32_t dataOffsetInBytes, ShaderStage stage, uint8_t vertexOffsetUserSgpr, uint8_t instanceOffsetUserSgpr, DrawModifier modifier);
	//void drawIndexIndirect(uint32_t dataOffsetInBytes, ShaderStage stage, uint8_t vertexOffsetUserSgpr, uint8_t instanceOffsetUserSgpr);
	//void drawIndexIndirectCountMulti(uint32_t dataOffsetInBytes, uint32_t drawCount, void *countAddress, ShaderStage stage, uint8_t vertexOffsetUserSgpr, uint8_t instanceOffsetUserSgpr, DrawModifier modifier);
	//void drawIndexIndirectCountMulti(uint32_t dataOffsetInBytes, uint32_t drawCount, void* countAddress, ShaderStage stage, uint8_t vertexOffsetUserSgpr, uint8_t instanceOffsetUserSgpr);
	//void drawIndexAuto(uint32_t indexCount, DrawModifier modifier);
	//void drawIndexAuto(uint32_t indexCount);
	//void drawIndexAuto(uint32_t indexCount, DrawModifier modifier);
	//void drawIndexAuto(uint32_t indexCount);
	//void drawIndexIndirect(uint32_t dataOffsetInBytes, ShaderStage stage, uint8_t vertexOffsetUserSgpr, uint8_t instanceOffsetUserSgpr, DrawModifier modifier);
	//void drawIndexIndirect(uint32_t dataOffsetInBytes, ShaderStage stage, uint8_t vertexOffsetUserSgpr, uint8_t instanceOffsetUserSgpr);
	//void drawIndexIndirectCountMulti(uint32_t dataOffsetInBytes, uint32_t drawCount, void *countAddress, ShaderStage stage, uint8_t vertexOffsetUserSgpr, uint8_t instanceOffsetUserSgpr, DrawModifier modifier);
	//void drawIndexIndirectCountMulti(uint32_t dataOffsetInBytes, uint32_t drawCount, void* countAddress, ShaderStage stage, uint8_t vertexOffsetUserSgpr, uint8_t instanceOffsetUserSgpr);
	//void drawIndexIndirect(uint32_t dataOffsetInBytes, ShaderStage stage, uint8_t vertexOffsetUserSgpr, uint8_t instanceOffsetUserSgpr, DrawModifier modifier);
	//void drawIndexIndirect(uint32_t dataOffsetInBytes, ShaderStage stage, uint8_t vertexOffsetUserSgpr, uint8_t instanceOffsetUserSgpr);
	//void drawIndexIndirectCountMulti(uint32_t dataOffsetInBytes, uint32_t drawCount, void *countAddress, ShaderStage stage, uint8_t vertexOffsetUserSgpr, uint8_t instanceOffsetUserSgpr, DrawModifier modifier);
	//void drawIndexIndirectCountMulti(uint32_t dataOffsetInBytes, uint32_t drawCount, void* countAddress, ShaderStage stage, uint8_t vertexOffsetUserSgpr, uint8_t instanceOffsetUserSgpr);
	//void drawIndexIndirectCountMulti(uint32_t dataOffsetInBytes, uint32_t drawCount, void *countAddress, ShaderStage stage, uint8_t vertexOffsetUserSgpr, uint8_t instanceOffsetUserSgpr, DrawModifier modifier);
	//void drawIndexIndirectCountMulti(uint32_t dataOffsetInBytes, uint32_t drawCount, void* countAddress, ShaderStage stage, uint8_t vertexOffsetUserSgpr, uint8_t instanceOffsetUserSgpr);
	//void drawIndexIndirectCountMulti(uint32_t dataOffsetInBytes, uint32_t drawCount, void *countAddress, ShaderStage stage, uint8_t vertexOffsetUserSgpr, uint8_t instanceOffsetUserSgpr, DrawModifier modifier);
	//void drawIndexIndirectCountMulti(uint32_t dataOffsetInBytes, uint32_t drawCount, void* countAddress, ShaderStage stage, uint8_t vertexOffsetUserSgpr, uint8_t instanceOffsetUserSgpr);
	//void drawIndexInline(uint32_t indexCount, const void *indices, uint32_t indicesSizeInBytes, DrawModifier modifier);
	//void drawIndexInline(uint32_t indexCount, const void *indices, uint32_t indicesSizeInBytes);
	//void drawIndexInline(uint32_t indexCount, const void *indices, uint32_t indicesSizeInBytes, DrawModifier modifier);
	//void drawIndexInline(uint32_t indexCount, const void *indices, uint32_t indicesSizeInBytes);
	//void drawIndexMultiInstanced(uint32_t indexCount, uint32_t instanceCount, const void *indexAddr, const void *objectIdAddr, DrawModifier modifier);
	//void drawIndexMultiInstanced(uint32_t indexCount, uint32_t instanceCount, const void *indexAddr, const void *objectIdAddr);
	//void drawIndexMultiInstanced(uint32_t indexCount, uint32_t instanceCount, const void *indexAddr, const void *objectIdAddr, DrawModifier modifier);
	//void drawIndexMultiInstanced(uint32_t indexCount, uint32_t instanceCount, const void *indexAddr, const void *objectIdAddr);
	//void drawIndexOffset(uint32_t indexOffset, uint32_t indexCount, DrawModifier modifier);
	//void drawIndexOffset(uint32_t indexOffset, uint32_t indexCount);
	//void drawIndexOffset(uint32_t indexOffset, uint32_t indexCount, DrawModifier modifier);
	//void drawIndexOffset(uint32_t indexOffset, uint32_t indexCount);
	//void drawIndirect(uint32_t dataOffsetInBytes, ShaderStage stage, uint8_t vertexOffsetUserSgpr, uint8_t instanceOffsetUserSgpr, DrawModifier modifier);
	//void drawIndirect(uint32_t dataOffsetInBytes, ShaderStage stage, uint8_t vertexOffsetUserSgpr, uint8_t instanceOffsetUserSgpr);
	//void drawIndirectCountMulti(uint32_t dataOffsetInBytes, uint32_t count, void *countAddress, ShaderStage stage, uint8_t vertexOffsetUserSgpr, uint8_t instanceOffsetUserSgpr, DrawModifier modifier);
	//void drawIndirectCountMulti(uint32_t dataOffsetInBytes, uint32_t count, void *countAddress, ShaderStage stage, uint8_t vertexOffsetUserSgpr, uint8_t instanceOffsetUserSgpr);
	//void drawIndirect(uint32_t dataOffsetInBytes, ShaderStage stage, uint8_t vertexOffsetUserSgpr, uint8_t instanceOffsetUserSgpr, DrawModifier modifier);
	//void drawIndirect(uint32_t dataOffsetInBytes, ShaderStage stage, uint8_t vertexOffsetUserSgpr, uint8_t instanceOffsetUserSgpr);
	//void drawIndirectCountMulti(uint32_t dataOffsetInBytes, uint32_t count, void *countAddress, ShaderStage stage, uint8_t vertexOffsetUserSgpr, uint8_t instanceOffsetUserSgpr, DrawModifier modifier);
	//void drawIndirectCountMulti(uint32_t dataOffsetInBytes, uint32_t count, void *countAddress, ShaderStage stage, uint8_t vertexOffsetUserSgpr, uint8_t instanceOffsetUserSgpr);
	//void drawIndirectCountMulti(uint32_t dataOffsetInBytes, uint32_t count, void *countAddress, ShaderStage stage, uint8_t vertexOffsetUserSgpr, uint8_t instanceOffsetUserSgpr, DrawModifier modifier);
	//void drawIndirectCountMulti(uint32_t dataOffsetInBytes, uint32_t count, void *countAddress, ShaderStage stage, uint8_t vertexOffsetUserSgpr, uint8_t instanceOffsetUserSgpr);
	//void drawIndirectCountMulti(uint32_t dataOffsetInBytes, uint32_t count, void *countAddress, ShaderStage stage, uint8_t vertexOffsetUserSgpr, uint8_t instanceOffsetUserSgpr, DrawModifier modifier);
	//void drawIndirectCountMulti(uint32_t dataOffsetInBytes, uint32_t count, void *countAddress, ShaderStage stage, uint8_t vertexOffsetUserSgpr, uint8_t instanceOffsetUserSgpr);
	//void drawOpaqueAuto(DrawModifier modifier);
	//void drawOpaqueAuto();
	//void drawOpaqueAuto(DrawModifier modifier);
	//void drawOpaqueAuto();
	//void enableGsMode(GsMaxOutputPrimitiveDwordSize maxPrimDwordSize);
	//void enableGsModeOnChip(GsMaxOutputPrimitiveDwordSize maxPrimDwordSize,
	//						uint32_t esVerticesPerSubGroup, uint32_t gsInputPrimitivesPerSubGroup);
	//void enableOrderedAppendAllocationCounter(uint32_t oaCounterIndex, uint32_t gdsDwOffsetOfCounter, ShaderStage stage, uint32_t oaOpIndex, uint32_t spaceInAllocationUnits);
	//void fillAndResume(uint64_t holeAddr, void *commandStream, uint32_t sizeInDW);
	//void flushShaderCachesAndWait(CacheAction cacheAction, uint32_t extendedCacheMask, StallCommandBufferParserMode commandBufferStallMode);
	//void flushStreamout();
	//void incrementDeCounter();
	//void initializeDefaultHardwareState();
	//void initializeToDefaultContextState();
	//void insertNop(uint32_t numDwords);
	//void markDispatchDrawAcbAddress(uint32_t const* addrAcb, uint32_t const* addrAcbBegin);
	//uint64_t pause(uint32_t reservedDWs);
	//void popMarker();
	//void prefetchIntoL2(void *dataAddr, uint32_t sizeInBytes);
	//void prepareFlip();
	void prepareFlip(void *labelAddr, uint32_t value);
	//void prepareFlipWithEopInterrupt(EndOfPipeEventType eventType, CacheAction cacheAction);
	//void prepareFlip();
	//void prepareFlip(void *labelAddr, uint32_t value);
	//void prepareFlipWithEopInterrupt(EndOfPipeEventType eventType, CacheAction cacheAction);
	//void prepareFlipWithEopInterrupt(EndOfPipeEventType eventType, CacheAction cacheAction);
	//void prepareFlipWithEopInterrupt(EndOfPipeEventType eventType, CacheAction cacheAction);
	//void pushMarker(const char *debugString);
	//void pushMarker(const char *debugString, uint32_t argbColor);
	//void pushMarker(const char *debugString);
	//void pushMarker(const char *debugString, uint32_t argbColor);
	//void readCpcPerfCounter(uint32_t counterSlot, void *pgpuOutput);
	//void readCpcPerfCounters(uint32_t counterSlotStart, uint32_t numCounterSlots, void *pgpuOutputStart);
	//void readCpcPerfCounters(uint32_t counterSlotStart, uint32_t numCounterSlots, void *pgpuOutputStart);
	//void readCpfPerfCounter(uint32_t counterSlot, void *pgpuOutput);
	//void readCpfPerfCounters(uint32_t counterSlotStart, uint32_t numCounterSlots, void *pgpuOutputStart);
	//void readCpfPerfCounters(uint32_t counterSlotStart, uint32_t numCounterSlots, void *pgpuOutputStart);
	//void readCpPerfCounter(uint32_t counterSlot, void *pgpuOutput);
	//void readCpPerfCounters(uint32_t counterSlotStart, uint32_t numCounterSlots, void *pgpuOutputStart);
	//void readCpPerfCounters(uint32_t counterSlotStart, uint32_t numCounterSlots, void *pgpuOutputStart);
	//void readDataFromGds(EndOfShaderEventType eventType, void *dstGpuAddr, uint32_t gdsOffsetInDwords, uint32_t gdsSizeInDwords);
	//void readGdsPerfCounter(uint32_t counterSlot, void *pgpuOutput);
	//void readGdsPerfCounters(uint32_t counterSlotStart, uint32_t numCounterSlots, void *pgpuOutputStart);
	//void readGdsPerfCounters(uint32_t counterSlotStart, uint32_t numCounterSlots, void *pgpuOutputStart);
	//void readTcsPerfCounter(uint32_t counterSlot, void *pgpuOutput);
	//void readTcsPerfCounters(uint32_t counterSlotStart, uint32_t numCounterSlots, void *pgpuOutputStart);
	//void readTcsPerfCounters(uint32_t counterSlotStart, uint32_t numCounterSlots, void *pgpuOutputStart);
	//void readWdPerfCounter(uint32_t counterSlot, void *pgpuOutput);
	//void readWdPerfCounters(uint32_t counterSlotStart, uint32_t numCounterSlots, void *pgpuOutputStart);
	//void readWdPerfCounters(uint32_t counterSlotStart, uint32_t numCounterSlots, void *pgpuOutputStart);
	//void requestMipStatsReportAndReset(void *outputBuffer, uint32_t sizeInByte);
	//void resetFoveatedWindow();
	//void resetVgtControl();
	//static void resume(uint64_t holeAddr);
	//void selectCpcPerfCounter(uint32_t counterSlot, CpcPerfCounter counterSelect);
	//void selectCpcPerfCounters(uint32_t counterSlotStart, uint32_t numCounterSlots, CpcPerfCounter const* aCounterSelect);
	//void selectCpcPerfCounters(uint32_t counterSlotStart, uint32_t numCounterSlots, CpcPerfCounter const* aCounterSelect);
	//void selectCpfPerfCounter(uint32_t counterSlot, CpfPerfCounter counterSelect);
	//void selectCpfPerfCounters(uint32_t counterSlotStart, uint32_t numCounterSlots, CpfPerfCounter const* aCounterSelect);
	//void selectCpfPerfCounters(uint32_t counterSlotStart, uint32_t numCounterSlots, CpfPerfCounter const* aCounterSelect);
	//void selectCpPerfCounter(uint32_t counterSlot, CpPerfCounter counterSelect);
	//void selectCpPerfCounters(uint32_t counterSlotStart, uint32_t numCounterSlots, CpPerfCounter const* aCounterSelect);
	//void selectCpPerfCounters(uint32_t counterSlotStart, uint32_t numCounterSlots, CpPerfCounter const* aCounterSelect);
	//void selectGdsPerfCounter(uint32_t counterSlot, GdsPerfCounter counterSelect);
	//void selectGdsPerfCounters(uint32_t counterSlotStart, uint32_t numCounterSlots, GdsPerfCounter const* aCounterSelect);
	//void selectGdsPerfCounters(uint32_t counterSlotStart, uint32_t numCounterSlots, GdsPerfCounter const* aCounterSelect);
	//void selectIaPerfCounter(uint32_t iaIndex, uint32_t counterSlot, IaPerfCounterSelect counterSelect);
	//void selectIaPerfCounters(uint32_t iaIndex, uint32_t counterSlotStart, uint32_t numCounterSlots, IaPerfCounterSelect const* aCounterSelect);
	//void selectIaPerfCounter(ShaderEngineBroadcast broadcast, uint32_t counterSlot, IaPerfCounterSelect counterSelect);
	//void selectIaPerfCounters(ShaderEngineBroadcast broadcast, uint32_t counterSlotStart, uint32_t numCounterSlots, IaPerfCounterSelect const* aCounterSelect);
	//void selectIaPerfCounters(uint32_t iaIndex, uint32_t counterSlotStart, uint32_t numCounterSlots, IaPerfCounterSelect const* aCounterSelect);
	//void selectIaPerfCounters(ShaderEngineBroadcast broadcast, uint32_t counterSlotStart, uint32_t numCounterSlots, IaPerfCounterSelect const* aCounterSelect);
	//void selectTcaPerfCounter(uint32_t tcaIndex, uint32_t counterSlot, TcaPerfCounterSelect counterSelect);
	//void selectTcaPerfCounters(uint32_t tcaIndex, uint32_t counterSlotStart, uint32_t numCounterSlots, TcaPerfCounterSelect const* aCounterSelect);
	//void selectTcaPerfCounter(ShaderEngineBroadcast broadcast, uint32_t counterSlot, TcaPerfCounterSelect counterSelect);
	//void selectTcaPerfCounters(ShaderEngineBroadcast broadcast, uint32_t counterSlotStart, uint32_t numCounterSlots, TcaPerfCounterSelect const* aCounterSelect);
	//void selectTcaPerfCounters(uint32_t tcaIndex, uint32_t counterSlotStart, uint32_t numCounterSlots, TcaPerfCounterSelect const* aCounterSelect);
	//void selectTcaPerfCounters(ShaderEngineBroadcast broadcast, uint32_t counterSlotStart, uint32_t numCounterSlots, TcaPerfCounterSelect const* aCounterSelect);
	//void selectTccPerfCounter(uint32_t tccIndex, uint32_t counterSlot, TccPerfCounterSelect counterSelect);
	//void selectTccPerfCounters(uint32_t tccIndex, uint32_t counterSlotStart, uint32_t numCounterSlots, TccPerfCounterSelect const* aCounterSelect);
	//void selectTccPerfCounter(ShaderEngineBroadcast broadcast, uint32_t counterSlot, TccPerfCounterSelect counterSelect);
	//void selectTccPerfCounters(ShaderEngineBroadcast broadcast, uint32_t counterSlotStart, uint32_t numCounterSlots, TccPerfCounterSelect const* aCounterSelect);
	//void selectTccPerfCounters(uint32_t tccIndex, uint32_t counterSlotStart, uint32_t numCounterSlots, TccPerfCounterSelect const* aCounterSelect);
	//void selectTccPerfCounters(ShaderEngineBroadcast broadcast, uint32_t counterSlotStart, uint32_t numCounterSlots, TccPerfCounterSelect const* aCounterSelect);
	//void selectTcsPerfCounter(uint32_t counterSlot, TcsPerfCounterSelect counterSelect);
	//void selectTcsPerfCounters(uint32_t counterSlotStart, uint32_t numCounterSlots, TcsPerfCounterSelect const* aCounterSelect);
	//void selectTcsPerfCounters(uint32_t counterSlotStart, uint32_t numCounterSlots, TcsPerfCounterSelect const* aCounterSelect);
	//void selectWdPerfCounter(uint32_t counterSlot, WdPerfCounterSelect counterSelect);
	//void selectWdPerfCounters(uint32_t counterSlotStart, uint32_t numCounterSlots, WdPerfCounterSelect const* aCounterSelect);
	//void selectWdPerfCounters(uint32_t counterSlotStart, uint32_t numCounterSlots, WdPerfCounterSelect const* aCounterSelect);
	//void setAaSampleCount(NumSamples logNumSamples, uint32_t maxSampleDistance);
	//void setAaSampleLocationControl(const AaSampleLocationControl *control);
	//void setAaSampleMask(uint64_t mask);
	//void setActiveShaderStages(ActiveShaderStages activeStages);
	//void setAlphaToMaskControl(AlphaToMaskControl alphaToMaskControl);
	//void setBaseIndirectArgs(ShaderType shaderType, void *indirectBaseAddr);
	//void setBlendColor(float red, float green, float blue, float alpha);
	//void setBlendControl(uint32_t rtSlot, BlendControl blendControl);
	//void setBorderColorTableAddr(void *tableAddr);
	//void setCbControl(CbMode mode, RasterOp op);
	//void setClipControl(ClipControl reg);
	//void setClipRectangle(uint32_t rectId, uint32_t left, uint32_t top, uint32_t right, uint32_t bottom);
	//void setClipRectangleRule(uint16_t clipRule);
	//void setClipRectangleRule(uint16_t clipRule);
	//void setCmaskClearColor(uint32_t rtSlot, const uint32_t clearColor[2]);
	void setComputeResourceManagement(ShaderEngine engine, uint16_t mask);
	void setComputeScratchSize(uint32_t maxNumWaves, uint32_t num1KByteChunksPerWave);
	void setComputeShaderControl(uint32_t wavesPerSh, uint32_t threadgroupsPerCu, uint32_t lockThreshold);
	//void setConfigRegister(uint32_t regAddr, uint32_t regValue);
	//void setConfigRegisterRange(uint32_t regAddr, const uint32_t *pRegValues, uint32_t numValues);
	//void setContextRegisterWithIndex(uint32_t regAddr, uint32_t regValue, uint32_t index);
	//void setContextRegisterRange(uint32_t regAddr, const uint32_t *pRegValues, uint32_t numValues);
	//void setContextRegisterWithIndex(uint32_t regAddr, uint32_t regValue, uint32_t index);
	//void setCsShader(const CsStageRegisters *csRegs, uint32_t shaderModifier);
	//void setDbCountControl(DbCountControlPerfectZPassCounts perfectZPassCounts, uint32_t log2SampleRate);
	//void setDbRenderControl(DbRenderControl reg);
	//void setDepthBoundsRange(float depthBoundsMin, float depthBoundsMax);
	//void setDepthClearValue(float clearValue);
	//void setDepthEqaaControl(DepthEqaaControl depthEqaa);
	//void setDepthRenderTarget(DepthRenderTarget const *depthTarget);
	//void setDepthStencilControl(DepthStencilControl depthControl);
	//void setDepthStencilDisable();
	//void setDispatchDrawIndexDeallocationMask(uint32_t indexMask);
	//void setDrawPayloadControl(DrawPayloadControl cntrl);
	//void setEmbeddedPsShader(EmbeddedPsShader shaderId);
	//void setEmbeddedVsShader(EmbeddedVsShader shaderId, uint32_t shaderModifier);
	//void setEsShader(const EsStageRegisters *esRegs, uint32_t shaderModifier);
	//void resetFoveatedWindow();
	//void setFoveatedWindow(uint8_t xMinLeftEye, uint8_t xMaxLeftEye, uint8_t xMinRightEye, uint8_t xMaxRightEye, uint8_t yMinBothEyes, uint8_t yMaxBothEyes);
	//void setGenericScissor(uint32_t left, uint32_t top, uint32_t right, uint32_t bottom, WindowOffsetMode windowOffsetEnable);
	//void setGraphicsScratchSize(uint32_t maxNumWaves, uint32_t num1KByteChunksPerWave);
	//void setGraphicsShaderControl(GraphicsShaderControl control);
	//void setGsMode(GsMode mode, GsMaxOutputPrimitiveDwordSize maxPrimDwordSize);
	//void setGsOnChipControl(uint32_t esVerticesPerSubGroup, uint32_t gsInputPrimitivesPerSubGroup);
	//void setGsShader(const GsStageRegisters *gsRegs);
	//void setGuardBands(float horzClip, float vertClip, float horzDiscard, float vertDiscard);
	//void setHardwareScreenOffset(uint32_t offsetX, uint32_t offsetY);
	//void setHsShader(const HsStageRegisters *hsRegs, const TessellationRegisters *tessRegs);
	//void setHsShader(const HsStageRegisters *hsRegs, const TessellationRegisters *tessRegs, TessellationDistributionMode distributionMode);
	//void setHtileStencil0(HtileStencilControl htileStencilControl);
	//void setHtileStencil1(HtileStencilControl htileStencilControl);
	//void setIndexBuffer(const void *indexAddr);
	//void setIndexCount(uint32_t indexCount);
	//void setIndexOffset(uint32_t offset);
	//void setIndexSize(IndexSize indexSize, CachePolicy cachePolicy);
	//void setInstanceStepRate(uint32_t step0, uint32_t step1);
	//void setLineWidth(uint16_t widthIn8ths);
	//void setLsShader(const LsStageRegisters *lsRegs, uint32_t shaderModifier);
	//void setMarker(const char *debugString);
	//void setMarker(const char *debugString, uint32_t argbColor);
	//void setMarker(const char *debugString);
	//void setMarker(const char *debugString, uint32_t argbColor);
	//void setNumInstances(uint32_t numInstances);
	//void setObjectIdMode(ObjectIdSource objIdSource, AddPrimitiveId addPrimitiveId);
	//void setObjectId(uint32_t id);
	//void setObjectIdMode(ObjectIdSource objIdSource, AddPrimitiveId addPrimitiveId);
	//void setPerfCounterControl(PerfCounterControl control);
	//void setPerfmonEnable(PerfmonEnable enable);
	//void setPersistentRegister(ShaderType shaderType, uint32_t regAddr, uint32_t regValue);
	//void setPersistentRegisterRange(ShaderType shaderType, uint32_t regAddr, const uint32_t *pRegValues, uint32_t numValues);
	void setPointerInUserData(ShaderStage stage, uint32_t startUserDataSlot, void *gpuAddr);
	//void setPointMinMax(uint16_t minRadius, uint16_t maxRadius);
	//void setPointSize(uint16_t halfWidth, uint16_t halfHeight);
	//void setPolygonOffsetBack(float scale, float offset);
	//void setPolygonOffsetClamp(float clamp);
	//void setPolygonOffsetFront(float scale, float offset);
	//void setPolygonOffsetZFormat(ZFormat format);
	//void setPredication(void *condAddr, uint32_t predCountInDwords);
	//void setPrimitiveIdEnable(bool enable);
	//void setPrimitiveResetIndexEnable(bool enable);
	//void setPrimitiveResetIndex(uint32_t resetIndex);
	//void setPrimitiveResetIndexEnable(bool enable);
	//void setPrimitiveSetup(PrimitiveSetup reg);
	//void setPrimitiveType(PrimitiveType primType);
	//void setPsShaderRate(PsShaderRate rate);
	//void setPsShaderSampleExclusionMask(uint16_t mask);
	//void setPsShaderUsage(const uint32_t *inputTable, uint32_t numItems);
	void setPsShader(const PsStageRegisters *psRegs);
	//void setPsShaderRate(PsShaderRate rate);
	//void setPsShaderSampleExclusionMask(uint16_t mask);
	//void setPsShaderUsage(const uint32_t *inputTable, uint32_t numItems);
	//void setRegisterRange(uint32_t header,
	//					  uint32_t regOffset,
	//					  const uint32_t *pData,
	//					  uint32_t numDword);
	//void setRenderOverride2Control(RenderOverride2Control renderOverride2Control);
	//void setRenderOverrideControl(RenderOverrideControl renderOverrideControl);
	//void setRenderTarget(uint32_t rtSlot, RenderTarget const *target);
	//void setRenderTargetMask(uint32_t mask);
	//void setRenderTargetMask(uint32_t mask);
	//void setScaledResolutionGrid(const ScaledResolutionGridAxis xAxisLeftEye, const ScaledResolutionGridAxis xAxisRightEye, const ScaledResolutionGridAxis yAxisBothEyes);
	//void setScanModeControl(ScanModeControlAa msaa, ScanModeControlViewportScissor viewportScissor);
	//void setScreenScissor(int32_t left, int32_t top, int32_t right, int32_t bottom);
	//void setSsharpInUserData(ShaderStage stage, uint32_t startUserDataSlot, const Sampler *sampler);
	//void setStencilClearValue(uint8_t clearValue);
	//void setStencil(StencilControl stencilControl);
	//void setStencilSeparate(StencilControl front, StencilControl back);
	//void setStencilOpControl(StencilOpControl stencilControl);
	//void setStencilClearValue(uint8_t clearValue);
	//void setStencilOpControl(StencilOpControl stencilControl);
	//void setStencilSeparate(StencilControl front, StencilControl back);
	//void setStreamoutBufferDimensions(StreamoutBufferId bufferId, uint32_t bufferSizeInDW, uint32_t bufferStrideInDW);
	//void setStreamoutMapping(const StreamoutBufferMapping* mapping);
	//void setTessellationDistributionThresholds(TessellationDistributionThresholds thresholds);
	//void setTextureGradientFactors(uint8_t factor00, uint8_t factor01, uint8_t factor10, uint8_t factor11,
	//							   TextureGradientFactor01SignNegationBehavior factor01sb, TextureGradientFactor10SignNegationBehavior factor10sb);
	//void setTsharpInUserData(ShaderStage stage, uint32_t startUserDataSlot, const Texture *tex);
	//void setupDrawOpaqueParameters(void *sizeLocation, uint32_t stride, uint32_t offset);
	//void setupEsGsRingRegisters(uint32_t maxExportVertexSizeInDword);
	//void setupGsVsRingRegisters(const uint32_t vertexSizePerStreamInDword[4], uint32_t maxOutputVertexCount);
	//void setUserClipPlane(uint32_t clipPlane, float x, float y, float z, float w);
	//void setUserConfigRegisterWithIndex(uint32_t regAddr, uint32_t regValue, uint32_t index);
	//void setUserConfigRegister(uint32_t regAddr, uint32_t regValue);
	//void setUserConfigRegisterRange(uint32_t regAddr, const uint32_t *pRegValues, uint32_t numValues);
	//void setUserConfigRegisterWithIndex(uint32_t regAddr, uint32_t regValue, uint32_t index);
	void setUserData(ShaderStage stage, uint32_t userDataSlot, uint32_t data);
	void setUserDataRegion(ShaderStage stage, uint32_t startUserDataSlot, const uint32_t *userData, uint32_t numDwords);
	//void setUserDataRegion(ShaderStage stage, uint32_t startUserDataSlot, const uint32_t *userData, uint32_t numDwords);
	//void setVertexQuantization(VertexQuantizationMode quantizeMode, VertexQuantizationRoundMode roundMode, VertexQuantizationCenterMode centerMode);
	//void setVertexReuseEnable(bool enable);
	//void resetVgtControl();
	//void setVgtControl(uint8_t primGroupSizeMinusOne);
	//void setVgtControl(uint8_t primGroupSizeMinusOne, VgtPartialVsWaveMode partialVsWaveMode);
	//void setVgtControlForBase(uint8_t primGroupSizeMinusOne, VgtPartialVsWaveMode partialVsWaveMode);
	//void setVgtControlForNeo(uint8_t primGroupSizeMinusOne, WdSwitchOnlyOnEopMode wdSwitchOnlyOnEopMode, VgtPartialVsWaveMode partialVsWaveMode);
	//void resetVgtControl();
	//void setVgtControl(uint8_t primGroupSizeMinusOne);
	//void setVgtControl(uint8_t primGroupSizeMinusOne, VgtPartialVsWaveMode partialVsWaveMode);
	//void setVgtControlForBase(uint8_t primGroupSizeMinusOne, VgtPartialVsWaveMode partialVsWaveMode);
	//void setVgtControlForNeo(uint8_t primGroupSizeMinusOne, WdSwitchOnlyOnEopMode wdSwitchOnlyOnEopMode, VgtPartialVsWaveMode partialVsWaveMode);
	//void setVgtControlForBase(uint8_t primGroupSizeMinusOne, VgtPartialVsWaveMode partialVsWaveMode);
	//void setVgtControlForNeo(uint8_t primGroupSizeMinusOne, WdSwitchOnlyOnEopMode wdSwitchOnlyOnEopMode, VgtPartialVsWaveMode partialVsWaveMode);
	//void setViewportTransformControl(ViewportTransformControl vportControl);
	//void setViewportScissor(uint32_t viewportId, uint32_t left, uint32_t top, uint32_t right, uint32_t bottom, WindowOffsetMode windowOffsetEnable);
	//void setViewport(uint32_t viewportId, float dmin, float dmax, const float scale[3], const float offset[3]);
	//void setViewportScissor(uint32_t viewportId, uint32_t left, uint32_t top, uint32_t right, uint32_t bottom, WindowOffsetMode windowOffsetEnable);
	//void setViewportTransformControl(ViewportTransformControl vportControl);
	//void setVsharpInUserData(ShaderStage stage, uint32_t startUserDataSlot, const Buffer *buffer);
	//void setVsShaderStreamoutEnable(bool enable);
	void setVsShader(const VsStageRegisters *vsRegs, uint32_t shaderModifier);
	//void setVsShaderStreamoutEnable(bool enable);
	//void setWindowOffset(int16_t offsetX, int16_t offsetY);
	//void setWindowScissor(uint32_t left, uint32_t top, uint32_t right, uint32_t bottom, WindowOffsetMode windowOffsetEnable);
	//void setZPassPredicationDisable(void);
	//void setZPassPredicationEnable(OcclusionQueryResults *queryResults, PredicationZPassHint hint, PredicationZPassAction action);
	//void signalSemaphore(uint64_t* semAddr, SemaphoreSignalBehavior behavior, SemaphoreUpdateConfirmMode updateConfirm);
	//void stallCommandBufferParser();
	//void triggerEndOfPipeInterrupt(EndOfPipeEventType eventType, CacheAction cacheAction);
	//void triggerEvent(EventType eventType);
	//void updateGsShader(const GsStageRegisters *gsRegs);
	//void updateHsShader(const HsStageRegisters *hsRegs, const TessellationRegisters *tessRegs);
	//void updatePsShader(const PsStageRegisters *psRegs);
	//void updateVsShader(const VsStageRegisters *vsRegs, uint32_t shaderModifier);
	void waitForGraphicsWrites(uint32_t baseAddr256, uint32_t sizeIn256ByteBlocks, uint32_t targetMask, CacheAction cacheAction, uint32_t extendedCacheMask,
							   StallCommandBufferParserMode commandBufferStallMode);
	//void waitForSetupDispatchDrawKickRingBuffer(uint32_t krbCount, uint32_t gdsDwOffsetKrb, uint32_t gdsDwOffsetKrbCounters, void *addrIrb, uint32_t sizeofIrbInBytes);
	//void waitOnAddress(void *gpuAddr, uint32_t mask, WaitCompareFunc compareFunc, uint32_t refValue);
	//void waitOnAddressAndStallCommandBufferParser(void *gpuAddr, uint32_t mask, uint32_t refValue);
	//void waitOnAddressAndStallCommandBufferParser(void *gpuAddr, uint32_t mask, uint32_t refValue);
	//void waitOnCe();
	//void waitOnRegister(uint16_t gpuReg, uint32_t mask, WaitCompareFunc compareFunc, uint32_t refValue);
	//void waitSemaphore(uint64_t* semAddr, SemaphoreWaitBehavior behavior);
	//void waitUntilSafeForRendering(uint32_t videoOutHandle, uint32_t displayBufferIndex);
	void writeAtEndOfPipe(EndOfPipeEventType eventType,
		EventWriteDest dstSelector, void *dstGpuAddr,
		EventWriteSource srcSelector, uint64_t immValue,
		CacheAction cacheAction, CachePolicy cachePolicy);
	void writeAtEndOfPipeWithInterrupt(EndOfPipeEventType eventType,
		EventWriteDest dstSelector, void *dstGpuAddr,
		EventWriteSource srcSelector, uint64_t immValue,
		CacheAction cacheAction, CachePolicy cachePolicy);
	//void writeAtEndOfShader(EndOfShaderEventType eventType, void *dstGpuAddr, uint32_t immValue);
	//void writeDataInline(void *dstGpuAddr, const void *data, uint32_t sizeInDwords, WriteDataConfirmMode writeConfirm);
	//void writeDataInlineThroughL2(void *dstGpuAddr, const void *data, uint32_t sizeInDwords, CachePolicy cachePolicy, WriteDataConfirmMode writeConfirm);
	//void writeDataInlineThroughL2(void *dstGpuAddr, const void *data, uint32_t sizeInDwords, CachePolicy cachePolicy, WriteDataConfirmMode writeConfirm);
	//void writeEventStats(EventStats eventStats, void *dstGpuAddr);
	//void writeOcclusionQuery(OcclusionQueryOp queryOp, OcclusionQueryResults *queryResults);
	//void writeStreamoutBufferOffset(StreamoutBufferId buffer, uint32_t offset);
	//void writeStreamoutBufferUpdate(StreamoutBufferId buffer, StreamoutBufferUpdateWrite sourceSelect,
	//								StreamoutBufferUpdateSaveFilledSize	updateMemory, void *dstAddr, uint64_t srcAddrOrImm);
	//void writeWaitMemCmd(
	//	uint32_t function,
	//	uint32_t engine,
	//	uint64_t pollRegAddrOrMemOffset,
	//	uint32_t reference,
	//	uint32_t mask,
	//	uint32_t interval
	//);



private:

};


