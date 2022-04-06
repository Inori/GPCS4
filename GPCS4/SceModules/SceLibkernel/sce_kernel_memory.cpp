#include "sce_libkernel.h"
#include "sce_kernel_memory.h"
#include "UtilMath.h"
#include "Emulator.h"
#include "Emulator/VirtualCPU.h"

#include <cstring>

LOG_CHANNEL(SceModules.SceLibkernel.memory);

int PS4API sceKernelAllocateDirectMemory(sce_off_t searchStart, sce_off_t searchEnd,
	size_t len, size_t alignment, int memoryType, sce_off_t *physAddrOut)
{
	auto& allocator = CPU().allocator();
	int err = allocator.allocateDirectMemory(
		searchStart, searchEnd, len, alignment, memoryType, physAddrOut);
	LOG_SCE_TRACE("physAddrOut %llx", *physAddrOut);
	return err;
}


int PS4API sceKernelMapDirectMemory(void **addr, size_t len, int prot, int flags,
	sce_off_t directMemoryStart, size_t maxPageSize)
{
	LOG_SCE_DUMMY_IMPL();
	auto& allocator = CPU().allocator();
	int err = allocator.mapDirectMemory(
		addr, len, prot, flags, directMemoryStart, maxPageSize);
	LOG_SCE_TRACE("addr:%llx, len:%zu", *addr, len);
	return err;
}


size_t PS4API sceKernelGetDirectMemorySize(void)
{
	size_t retSize = SCE_KERNEL_MAIN_DMEM_SIZE;
	LOG_SCE_TRACE("size %llx", retSize);
	return retSize;
}


int PS4API sceKernelReleaseDirectMemory(sce_off_t start, size_t len)
{
	LOG_SCE_TRACE("start:%llx, len:%zu", start, len);
	auto& allocator = CPU().allocator();
	return allocator.checkedReleaseDirectMemory(start, len);
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
	auto& allocator = CPU().allocator();
	int   err       = allocator.mapFlexibleMemory(addrInOut, len, prot, flags);
	LOG_SCE_TRACE("addrInOut:%llx, len:%zu name:%s", *addrInOut, len, name);
	return err;
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


int PS4API sceKernelMunmap(void* addr, size_t len)
{
	LOG_SCE_TRACE("addr:%llx, len:%zu", addr, len);
	auto& allocator = CPU().allocator();
	return allocator.memoryUnmap(addr, len);
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
	auto& allocator = CPU().allocator();
	return allocator.queryMemoryProtection(addr, start, end, prot);
}


int PS4API sceKernelReserveVirtualRange(void **addr, size_t len, int flags, size_t alignment)
{
	auto& allocator = CPU().allocator();
	int   err       = allocator.reserveVirtualRange(addr, len, flags, alignment);
	LOG_SCE_TRACE("addr:%llx, len:%zu", *addr, len);
	return err;
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
	auto& allocator = CPU().allocator();
	int   err       = allocator.mapFlexibleMemory(addrInOut, len, prot, flags);
	LOG_SCE_TRACE("addrInOut:%llx, len:%zu", *addrInOut, len);
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
	auto& allocator = CPU().allocator();
	void* p         = allocator.sce_mmap(start, length, prot, flags, fd, offset);
	LOG_SCE_TRACE("%p, 0x%lx, 0x%x, 0x%x, %d, %ld = %p", start, length, prot, flags, fd, offset, p);
	return p;
}
