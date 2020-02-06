#pragma once
#include "GPCS4Common.h"

#include <memory>

//Virtual memory stuffs

namespace UtilMemory
{;



enum VM_PROTECT_FLAG
{
	VMPF_NOACCESS	=		0x00000000,
	VMPF_CPU_READ	=		0x00000001,
	VMPF_CPU_WRITE	=		0x00000002,
	VMPF_CPU_EXEC	=		0x00000004,
	VMPF_READ_WRITE	=		VMPF_CPU_READ | VMPF_CPU_WRITE,
	VMPF_READ_WRITE_EXEC =	VMPF_CPU_READ | VMPF_CPU_WRITE | VMPF_CPU_EXEC,
	VMPF_GPU_READ	=		0x00000010,
	VMPF_GPU_WRITE	=		0x00000020,
	VMPF_GPU_RW		=		VMPF_GPU_READ | VMPF_GPU_WRITE,
};

enum VM_ALLOCATION_TYPE
{
	VMAT_RESERVE	=	0x00000001,
	VMAT_COMMIT		=	0x00000010,
	VMAT_RESERVE_COMMIT = VMAT_RESERVE | VMAT_COMMIT
};

void* VMMapFlexible(void *addrIn, size_t nSize, uint32_t nProtectFlag);

void* VMMapDirect(size_t nSize, uint32_t nProtectFlag, uint32_t nType);

void* VMAllocateDirect(/*TODO: use parameters*/);

void VMUnMap(void* pAddr, size_t nSize);

bool VMProtect(void* pAddr, size_t nSize, uint32_t nProtectFlag);

int VMQueryProtection(void* addr, void** start, void** end, uint32_t* prot);

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

typedef std::unique_ptr<uint8_t, MemoryUnMapper> memory_uptr;

}