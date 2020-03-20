#pragma once

#include "VltCommon.h"

#include <array>
#include <vector>
#include <mutex>

namespace vlt
{;

class VltDevice;;
class VltMemoryAllocator;
class VltMemoryChunk;

/**
 * \brief Memory stats
 *
 * Reports the amount of device memory
 * allocated and used by the application.
 */
struct VltMemoryStats 
{
	VkDeviceSize memoryAllocated = 0;
	VkDeviceSize memoryUsed      = 0;
};


/**
 * \brief Device memory object
 *
 * Stores a Vulkan memory object. If the object
 * was allocated on host-visible memory, it will
 * be persistently mapped.
 */
struct VltDeviceMemory 
{
	VkDeviceMemory        memHandle  = VK_NULL_HANDLE;
	void*                 memPointer = nullptr;
	VkDeviceSize          memSize    = 0;
	VkMemoryPropertyFlags memFlags   = 0;
	float                 priority   = 0.0f;
};


/**
 * \brief Memory heap
 *
 * Corresponds to a Vulkan memory heap and stores
 * its properties as well as allocation statistics.
 */
struct VltMemoryHeap 
{
	VkMemoryHeap     properties;
	VltMemoryStats   stats;
};


/**
 * \brief Memory type
 *
 * Corresponds to a Vulkan memory type and stores
 * memory chunks used to sub-allocate memory on
 * this memory type.
 */
struct VltMemoryType 
{
	VltMemoryHeap*    heap;
	uint32_t          heapId;

	VkMemoryType      memType;
	uint32_t          memTypeId;

	VkDeviceSize      chunkSize;

	std::vector<RcPtr<VltMemoryChunk>> chunks;
};


// Move construct only
class VltMappedPtr
{
public:
	VltMappedPtr(
		VkDevice       device,
		VkDeviceMemory memory,
		VkDeviceSize   offset,
		VkDeviceSize   size);
	~VltMappedPtr();

	VltMappedPtr(const VltMappedPtr&) = delete;
	VltMappedPtr& operator=(const VltMappedPtr&) = delete;

private:
	VkDevice       m_device;
	VkDeviceMemory m_memory;
	VkDeviceSize   m_offset;
	VkDeviceSize   m_size;
	void*          m_ptr;
};


/**
 * \brief Memory slice
 *
 * Represents a slice of memory that has
 * been sub-allocated from a bigger chunk.
 */
class VltMemory 
{
	friend class VltMemoryAllocator;
public:

	VltMemory();
	VltMemory(
		VltMemoryAllocator*   alloc,
		VltMemoryChunk*       chunk,
		VltMemoryType*        type,
		VkDeviceMemory        memory,
		VkDeviceSize          offset,
		VkDeviceSize          length,
		void*                 mapPtr);
	VltMemory(VltMemory&& other);
	VltMemory& operator = (VltMemory&& other);
	~VltMemory();

	/**
	 * \brief Memory object
	 *
	 * This information is required when
	 * binding memory to Vulkan objects.
	 * \returns Memory object
	 */
	VkDeviceMemory memory() const 
	{
		return m_memory;
	}

	/**
	 * \brief Offset into device memory
	 *
	 * This information is required when
	 * binding memory to Vulkan objects.
	 * \returns Offset into device memory
	 */
	VkDeviceSize offset() const 
	{
		return m_offset;
	}

	/**
	 * \brief Pointer to mapped data
	 *
	 * \param [in] offset Byte offset
	 * \returns Pointer to mapped data
	 */
	void* mapPtr(VkDeviceSize offset) const 
	{
		return reinterpret_cast<char*>(m_mapPtr) + offset;
	}

	/**
	 * \brief Checks whether the memory slice is defined
	 *
	 * \returns \c true if this slice points to actual device
	 *          memory, and \c false if it is undefined.
	 */
	operator bool() const 
	{
		return m_memory != VK_NULL_HANDLE;
	}

private:
	void free();

private:

	VltMemoryAllocator*   m_alloc = nullptr;
	VltMemoryChunk*       m_chunk = nullptr;
	VltMemoryType*        m_type = nullptr;
	VkDeviceMemory        m_memory = VK_NULL_HANDLE;
	VkDeviceSize          m_offset = 0;
	VkDeviceSize          m_length = 0;
	void*                 m_mapPtr = nullptr;
};


/**
 * \brief Memory chunk
 *
 * A single chunk of memory that provides a
 * sub-allocator. This is not thread-safe.
 */
class VltMemoryChunk : public RcObject 
{

public:

