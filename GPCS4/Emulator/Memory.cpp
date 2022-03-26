#include "Memory.h"

#include "SceModules/sce_errors.h"
#include <mutex>

LOG_CHANNEL(Memory);

MemoryAllocator::MemoryAllocator()
{
}

MemoryAllocator::~MemoryAllocator()
{
}

int32_t MemoryAllocator::reserveVirtualRange(void** addr, size_t len, int flags, size_t alignment)
{
	LOG_DEBUG("not really implemented.");
	*addr = new uint8_t[1024 * 1024 * 256];
	return SCE_OK;
}

// well, should pre allocate flexible memory and 'map' to the given address in this function.
// but currently we just do the allocation in map
int32_t MemoryAllocator::mapFlexibleMemory(void** addrInOut, size_t len, int prot, int flags)
{
	int err = SCE_KERNEL_ERROR_UNKNOWN;
	do
	{
		if (!addrInOut || !len || !util::isAligned(len, (size_t)SCE_LOGICAL_PAGE_SIZE))
		{
			err = SCE_KERNEL_ERROR_EINVAL;
			break;
		}

		if (flags & SCE_KERNEL_MAP_FIXED && !util::isAligned((uint64_t)*addrInOut, (size_t)SCE_LOGICAL_PAGE_SIZE))
		{
			err = SCE_KERNEL_ERROR_EINVAL;
			break;
		}
		// TODO:
		// check more parameters

		// TODO:
		// implement flags
		LOG_ASSERT(flags == 0, "Non-zero flags is not implemented.");

		void* addrOut = allocateInternal(*addrInOut, len, SCE_KERNEL_PAGE_SIZE, prot);
		if (!addrOut)
		{
			err = SCE_KERNEL_ERROR_ENOMEM;
			break;
		}

		*addrInOut = addrOut;
		err        = SCE_OK;
	} while (false);
	return err;
}

int32_t MemoryAllocator::unmapFlexibleMemory(void* addr, size_t len)
{
	return memoryUnmap(addr, len);
}

int32_t MemoryAllocator::allocateDirectMemory(
	int64_t searchStart, int64_t searchEnd, size_t len, size_t alignment, int memoryType, int64_t* physAddrOut)
{
	int err = SCE_KERNEL_ERROR_UNKNOWN;
	do
	{
		if (!len || !util::isAligned(len, (size_t)SCE_KERNEL_PAGE_SIZE))
		{
			err = SCE_KERNEL_ERROR_EINVAL;
			break;
		}

		if (alignment != 0 && !util::isAligned(alignment, (size_t)SCE_KERNEL_PAGE_SIZE))
		{
			err = SCE_KERNEL_ERROR_EINVAL;
			break;
		}
		// TODO:
		// check more parameters

		// return a dummy address in case the game zero checks it.
		// real memory will be allocated in map function.
		*physAddrOut = SCE_KERNEL_APP_MAP_AREA_START_ADDR;
		err          = SCE_OK;
	} while (false);
	return err;
}

int32_t MemoryAllocator::mapDirectMemory(
	void** addr, size_t len, int prot, int flags, int64_t directMemoryStart, size_t alignment)
{
	int err = SCE_KERNEL_ERROR_UNKNOWN;
	do
	{
		if (!addr || !len || !util::isAligned(len, (size_t)SCE_KERNEL_PAGE_SIZE))
		{
			err = SCE_KERNEL_ERROR_EINVAL;
			break;
		}

		if ((flags & SCE_KERNEL_MAP_FIXED) && !util::isAligned((uint64_t)*addr, (size_t)SCE_KERNEL_PAGE_SIZE))
		{
			err = SCE_KERNEL_ERROR_EINVAL;
			break;
		}

		if (!util::isAligned((size_t)directMemoryStart, (size_t)SCE_KERNEL_PAGE_SIZE))
		{
			err = SCE_KERNEL_ERROR_EINVAL;
			break;
		}

		if ((alignment != 0) && !util::isAligned((size_t)alignment, (size_t)SCE_KERNEL_PAGE_SIZE))
		{
			err = SCE_KERNEL_ERROR_EINVAL;
			break;
		}
		// TODO:
		// check more parameters

		// TODO:
		// implement flags
		LOG_ASSERT(flags == 0, "Non-zero flags is not implemented.");

		void* addrOut = allocateInternal(*addr, len, alignment, prot);
		if (!addrOut)
		{
			err = SCE_KERNEL_ERROR_ENOMEM;
			break;
		}

		*addr = addrOut;
		err   = SCE_OK;
	} while (false);
	return err;
}

