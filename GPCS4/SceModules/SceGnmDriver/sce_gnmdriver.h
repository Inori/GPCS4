/*
 *    GPCS4
 *    
 *    This file implements:
 *    module: libSceGnmDriver
 *        library: libSceGnmDriver
 *    
 */

#pragma once

#include "sce_module_common.h"
#include "Graphic/Gnm/GnmConstant.h"
#include "Graphic/Gnm/GnmStructure.h"

extern const SCE_EXPORT_MODULE g_ExpModuleSceGnmDriver;


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs



//////////////////////////////////////////////////////////////////////////
// library: libSceGnmDriver
//////////////////////////////////////////////////////////////////////////

int PS4API _import_145559702BB7CD65(void);


int PS4API _import_8A6D99B88B5A6EEE(void);


int PS4API _import_B616CF706EC4EEA9(void);


int PS4API sceGnmAddEqEvent(void);


int PS4API sceGnmAreSubmitsAllowed(void);


int PS4API sceGnmBeginWorkload(void);


int PS4API sceGnmCreateWorkloadStream(void);


int PS4API sceGnmDebugHardwareStatus(void);


int PS4API sceGnmDeleteEqEvent(void);


int PS4API sceGnmDestroyWorkloadStream(void);


int PS4API sceGnmDingDong(uint32_t vqueueId, uint32_t nextStartOffsetInDw);


int PS4API sceGnmDingDongForWorkload(void);


int PS4API sceGnmDisableMipStatsReport(void);


int PS4API sceGnmDispatchDirect(uint32_t* cmdBuffer, uint32_t numDwords, uint32_t threadGroupX, uint32_t threadGroupY, uint32_t threadGroupZ, uint32_t pred);


int PS4API sceGnmDispatchIndirect(uint32_t *cmdBuffer,
								  uint32_t numDwords,
								  uint32_t dataOffsetInBytes,
								  uint32_t flag);


int PS4API sceGnmDispatchIndirectOnMec(void);


uint32_t PS4API sceGnmDispatchInitDefaultHardwareState(uint32_t* cmdBuffer, uint32_t numDwords);


int PS4API sceGnmDrawIndex(uint32_t* cmdBuffer, uint32_t numDwords,
	uint32_t indexCount, const void *indexAddr, uint32_t predAndMod, uint32_t inlineMode);


int PS4API sceGnmDrawIndexAuto(uint32_t* cmdBuffer, uint32_t numDwords,
	uint32_t indexCount, uint32_t pred);


int PS4API sceGnmDrawIndexIndirect(uint32_t* cmdBuffer, uint32_t numDwords,
	uint32_t dataOffsetInBytes,
	ShaderStage stage,
	uint8_t vertexOffsetUserSgpr,
	uint8_t instanceOffsetUserSgpr,
	uint32_t pred);


int PS4API sceGnmDrawIndexIndirectMulti(uint32_t* cmdBuffer, uint32_t numDwords);


int PS4API sceGnmDrawIndexMultiInstanced(uint32_t* cmdBuffer, uint32_t numDwords);


int PS4API sceGnmDrawIndexOffset(uint32_t* cmdBuffer, uint32_t numDwords);


int PS4API sceGnmDrawIndirect(uint32_t* cmdBuffer, uint32_t numDwords);


int PS4API sceGnmDrawIndirectCountMulti(uint32_t* cmdBuffer, uint32_t numDwords);


int PS4API sceGnmDrawIndirectMulti(uint32_t* cmdBuffer, uint32_t numDwords);


uint32_t PS4API sceGnmDrawInitDefaultHardwareState350(uint32_t* cmdBuffer, uint64_t numDwords);


uint32_t PS4API sceGnmDrawInitDefaultHardwareState200(uint32_t* cmdBuffer, uint64_t numDwords);


uint32_t PS4API sceGnmDrawInitDefaultHardwareState175(uint32_t* cmdBuffer, uint64_t numDwords);


int PS4API sceGnmDrawOpaqueAuto(uint32_t* cmdBuffer, uint32_t numDwords);


int PS4API sceGnmDriverCaptureInProgress(void);


int PS4API sceGnmDriverTraceInProgress(void);


int PS4API sceGnmDriverTriggerCapture(void);


