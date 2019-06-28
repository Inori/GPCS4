#include "sce_libkernel.h"
#include "Platform/PlatformUtils.h"


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


int PS4API sceKernelReadTsc(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}

