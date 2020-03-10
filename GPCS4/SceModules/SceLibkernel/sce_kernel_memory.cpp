#include "sce_libkernel.h"
#include "sce_kernel_memory.h"
#include "UtilMath.h"
#include "Platform/UtilMemory.h"
#include <cstring>

LOG_CHANNEL(SceModules.SceLibkernel.memory);

int PS4API sceKernelAllocateDirectMemory(sce_off_t searchStart, sce_off_t searchEnd,
	size_t len, size_t alignment, int memoryType, sce_off_t *physAddrOut)
{
	LOG_SCE_DUMMY_IMPL();
	*physAddrOut = (uint64_t)UtilMemory::VMAllocateDirect();
	return SCE_OK;
}


int PS4API sceKernelMapDirectMemory(void **addr, size_t len, int prot, int flags,
	sce_off_t directMemoryStart, size_t maxPageSize)
{
	LOG_SCE_DUMMY_IMPL();
	*addr = UtilMemory::VMMapDirect(len, prot, UtilMemory::VMAT_RESERVE_COMMIT);
	return *addr == nullptr ? SCE_KERNEL_ERROR_ENOMEM : SCE_OK;
}


size_t PS4API sceKernelGetDirectMemorySize(void)
{
	size_t retSize = SCE_KERNEL_MAIN_DMEM_SIZE;
	LOG_SCE_TRACE("size %llx", retSize);
	return retSize;
}


int PS4API sceKernelReleaseDirectMemory(sce_off_t start, size_t len)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceKernelGetDirectMemoryType(sce_off_t start, int *memoryType, 
	sce_off_t *regionStartOut, sce_off_t *regionEndOut)
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
	sce_off_t directMemoryStart, size_t alignment, 
	const char *name)
{
	LOG_SCE_DUMMY_IMPL();
	*addr = (void*)directMemoryStart;
	return SCE_OK;
}


int PS4API sceKernelMapNamedFlexibleMemory(void** addrInOut, size_t len, int prot, int flags, const char* name)
{
	LOG_SCE_DUMMY_IMPL();
	void* direct = malloc(len);
	*addrInOut   = direct;  // TODO: alignment, prot, flags
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


int PS4API sceKernelMlock(const void *addr, size_t len)
{
	LOG_SCE_DUMMY_IMPL();
	LOG_DEBUG("addr %p len %zd", addr, len);
	return SCE_OK;
}


int PS4API sceKernelQueryMemoryProtection(void* addr, void** start, void** end, uint32_t* prot)
{
	LOG_SCE_TRACE("%p", addr);
	return UtilMemory::VMQueryProtection(addr, start, end, prot);
}


int PS4API sceKernelReserveVirtualRange(void **addr, size_t len, int flags, size_t alignment)
{
	LOG_SCE_DUMMY_IMPL();
	*addr = new uint8_t[1024 * 1024 * 256];
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

		if (flags & SCE_KERNEL_MAP_FIXED && !util::isAligned((uint64_t)*addrInOut, (size_t)SCE_LOGICAL_PAGE_SIZE))
		{
			err = SCE_KERNEL_ERROR_EINVAL;
			break;
		}

		// TODO:
		// 1. we should use the correct flag and protection type
		// 2. maybe we should fix memory range
		void* pAddr = UtilMemory::VMMapFlexible(*addrInOut, len, UtilMemory::VMPF_READ_WRITE);
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

int PS4API sceKernelIsAddressSanitizerEnabled(void)
{
	LOG_FIXME("Not implemented");
	return 0;
}


void* PS4API scek_mmap(void* start, size_t length, uint32_t prot, uint32_t flags, int fd, int64_t offset) 
{
	auto addr = UtilMemory::VMMap(start, length, prot, flags, fd, offset);
	LOG_SCE_TRACE("%p, 0x%lx, 0x%x, 0x%x, %d, %ld = %p", start, length, prot, flags, fd, offset, addr);
	return addr;
}
