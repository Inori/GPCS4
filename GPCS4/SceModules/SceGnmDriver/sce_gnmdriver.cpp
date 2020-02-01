#include "sce_gnmdriver.h"


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs

LOG_CHANNEL(SceModules.SceGnmDriver);

//////////////////////////////////////////////////////////////////////////
// library: libSceGnmDriver
//////////////////////////////////////////////////////////////////////////

int PS4API _import_145559702BB7CD65(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API _import_8A6D99B88B5A6EEE(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API _import_B616CF706EC4EEA9(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmBeginWorkload(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmCreateWorkloadStream(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmDebugHardwareStatus(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}



int PS4API sceGnmDestroyWorkloadStream(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}



int PS4API sceGnmDriverCaptureInProgress(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmDriverTraceInProgress(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmDriverTriggerCapture(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmEndWorkload(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmFlushGarlic(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmGetEqTimeStamp(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmGetGpuBlockStatus(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmGetGpuInfoStatus(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmGetLastWaitedAddress(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmGetNumTcaUnits(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmGetProtectionFaultTimeStamp(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmGetShaderProgramBaseAddress(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmGetShaderStatus(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmGetTheTessellationFactorRingBufferBaseAddress(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


bool PS4API sceGnmIsUserPaEnabled(void)
{
	// Checks if performance counters are available
	bool paEnable = false;
	LOG_SCE_GRAPHIC("user pa enable %d", paEnable);
	return paEnable;
}


int PS4API sceGnmLogicalCuIndexToPhysicalCuIndex(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmLogicalCuMaskToPhysicalCuMask(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmDingDong(uint32_t vqueueId, uint32_t nextStartOffsetInDw)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmDingDongForWorkload(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


// Note:
// it seems that nier:automata use an old version of this function
// and it has 5 params and the vqueueId is the return value.

//int PS4API sceGnmMapComputeQueue(uint32_t *vqueueId, 
//	uint32_t globalPipeId, uint32_t queueId, 
//	void *ringBaseAddr, uint32_t ringSizeInDW, void *readPtrAddr)

int PS4API sceGnmMapComputeQueue(uint32_t globalPipeId, uint32_t queueId,
	void *ringBaseAddr, uint32_t ringSizeInDW, void *readPtrAddr)
{
	LOG_SCE_GRAPHIC("");
	int vqueueId = 0;
	do 
	{
		if (globalPipeId >= 7)
		{
			break;
		}

		if (queueId >= 8)
		{
			break;
		}

		if ((uintptr_t)ringBaseAddr % 256 != 0)
		{
			break;
		}

		if ((uintptr_t)readPtrAddr % 4 != 0)
		{
			break;
		}

		*(sce_off_t*)readPtrAddr = 0;

		vqueueId = 0x567;
	} while (false);

	return vqueueId;
}


int PS4API sceGnmUnmapComputeQueue(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmMapComputeQueueWithPriority(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmRequestFlipAndSubmitDone(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmRequestFlipAndSubmitDoneForWorkload(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmRequestMipStatsReportAndReset(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}



int PS4API sceGnmSetGsRingSizes(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmSetWaveLimitMultipliers(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmSetupMipStatsReport(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceRazorCaptureImmediate(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceRazorIsLoaded(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API _import_75EC17C39AE82ECD(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}

int PS4API sceGnmFindResourcesPublic(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmGetOwnerName(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmGetResourceBaseAddressAndSizeInBytes(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmGetResourceName(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmGetResourceShaderGuid(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmGetResourceType(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmGetResourceUserData(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmQueryResourceRegistrationUserMemoryRequirements(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmRegisterGdsResource(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmRegisterOwner(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmRegisterResource(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmSetResourceRegistrationUserMemory(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmSetResourceUserData(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmUnregisterAllResourcesForOwner(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmUnregisterOwnerAndResources(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmUnregisterResource(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API _import_060337B772EF70D9(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmValidateDisableDiagnostics(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmValidateDispatchCommandBuffers(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmValidateDrawCommandBuffers(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmValidateGetDiagnosticInfo(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmValidateGetDiagnostics(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmValidateGetVersion(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmValidateOnSubmitEnabled(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmValidateResetState(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmValidationRegisterMemoryCheckCallback(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmValidateCommandBuffers(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}

int PS4API sceRazorCaptureCommandBuffersOnlyImmediate(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}

int PS4API sceRazorCaptureCommandBuffersOnlySinceLastFlip(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}

int PS4API sceRazorCaptureSinceLastFlip(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}





