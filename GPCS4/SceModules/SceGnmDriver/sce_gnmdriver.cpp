#include "sce_gnmdriver.h"


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs




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




int PS4API sceGnmIsUserPaEnabled(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
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


int PS4API sceGnmDingDong(void)
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

		if ((ulong_ptr)ringBaseAddr % 256 != 0)
		{
			break;
		}

		if ((ulong_ptr)readPtrAddr % 4 != 0)
		{
			break;
		}

		*(sceoff_t*)readPtrAddr = 0;

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