int32_t MemoryAllocator::memoryUnmap(void* addr, size_t len)
{
	pmemory::VMFree(addr);

	auto iter = findMemoryBlock(addr);
	if (iter.has_value())
	{
		m_memBlocks.erase(*iter);
	}

	return SCE_OK;
}

int32_t MemoryAllocator::checkedReleaseDirectMemory(int64_t start, size_t len)
{
	// umemory::VMFree(reinterpret_cast<void*>(start));
	return SCE_OK;
}

int32_t MemoryAllocator::queryMemoryProtection(
	void* addr, void** start, void** end, uint32_t* prot)
{
	int32_t err = SCE_KERNEL_ERROR_UNKNOWN;
	do
	{
		if (reinterpret_cast<size_t>(addr) < SCE_KERNEL_SYS_MANAGE_AREA_START_ADDR || 
			reinterpret_cast<size_t>(addr) >= SCE_KERNEL_VIRTUAL_ADDRESS_END_ADDR)
		{
			err = SCE_KERNEL_ERROR_EINVAL;
			break;
		}

		auto iter = findMemoryBlock(addr);
		if (!iter)
		{
			err = SCE_KERNEL_ERROR_EACCES;
			break;
		}

		if (start)
		{
			*start = reinterpret_cast<void*>((*iter)->start);
		}
		if (end)
		{
			*end = reinterpret_cast<void*>((*iter)->start + (*iter)->size);
		}
		if (prot)
		{
			*prot = (*iter)->protection;
		}

		err = SCE_OK;
	}while(false);
	return err;
}

// TODO:
// for malloc series functions, we need to implement somewhat memory pool
// algorithm, Nginx Slab may be a choice.
void* MemoryAllocator::sce_malloc(size_t size)
{
	return allocateInternal(0, size, 0, SCE_KERNEL_PROT_CPU_RW);
}

void* MemoryAllocator::sce_realloc(void* ptr, size_t new_size)
{
	void* ret = nullptr;
	void* backup = nullptr;
	do 
	{
		auto iter = findMemoryBlock(ptr);
		if (!iter)
		{
			break;
		}

		auto oldSize = iter.value()->size;
		backup = malloc(oldSize);
		if (!backup)
		{
			break;
		}
		memcpy(backup, ptr, oldSize);

		memoryUnmap(ptr, 0);
		ret = MemoryAllocator::sce_malloc(new_size);
		if (!ret)
		{
			break;
		}
		memcpy(ret, backup, oldSize);

	} while (false);
	
	if (backup)
	{
		free(backup);
	}
	
	return ret;
}

void* MemoryAllocator::sce_calloc(size_t num, size_t size)
{
	size_t total_size = num * size;
	void*  mem        = sce_malloc(total_size);
	memset(mem, 0, total_size);
	return mem;
}

void MemoryAllocator::sce_free(void* ptr)
{
	memoryUnmap(ptr, 0);
}

void* MemoryAllocator::sce_mmap(void* addr, size_t length, int prot, int flags, int fd, int64_t offset)
{
	return allocateInternal(addr, length, 0, SCE_KERNEL_PROT_GPU_RW);
}

int MemoryAllocator::sce_munmap(void* addr, size_t length)
{
	memoryUnmap(addr, length);
	return 0;
}

pmemory::VM_PROTECT_FLAG MemoryAllocator::convertProtectFlags(int sceFlags)
{
	uint32_t utlFlags = 0;
	if ((sceFlags & SCE_KERNEL_PROT_CPU_READ) || (sceFlags & SCE_KERNEL_PROT_GPU_READ))
	{
		utlFlags |= pmemory::VMPF_CPU_READ;
	}
	if ((sceFlags & SCE_KERNEL_PROT_CPU_WRITE) || (sceFlags & SCE_KERNEL_PROT_GPU_WRITE))
	{
		utlFlags |= pmemory::VMPF_CPU_WRITE;
	}
	if (sceFlags & SCE_KERNEL_PROT_CPU_EXEC)
	{
		utlFlags |= pmemory::VMPF_CPU_EXEC;
	}
	return static_cast<pmemory::VM_PROTECT_FLAG>(utlFlags);
}

