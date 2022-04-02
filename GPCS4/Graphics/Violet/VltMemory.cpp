#include "VltMemory.h"

#include "VltDevice.h"

namespace sce::vlt
{

	VltMemory::VltMemory()
	{
	}

	VltMemory::VltMemory(
		VltMemoryAllocator* alloc,
		VltMemoryChunk*     chunk,
		VltMemoryType*      type,
		VkDeviceMemory      memory,
		VkDeviceSize        offset,
		VkDeviceSize        length,
		void*               mapPtr) :
		m_alloc(alloc),
		m_chunk(chunk),
		m_type(type),
		m_memory(memory),
		m_offset(offset),
		m_length(length),
		m_mapPtr(mapPtr)
	{
	}

	VltMemory::VltMemory(VltMemory&& other) :
		m_alloc(std::exchange(other.m_alloc, nullptr)),
		m_chunk(std::exchange(other.m_chunk, nullptr)),
		m_type(std::exchange(other.m_type, nullptr)),
		m_memory(std::exchange(other.m_memory, VkDeviceMemory(VK_NULL_HANDLE))),
		m_offset(std::exchange(other.m_offset, 0)),
		m_length(std::exchange(other.m_length, 0)),
		m_mapPtr(std::exchange(other.m_mapPtr, nullptr))
	{
	}

	VltMemory& VltMemory::operator=(VltMemory&& other)
	{
		this->free();
		m_alloc  = std::exchange(other.m_alloc, nullptr);
		m_chunk  = std::exchange(other.m_chunk, nullptr);
		m_type   = std::exchange(other.m_type, nullptr);
		m_memory = std::exchange(other.m_memory, VkDeviceMemory(VK_NULL_HANDLE));
		m_offset = std::exchange(other.m_offset, 0);
		m_length = std::exchange(other.m_length, 0);
		m_mapPtr = std::exchange(other.m_mapPtr, nullptr);
		return *this;
	}

	VltMemory::~VltMemory()
	{
		this->free();
	}

	void VltMemory::free()
	{
		if (m_alloc != nullptr)
			m_alloc->free(*this);
	}

	VltMemoryChunk::VltMemoryChunk(
		VltMemoryAllocator* alloc,
		VltMemoryType*      type,
		VltDeviceMemory     memory,
		VltMemoryFlags      hints) :
		m_alloc(alloc),
		m_type(type), m_memory(memory), m_hints(hints)
	{
		// Mark the entire chunk as free
		m_freeList.push_back(FreeSlice{ 0, memory.memSize });
	}

	VltMemoryChunk::~VltMemoryChunk()
	{
		// This call is technically not thread-safe, but it
		// doesn't need to be since we don't free chunks
		m_alloc->freeDeviceMemory(m_type, m_memory);
	}

	VltMemory VltMemoryChunk::alloc(
		VkMemoryPropertyFlags flags,
		VkDeviceSize          size,
		VkDeviceSize          align,
		VltMemoryFlags        hints)
	{
		// Property flags must be compatible. This could
		// be refined a bit in the future if necessary.
		if (m_memory.memFlags != flags || !checkHints(hints))
			return VltMemory();

		// If the chunk is full, return
		if (m_freeList.size() == 0)
			return VltMemory();

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
		const VkDeviceSize sliceEnd   = bestSlice->offset + bestSlice->length;

		const VkDeviceSize allocStart = util::align(sliceStart, align);
		const VkDeviceSize allocEnd   = util::align(allocStart + size, align);

		if (allocEnd > sliceEnd)
			return VltMemory();

		// We can use this slice, but we'll have to add
		// the unused parts of it back to the free list.
		m_freeList.erase(bestSlice);

		if (allocStart != sliceStart)
			m_freeList.push_back({ sliceStart, allocStart - sliceStart });

		if (allocEnd != sliceEnd)
			m_freeList.push_back({ allocEnd, sliceEnd - allocEnd });

		// Create the memory object with the aligned slice
		return VltMemory(m_alloc, this, m_type,
						 m_memory.memHandle, allocStart, allocEnd - allocStart,
						 reinterpret_cast<char*>(m_memory.memPointer) + allocStart);
	}

