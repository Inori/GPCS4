#include "sce_libkernel.h"
#include "sce_kernel_eventflag.h"
#include "SceEventFlag.h"

LOG_CHANNEL(SceModules.SceLibkernel.eventflag);

int PS4API sceKernelCreateEventFlag(SceKernelEventFlag *ef, const char *pName, uint32_t attr, 
	uint64_t initPattern, const SceKernelEventFlagOptParam *pOptParam)
{
	LOG_SCE_TRACE("ef %p name %s attr %x initpat %x opt %p", ef, pName, attr, initPattern, pOptParam);
	int err = SCE_KERNEL_ERROR_UNKNOWN;
	do 
	{
		if (pOptParam || !pName ||
			attr > (SCE_KERNEL_EVF_ATTR_MULTI | SCE_KERNEL_EVF_ATTR_TH_PRIO))
		{
			err = SCE_KERNEL_ERROR_EINVAL;
			break;
		}

		if (strlen(pName) >= 32)
		{
			err = SCE_KERNEL_ERROR_ENAMETOOLONG;
			break;
		}

		*ef = (SceKernelEventFlag)new CSceEventFlag(pName, attr, initPattern);

		err = SCE_OK;
	} while (false);
	return err;
}


int PS4API sceKernelDeleteEventFlag(SceKernelEventFlag ef)
{
	LOG_SCE_TRACE("ef %p", ef);
	delete (CSceEventFlag*)ef;
	return SCE_OK;
}


int PS4API sceKernelSetEventFlag(SceKernelEventFlag ef, uint64_t bitPattern)
{
	//LOG_SCE_TRACE("ef %p bitpat %x", ef, bitPattern);
	return ((CSceEventFlag*)ef)->Set(bitPattern);
}


int PS4API sceKernelWaitEventFlag(SceKernelEventFlag ef, 
	uint64_t bitPattern, uint32_t waitMode, 
	uint64_t *pResultPat, SceKernelUseconds *pTimeout)
{
	//LOG_SCE_TRACE("ef %p bitpat %x mode %x timo %p", ef, bitPattern, waitMode, pTimeout);
	return ((CSceEventFlag*)ef)->Wait(bitPattern, waitMode, pResultPat, pTimeout);
}

int PS4API sceKernelPollEventFlag(SceKernelEventFlag ef, uint64_t bitPattern, uint32_t waitMode, uint64_t* pResultPat)
{
	LOG_SCE_DUMMY_IMPL();
	return ((CSceEventFlag*)ef)->Poll(bitPattern, waitMode, pResultPat);
}