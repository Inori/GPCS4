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




