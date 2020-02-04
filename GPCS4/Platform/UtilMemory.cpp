#include "UtilMemory.h"

#include <vector>

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
static uint64_t baseDirectMemory = 0x400000;

struct MemoryRange {
	uint64_t start;
	uint64_t end;
	uint32_t protection;
};

static std::vector<MemoryRange> ranges;

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

void* VMMapFlexible(void *addrIn, size_t nSize, uint32_t nProtectFlag)
{
	void* pAddr = NULL;
	do 
	{
		pAddr = VirtualAlloc(addrIn, nSize, MEM_RESERVE | MEM_COMMIT, GetProtectFlag(nProtectFlag));

		MemoryRange range{ (uint64_t)pAddr, nSize + (uint64_t)pAddr, nProtectFlag };
		ranges.emplace_back(range);
	} while (false);
	return pAddr;
}

void* VMMapDirect(size_t nSize, uint32_t nProtectFlag, uint32_t nType)
{
	void* pAddr = nullptr;
	do
	{
		uint64_t temp = baseDirectMemory;
		do
		{
			pAddr = VirtualAlloc((void*)temp, nSize, GetTypeFlag(nType), GetProtectFlag(nProtectFlag));
			temp += 0x1000;
		} while (pAddr == nullptr && (temp + nSize) <= DIRECT_MEMORY_HIGH);

		if (pAddr == nullptr)
		{
			break; // Unable to allocate memory
		}

		baseDirectMemory = nSize + (uint64_t)pAddr;

		MemoryRange range{ (uint64_t)pAddr, nSize + (uint64_t)pAddr, nProtectFlag };
		ranges.emplace_back(range);

	} while (false);
	return pAddr;
}

void* VMAllocateDirect() 
{
	return (void*)baseDirectMemory;
}

void VMUnMap(void* pAddr, size_t nSize)
{
	VirtualFree(pAddr, nSize, MEM_RELEASE);
	// TODO: remove from ranges[]
}

bool VMProtect(void* pAddr, size_t nSize, uint32_t nProtectFlag)
{
	LOG_FIXME("Not Implemented.");
	return  true;
}

int VMQueryProtection(void* addr, void** start, void** end, uint32_t* prot)
{
	int bRet = -1;
	do {
		uint64_t a = (uint64_t)addr;
		for (const auto& range : ranges)
		{
			if (a >= range.start && a < range.end)
			{
				*start = (void*)range.start;
				*end = (void*)range.end;
				*prot = range.protection;
				bRet = 0;
				break;
			}
		}
	} while (false);
	return bRet;
}

#elif defined(GPCS4_LINUX)

//TODO: Other platform implementation 

#endif  //GPCS4_WINDOWS
}