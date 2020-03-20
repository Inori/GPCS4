#pragma once

#include "VltCommon.h"
#include "VltMemory.h"
#include "VltDescriptor.h"
#include "VltGpuResource.h"


namespace vlt
{;

class VltDevice;

struct VltBufferCreateInfo
{
	// Size of the buffer, in bytes
	VkDeviceSize size;

	// Buffer usage flags
	VkBufferUsageFlags usage;

	// Pipeline stages that can access
	// the contents of the buffer.
	VkPipelineStageFlags stages;

	// Allowed access patterns
	VkAccessFlags access;
};


// Convenient when we don't want to hold a reference 
struct VltBufferSliceHandle
{
	VkBuffer     buffer = VK_NULL_HANDLE;
	VkDeviceSize offset = 0;
	VkDeviceSize length = 0;
	void*        mapPtr = nullptr;
};


class VltBuffer : public VltGpuResource
{
public:

	VltBuffer(const RcPtr<VltDevice>&    device,
			  const VltBufferCreateInfo& createInfo,
			  VltMemoryAllocator*        memAlloc,
			  VkMemoryPropertyFlags      memFlags);

	~VltBuffer();

	VkDeviceSize length() const;

	const VltBufferCreateInfo& info() const;

	const VltBufferSliceHandle& slice();

	VltBufferSliceHandle slice(
		VkDeviceSize offset,
		VkDeviceSize length);

	void* mapPtr(VkDeviceSize offset) const;

private:

	void createBuffer(const VkBufferCreateInfo& info);

private:
	RcPtr<VltDevice>      m_device;
	VltBufferCreateInfo   m_info;
	VltMemoryAllocator*   m_memAlloc;
	VkMemoryPropertyFlags m_memFlags;

	VkBuffer             m_buffer = VK_NULL_HANDLE;
	VltMemory            m_memory;
	VltBufferSliceHandle m_slice;
};


class VltBufferSlice
{
public:
	VltBufferSlice();

	VltBufferSlice(
		const RcPtr<VltBuffer>& buffer,
		VkDeviceSize            offset,
		VkDeviceSize            length);

	VltBufferSlice(const RcPtr<VltBuffer>& buffer);

	~VltBufferSlice();

	bool defined() const;

	RcPtr<VltBuffer> buffer() const;

	VltBufferSliceHandle getHandle() const;

	VkDeviceSize offset() const;

	VkDeviceSize length() const;

	void* mapPtr(VkDeviceSize offset) const;

private:
	RcPtr<VltBuffer> m_buffer;
	VkDeviceSize     m_offset;
	VkDeviceSize     m_length;
};


class VltBufferView : public VltGpuResource
{
public:
	VltBufferView();
	~VltBufferView();

private:

};



}  // namespace vlt