	VltMemoryChunk(
		VltMemoryAllocator*  alloc,
		VltMemoryType*       type,
		VltDeviceMemory      memory);

	~VltMemoryChunk();

	/**
	 * \brief Allocates memory from the chunk
	 *
	 * On failure, this returns a slice with
	 * \c VK_NULL_HANDLE as the memory handle.
	 * \param [in] flags Requested memory flags
	 * \param [in] size Number of bytes to allocate
	 * \param [in] align Required alignment
	 * \param [in] priority Requested priority
	 * \returns The allocated memory slice
	 */
	VltMemory alloc(
		VkMemoryPropertyFlags flags,
		VkDeviceSize          size,
		VkDeviceSize          align,
		float                 priority);

	/**
	 * \brief Frees memory
	 *
	 * Returns a slice back to the chunk.
	 * Called automatically when a memory
	 * slice runs out of scope.
	 * \param [in] offset Slice offset
	 * \param [in] length Slice length
	 */
	void free(
		VkDeviceSize  offset,
		VkDeviceSize  length);

private:

	struct FreeSlice 
	{
		VkDeviceSize offset;
		VkDeviceSize length;
	};

	VltMemoryAllocator*  m_alloc;
	VltMemoryType*       m_type;
	VltDeviceMemory      m_memory;

	std::vector<FreeSlice> m_freeList;

};


/**
 * \brief Memory allocator
 *
 * Allocates device memory for Vulkan resources.
 * Memory objects will be destroyed automatically.
 */
class VltMemoryAllocator 
{
	friend class VltMemory;
	friend class VltMemoryChunk;
public:

	VltMemoryAllocator(VltDevice* device);
	~VltMemoryAllocator();

	/**
	 * \brief Buffer-image granularity
	 *
	 * The granularity between linear and non-linear
	 * resources in adjacent memory locations. See
	 * section 11.6 of the Vulkan spec for details.
	 * \returns Buffer-image granularity
	 */
	VkDeviceSize bufferImageGranularity() const 
	{
		return m_devProps.limits.bufferImageGranularity;
	}

	/**
	 * \brief Allocates device memory
	 *
	 * \param [in] req Memory requirements
	 * \param [in] dedAllocReq Dedicated allocation requirements
	 * \param [in] dedAllocInfo Dedicated allocation info
	 * \param [in] flags Memory type flags
	 * \param [in] priority Device-local memory priority
	 * \returns Allocated memory slice
	 */
	VltMemory alloc(
		const VkMemoryRequirements*             req,
		const VkMemoryDedicatedRequirements&    dedAllocReq,
		const VkMemoryDedicatedAllocateInfoKHR& dedAllocInfo,
		VkMemoryPropertyFlags             flags,
		float                             priority);

	/**
	 * \brief Queries memory stats
	 *
	 * Returns the total amount of device memory
	 * allocated and used by all available heaps.
	 * \returns Global memory stats
	 */
	VltMemoryStats getMemoryStats();

private:

	VltDevice*							   m_device;
	const VkPhysicalDeviceProperties       m_devProps;
	const VkPhysicalDeviceMemoryProperties m_memProps;

	std::mutex                                     m_mutex;
	std::array<VltMemoryHeap, VK_MAX_MEMORY_HEAPS> m_memHeaps;
	std::array<VltMemoryType, VK_MAX_MEMORY_TYPES> m_memTypes;

	VltMemory tryAlloc(
		const VkMemoryRequirements*             req,
		const VkMemoryDedicatedAllocateInfoKHR* dedAllocInfo,
		VkMemoryPropertyFlags             flags,
		float                             priority);

	VltMemory tryAllocFromType(
		VltMemoryType*                    type,
		VkMemoryPropertyFlags             flags,
		VkDeviceSize                      size,
		VkDeviceSize                      align,
		float                             priority,
		const VkMemoryDedicatedAllocateInfoKHR* dedAllocInfo);

	VltDeviceMemory tryAllocDeviceMemory(
		VltMemoryType*                   type,
		VkMemoryPropertyFlags             flags,
		VkDeviceSize                      size,
		float                             priority,
		const VkMemoryDedicatedAllocateInfoKHR* dedAllocInfo);

	void free(
		const VltMemory&           memory);

	void freeChunkMemory(
		VltMemoryType*       type,
		VltMemoryChunk*      chunk,
		VkDeviceSize         offset,
		VkDeviceSize         length);

	void freeDeviceMemory(
		VltMemoryType*       type,
		VltDeviceMemory      memory);

	VkDeviceSize pickChunkSize(
		uint32_t              memTypeId) const;

};

}  // namespace vlt
