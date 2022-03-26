#pragma once

#include "GPCS4Common.h"
#include "PlatMemory.h"
#include "UtilSync.h"

#include "SceLibkernel/sce_kernel_memory.h"
#include "tinydbr/memory_callback.h"

#include <list>
#include <optional>

// The emulated target process's memory must be allocated using this class.
// Emulator itself's memory is free to use 'new', 'malloc' or functions in UtilMemory.
// TODO:
// In theory, we should take control of all code, stack, heap and graphics memory,
// but currently we ignore code and stack since the host and target are all x86-64 architecture.

class MemoryAllocator
{
private:
	struct MemoryBlock
	{
		size_t   start;
		size_t   size;
		uint32_t protection;
	};

	using MemoryBlockList = std::list<MemoryBlock>;

public:
	MemoryAllocator();
	~MemoryAllocator();

	// SCE functions

	int32_t reserveVirtualRange(
		void** addr,
		size_t len,
		int    flags,
		size_t alignment);

	int32_t mapFlexibleMemory(
		void** addrInOut,
		size_t len,
		int    prot,
		int    flags);

	int32_t unmapFlexibleMemory(
		void*  addr,
		size_t len);

	int32_t allocateDirectMemory(
		int64_t  searchStart,
		int64_t  searchEnd,
		size_t   len,
		size_t   alignment,
		int      memoryType,
		int64_t* physAddrOut);

	int32_t mapDirectMemory(
		void**  addr,
		size_t  len,
		int     prot,
		int     flags,
		int64_t directMemoryStart,
		size_t  alignment);

	int32_t memoryUnmap(
		void*  addr,
		size_t len);

	int32_t checkedReleaseDirectMemory(
		int64_t start,
		size_t  len);

	int32_t queryMemoryProtection(
		void*     addr,
		void**    start,
		void**    end,
		uint32_t* prot);

	// C functions

	void* sce_malloc(size_t size);

	void* sce_realloc(void* ptr, size_t new_size);

	void* sce_calloc(size_t num, size_t size);

	void sce_free(void* ptr);

	void* sce_mmap(void* addr, size_t length, int prot, int flags, int fd, int64_t offset);

	int sce_munmap(void* addr, size_t length);

private:
	// convert SCE flags to UtilMemory flags.
	plat::VM_PROTECT_FLAG convertProtectFlags(int sceFlags);

	void* allocateInternal(void* addrIn, size_t len, size_t alignment, int prot);

	std::optional<MemoryBlockList::iterator>
	findMemoryBlock(void* addr);

private:
	util::sync::Spinlock m_lock;
	MemoryBlockList m_memBlocks;
};

class MemoryController : public MemoryCallback
{
public:
	MemoryController();
	virtual ~MemoryController();


private:
	void OnMemoryRead(void* address, size_t size) override;

	void OnMemoryWrite(void* address, size_t size) override;

private:

};
