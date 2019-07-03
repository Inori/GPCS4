#include "sce_libkernel.h"
#include "Platform/PlatformUtils.h"
#include <ctime>

uint64_t PS4API sceKernelGetProcessTime(void)
{
	LOG_SCE_DUMMY_IMPL();
	return 0x123456;
}


uint64_t PS4API sceKernelGetProcessTimeCounter(void)
{
	uint64_t nCount = UtilProcess::GetProcessTimeCounter();
	LOG_SCE_TRACE("process time counter %lld", nCount);
	return nCount;
}


uint64_t PS4API sceKernelGetProcessTimeCounterFrequency(void)
{
	uint64 nFreq = UtilProcess::GetProcessTimeFrequency();
	LOG_SCE_TRACE("process time frequency %lld", nFreq);
	return nFreq;
}


uint64_t PS4API sceKernelGetTscFrequency(void)
{
	uint64_t nFreq = UtilHardware::GetTscFrequency();
	LOG_SCE_TRACE("freq %llx", nFreq);
	return nFreq;
}


int PS4API sceKernelReadTsc(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}



//////////////////////////////////////////////////////////////////////////
// library: libScePosix
//////////////////////////////////////////////////////////////////////////

int PS4API scek_clock_gettime(sceclockid_t clk_id, struct sce_timespec * tp)
{
	LOG_SCE_TRACE("id %d tp %p", clk_id, tp);
	// TODO:
	// TIME_UTC is CLOCK_REALTIME,
	// we need CLOCK_MONOTONIC
	struct timespec ts;
	timespec_get(&ts, TIME_UTC);
	tp->tv_sec = ts.tv_sec;
	tp->tv_nsec = ts.tv_nsec;
	return 0;
}


scetime_t PS4API scec_time(scetime_t* pt)
{
	LOG_SCE_TRACE("pt %p", pt);
	return time(pt);
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


int PS4API scek_usleep(sceuseconds_t microsecond)
{
	LOG_SCE_TRACE("micro second %d", microsecond);
	UtilTime::MicroSleep(microsecond);
	return 0;
}

