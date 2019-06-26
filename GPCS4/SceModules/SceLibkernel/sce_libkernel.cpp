#include "sce_libkernel.h"
#include "pthreads4w/pthread.h"
#include "Platform/PlatformUtils.h"

// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs



//////////////////////////////////////////////////////////////////////////
// library: libkernel
//////////////////////////////////////////////////////////////////////////

int PS4API __error(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API __stack_chk_fail(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API __stack_chk_guard(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API __tls_get_addr(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}



int PS4API sceKernelClockGettime(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}



int PS4API sceKernelCreateEqueue(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceKernelCreateEventFlag(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceKernelCreateSema(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceKernelDeleteEqueue(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceKernelDeleteEventFlag(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceKernelDeleteSema(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}



int PS4API sceKernelGetCpumode(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceKernelGetProcessTime(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceKernelGetProcessTimeCounter(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceKernelGetProcessTimeCounterFrequency(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


uint64_t PS4API sceKernelGetTscFrequency(void)
{
	uint64_t nFreq = UtilHardware::GetTscFrequency();
	LOG_SCE_TRACE("freq %llx", nFreq);
	return nFreq;
}



// Is PS4 Pro
int PS4API sceKernelIsNeoMode(void)
{
	int isNeoMode = 1;
	LOG_SCE_TRACE("return %d", isNeoMode);
	return isNeoMode;
}



int PS4API sceKernelReadTsc(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}



int PS4API sceKernelSetEventFlag(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceKernelSignalSema(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceKernelUsleep(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceKernelWaitEqueue(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceKernelWaitEventFlag(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceKernelWaitSema(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceKernelBatchMap(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceKernelCheckedReleaseDirectMemory(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceKernelDlsym(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceKernelGetDirectMemoryType(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceKernelGetGPI(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceKernelGetPageTableStats(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceKernelGettimeofday(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceKernelIsStack(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceKernelLoadStartModule(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceKernelMapNamedDirectMemory(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceKernelMapNamedFlexibleMemory(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceKernelMprotect(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceKernelMtypeprotect(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceKernelMunmap(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceKernelQueryMemoryProtection(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceKernelReserveVirtualRange(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceKernelSetPrtAperture(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceKernelSetVirtualRangeName(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceKernelUuidCreate(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceKernelVirtualQuery(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scePthreadCancel(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scePthreadDetach(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scePthreadGetname(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scePthreadGetschedparam(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scePthreadGetspecific(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scePthreadKeyCreate(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scePthreadMutexTimedlock(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scePthreadMutexTrylock(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scePthreadSetschedparam(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scePthreadSetspecific(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


//////////////////////////////////////////////////////////////////////////
// library: libSceCoredump
//////////////////////////////////////////////////////////////////////////

int PS4API sceCoredumpAttachMemoryRegion(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceCoredumpRegisterCoredumpHandler(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceCoredumpWriteUserData(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}



//////////////////////////////////////////////////////////////////////////
// library: libScePosix
//////////////////////////////////////////////////////////////////////////

int PS4API scek_clock_gettime(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scek_gettimeofday(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scek_nanosleep(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}



int PS4API scek_sched_yield(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scek_usleep(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scek_close(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scek_connect(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scek_pthread_setschedparam(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scek_recv(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scek_select(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scek_sem_destroy(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scek_sem_init(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scek_sem_post(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scek_sem_timedwait(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scek_sem_wait(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scek_send(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scek_shutdown(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}






