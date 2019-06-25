#include "sce_libkernel.h"
#include "sce_kernel_memory.h"
#include "Platform/UtilMemory.h"

int PS4API sceKernelAllocateDirectMemory(sce::off_t searchStart, sce::off_t searchEnd, 
	size_t len, size_t alignment, int memoryType, sce::off_t *physAddrOut)
{
	LOG_FIXME("Dummy implemented");
	*physAddrOut = (sce::off_t)new byte[len];
	return SCE_OK;
}


int PS4API sceKernelMapDirectMemory(void **addr, size_t len, int prot, int flags, 
	sce::off_t directMemoryStart, size_t maxPageSize)
{
	LOG_FIXME("Dummy implemented");
	*addr = (void*)directMemoryStart;
	return SCE_OK;
}


size_t PS4API sceKernelGetDirectMemorySize(void)
{
	size_t retSize = SCE_KERNEL_MAIN_DMEM_SIZE;
	LOG_SCE_TRACE("size %x", retSize);
	return retSize;
}


int PS4API sceKernelReleaseDirectMemory(sce::off_t start, size_t len)
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
