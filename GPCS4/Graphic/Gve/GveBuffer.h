#pragma once

#include "GveCommon.h"
#include "GveMemory.h"
#include "GveDescriptor.h"


namespace gve
{;

class GveDevice;

struct GveBufferCreateInfo
{
	VkDeviceSize size;
	VkBufferUsageFlags usage;
};

class GveBuffer : public RcObject
{
public:

	GveBuffer(const RcPtr<GveDevice>& device,
		const GveBufferCreateInfo& createInfo,
		GveMemoryAllocator&  memAlloc,
		VkMemoryPropertyFlags memFlags);

	~GveBuffer();

	VkBuffer handle() const;

	void* mapPtr(VkDeviceSize offset) const;

	VkDeviceSize size() const;


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


class GveBufferView : public RcObject
{
public:
	GveBufferView();
	~GveBufferView();

private:

};



}  // namespace gve
