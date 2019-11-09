#pragma once
#include "GPCS4Common.h"

#include <memory>

//Virtual memory stuffs

namespace UtilMemory
{;



enum VM_PROTECT_FLAG
{
	VMPF_NOACCESS	=		0x00000001,
	VMPF_READ		=		0x00000010,
	VMPF_WRITE		=		0x00000100,
	VMPF_EXECUTE	=		0x00001000,
	VMPF_READ_WRITE	=		VMPF_READ | VMPF_WRITE,
	VMPF_READ_WRITE_EXECUTE = VMPF_READ | VMPF_WRITE | VMPF_EXECUTE
};

enum VM_ALLOCATION_TYPE
{
	VMAT_RESERVE	=	0x00000001,
	VMAT_COMMIT		=	0x00000010,
	VMAT_RESERVE_COMMIT = VMAT_RESERVE | VMAT_COMMIT
};

void* VMMap(size_t nSize, uint nProtectFlag);

void* VMMapEx(void* pAddr, size_t nSize, uint nProtectFlag, uint nType);

void VMUnMap(void* pAddr, size_t nSize);

bool VMProtect(void* pAddr, size_t nSize, uint nProtectFlag);

struct MemoryUnMapper
{
	void operator()(void *vMem) const noexcept
	{
		if (vMem != nullptr) 
		{
			VMUnMap(vMem, 0);
		}
	}
};

typedef std::unique_ptr<byte, MemoryUnMapper> memory_uptr;

}