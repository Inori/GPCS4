#pragma once

#include "GPCS4Common.h"
#include "UtilMemory.h"
#include "UtilSync.h"
#include "SceLibkernel/sce_kernel_memory.h"
#include "tinydbr/arch/x86/x86_memory_monitor.h"

#include <list>


// The emulated target process's memory must be allocated using this class.
// Emulator itself's memory is free to use 'new', 'malloc' or functions in UtilMemory.
// TODO:
// In theory, we should take control of all code, stack, heap and graphics memory,
// but currently we ignore code and stack since the host and target are all x86-64 architecture.

class MemoryAllocator
{
private:
	struct MemoryRange
	{
		uintptr_t start;
		uintptr_t end;
		uint32_t  protection;
	};


public:
	MemoryAllocator();
	~MemoryAllocator();

	// SCE functions

	int32_t mapFlexibleMemory(
		void** addrInOut,
		size_t len,
		int    prot,
		int    flags);

	int unmapFlexibleMemory(
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

	int32_t checkedReleaseDirectMemory(
		int64_t start,
		size_t  len);

	int32_t queryMemoryProtection(
		void*  addr,
		void** start,
		void** end,
		int*   prot);

	// C functions

	void* malloc(size_t size);

	void* realloc(void* ptr, size_t new_size);

	void* calloc(size_t num, size_t size);

	void  free(void* ptr);

	void* mmap(void* addr, size_t length,
		int prot, int flags, int fd, int64_t offset);

	int munmap(void* addr, size_t length);

private:


private:
	std::list<MemoryRange> m_memRecords;

};



class MemoryController :
	public MemoryCallback
{

public:


private:

void OnMemoryRead(void* address, size_t size) override;

void OnMemoryWrite(void* address, size_t size) override;

private:
	MemoryController();
	~MemoryController();
};

