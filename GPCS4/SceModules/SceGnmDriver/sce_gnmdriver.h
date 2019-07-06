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


int PS4API sceGnmDingDong(void);


int PS4API sceGnmDingDongForWorkload(void);


int PS4API sceGnmDisableMipStatsReport(void);


int PS4API sceGnmDispatchDirect(void);


int PS4API sceGnmDispatchIndirect(void);


int PS4API sceGnmDispatchIndirectOnMec(void);


int PS4API sceGnmDispatchInitDefaultHardwareState(void);


int PS4API sceGnmDrawIndex(void);


int PS4API sceGnmDrawIndexAuto(void);


int PS4API sceGnmDrawIndexIndirect(void);


int PS4API sceGnmDrawIndexIndirectMulti(void);


int PS4API sceGnmDrawIndexMultiInstanced(void);


int PS4API sceGnmDrawIndexOffset(void);


int PS4API sceGnmDrawIndirect(void);


int PS4API sceGnmDrawIndirectCountMulti(void);


int PS4API sceGnmDrawIndirectMulti(void);


int PS4API sceGnmDrawInitDefaultHardwareState350(void);


int PS4API sceGnmDrawOpaqueAuto(void);


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


int PS4API sceGnmInsertPopMarker(void);


int PS4API sceGnmInsertPushColorMarker(void);


int PS4API sceGnmInsertPushMarker(void);


int PS4API sceGnmInsertSetColorMarker(void);


int PS4API sceGnmInsertSetMarker(void);


int PS4API sceGnmInsertThreadTraceMarker(void);


int PS4API sceGnmInsertWaitFlipDone(void);


int PS4API sceGnmIsUserPaEnabled(void);


int PS4API sceGnmLogicalCuIndexToPhysicalCuIndex(void);


int PS4API sceGnmLogicalCuMaskToPhysicalCuMask(void);


//int PS4API sceGnmMapComputeQueue(uint32_t *vqueueId, uint32_t globalPipeId, uint32_t queueId, void *ringBaseAddr, uint32_t ringSizeInDW, void *readPtrAddr);
int PS4API sceGnmMapComputeQueue(uint32_t globalPipeId, uint32_t queueId, void *ringBaseAddr, uint32_t ringSizeInDW, void *readPtrAddr);


int PS4API sceGnmMapComputeQueueWithPriority(void);


int PS4API sceGnmRequestFlipAndSubmitDone(void);


int PS4API sceGnmRequestFlipAndSubmitDoneForWorkload(void);


int PS4API sceGnmRequestMipStatsReportAndReset(void);


int PS4API sceGnmResetVgtControl(void);


int PS4API sceGnmSetCsShaderWithModifier(void);


int PS4API sceGnmSetEmbeddedPsShader(void);


int PS4API sceGnmSetEmbeddedVsShader(void);


int PS4API sceGnmSetEsShader(void);


int PS4API sceGnmSetGsRingSizes(void);


int PS4API sceGnmSetGsShader(void);


int PS4API sceGnmSetHsShader(void);


int PS4API sceGnmSetLsShader(void);


int PS4API sceGnmSetPsShader350(void);


int PS4API sceGnmSetVgtControl(void);


int PS4API sceGnmSetVsShader(void);


int PS4API sceGnmSetWaveLimitMultipliers(void);


int PS4API sceGnmSetupMipStatsReport(void);


int PS4API sceGnmSubmitAndFlipCommandBuffers(uint32_t count, void *dcbGpuAddrs[], uint32_t *dcbSizesInBytes, void *ccbGpuAddrs[], uint32_t *ccbSizesInBytes, uint32_t videoOutHandle, uint32_t displayBufferIndex, uint32_t flipMode, int64_t flipArg);


int PS4API sceGnmSubmitAndFlipCommandBuffersForWorkload(void);


int PS4API sceGnmSubmitCommandBuffers(void);


int PS4API sceGnmSubmitCommandBuffersForWorkload(void);


int PS4API sceGnmSubmitDone(void);


int PS4API sceGnmUnmapComputeQueue(void);


int PS4API sceGnmUpdateGsShader(void);


int PS4API sceGnmUpdateHsShader(void);


int PS4API sceGnmUpdatePsShader350(void);


int PS4API sceGnmUpdateVsShader(void);


int PS4API sceRazorCaptureImmediate(void);


int PS4API sceRazorIsLoaded(void);


int PS4API _import_75EC17C39AE82ECD(void);


int PS4API sceGnmComputeWaitOnAddress(void);


int PS4API sceGnmComputeWaitSemaphore(void);


int PS4API sceGnmLogicalTcaUnitToPhysical(void);




