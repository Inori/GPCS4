#include "sce_libkernel.h"

int PS4API sceKernelCreateEqueue(SceKernelEqueue *eq, const char *name)
{
	LOG_SCE_DUMMY_IMPL();
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
	LOG_FIXME("Not implemented");
	return SCE_OK;
}