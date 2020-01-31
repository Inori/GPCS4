#include "GveBuffer.h"
#include "GveDevice.h"

LOG_CHANNEL(Graphic.Gve.GveBuffer);

namespace gve
{;

GveBuffer::GveBuffer(const RcPtr<GveDevice>& device, 
	const GveBufferCreateInfo& createInfo,
	GveMemoryAllocator*  memAlloc,
	VkMemoryPropertyFlags memFlags):
	m_device(device),
	m_info(createInfo),
	m_memAlloc(memAlloc),
	m_memFlags(memFlags)
{
	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = createInfo.size;
	bufferInfo.usage = createInfo.usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	createBuffer(bufferInfo);

	m_slice.buffer = m_buffer;
	m_slice.offset = 0;
	m_slice.length = m_info.size;
	m_slice.mapPtr = m_memory.mapPtr(0);
}

GveBuffer::~GveBuffer()
{
	vkDestroyBuffer(*m_device, m_buffer, nullptr);
}


VkBuffer GveBuffer::handle() const
{
	return m_buffer;
}

VkDeviceSize GveBuffer::length() const
{
	return m_info.size;
}

const GveBufferCreateInfo& GveBuffer::info() const
{
	return m_info;
}

const GveBufferSliceWeak& GveBuffer::sliceWeak()
{
	return m_slice;
}

GveBufferSliceWeak GveBuffer::sliceWeak(VkDeviceSize offset, VkDeviceSize length)
{
	GveBufferSliceWeak result;
	result.buffer = m_slice.buffer;
	result.offset = m_slice.offset + offset;
	result.length = length;
	result.mapPtr = mapPtr(offset);
	return result;
}

void* GveBuffer::mapPtr(VkDeviceSize offset) const
{
	return m_memory.mapPtr(offset);
}



void GveBuffer::createBuffer(const VkBufferCreateInfo& info)
{
	do 
	{
		if (vkCreateBuffer(*m_device, &info, nullptr, &m_buffer) != VK_SUCCESS)
		{
			LOG_ERR("Failed to create buffer");
			break;
		}

		VkMemoryDedicatedRequirementsKHR dedicatedRequirements;
		dedicatedRequirements.sType = VK_STRUCTURE_TYPE_MEMORY_DEDICATED_REQUIREMENTS_KHR;
		dedicatedRequirements.pNext = VK_NULL_HANDLE;
		dedicatedRequirements.prefersDedicatedAllocation = VK_FALSE;
		dedicatedRequirements.requiresDedicatedAllocation = VK_FALSE;

		VkMemoryRequirements2 memReq;
		memReq.sType = VK_STRUCTURE_TYPE_MEMORY_REQUIREMENTS_2_KHR;
		memReq.pNext = &dedicatedRequirements;

		VkBufferMemoryRequirementsInfo2 memReqInfo;
		memReqInfo.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_REQUIREMENTS_INFO_2_KHR;
		memReqInfo.buffer = m_buffer;
		memReqInfo.pNext = VK_NULL_HANDLE;

		VkMemoryDedicatedAllocateInfoKHR dedMemoryAllocInfo;
		dedMemoryAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_DEDICATED_ALLOCATE_INFO_KHR;
		dedMemoryAllocInfo.pNext = VK_NULL_HANDLE;
		dedMemoryAllocInfo.buffer = m_buffer;
		dedMemoryAllocInfo.image = VK_NULL_HANDLE;

		vkGetBufferMemoryRequirements2(*m_device, &memReqInfo, &memReq);

		// Use high memory priority for GPU-writable resources
		bool isGpuWritable = (info.usage & (
			VK_BUFFER_USAGE_STORAGE_BUFFER_BIT |
			VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT)) != 0;

		float priority = isGpuWritable ? 1.0f : 0.5f;

		// Ask driver whether we should be using a dedicated allocation
		m_memory = m_memAlloc->alloc(&memReq.memoryRequirements,
			dedicatedRequirements, dedMemoryAllocInfo, m_memFlags, priority);

		if (vkBindBufferMemory(*m_device, m_buffer, m_memory.memory(), m_memory.offset()) != VK_SUCCESS)
		{
			LOG_ERR("Failed to bind device memory");
			break;
		}
	} while (false);
}

///

GveBufferSlice::GveBufferSlice(const RcPtr<GveBuffer>& buffer, 
	VkDeviceSize offset, VkDeviceSize length):
	m_buffer(buffer),
	m_offset(offset),
	m_length(length)
{

}

GveBufferSlice::GveBufferSlice(const RcPtr<GveBuffer>& buffer):
	GveBufferSlice(buffer, 0, buffer->length())
{

}

GveBufferSlice::GveBufferSlice():
	GveBufferSlice(nullptr, 0, 0)
{

}

GveBufferSlice::~GveBufferSlice()
{
}


bool GveBufferSlice::defined() const
{
	return m_buffer != nullptr;
}

RcPtr<GveBuffer> GveBufferSlice::buffer()
{
	return m_buffer;
}

VkBuffer GveBufferSlice::handle() const
{
	return m_buffer->handle();
}

VkDeviceSize GveBufferSlice::offset() const
{
	return m_offset;
}

VkDeviceSize GveBufferSlice::length() const
{
	return m_length;
}

void* GveBufferSlice::mapPtr(VkDeviceSize offset) const
{
	return m_buffer != nullptr ? m_buffer->mapPtr(m_offset + offset) : nullptr;
}

GveBufferSlice GveBufferSlice::fromBuffer(const RcPtr<GveBuffer>& buffer)
{
	return GveBufferSlice(buffer);
}

///

GveBufferView::GveBufferView()
{
}

GveBufferView::~GveBufferView()
{
}



}  // namespace gve