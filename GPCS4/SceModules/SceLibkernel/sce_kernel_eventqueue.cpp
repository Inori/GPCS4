#include "sce_libkernel.h"
#include "Platform/UtilTime.h"

LOG_CHANNEL(SceModules.SceLibkernel.eventqueue);

int PS4API sceKernelCreateEqueue(SceKernelEqueue *eq, const char *name)
{
	LOG_SCE_GRAPHIC("eq %p, name %s", eq, name);
	*eq = (void*)0xCAFEBABE;
	LOG_DEBUG("name %s", name);
	return SCE_OK;
}


int PS4API sceKernelDeleteEqueue(SceKernelEqueue eq)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceKernelWaitEqueue(SceKernelEqueue eq, SceKernelEvent *ev, 
	int num, int *out, SceKernelUseconds *timo)
{
	LOG_SCE_GRAPHIC("eq %p, num %d", eq, num);
	if (out)
	{
		*out = 1;
	}

	UtilTime::MicroSleep(10000);
	return SCE_OK;
}