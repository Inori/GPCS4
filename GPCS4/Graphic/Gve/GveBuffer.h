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
};


class GveBufferSlice
{
public:
	GveBufferSlice();
	GveBufferSlice(const RcPtr<GveBuffer>& buffer, VkDeviceSize offset, VkDeviceSize length);
	GveBufferSlice(const RcPtr<GveBuffer>& buffer);
	~GveBufferSlice();

	RcPtr<GveBuffer> buffer();

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