void* MemoryAllocator::allocateInternal(void* addrIn, size_t len, size_t alignment, int prot)
{
	void* addrOut = nullptr;
	do
	{
		auto uprot = convertProtectFlags(prot);

		if (alignment == 0)
		{
			alignment = SCE_KERNEL_PAGE_SIZE;
		}

		// Memory will be placed in system managed area
		// when 0 is specified to the addrInOut argument of sceKernelMapFlexibleMemory()
		size_t searchAddr = 0;
		if (addrIn != nullptr)
		{
			searchAddr = reinterpret_cast<size_t>(addrIn);
		}
		else
		{
			searchAddr = SCE_KERNEL_SYS_MANAGE_AREA_START_ADDR;
		}

		while (searchAddr < SCE_KERNEL_APP_MAP_AREA_END_ADDR)
		{
			pmemory::MemoryInformation mi = {};
			if (!pmemory::VMQuery(reinterpret_cast<void*>(searchAddr), &mi))
			{
				break;
			}

			if (mi.nRegionState != pmemory::VMRS_FREE)
			{
				searchAddr = reinterpret_cast<size_t>(mi.pRegionStart) + mi.nRegionSize;
				continue;
			}

			size_t regionAddress = reinterpret_cast<size_t>(mi.pRegionStart);
			size_t regionSize    = mi.nRegionSize;
			size_t alignPad      = regionAddress & (alignment - 1);
			if (alignPad)
			{
				size_t offset = (alignment - alignPad);
				regionAddress += offset;
				if (regionSize > offset)
				{
					regionSize -= offset;
				}
				else
				{
					regionSize = 0;
				}
			}

			if (regionSize < len)
			{
				searchAddr = reinterpret_cast<size_t>(mi.pRegionStart) + mi.nRegionSize;
				continue;
			}

			void* retAddress = VMAllocate(reinterpret_cast<void*>(regionAddress), len,
										  pmemory::VMAT_RESERVE_COMMIT, uprot);
			if (!retAddress)
			{
				searchAddr = reinterpret_cast<size_t>(mi.pRegionStart) + mi.nRegionSize;
				continue;
			}

			if (reinterpret_cast<size_t>(retAddress) >= SCE_KERNEL_SYS_MANAGE_AREA_START_ADDR &&
				reinterpret_cast<size_t>(retAddress) + len <= SCE_KERNEL_SYS_MANAGE_AREA_END_ADDR)
			{
				// found it in system managed area
				addrOut = retAddress;
				break;
			}

			if (reinterpret_cast<size_t>(retAddress) >= SCE_KERNEL_APP_MAP_AREA_START_ADDR &&
				reinterpret_cast<size_t>(retAddress) + len <= SCE_KERNEL_APP_MAP_AREA_END_ADDR)
			{
				// found it in user area
				addrOut = retAddress;
				break;
			}

			if (retAddress)
			{
				// unlikely
				pmemory::VMFree(retAddress);
			}

			searchAddr = reinterpret_cast<size_t>(mi.pRegionStart) + mi.nRegionSize;
		}

		if (addrOut)
		{
			std::lock_guard<sync::Spinlock> guard(m_lock);

			MemoryBlock block = 
			{
				reinterpret_cast<size_t>(addrOut),
				len,
				static_cast<uint32_t>(prot) 
			};
			m_memBlocks.emplace_back(block);
		}

	} while (false);
	return addrOut;
}

std::optional<MemoryAllocator::MemoryBlockList::iterator>
MemoryAllocator::findMemoryBlock(void* addr)
{
	std::lock_guard<sync::Spinlock> guard(m_lock);

	std::optional<MemoryBlockList::iterator> optResult;
	auto iter = std::find_if(m_memBlocks.begin(), m_memBlocks.end(),
							 [=](const MemoryBlock& block)
							 {
								 uintptr_t a = reinterpret_cast<size_t>(addr);
								 return (a >= block.start && a < block.start + block.size);
							 });
	if (iter != m_memBlocks.end())
	{
		optResult.emplace(iter);
	}
	return optResult;
}


//////////////////////////////////////////////////////////////////////////

MemoryController::MemoryController()
{
}

MemoryController::~MemoryController()
{
}

void MemoryController::OnMemoryRead(void* address, size_t size)
{
}

void MemoryController::OnMemoryWrite(void* address, size_t size)
{
}