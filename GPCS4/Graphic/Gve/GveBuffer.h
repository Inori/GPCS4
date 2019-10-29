#pragma once

#include "GveCommon.h"
#include "GveMemory.h"
#include "GveDescriptor.h"
#include "../Gnm/GnmBuffer.h"

namespace gve
{;

class GveDevice;

struct GveBufferCreateInfoGnm
{
	GnmBuffer buffer;
	VkBufferUsageFlags usage;
};

struct GveBufferCreateInfoVk
{
	VkDeviceSize size;
	VkBufferUsageFlags usage;
};

class GveBuffer : public RcObject
{

public:
	GveBuffer(const RcPtr<GveDevice>& device,
		const GveBufferCreateInfoGnm& createInfo,
		GveMemoryAllocator&  memAlloc,
		VkMemoryPropertyFlags memFlags);

	GveBuffer(const RcPtr<GveDevice>& device,
		const GveBufferCreateInfoVk& createInfo,
		GveMemoryAllocator&  memAlloc,
		VkMemoryPropertyFlags memFlags);

	~GveBuffer();

	VkBuffer handle() const;

	void* mapPtr(VkDeviceSize offset) const;

	const GnmBuffer* getGnmBuffer() const;

private:
	void convertCreateInfo(const GveBufferCreateInfoGnm& gveInfo,
		VkBufferCreateInfo& vkInfo);

	void createBuffer(const VkBufferCreateInfo& info);

private:
	RcPtr<GveDevice> m_device;
	GveBufferCreateInfoGnm m_gnmInfo;
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
