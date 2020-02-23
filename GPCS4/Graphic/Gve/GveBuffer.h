#pragma once

#include "GveCommon.h"
#include "GveMemory.h"
#include "GveDescriptor.h"
#include "GveGpuResource.h"


namespace gve
{;

class GveDevice;

struct GveBufferCreateInfo
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
struct GveBufferSliceWeak
{
	VkBuffer     buffer = VK_NULL_HANDLE;
	VkDeviceSize offset = 0;
	VkDeviceSize length = 0;
	void*        mapPtr = nullptr;
};


class GveBuffer : public GveGpuResource
{
public:

	GveBuffer(const RcPtr<GveDevice>&    device,
			  const GveBufferCreateInfo& createInfo,
			  GveMemoryAllocator*        memAlloc,
			  VkMemoryPropertyFlags      memFlags);

	~GveBuffer();

	VkDeviceSize length() const;

	const GveBufferCreateInfo& info() const;

	const GveBufferSliceWeak& slice();

	GveBufferSliceWeak slice(VkDeviceSize offset, VkDeviceSize length);

	void* mapPtr(VkDeviceSize offset) const;

private:

	void createBuffer(const VkBufferCreateInfo& info);

private:
	RcPtr<GveDevice> m_device;
	GveBufferCreateInfo m_info;
	GveMemoryAllocator* m_memAlloc;
	VkMemoryPropertyFlags m_memFlags;

	VkBuffer m_buffer = VK_NULL_HANDLE;
	GveMemory m_memory;
	GveBufferSliceWeak m_slice;
};


class GveBufferSlice
{
public:
	GveBufferSlice();
	GveBufferSlice(const RcPtr<GveBuffer>& buffer, VkDeviceSize offset, VkDeviceSize length);
	GveBufferSlice(const RcPtr<GveBuffer>& buffer);
	~GveBufferSlice();

	bool defined() const;

	RcPtr<GveBuffer> buffer() const;

	GveBufferSliceWeak slice() const;

	VkDeviceSize offset() const;

	VkDeviceSize length() const;

	void* mapPtr(VkDeviceSize offset) const;

private:
	RcPtr<GveBuffer> m_buffer;
	VkDeviceSize     m_offset;
	VkDeviceSize     m_length;
};


class GveBufferView : public GveGpuResource
{
public:
	GveBufferView();
	~GveBufferView();

private:

};



}  // namespace gve
