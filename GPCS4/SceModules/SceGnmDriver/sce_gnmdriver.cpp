#include "sce_gnmdriver.h"
#include "Graphic/SceVideoOut.h"

// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs


extern CSceVideoOut* g_VideoOutHanleMap[3];

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


int PS4API sceGnmAddEqEvent(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmAreSubmitsAllowed(void)
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


int PS4API sceGnmDeleteEqEvent(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmDestroyWorkloadStream(void)
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


int PS4API sceGnmDisableMipStatsReport(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmDispatchDirect(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmDispatchIndirect(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmDispatchIndirectOnMec(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}



int PS4API sceGnmDrawIndex(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmDrawIndexAuto(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmDrawIndexIndirect(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmDrawIndexIndirectMulti(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmDrawIndexMultiInstanced(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmDrawIndexOffset(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmDrawIndirect(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmDrawIndirectCountMulti(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmDrawIndirectMulti(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmDispatchInitDefaultHardwareState(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return 0;
}

// TODO:
// from reverse engining of libSceGnmDriverForNeoMode.sprx,
// it seems that the 'pStruct' points to a complex structure.
// and 'size' is some what a size or offset.
// and this function returns a size value, depend on an ioctrl syscall
// the returned value will affect the address passed to sceGnmInsertWaitFlipDone
//
/*
LOAD:0000000000004700                 cmp     byte ptr [rax], 0  ; [rax] depend on a ioctrl
LOAD:0000000000004703                 mov     eax, 0C0017900h
LOAD:0000000000004708                 cmovz   edx, eax
LOAD:000000000000470B                 mov     eax, 40000258h
LOAD:0000000000004710                 cmovz   esi, eax
LOAD:0000000000004713                 mov     eax, 0D00FFh
LOAD:0000000000004718                 mov     [rcx+20Ch], edx
LOAD:000000000000471E                 mov     edx, 100h
LOAD:0000000000004723                 cmovz   eax, r8d
LOAD:0000000000004727                 mov     [rcx+210h], esi
LOAD:000000000000472D                 mov     [rcx+214h], eax
LOAD:0000000000004733                 lea     rax, [rcx+218h]
LOAD:000000000000473A                 sub     rax, rdi
LOAD:000000000000473D                 shr     rax, 2
LOAD:0000000000004741                 sub     rdx, rax
LOAD:0000000000004744                 lea     rax, [rcx+rdx*4+218h]
LOAD:000000000000474C                 shl     edx, 10h
LOAD:000000000000474F                 add     edx, 3FFE0000h
LOAD:0000000000004755                 sub     rax, rdi
LOAD:0000000000004758                 or      edx, 0C0001000h
LOAD:000000000000475E                 shr     rax, 2
*/
uint64_t PS4API sceGnmDrawInitDefaultHardwareState350(void* pStruct, uint64_t size)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return 0;
}


// TODO:
// this is just a hack on Nier:Automata to let the game goes on without trapping
// into the infinity loop checking a label.
// and I can focus on more important things
//
// from reverse engining of libSceGnmDriverForNeoMode.sprx,
// this function will fill a struct pointed to by  'gpuAddress'
// the value of 'gpuAddress' is affected by the return value of sceGnmDrawInitDefaultHardwareState350
//
// the purpose of this function is to insert a wait token in the command buffer,
// and when the flip is done on gpu/kernel,
// the gpu/kernel will set the value specified by 'value' param on gpuAddress.
// I just set this value immediately.

/*
LOAD:000000000000493D      mov     dword ptr [rbx], 0C0053C00h ; rbx = gpuAddress
LOAD:0000000000004943      mov     dword ptr [rbx+4], 13h
LOAD:000000000000494A      lea     rax, [rax+rcx*8]
LOAD:000000000000494E      mov     ecx, eax
LOAD:0000000000004950      shr     rax, 20h
LOAD:0000000000004954      and     ecx, 0FFFFFFFCh
LOAD:0000000000004957      movzx   eax, ax
LOAD:000000000000495A      mov     [rbx+8], ecx
LOAD:000000000000495D      mov     [rbx+0Ch], eax
LOAD:0000000000004960      xor     eax, eax
LOAD:0000000000004962      mov     dword ptr [rbx+10h], 0
LOAD:0000000000004969      mov     dword ptr [rbx+14h], 0FFFFFFFFh
LOAD:0000000000004970      mov     dword ptr [rbx+18h], 0Ah
*/
int PS4API sceGnmInsertWaitFlipDone(void* gpuAddress, int type_or_mask, int uk, int value)
{
	LOG_SCE_GRAPHIC("gpuaddr %p type %d uk %d val %d", gpuAddress, type_or_mask, uk, value);


	static uint nCount = 0;
	static uint nMod = 0;
	*(uint32_t*)((uint8_t*)gpuAddress + 0x2000034) = value + nCount;
	*(uint32_t*)((uint8_t*)gpuAddress + 0x200008C) = value + nCount;
	++nMod;
	if (nMod % 3 == 0)
	{
		nCount += 3;
	}

	return SCE_OK;
}


int PS4API sceGnmDrawOpaqueAuto(void)
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


int PS4API sceGnmGetEqEventType(void)
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


int PS4API sceGnmInsertDingDongMarker(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmInsertPopMarker(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmInsertPushColorMarker(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmInsertPushMarker(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmInsertSetColorMarker(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmInsertSetMarker(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmInsertThreadTraceMarker(void)
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


int PS4API sceGnmResetVgtControl(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmSetCsShaderWithModifier(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmSetEmbeddedPsShader(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmSetEmbeddedVsShader(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmSetEsShader(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmSetGsRingSizes(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmSetGsShader(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmSetHsShader(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmSetLsShader(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmSetPsShader350(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmSetVgtControl(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmSetVsShader(void)
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


int PS4API sceGnmSubmitAndFlipCommandBuffers(uint32_t count, 
	void *dcbGpuAddrs[], uint32_t *dcbSizesInBytes, 
	void *ccbGpuAddrs[], uint32_t *ccbSizesInBytes, 
	uint32_t videoOutHandle, uint32_t displayBufferIndex, 
	uint32_t flipMode, int64_t flipArg)
{
	LOG_SCE_GRAPHIC("displayBuffIdx %d", displayBufferIndex);
	CSceVideoOut* pVdOut = g_VideoOutHanleMap[videoOutHandle];
	pVdOut->Flip(displayBufferIndex);
	return SCE_OK;
}


int PS4API sceGnmSubmitAndFlipCommandBuffersForWorkload(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmSubmitCommandBuffers(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmSubmitCommandBuffersForWorkload(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmSubmitDone(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmUnmapComputeQueue(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmUpdateGsShader(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmUpdateHsShader(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmUpdatePsShader350(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmUpdateVsShader(void)
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


int PS4API sceGnmComputeWaitOnAddress(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmComputeWaitSemaphore(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}


int PS4API sceGnmLogicalTcaUnitToPhysical(void)
{
	LOG_SCE_GRAPHIC("Not implemented");
	return SCE_OK;
}





