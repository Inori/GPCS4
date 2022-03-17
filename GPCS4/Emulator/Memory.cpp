#include "Memory.h"

MemoryAllocator::MemoryAllocator()
{
}

MemoryAllocator::~MemoryAllocator()
{
}


int32_t MemoryAllocator::mapFlexibleMemory(void** addrInOut, size_t len, int prot, int flags)
{
}

int MemoryAllocator::unmapFlexibleMemory(void* addr, size_t len)
{
}

int32_t MemoryAllocator::allocateDirectMemory(
	int64_t searchStart, int64_t searchEnd, 
	size_t len, size_t alignment, int memoryType, int64_t* physAddrOut)
{
}

int32_t MemoryAllocator::mapDirectMemory(
	void** addr, size_t len, int prot, int flags, int64_t directMemoryStart, size_t alignment)
{
}

int32_t MemoryAllocator::checkedReleaseDirectMemory(int64_t start, size_t len)
{
}

int32_t MemoryAllocator::queryMemoryProtection(
	void* addr, void** start, void** end, int* prot)
{
}

void* MemoryAllocator::malloc(size_t size)
{
}

void* MemoryAllocator::realloc(void* ptr, size_t new_size)
{
}

void* MemoryAllocator::calloc(size_t num, size_t size)
{
}

void MemoryAllocator::free(void* ptr)
{
}

void* MemoryAllocator::mmap(void* addr, size_t length,
	int prot, int flags, int fd, int64_t offset)
{
}

int MemoryAllocator::munmap(void* addr, size_t length)
{
}

void MemoryController::OnMemoryRead(void* address, size_t size)
{
	
}

void MemoryController::OnMemoryWrite(void* address, size_t size)
{
	
}

//////////////////////////////////////////////////////////////////////////


MemoryController::MemoryController()
{
}

MemoryController::~MemoryController()
{
}