	void VltMemoryChunk::free(
		VkDeviceSize offset,
		VkDeviceSize length)
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

	bool VltMemoryChunk::isEmpty() const
	{
		return m_freeList.size() == 1 && m_freeList[0].length == m_memory.memSize;
	}

	bool VltMemoryChunk::isCompatible(const Rc<VltMemoryChunk>& other) const
	{
		return other->m_memory.memFlags == m_memory.memFlags && other->m_hints == m_hints;
	}

	bool VltMemoryChunk::checkHints(VltMemoryFlags hints) const
	{
		VltMemoryFlags mask(
			VltMemoryFlag::Small,
			VltMemoryFlag::GpuReadable,
			VltMemoryFlag::GpuWritable,
			VltMemoryFlag::Transient);

		if (hints.test(VltMemoryFlag::IgnoreConstraints))
			mask = VltMemoryFlags();

		return (m_hints & mask) == (hints & mask);
	}

	VltMemoryAllocator::VltMemoryAllocator(const VltDevice* device) :
		m_device(device),
		m_devProps(device->adapter()->deviceProperties()),
		m_memProps(device->adapter()->memoryProperties())
	{
		for (uint32_t i = 0; i < m_memProps.memoryHeapCount; i++)
		{
			m_memHeaps[i].properties = m_memProps.memoryHeaps[i];
			m_memHeaps[i].stats      = VltMemoryStats{ 0, 0 };
			m_memHeaps[i].budget     = 0;

			/* Target 80% of a heap on systems where we want
             * to avoid oversubscribing memory heaps */
			if ((m_memProps.memoryHeaps[i].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) && (m_device->isUnifiedMemoryArchitecture()))
				m_memHeaps[i].budget = (8 * m_memProps.memoryHeaps[i].size) / 10;
		}

		for (uint32_t i = 0; i < m_memProps.memoryTypeCount; i++)
		{
			m_memTypes[i].heap      = &m_memHeaps[m_memProps.memoryTypes[i].heapIndex];
			m_memTypes[i].heapId    = m_memProps.memoryTypes[i].heapIndex;
			m_memTypes[i].memType   = m_memProps.memoryTypes[i];
			m_memTypes[i].memTypeId = i;
		}

		/* Check what kind of heap the HVV memory type is on, if any. If the
         * HVV memory type is on the largest device-local heap, we either have
         * an UMA system or an RBAR-enabled system. Otherwise, there will likely
         * be a separate, smaller heap for it. */
		VkDeviceSize largestDeviceLocalHeap = 0;

		for (uint32_t i = 0; i < m_memProps.memoryTypeCount; i++)
		{
			if (m_memTypes[i].memType.propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
				largestDeviceLocalHeap = std::max(largestDeviceLocalHeap, m_memTypes[i].heap->properties.size);
		}

		/* Work around an issue on Nvidia drivers where using the entire
         * device_local | host_visible heap can cause crashes or slowdowns */

		//if (m_device->properties().core.properties.vendorID == uint16_t(DxvkGpuVendor::Nvidia))
		//{
		//	bool shrinkNvidiaHvvHeap = device->adapter()->matchesDriver(DxvkGpuVendor::Nvidia,
		//																VK_DRIVER_ID_NVIDIA_PROPRIETARY_KHR, 0, VK_MAKE_VERSION(465, 0, 0));

		//	applyTristate(shrinkNvidiaHvvHeap, device->config().shrinkNvidiaHvvHeap);

		//	if (shrinkNvidiaHvvHeap)
		//	{
		//		for (uint32_t i = 0; i < m_memProps.memoryTypeCount; i++)
		//		{
		//			VkMemoryPropertyFlags hvvFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;

		//			if ((m_memTypes[i].memType.propertyFlags & hvvFlags) == hvvFlags && (m_memTypes[i].heap->properties.size < largestDeviceLocalHeap))
		//				m_memTypes[i].heap->budget = 32 << 20;
		//		}
		//	}
		//}
	}

	VltMemoryAllocator::~VltMemoryAllocator()
	{
	}

	VltMemory VltMemoryAllocator::alloc(
		const VkMemoryRequirements*          req,
		const VkMemoryDedicatedRequirements& dedAllocReq,
		const VkMemoryDedicatedAllocateInfo& dedAllocInfo,
		VkMemoryPropertyFlags                flags,
		VltMemoryFlags                       hints)
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		// Keep small allocations together to avoid fragmenting
		// chunks for larger resources with lots of small gaps,
		// as well as resources with potentially weird lifetimes
		if (req->size <= SmallAllocationThreshold)
		{
			hints.set(VltMemoryFlag::Small);
			hints.clr(VltMemoryFlag::GpuWritable, VltMemoryFlag::GpuReadable);
		}

		// Ignore most hints for host-visible allocations since they
		// usually don't make much sense for those resources
		if (flags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
			hints = hints & VltMemoryFlag::Transient;

		// Try to allocate from a memory type which supports the given flags exactly
		auto      dedAllocPtr = dedAllocReq.prefersDedicatedAllocation ? &dedAllocInfo : nullptr;
		VltMemory result      = this->tryAlloc(req, dedAllocPtr, flags, hints);

		// If the first attempt failed, try ignoring the dedicated allocation
		if (!result && dedAllocPtr && !dedAllocReq.requiresDedicatedAllocation)
		{
			result      = this->tryAlloc(req, nullptr, flags, hints);
			dedAllocPtr = nullptr;
		}

		// Retry without the hint constraints
		if (!result)
		{
			hints.set(VltMemoryFlag::IgnoreConstraints);
			result = this->tryAlloc(req, nullptr, flags, hints);
		}

		// If that still didn't work, probe slower memory types as well
		VkMemoryPropertyFlags optFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT;
		VkMemoryPropertyFlags remFlags = 0;

		while (!result && (flags & optFlags))
		{
			remFlags |= optFlags & -optFlags;
			optFlags &= ~remFlags;

			result = this->tryAlloc(req, dedAllocPtr, flags & ~remFlags, hints);
		}

		if (!result)
		{
			VltAdapterMemoryInfo memHeapInfo = m_device->adapter()->getMemoryHeapInfo();

			Logger::err(util::str::formatex(
				"DxvkMemoryAllocator: Memory allocation failed",
				"\n  Size:      ", req->size,
				"\n  Alignment: ", req->alignment,
				"\n  Mem flags: ", "0x", std::hex, flags,
				"\n  Mem types: ", "0x", std::hex, req->memoryTypeBits));

			for (uint32_t i = 0; i < m_memProps.memoryHeapCount; i++)
			{
				Logger::err(util::str::formatex("Heap ", i, ": ",
												(m_memHeaps[i].stats.memoryAllocated >> 20), " MB allocated, ",
												(m_memHeaps[i].stats.memoryUsed >> 20), " MB used, ",
												m_device->extensions().extMemoryBudget
													? util::str::formatex(
														  (memHeapInfo.heaps[i].memoryAllocated >> 20), " MB allocated (driver), ",
														  (memHeapInfo.heaps[i].memoryBudget >> 20), " MB budget (driver), ",
														  (m_memHeaps[i].properties.size >> 20), " MB total")
													: util::str::formatex(
														  (m_memHeaps[i].properties.size >> 20), " MB total")));
			}

			Logger::exception("DxvkMemoryAllocator: Memory allocation failed");
		}

		return result;
	}

	VltMemory VltMemoryAllocator::tryAlloc(
		const VkMemoryRequirements*          req,
		const VkMemoryDedicatedAllocateInfo* dedAllocInfo,
		VkMemoryPropertyFlags                flags,
		VltMemoryFlags                       hints)
	{
		VltMemory result;

		for (uint32_t i = 0; i < m_memProps.memoryTypeCount && !result; i++)
		{
			const bool supported = (req->memoryTypeBits & (1u << i)) != 0;
			const bool adequate  = (m_memTypes[i].memType.propertyFlags & flags) == flags;

			if (supported && adequate)
			{
				result = this->tryAllocFromType(&m_memTypes[i],
												flags, req->size, req->alignment, hints, dedAllocInfo);
			}
		}

		return result;
	}

	VltMemory VltMemoryAllocator::tryAllocFromType(
		VltMemoryType*                       type,
		VkMemoryPropertyFlags                flags,
		VkDeviceSize                         size,
		VkDeviceSize                         align,
		VltMemoryFlags                       hints,
		const VkMemoryDedicatedAllocateInfo* dedAllocInfo)
	{
		VkDeviceSize chunkSize = pickChunkSize(type->memTypeId, hints);

		VltMemory memory;

		if (size >= chunkSize || dedAllocInfo)
		{
			if (this->shouldFreeEmptyChunks(type->heap, size))
				this->freeEmptyChunks(type->heap);

			VltDeviceMemory devMem = this->tryAllocDeviceMemory(
				type, flags, size, hints, dedAllocInfo);

			if (devMem.memHandle != VK_NULL_HANDLE)
				memory = VltMemory(this, nullptr, type, devMem.memHandle, 0, size, devMem.memPointer);
		}
		else
		{
			for (uint32_t i = 0; i < type->chunks.size() && !memory; i++)
				memory = type->chunks[i]->alloc(flags, size, align, hints);

			if (!memory)
			{
				VltDeviceMemory devMem;

				if (this->shouldFreeEmptyChunks(type->heap, chunkSize))
					this->freeEmptyChunks(type->heap);

				for (uint32_t i = 0; i < 6 && (chunkSize >> i) >= size && !devMem.memHandle; i++)
					devMem = tryAllocDeviceMemory(type, flags, chunkSize >> i, hints, nullptr);

				if (devMem.memHandle)
				{
					Rc<VltMemoryChunk> chunk = new VltMemoryChunk(this, type, devMem, hints);
					memory                   = chunk->alloc(flags, size, align, hints);

					type->chunks.push_back(std::move(chunk));
				}
			}
		}

		if (memory)
			type->heap->stats.memoryUsed += memory.m_length;

		return memory;
	}

	VltDeviceMemory VltMemoryAllocator::tryAllocDeviceMemory(
		VltMemoryType*                       type,
		VkMemoryPropertyFlags                flags,
		VkDeviceSize                         size,
		VltMemoryFlags                       hints,
		const VkMemoryDedicatedAllocateInfo* dedAllocInfo)
	{
		bool useMemoryPriority = (flags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) &&
			(m_device->features().extMemoryPriority.memoryPriority);

		if (type->heap->budget && type->heap->stats.memoryAllocated + size > type->heap->budget)
			return VltDeviceMemory();

		float priority = 0.0f;

		if (hints.test(VltMemoryFlag::GpuReadable))
			priority = 0.5f;
		if (hints.test(VltMemoryFlag::GpuWritable))
			priority = 1.0f;

		VltDeviceMemory result;
		result.memSize  = size;
		result.memFlags = flags;
		result.priority = priority;

		VkMemoryPriorityAllocateInfoEXT prio;
		prio.sType    = VK_STRUCTURE_TYPE_MEMORY_PRIORITY_ALLOCATE_INFO_EXT;
		prio.pNext    = dedAllocInfo;
		prio.priority = priority;

		VkMemoryAllocateInfo info;
		info.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		info.pNext           = useMemoryPriority ? &prio : prio.pNext;
		info.allocationSize  = size;
		info.memoryTypeIndex = type->memTypeId;

		if (vkAllocateMemory(m_device->handle(), &info, nullptr, &result.memHandle) != VK_SUCCESS)
			return VltDeviceMemory();

		if (flags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
		{
			VkResult status = vkMapMemory(m_device->handle(), result.memHandle, 0, VK_WHOLE_SIZE, 0, &result.memPointer);

			if (status != VK_SUCCESS)
			{
				Logger::err(util::str::formatex("DxvkMemoryAllocator: Mapping memory failed with ", status));
				vkFreeMemory(m_device->handle(), result.memHandle, nullptr);
				return VltDeviceMemory();
			}
		}

		type->heap->stats.memoryAllocated += size;
		m_device->adapter()->notifyHeapMemoryAlloc(type->heapId, size);
		return result;
	}

	void VltMemoryAllocator::free(
		const VltMemory& memory)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		memory.m_type->heap->stats.memoryUsed -= memory.m_length;

		if (memory.m_chunk != nullptr)
		{
			this->freeChunkMemory(
				memory.m_type,
				memory.m_chunk,
				memory.m_offset,
				memory.m_length);
		}
		else
		{
			VltDeviceMemory devMem;
			devMem.memHandle  = memory.m_memory;
			devMem.memPointer = nullptr;
			devMem.memSize    = memory.m_length;
			this->freeDeviceMemory(memory.m_type, devMem);
		}
	}

	void VltMemoryAllocator::freeChunkMemory(
		VltMemoryType*  type,
		VltMemoryChunk* chunk,
		VkDeviceSize    offset,
		VkDeviceSize    length)
	{
		chunk->free(offset, length);

		if (chunk->isEmpty())
		{
			Rc<VltMemoryChunk> chunkRef = chunk;

			// Free the chunk if we have to, or at least put it at the end of
			// the list so that chunks that are already in use and cannot be
			// freed are prioritized for allocations to reduce memory pressure.
			type->chunks.erase(std::remove(type->chunks.begin(), type->chunks.end(), chunkRef));

			if (!this->shouldFreeChunk(type, chunkRef))
				type->chunks.push_back(std::move(chunkRef));
		}
	}

	void VltMemoryAllocator::freeDeviceMemory(
		VltMemoryType*  type,
		VltDeviceMemory memory)
	{
		vkFreeMemory(m_device->handle(), memory.memHandle, nullptr);
		type->heap->stats.memoryAllocated -= memory.memSize;
		m_device->adapter()->notifyHeapMemoryFree(type->heapId, memory.memSize);
	}

	VkDeviceSize VltMemoryAllocator::pickChunkSize(uint32_t memTypeId, VltMemoryFlags hints) const
	{
		VkMemoryType type = m_memProps.memoryTypes[memTypeId];
		VkMemoryHeap heap = m_memProps.memoryHeaps[type.heapIndex];

		// Default to a chunk size of 128 MiB
		VkDeviceSize chunkSize = 128 << 20;

		if (hints.test(VltMemoryFlag::Small))
			chunkSize = 16 << 20;

		if (type.propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
			chunkSize = 64 << 20;

		// Reduce the chunk size on small heaps so
		// we can at least fit in 15 allocations
		while (chunkSize * 15 > heap.size)
			chunkSize >>= 1;

		return chunkSize;
	}

	bool VltMemoryAllocator::shouldFreeChunk(
		const VltMemoryType*      type,
		const Rc<VltMemoryChunk>& chunk) const
	{
		// Under memory pressure, we should start freeing everything.
		if (this->shouldFreeEmptyChunks(type->heap, 0))
			return true;

		// Even if we have enough memory to spare, only keep
		// one chunk of each type around to save memory.
		for (const auto& c : type->chunks)
		{
			if (c != chunk && c->isEmpty() && c->isCompatible(chunk))
				return true;
		}

		return false;
	}

	bool VltMemoryAllocator::shouldFreeEmptyChunks(
		const VltMemoryHeap* heap,
		VkDeviceSize         allocationSize) const
	{
		VkDeviceSize budget = heap->budget;

		if (!budget)
			budget = (heap->properties.size * 4) / 5;

		return heap->stats.memoryAllocated + allocationSize > budget;
	}

	void VltMemoryAllocator::freeEmptyChunks(
		const VltMemoryHeap* heap)
	{
		for (uint32_t i = 0; i < m_memProps.memoryTypeCount; i++)
		{
			VltMemoryType* type = &m_memTypes[i];

			if (type->heap != heap)
				continue;

			type->chunks.erase(
				std::remove_if(type->chunks.begin(), type->chunks.end(),
							   [](const Rc<VltMemoryChunk>& chunk)
							   { return chunk->isEmpty(); }),
				type->chunks.end());
		}
	}
}  // namespace sce::vlt