#include "UtilMemory.h"
#include "sce_errors.h"
#include "UtilMath.h"
#include <vector>
#include <algorithm>

LOG_CHANNEL(Platform.UtilMemory);

namespace UtilMemory
{;

#ifdef GPCS4_WINDOWS

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#undef WIN32_LEAN_AND_MEAN

// Note:
// Direct memory address is supposed to be within 0x000000FFFFFFFFFF
#define DIRECT_MEMORY_HIGH 0x000000FFFFFFFFFFull
// TODO:
// Access to this should be thread safe
static uintptr_t g_baseDirectMemory = 0x400000;

struct MemoryRange 
{
	uintptr_t start;
	uintptr_t end;
	uint32_t protection;
};

// TODO:
// Access to this should be thread safe
static std::vector<MemoryRange> g_memRanges;

inline std::vector<MemoryRange>::iterator findMemoryRange(void* addr)
{
	std::vector<MemoryRange>::iterator iter = std::find_if(g_memRanges.begin(), g_memRanges.end(), 
		[=](const MemoryRange& range) 
		{
			uintptr_t a = reinterpret_cast<uintptr_t>(addr);
			return (a >= range.start && a < range.end);
		});
	return iter;
}

inline uint32_t GetProtectFlag(uint32_t nOldFlag)
{
	uint32_t nNewFlag = 0;
	do 
	{
		if (nOldFlag & VMPF_NOACCESS)
		{
			nNewFlag |= PAGE_NOACCESS;
			break;
		}

		if ((nOldFlag & VMPF_CPU_READ) || (nOldFlag & VMPF_GPU_READ))
		{
			nNewFlag = PAGE_READONLY;
		}

		if ((nOldFlag & VMPF_CPU_WRITE) || (nOldFlag & VMPF_GPU_WRITE))
		{
			nNewFlag = PAGE_READWRITE;
		}

		if (nOldFlag & VMPF_CPU_EXEC)
		{
			nNewFlag = PAGE_EXECUTE_READWRITE;
		}

	} while (false);
	return nNewFlag;
}

inline uint32_t GetTypeFlag(uint32_t nOldFlag)
{
	uint32_t nNewFlag = 0;
	do
	{
		if (nOldFlag & VMAT_RESERVE)
		{
			nNewFlag |= MEM_RESERVE;
		}

		if (nOldFlag & VMAT_COMMIT)
		{
			nNewFlag |= MEM_COMMIT;
		}

	} while (false);
	return nNewFlag;
}

void* VMMapAligned(size_t nSize, uint32_t nProtectFlag, int align)
{
#ifndef GPCS4_DEBUG
	align = 0x1000;  // use default alignment on release build
#endif

	void* pAlignedAddr = nullptr;
	void* pAddr        = nullptr;
	do
	{
		pAddr             = VirtualAlloc(nullptr, nSize, MEM_RESERVE | MEM_COMMIT, GetProtectFlag(nProtectFlag));
		uintptr_t refAddr = util::alignRound((uintptr_t)pAddr, align);

		do
		{
			pAlignedAddr = VirtualAlloc((void*)refAddr, nSize, MEM_RESERVE | MEM_COMMIT, GetProtectFlag(nProtectFlag));
			refAddr += align;
		} while (pAlignedAddr == nullptr);

	} while (false);

	if (pAddr)
	{
		VirtualFree(pAddr, nSize, MEM_RELEASE);
	}

	return pAlignedAddr;
}

void* VMMapFlexible(void *addrIn, size_t nSize, uint32_t nProtectFlag)
{
	void* pAddr = NULL;
	do 
	{
		pAddr = VirtualAlloc(addrIn, nSize, MEM_RESERVE | MEM_COMMIT, GetProtectFlag(nProtectFlag));

		MemoryRange range 
		{ 
			reinterpret_cast<uintptr_t>(pAddr),
			reinterpret_cast<uintptr_t>(pAddr) + nSize, 
			nProtectFlag 
		};

		g_memRanges.emplace_back(range);
	} while (false);
	return pAddr;
}

void* VMMapDirect(size_t nSize, uint32_t nProtectFlag, uint32_t nType)
{
	void* pAddr = nullptr;
	do
	{
		uintptr_t temp = g_baseDirectMemory;
		do
		{
			pAddr = VirtualAlloc((void*)temp, nSize, GetTypeFlag(nType), GetProtectFlag(nProtectFlag));
			temp += 0x1000;
		} while (pAddr == nullptr && (temp + nSize) <= DIRECT_MEMORY_HIGH);

		if (pAddr == nullptr)
		{
			break; // Unable to allocate memory
		}

		g_baseDirectMemory = nSize + reinterpret_cast<uintptr_t>(pAddr);

		MemoryRange range
		{
			reinterpret_cast<uintptr_t>(pAddr),
			reinterpret_cast<uintptr_t>(pAddr) + nSize,
			nProtectFlag
		};
		g_memRanges.emplace_back(range);

	} while (false);
	return pAddr;
}

void* VMAllocateDirect() 
{
	return (void*)g_baseDirectMemory;
}

void* VMMap(void* start, size_t nSize, uint32_t nProtectFlag, uint32_t flags, int fd, int64_t offset)
{
	return VirtualAlloc(nullptr, nSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
}

void VMUnMap(void* pAddr, size_t nSize)
{
	VirtualFree(pAddr, nSize, MEM_RELEASE);

	do 
	{
		auto iter = findMemoryRange(pAddr);
		if (iter == g_memRanges.end())
		{
			LOG_WARN("can not find range for %p", pAddr);
			break;
		}

		g_memRanges.erase(iter);
	} while (false);
}

bool VMProtect(void* pAddr, size_t nSize, uint32_t nProtectFlag)
{
	LOG_FIXME("Not Implemented.");
	return  true;
}

int VMQueryProtection(void* addr, void** start, void** end, uint32_t* prot)
{
	int bRet = SCE_ERROR_UNKNOWN;
	do 
	{
		auto iter = findMemoryRange(addr);
		if (iter == g_memRanges.end())
		{
			LOG_WARN("can not find range for %p", addr);
			break;
		}

		*start = reinterpret_cast<void*>(iter->start);
		*end   = reinterpret_cast<void*>(iter->end);
		*prot  = iter->protection;

		bRet = SCE_OK;
	} while (false);
	return bRet;
}

#elif defined(GPCS4_LINUX)

//TODO: Other platform implementation 

#endif  //GPCS4_WINDOWS
}