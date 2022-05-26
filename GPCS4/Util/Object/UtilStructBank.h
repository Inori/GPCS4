#pragma once

#include <cstdint>
#include <cstring>

namespace util
{
	struct StructBank;

	// Create an empty bank object
	StructBank* sbCreate();

	// Destroy the bank created by sbCreate
	void sbDestroy(StructBank* pBank);

	// Initialize.. pass in the size of your structure and how much to cache.
	void sbInit(StructBank* pBank, uint32_t structSize, uint32_t cacheSize);

	// Initialize and preallocate some (to avoid allocation the first time).
	// Returns 1 if successful, 0 otherwise.  Even if it returns 0, the StructBank
	// will be initialized and you might be able to allocate thru it.
	bool sbInitEx(StructBank* pBank, uint32_t structSize, uint32_t cacheSize, uint32_t nPreallocations);

	// Shutdown.
	void sbTerm(StructBank* pBank);

	// Allocate an instance.
	void* sbAllocate(StructBank* pBank);

	// Allocate a zero-initted instance.
	void* sbAllocateZero(StructBank* pBank);

	void sbFree(StructBank* pBank, void* pObj);

	// This puts everything that has been allocated into the free list.  It doesn't
	// actually free any memory.  It can be useful if you want easy access to a bunch
	// of StructBank'd structures and don't want to worry about freeing them all individually.
	void sbFreeAll(StructBank* pBank);

}  // namespace util
