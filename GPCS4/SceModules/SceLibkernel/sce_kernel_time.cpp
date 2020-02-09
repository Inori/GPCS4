#include "sce_libkernel.h"
#include "Platform/PlatformUtils.h"
#include <ctime>

LOG_CHANNEL(SceModules.SceLibkernel.time);

uint64_t PS4API sceKernelGetProcessTime(void)
{
	LOG_SCE_DUMMY_IMPL();
	return 0x123456;
}


uint64_t PS4API sceKernelGetProcessTimeCounter(void)
{
	uint64_t nCount = UtilProcess::GetProcessTimeCounter();
	//LOG_SCE_TRACE("process time counter %lld", nCount);
	return nCount;
}


uint64_t PS4API sceKernelGetProcessTimeCounterFrequency(void)
{
	uint64_t nFreq = UtilProcess::GetProcessTimeFrequency();
	LOG_SCE_TRACE("process time frequency %lld", nFreq);
	return nFreq;
}


uint64_t PS4API sceKernelGetTscFrequency(void)
{
	uint64_t nFreq = UtilHardware::GetTscFrequency();
	LOG_SCE_TRACE("freq %llx", nFreq);
	return nFreq;
}


uint64_t PS4API sceKernelReadTsc(void)
{
	uint64_t nCount = UtilProcess::GetProcessTimeCounter();
	//LOG_SCE_TRACE("tsc %lld", nCount);
	return nCount;
}



//////////////////////////////////////////////////////////////////////////
// library: libScePosix
//////////////////////////////////////////////////////////////////////////

int PS4API scek_clock_gettime(sce_clockid_t clk_id, struct sce_timespec * tp)
{
	//LOG_SCE_TRACE("id %d tp %p", clk_id, tp);
	// TODO:
	// TIME_UTC is CLOCK_REALTIME,
	// we need CLOCK_MONOTONIC
	struct timespec ts;
	memset(tp, 0, sizeof(*tp));
	timespec_get(&ts, TIME_UTC);
	tp->tv_sec = ts.tv_sec;
	tp->tv_nsec = ts.tv_nsec;
	return 0;
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


int PS4API scek_usleep(sce_useconds_t microsecond)
{
	LOG_SCE_TRACE("micro second %d", microsecond);
	UtilTime::MicroSleep(microsecond);
	return 0;
}