int PS4API sceGnmEndWorkload(void);


int PS4API sceGnmFlushGarlic(void);


int PS4API sceGnmGetEqEventType(void);


int PS4API sceGnmGetEqTimeStamp(void);


int PS4API sceGnmGetGpuBlockStatus(void);


int PS4API sceGnmGetGpuInfoStatus(void);


int PS4API sceGnmGetLastWaitedAddress(void);


int PS4API sceGnmGetNumTcaUnits(void);


int PS4API sceGnmGetProtectionFaultTimeStamp(void);


int PS4API sceGnmGetShaderProgramBaseAddress(void);


int PS4API sceGnmGetShaderStatus(void);


int PS4API sceGnmGetTheTessellationFactorRingBufferBaseAddress(void);


int PS4API sceGnmInsertDingDongMarker(void);


int PS4API sceGnmInsertPopMarker(uint32_t* cmdBuffer, uint32_t numDwords);


int PS4API sceGnmInsertPushColorMarker(void);


int PS4API sceGnmInsertPushMarker(uint32_t* cmdBuffer, uint32_t numDwords, const char *debugString);


int PS4API sceGnmInsertSetColorMarker(void);


int PS4API sceGnmInsertSetMarker(void);


int PS4API sceGnmInsertThreadTraceMarker(void);


int PS4API sceGnmInsertWaitFlipDone(uint32_t* cmdBuffer, uint32_t numDwords, int videoOutHandle, uint32_t displayBufferIndex);


bool PS4API sceGnmIsUserPaEnabled(void);


int PS4API sceGnmLogicalCuIndexToPhysicalCuIndex(void);


int PS4API sceGnmLogicalCuMaskToPhysicalCuMask(void);


//int PS4API sceGnmMapComputeQueue(uint32_t *vqueueId, uint32_t globalPipeId, uint32_t queueId, void *ringBaseAddr, uint32_t ringSizeInDW, void *readPtrAddr);
int PS4API sceGnmMapComputeQueue(uint32_t globalPipeId, uint32_t queueId, void *ringBaseAddr, uint32_t ringSizeInDW, void *readPtrAddr);


int PS4API sceGnmMapComputeQueueWithPriority(void);


int PS4API sceGnmRequestFlipAndSubmitDone(void);


int PS4API sceGnmRequestFlipAndSubmitDoneForWorkload(void);


int PS4API sceGnmRequestMipStatsReportAndReset(void);


int PS4API sceGnmResetVgtControl(uint32_t* cmdBuffer, uint32_t numDwords);


int PS4API sceGnmSetCsShaderWithModifier(uint32_t* cmdBuffer, uint32_t numDwords,
	const pssl::CsStageRegisters *csRegs, uint32_t shaderModifier);


int PS4API sceGnmSetEmbeddedPsShader(uint32_t* cmdBuffer, uint32_t numDwords);


int PS4API sceGnmSetEmbeddedVsShader(uint32_t* cmdBuffer, uint32_t numDwords, EmbeddedVsShader shaderId, uint32_t shaderModifier);


int PS4API sceGnmSetEsShader(uint32_t* cmdBuffer, uint32_t numDwords);


int PS4API sceGnmSetGsRingSizes(void);


int PS4API sceGnmSetGsShader(uint32_t* cmdBuffer, uint32_t numDwords);


int PS4API sceGnmSetHsShader(uint32_t* cmdBuffer, uint32_t numDwords);


int PS4API sceGnmSetLsShader(uint32_t* cmdBuffer, uint32_t numDwords);


int PS4API sceGnmSetPsShader350(uint32_t* cmdBuffer, uint32_t numDwords, const pssl::PsStageRegisters *psRegs);


int PS4API sceGnmSetVgtControl(uint32_t* cmdBuffer, uint32_t numDwords,
	uint8_t primGroupSizeMinusOne,
	VgtPartialVsWaveMode partialVsWaveMode,
	WdSwitchOnlyOnEopMode wdSwitchOnlyOnEopMode);


int PS4API sceGnmSetVsShader(uint32_t* cmdBuffer, uint32_t numDwords, const pssl::VsStageRegisters *vsRegs, uint32_t shaderModifier);


