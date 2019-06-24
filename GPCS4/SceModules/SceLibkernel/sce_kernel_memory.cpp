#include "sce_libkernel.h"
#include "sce_kernel_memory.h"
#include "Platform/UtilMemory.h"

int PS4API sceKernelAllocateDirectMemory(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceKernelMapDirectMemory(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceKernelGetDirectMemorySize(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceKernelReleaseDirectMemory(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}

//////////////////////////////////////////////////////////////////////////

int PS4API sceKernelMapFlexibleMemory(void **addrInOut, size_t len, int prot, int flags)
{
	int err = SCE_KERNEL_ERROR_UNKNOWN;
	do 
	{
		if (!addrInOut || !len || !IS_ALIGNED(len, SCE_LOGICAL_PAGE_SIZE))
		{
			err = SCE_KERNEL_ERROR_EINVAL;
			break;
		}

		if (flags & SCE_KERNEL_MAP_FIXED && !IS_ALIGNED((uint64)*addrInOut, SCE_LOGICAL_PAGE_SIZE))
		{
			err = SCE_KERNEL_ERROR_EINVAL;
			break;
		}

		// TODO:
		// 1. we should use the correct flag and protection type
		// 2. maybe we should fix memory range
		void* pAddr = UtilMemory::VMMapEx(*addrInOut, len,
			UtilMemory::VMPF_READ_WRITE, UtilMemory::VMAT_RESERVE_COMMIT);
		if (!pAddr)
		{
			err = SCE_KERNEL_ERROR_ENOMEM;
			break;
		}

		*addrInOut = pAddr;
		err = SCE_OK;
	} while (false);
	return err;
}


int PS4API sceKernelReleaseFlexibleMemory(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}
