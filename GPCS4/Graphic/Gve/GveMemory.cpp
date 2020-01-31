#include "GveMemory.h"
#include "GvePhysicalDevice.h"
#include "GveDevice.h"

LOG_CHANNEL(Graphic.Gve.GveMemory);

namespace gve
{;


GveMemory::GveMemory() { }

GveMemory::GveMemory(
	GveMemoryAllocator*  alloc,
	GveMemoryChunk*      chunk,
	GveMemoryType*       type,
	VkDeviceMemory       memory,
	VkDeviceSize         offset,
	VkDeviceSize         length,
	void*                mapPtr)
	: 
	m_alloc(alloc),
	m_chunk(chunk),
	m_type(type),
	m_memory(memory),
	m_offset(offset),
	m_length(length),
	m_mapPtr(mapPtr) 
{
}


GveMemory::GveMemory(GveMemory&& other)
	: 
	m_alloc(std::exchange(other.m_alloc, nullptr)),
	m_chunk(std::exchange(other.m_chunk, nullptr)),
	m_type(std::exchange(other.m_type, nullptr)),
	m_memory(std::exchange(other.m_memory, VkDeviceMemory(VK_NULL_HANDLE))),
	m_offset(std::exchange(other.m_offset, 0)),
	m_length(std::exchange(other.m_length, 0)),
	m_mapPtr(std::exchange(other.m_mapPtr, nullptr)) 
{ 
}


GveMemory& GveMemory::operator = (GveMemory&& other) 
{
	free();
	m_alloc = std::exchange(other.m_alloc, nullptr);
	m_chunk = std::exchange(other.m_chunk, nullptr);
	m_type = std::exchange(other.m_type, nullptr);
	m_memory = std::exchange(other.m_memory, VkDeviceMemory(VK_NULL_HANDLE));
	m_offset = std::exchange(other.m_offset, 0);
	m_length = std::exchange(other.m_length, 0);
	m_mapPtr = std::exchange(other.m_mapPtr, nullptr);
	return *this;
}


GveMemory::~GveMemory() 
{
	free();
}


void GveMemory::free() 
{
	if (m_alloc != nullptr)
	{
		m_alloc->free(*this);
	}
}


GveMemoryChunk::GveMemoryChunk(
	GveMemoryAllocator*  alloc,
	GveMemoryType*       type,
	GveDeviceMemory      memory)
	: m_alloc(alloc), m_type(type), m_memory(memory) 
{
	// Mark the entire chunk as free
	m_freeList.push_back(FreeSlice{ 0, memory.memSize });
}


GveMemoryChunk::~GveMemoryChunk() 
{
	// This call is technically not thread-safe, but it
	// doesn't need to be since we don't free chunks
	m_alloc->freeDeviceMemory(m_type, m_memory);
}


GveMemory GveMemoryChunk::alloc(
	VkMemoryPropertyFlags flags,
	VkDeviceSize          size,
	VkDeviceSize          align,
	float                 priority) 
{
	GveMemory memory;
	do
	{
		// Property flags must be compatible. This could
		// be refined a bit in the future if necessary.
		if (m_memory.memFlags != flags || m_memory.priority != priority)
		{
			break;
		}

		// If the chunk is full, break
		if (m_freeList.size() == 0)
		{
			break;
		}

		// Select the slice to allocate from in a worst-fit
		// manner. This may help keep fragmentation low.
		auto bestSlice = m_freeList.begin();

		for (auto slice = m_freeList.begin(); slice != m_freeList.end(); slice++)
		{
			if (slice->length == size)
			{
				bestSlice = slice;
				break;
			}
			else if (slice->length > bestSlice->length)
			{
				bestSlice = slice;
			}
		}

		// We need to align the allocation to the requested alignment
		const VkDeviceSize sliceStart = bestSlice->offset;
		const VkDeviceSize sliceEnd = bestSlice->offset + bestSlice->length;

		const VkDeviceSize allocStart = util::alignRound(sliceStart, align);
		const VkDeviceSize allocEnd   = util::alignRound(allocStart + size, align);

		if (allocEnd > sliceEnd)
		{
			break;
		}

		// We can use this slice, but we'll have to add
		// the unused parts of it back to the free list.
		m_freeList.erase(bestSlice);

		if (allocStart != sliceStart)
		{
			m_freeList.push_back({ sliceStart, allocStart - sliceStart });
		}

		if (allocEnd != sliceEnd)
		{
			m_freeList.push_back({ allocEnd, sliceEnd - allocEnd });
		}

		// Create the memory object with the aligned slice
		memory = GveMemory(m_alloc, this, m_type,
			m_memory.memHandle, allocStart, allocEnd - allocStart,
			reinterpret_cast<char*>(m_memory.memPointer) + allocStart);
	} while (false);
	return memory;
}


void GveMemoryChunk::free(
	VkDeviceSize  offset,
	VkDeviceSize  length) 
{
	// Remove adjacent entries from the free list and then add
	// a new slice that covers all those entries. Without doing
	// so, the slice could not be reused for larger allocations.
	auto curr = m_freeList.begin();

	while (curr != m_freeList.end()) 
	{
		if (curr->offset == offset + length) 
		{
			length += curr->length;
			curr = m_freeList.erase(curr);
		}
		else if (curr->offset + curr->length == offset)
		{
			offset -= curr->length;
			length += curr->length;
			curr = m_freeList.erase(curr);
		}
		else 
		{
			curr++;
		}
	}

	m_freeList.push_back({ offset, length });
}


GveMemoryAllocator::GveMemoryAllocator(GveDevice* device):
	m_device(device),
	m_devProps(device->physicalDevice()->deviceProperties()),
	m_memProps(device->physicalDevice()->memoryProperties())
{
	for (uint32_t i = 0; i < m_memProps.memoryHeapCount; i++) 
	{
		m_memHeaps[i].properties = m_memProps.memoryHeaps[i];
		m_memHeaps[i].stats = GveMemoryStats{ 0, 0 };
	}

	for (uint32_t i = 0; i < m_memProps.memoryTypeCount; i++) 
	{
		m_memTypes[i].heap = &m_memHeaps[m_memProps.memoryTypes[i].heapIndex];
		m_memTypes[i].heapId = m_memProps.memoryTypes[i].heapIndex;
		m_memTypes[i].memType = m_memProps.memoryTypes[i];
		m_memTypes[i].memTypeId = i;
		m_memTypes[i].chunkSize = pickChunkSize(i);
	}
}


GveMemoryAllocator::~GveMemoryAllocator() 
{

}


GveMemory GveMemoryAllocator::alloc(
	const VkMemoryRequirements*             req,
	const VkMemoryDedicatedRequirements&    dedAllocReq,
	const VkMemoryDedicatedAllocateInfoKHR& dedAllocInfo,
	VkMemoryPropertyFlags             flags,
	float                             priority) 
{
	std::lock_guard<std::mutex> lock(m_mutex);

	// Try to allocate from a memory type which supports the given flags exactly
	auto dedAllocPtr = dedAllocReq.prefersDedicatedAllocation ? &dedAllocInfo : nullptr;
	GveMemory result = this->tryAlloc(req, dedAllocPtr, flags, priority);

	// If the first attempt failed, try ignoring the dedicated allocation
	if (!result && dedAllocPtr && !dedAllocReq.requiresDedicatedAllocation) 
	{
		result = this->tryAlloc(req, nullptr, flags, priority);
		dedAllocPtr = nullptr;
	}

	// If that still didn't work, probe slower memory types as well
	VkMemoryPropertyFlags optFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
		| VK_MEMORY_PROPERTY_HOST_CACHED_BIT;

	if (!result && (flags & optFlags))
	{
		result = this->tryAlloc(req, dedAllocPtr, flags & ~optFlags, priority);
	}
		

	//if (!result) 
	//{
	//	DxvkAdapterMemoryInfo memHeapInfo = m_device->adapter()->getMemoryHeapInfo();

	//	LOG_ERR(str::format(
	//		"DxvkMemoryAllocator: Memory allocation failed",
	//		"\n  Size:      ", req->size,
	//		"\n  Alignment: ", req->alignment,
	//		"\n  Mem flags: ", "0x", std::hex, flags,
	//		"\n  Mem types: ", "0x", std::hex, req->memoryTypeBits));

	//	for (uint32_t i = 0; i < m_memProps.memoryHeapCount; i++) 
	//	{
	//		Logger::err(str::format("Heap ", i, ": ",
	//			(m_memHeaps[i].stats.memoryAllocated >> 20), " MB allocated, ",
	//			(m_memHeaps[i].stats.memoryUsed >> 20), " MB used, ",
	//			m_device->extensions().extMemoryBudget
	//			? str::format(
	//			(memHeapInfo.heaps[i].memoryAllocated >> 20), " MB allocated (driver), ",
	//				(memHeapInfo.heaps[i].memoryBudget >> 20), " MB budget (driver), ",
	//				(m_memHeaps[i].properties.size >> 20), " MB total")
	//			: str::format(
	//			(m_memHeaps[i].properties.size >> 20), " MB total")));
	//	}

	//	LOG_ERR("DxvkMemoryAllocator: Memory allocation failed");
	//}

	return result;
}


GveMemoryStats GveMemoryAllocator::getMemoryStats() 
{
	std::lock_guard<std::mutex> lock(m_mutex);

	GveMemoryStats totalStats;

	for (size_t i = 0; i < m_memProps.memoryHeapCount; i++) 
	{
		totalStats.memoryAllocated += m_memHeaps[i].stats.memoryAllocated;
		totalStats.memoryUsed += m_memHeaps[i].stats.memoryUsed;
	}

	return totalStats;
}


GveMemory GveMemoryAllocator::tryAlloc(
	const VkMemoryRequirements*             req,
	const VkMemoryDedicatedAllocateInfoKHR* dedAllocInfo,
	VkMemoryPropertyFlags             flags,
	float                             priority) 
{
	GveMemory result;

	for (uint32_t i = 0; i < m_memProps.memoryTypeCount && !result; i++)
	{
		const bool supported = (req->memoryTypeBits & (1u << i)) != 0;
		const bool adequate = (m_memTypes[i].memType.propertyFlags & flags) == flags;

		if (supported && adequate)
		{
			result = this->tryAllocFromType(&m_memTypes[i],
				flags, req->size, req->alignment, priority, dedAllocInfo);
		}
	}

	return result;
}


GveMemory GveMemoryAllocator::tryAllocFromType(
	GveMemoryType*                   type,
	VkMemoryPropertyFlags             flags,
	VkDeviceSize                      size,
	VkDeviceSize                      align,
	float                             priority,
	const VkMemoryDedicatedAllocateInfoKHR* dedAllocInfo) 
{
	// Prevent unnecessary external host memory fragmentation
	bool isDeviceLocal = (flags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) != 0;

	if (!isDeviceLocal)
	{
		priority = 0.0f;
	}

	GveMemory memory;

	if (size >= type->chunkSize || dedAllocInfo) 
	{
		GveDeviceMemory devMem = this->tryAllocDeviceMemory(
			type, flags, size, priority, dedAllocInfo);

		if (devMem.memHandle != VK_NULL_HANDLE)
		{
			memory = GveMemory(this, nullptr, type, devMem.memHandle, 0, size, devMem.memPointer);
		}
	}
	else 
	{
		for (uint32_t i = 0; i < type->chunks.size() && !memory; i++)
		{
			memory = type->chunks[i]->alloc(flags, size, align, priority);
		}
			
		if (!memory) 
		{
			GveDeviceMemory devMem;

			for (uint32_t i = 0; i < 6 && (type->chunkSize >> i) >= size && !devMem.memHandle; i++)
			{
				devMem = tryAllocDeviceMemory(type, flags, type->chunkSize >> i, priority, nullptr);
			}
				
			if (devMem.memHandle) 
			{
				RcPtr<GveMemoryChunk> chunk = new GveMemoryChunk(this, type, devMem);
				memory = chunk->alloc(flags, size, align, priority);

				type->chunks.push_back(std::move(chunk));
			}
		}
	}

	if (memory)
	{
		type->heap->stats.memoryUsed += memory.m_length;
	}
		
	return memory;
}


GveDeviceMemory GveMemoryAllocator::tryAllocDeviceMemory(
	GveMemoryType*                    type,
	VkMemoryPropertyFlags             flags,
	VkDeviceSize                      size,
	float                             priority,
	const VkMemoryDedicatedAllocateInfoKHR* dedAllocInfo) 
{
	GveDeviceMemory result;
	result.memSize = size;
	result.memFlags = flags;
	result.priority = priority;

	do 
	{
		bool useMemoryPriority = (flags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
			&& (m_device->physicalDevice()->features().extMemoryPriority.memoryPriority);

		VkMemoryPriorityAllocateInfoEXT prio;
		prio.sType = VK_STRUCTURE_TYPE_MEMORY_PRIORITY_ALLOCATE_INFO_EXT;
		prio.pNext = dedAllocInfo;
		prio.priority = priority;

		VkMemoryAllocateInfo info;
		info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		info.pNext = useMemoryPriority ? &prio : prio.pNext;
		info.allocationSize = size;
		info.memoryTypeIndex = type->memTypeId;

		if (vkAllocateMemory(*m_device, &info, nullptr, &result.memHandle) != VK_SUCCESS)
		{
			break;
		}

		if (flags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
		{
			VkResult status = vkMapMemory(*m_device, result.memHandle, 0, VK_WHOLE_SIZE, 0, &result.memPointer);

			if (status != VK_SUCCESS)
			{
				LOG_ERR("GveMemoryAllocator: Mapping memory failed with ", status);
				vkFreeMemory(*m_device, result.memHandle, nullptr);
				break;
			}
		}

		type->heap->stats.memoryAllocated += size;
	} while (false);

	return result;
}


void GveMemoryAllocator::free(
	const GveMemory&           memory) 
{
	std::lock_guard<std::mutex> lock(m_mutex);
	memory.m_type->heap->stats.memoryUsed -= memory.m_length;

	if (memory.m_chunk != nullptr) 
	{
		freeChunkMemory(
			memory.m_type,
			memory.m_chunk,
			memory.m_offset,
			memory.m_length);
	}
	else 
	{
		GveDeviceMemory devMem;
		devMem.memHandle = memory.m_memory;
		devMem.memPointer = nullptr;
		devMem.memSize = memory.m_length;
		freeDeviceMemory(memory.m_type, devMem);
	}
}


void GveMemoryAllocator::freeChunkMemory(
	GveMemoryType*       type,
	GveMemoryChunk*      chunk,
	VkDeviceSize          offset,
	VkDeviceSize          length) 
{
	chunk->free(offset, length);
}


void GveMemoryAllocator::freeDeviceMemory(
	GveMemoryType*       type,
	GveDeviceMemory      memory) 
{
	vkFreeMemory(*m_device, memory.memHandle, nullptr);
	type->heap->stats.memoryAllocated -= memory.memSize;
}


VkDeviceSize GveMemoryAllocator::pickChunkSize(uint32_t memTypeId) const 
{
	VkMemoryType type = m_memProps.memoryTypes[memTypeId];
	VkMemoryHeap heap = m_memProps.memoryHeaps[type.heapIndex];

	// Default to a chunk size of 128 MiB
	VkDeviceSize chunkSize = 128 << 20;

	// Reduce the chunk size on small heaps so
	// we can at least fit in 15 allocations
	while (chunkSize * 15 > heap.size)
	{
		chunkSize >>= 1;
	}
		
	return chunkSize;
}

///

GveMappedPtr::GveMappedPtr(VkDevice device, VkDeviceMemory memory, 
	VkDeviceSize offset, VkDeviceSize size):
	m_device(device),
	m_memory(memory),
	m_offset(offset),
	m_size(size)
{
	vkMapMemory(m_device, m_memory, m_offset, m_size, 0, &m_ptr);
}

GveMappedPtr::~GveMappedPtr()
{
	vkUnmapMemory(m_device, m_memory);
}

}  // namespace gve