#include "sce_libkernel.h"
#include "sce_kernel_memory.h"
#include "UtilMath.h"
#include "Platform/UtilMemory.h"
#include <cstring>

int PS4API sceKernelAllocateDirectMemory(sceoff_t searchStart, sceoff_t searchEnd, 
	size_t len, size_t alignment, int memoryType, sceoff_t *physAddrOut)
{
	LOG_SCE_DUMMY_IMPL();
	*physAddrOut = (sceoff_t)UtilMemory::VMMapEx(NULL, len, UtilMemory::VMPF_READ_WRITE, UtilMemory::VMAT_RESERVE_COMMIT);
	return SCE_OK;
}


int PS4API sceKernelMapDirectMemory(void **addr, size_t len, int prot, int flags, 
	sceoff_t directMemoryStart, size_t maxPageSize)
{
	LOG_SCE_DUMMY_IMPL();
	*addr = (void*)directMemoryStart;
	return SCE_OK;
}


size_t PS4API sceKernelGetDirectMemorySize(void)
{
	size_t retSize = SCE_KERNEL_MAIN_DMEM_SIZE;
	LOG_SCE_TRACE("size %llx", retSize);
	return retSize;
}


int PS4API sceKernelReleaseDirectMemory(sceoff_t start, size_t len)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceKernelGetDirectMemoryType(sceoff_t start, int *memoryType, 
	sceoff_t *regionStartOut, sceoff_t *regionEndOut)
{
	LOG_SCE_DUMMY_IMPL();
	*memoryType = SCE_KERNEL_WB_GARLIC;
	*memoryType = SCE_KERNEL_WC_GARLIC;
	return SCE_OK;
}


int PS4API sceKernelGetPageTableStats(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceKernelIsStack(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceKernelMapNamedDirectMemory(void **addr, size_t len, 
	int prot, int flags, 
	sceoff_t directMemoryStart, size_t alignment, 
	const char *name)
{
	LOG_SCE_DUMMY_IMPL();
	*addr = (void*)directMemoryStart;
	return SCE_OK;
}


int PS4API sceKernelMapNamedFlexibleMemory(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceKernelMprotect(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceKernelMtypeprotect(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceKernelMunmap(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceKernelQueryMemoryProtection(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceKernelReserveVirtualRange(void **addr, size_t len, int flags, size_t alignment)
{
	LOG_SCE_DUMMY_IMPL();
	*addr = new byte[1024 * 1024 * 256];
	return SCE_OK;
}


int PS4API sceKernelSetVirtualRangeName(void* start, size_t len, const char *name)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceKernelVirtualQuery(const void *addr, int flags, SceKernelVirtualQueryInfo *info, size_t infoSize)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceKernelMapFlexibleMemory(void **addrInOut, size_t len, int prot, int flags)
{
	int err = SCE_KERNEL_ERROR_UNKNOWN;
	do 
	{
		if (!addrInOut || !len || !util::isAligned(len, (size_t)SCE_LOGICAL_PAGE_SIZE))
		{
			err = SCE_KERNEL_ERROR_EINVAL;
			break;
		}

		if (flags & SCE_KERNEL_MAP_FIXED && !util::isAligned((uint64)*addrInOut, (size_t)SCE_LOGICAL_PAGE_SIZE))
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
