#pragma once

#include <cstdint>
#include <cstring>

namespace util
{

	struct StructLink
	{
		struct StructLink* m_slNext;
	};

	struct StructBankPage
	{
		struct StructBankPage* m_next;
		unsigned long          m_objects;  // How many objects are in this page?
		uint32_t               m_data[1];
	};

	struct StructBank
	{
		// Struct size.
		unsigned long m_structSize;

		// The actual size we allocate with (aligned to 4 bytes and with 4 bytes
		// extra for the StructLink).
		unsigned long m_alignedStructSize;

		// How many structs per page.
		unsigned long m_cacheSize;

		// How many pages have we allocated?
		unsigned long m_numPages;

		// Total number of objects this StructBank has allocated.
		unsigned long m_numTotalObjects;

		// The first page.
		StructBankPage* m_pageHead;

		// The free list.
		StructLink* m_freeListHead;
	};

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
