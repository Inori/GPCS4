#include "sce_libkernel.h"
#include "SceSemaphore.h"

LOG_CHANNEL(SceModules.SceLibkernel.semaphore);

int PS4API sceKernelCreateSema(SceKernelSema *sem, const char *name, 
	uint32_t attr, int init, int max, const SceKernelSemaOptParam *opt)
{
	LOG_SCE_TRACE("sem %p name %s attr %d init %d max %d opt %p", sem, name, attr, init, max, opt);
	*sem = (SceKernelSema)new CSceSemaphore(name, init, max, attr);
	return SCE_OK;
}


int PS4API sceKernelDeleteSema(SceKernelSema sem)
{
	LOG_SCE_TRACE("sem %p", sem);
	//delete (CSceSemaphore*)sem;
	return SCE_OK;
}


int PS4API sceKernelSignalSema(SceKernelSema sem, int count)
{
	LOG_SCE_TRACE("sem %p count %d", sem, count);
	int err = SCE_KERNEL_ERROR_UNKNOWN;
	do 
	{
		if (!sem)  // nier:automata will use empty sem, very strange...
		{
			err = SCE_KERNEL_ERROR_ESRCH;
			break;
		}

		err = ((CSceSemaphore*)sem)->Signal(count);
	} while (false);
	return err;
}


int PS4API sceKernelWaitSema(SceKernelSema sem, int need, SceKernelUseconds *timo)
{
	LOG_SCE_TRACE("sem %p need %d time %d", sem, need, timo ? *timo : 0);
	return ((CSceSemaphore*)sem)->Wait(need, timo);
}

