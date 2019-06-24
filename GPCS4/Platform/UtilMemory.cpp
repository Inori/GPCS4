#include "UtilMemory.h"

namespace UtilMemory
{;

#ifdef GPCS4_WINDOWS

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#undef WIN32_LEAN_AND_MEAN

void* VMMap(size_t nSize, uint nProtectFlag)
{
	void* pAddr = NULL;
	do 
	{
		pAddr = VirtualAlloc(NULL, nSize, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	} while (false);
	return pAddr;
}

void* VMMapEx(void* pAddr, size_t nSize, uint nProtectFlag, uint nType)
{
	do
	{
		pAddr = VirtualAlloc(pAddr, nSize, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	} while (false);
	return pAddr;
}

void VMUnMap(void* pAddr, size_t nSize)
{
	VirtualFree(pAddr, nSize, MEM_RELEASE);
}

bool VMProtect(void* pAddr, size_t nSize, uint nProtectFlag)
{
	return false;
}

#elif defined(GPCS4_LINUX)

//TODO: Other platform implementation 

#endif  //GPCS4_WINDOWS
}