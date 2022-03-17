#pragma once
#include "GPCS4Common.h"

#include <memory>

//Virtual memory stuffs
// NOTE:
// these functions are just platform abstraction,
// and shouldn't be called from HLE functions directly,
// instead, they should be used by emulator code.
// For HLE use, see Memory.h

namespace UtilMemory
{;

constexpr uint32_t VM_PAGE_SIZE = 0x1000;


enum VM_PROTECT_FLAG
{
	VMPF_NOACCESS  = 0x00000000,
	VMPF_CPU_READ  = 0x00000001,
	VMPF_CPU_WRITE = 0x00000002,
	VMPF_CPU_EXEC  = 0x00000004,
	VMPF_CPU_RW    = VMPF_CPU_READ | VMPF_CPU_WRITE,
	VMPF_CPU_RWX   = VMPF_CPU_READ | VMPF_CPU_WRITE | VMPF_CPU_EXEC,
};

enum VM_ALLOCATION_TYPE
{
	VMAT_RESERVE	=	0x00000001,
	VMAT_COMMIT		=	0x00000010,
	VMAT_RESERVE_COMMIT = VMAT_RESERVE | VMAT_COMMIT
};


void* VMAllocate(void* pAddress, size_t nSize, 
	VM_ALLOCATION_TYPE nType, VM_PROTECT_FLAG nProtect);

void* VMAllocateAlign(void* pAddress, size_t nSize, size_t nAlign, 
	VM_ALLOCATION_TYPE nType, VM_PROTECT_FLAG nProtect);

void VMFree(void* pAddress);

bool VMProtect(void* pAddress, size_t nSize, 
	VM_PROTECT_FLAG nNewProtect, VM_PROTECT_FLAG* pOldProtect);


struct MemoryUnMapper
{
	void operator()(void* pMem) const noexcept
	{
		if (pMem != nullptr)
		{
			VMFree(pMem);
		}
	}
};

// auto release smart memory pointer
typedef std::unique_ptr<uint8_t, MemoryUnMapper> memory_ptr;

}