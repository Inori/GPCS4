#pragma once
#include "GPCS4Common.h"

//Virtual memory stuffs

namespace UtilMemory
{;



enum VM_PROTECT_FLAG
{
	VMPF_NOACCESS,
	VMPF_READ,
	VMPF_WRITE,
	VMPF_EXECUTE,
};

void* VMMap(size_t nSize, uint nProtectFlag);

void VMUnMap(void* pAddr, size_t nSize);

bool VMProtect(void* pAddr, size_t nSize, uint nProtectFlag);


}