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
	VkDeviceSize size;
	VkBufferUsageFlags usage;
};


// Convenient when we don't want to hold a reference 
struct GveBufferSliceWeak
{
	VkBuffer buffer = VK_NULL_HANDLE;
	VkDeviceSize offset = 0;
	VkDeviceSize length = 0;
	void* mapPtr = nullptr;
};


class GveBuffer : public GveGpuResource
{
public:

	GveBuffer(const RcPtr<GveDevice>& device,
		const GveBufferCreateInfo& createInfo,
		GveMemoryAllocator*  memAlloc,
		VkMemoryPropertyFlags memFlags);

	~GveBuffer();

	VkBuffer handle() const;

	VkDeviceSize length() const;

	const GveBufferCreateInfo& info() const;

	const GveBufferSliceWeak& sliceWeak();

	GveBufferSliceWeak sliceWeak(VkDeviceSize offset, VkDeviceSize length);

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

	RcPtr<GveBuffer> buffer();

	VkBuffer handle() const;

	VkDeviceSize offset() const;

	VkDeviceSize length() const;

	void* mapPtr(VkDeviceSize offset) const;

	static GveBufferSlice fromBuffer(const RcPtr<GveBuffer>& buffer);

private:
	RcPtr<GveBuffer> m_buffer;
	VkDeviceSize m_offset;
	VkDeviceSize m_length;
};


class GveBufferView : public GveGpuResource
{
public:
	GveBufferView();
	~GveBufferView();

private:

};



}  // namespace gve