int PS4API sceGnmSetWaveLimitMultipliers(void);


int PS4API sceGnmSetupMipStatsReport(void);


int PS4API sceGnmSubmitAndFlipCommandBuffers(uint32_t count, void *dcbGpuAddrs[], uint32_t *dcbSizesInBytes, void *ccbGpuAddrs[], uint32_t *ccbSizesInBytes, uint32_t videoOutHandle, uint32_t displayBufferIndex, uint32_t flipMode, int64_t flipArg);


int PS4API sceGnmSubmitAndFlipCommandBuffersForWorkload(void);


int PS4API sceGnmSubmitCommandBuffers(uint32_t count, void *dcbGpuAddrs[], uint32_t *dcbSizesInBytes, void *ccbGpuAddrs[], uint32_t *ccbSizesInBytes);


int PS4API sceGnmSubmitCommandBuffersForWorkload(void);


int PS4API sceGnmSubmitDone(void);


int PS4API sceGnmUnmapComputeQueue(void);


int PS4API sceGnmUpdateGsShader(void);


int PS4API sceGnmUpdateHsShader(void);


int PS4API sceGnmUpdatePsShader350(uint32_t* cmdBuffer, uint32_t numDwords, const pssl::PsStageRegisters *psRegs);


int PS4API sceGnmUpdateVsShader(uint32_t* cmdBuffer, uint32_t numDwords, const pssl::VsStageRegisters *vsRegs, uint32_t shaderModifier);


int PS4API sceRazorCaptureImmediate(void);


int PS4API sceRazorIsLoaded(void);


int PS4API _import_75EC17C39AE82ECD(void);


int PS4API sceGnmComputeWaitOnAddress(void);


int PS4API sceGnmComputeWaitSemaphore(void);


int PS4API sceGnmLogicalTcaUnitToPhysical(void);


int PS4API sceGnmFindResourcesPublic(void);


int PS4API sceGnmGetOwnerName(void);


int PS4API sceGnmGetResourceBaseAddressAndSizeInBytes(void);


int PS4API sceGnmGetResourceName(void);


int PS4API sceGnmGetResourceShaderGuid(void);


int PS4API sceGnmGetResourceType(void);


int PS4API sceGnmGetResourceUserData(void);


int PS4API sceGnmQueryResourceRegistrationUserMemoryRequirements(void);


int PS4API sceGnmRegisterGdsResource(void);


int PS4API sceGnmRegisterOwner(void);


int PS4API sceGnmRegisterResource(void);


int PS4API sceGnmSetResourceRegistrationUserMemory(void);


int PS4API sceGnmSetResourceUserData(void);


int PS4API sceGnmUnregisterAllResourcesForOwner(void);


int PS4API sceGnmUnregisterOwnerAndResources(void);


int PS4API sceGnmUnregisterResource(void);


int PS4API _import_060337B772EF70D9(void);


int PS4API sceGnmValidateDisableDiagnostics(void);


int PS4API sceGnmValidateDispatchCommandBuffers(void);


int PS4API sceGnmValidateDrawCommandBuffers(void);


int PS4API sceGnmValidateGetDiagnosticInfo(void);


int PS4API sceGnmValidateGetDiagnostics(void);


int PS4API sceGnmValidateGetVersion(void);


int PS4API sceGnmValidateOnSubmitEnabled(void);


int PS4API sceGnmValidateResetState(void);


int PS4API sceGnmValidationRegisterMemoryCheckCallback(void);


int PS4API sceGnmSetCsShader(uint32_t* cmdBuffer, uint32_t numDwords, const pssl::CsStageRegisters *csRegs, uint32_t shaderModifier);


int PS4API sceGnmSetPsShader(uint32_t* cmdBuffer, uint32_t numDwords, const pssl::PsStageRegisters *psRegs);


int PS4API sceGnmUpdatePsShader(uint32_t* cmdBuffer, uint32_t numDwords, const pssl::PsStageRegisters *psRegs);


int PS4API sceGnmValidateCommandBuffers(void);


int PS4API sceRazorCaptureCommandBuffersOnlyImmediate(void);


int PS4API sceRazorCaptureCommandBuffersOnlySinceLastFlip(void);


int PS4API sceRazorCaptureSinceLastFlip(void);



