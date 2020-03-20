#include "VltBuffer.h"
#include "VltDevice.h"

LOG_CHANNEL(Graphic.Violet.VltBuffer);

namespace vlt
{;

VltBuffer::VltBuffer(const RcPtr<VltDevice>&    device,
					 const VltBufferCreateInfo& createInfo,
					 VltMemoryAllocator*        memAlloc,
					 VkMemoryPropertyFlags      memFlags) :
	m_device(device),
	m_info(createInfo),
	m_memAlloc(memAlloc),
	m_memFlags(memFlags)
{
	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType              = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size               = createInfo.size;
	bufferInfo.usage              = createInfo.usage;
	bufferInfo.sharingMode        = VK_SHARING_MODE_EXCLUSIVE;

	createBuffer(bufferInfo);

	m_slice.buffer = m_buffer;
	m_slice.offset = 0;
	m_slice.length = m_info.size;
	m_slice.mapPtr = m_memory.mapPtr(0);
}

VltBuffer::~VltBuffer()
{
	vkDestroyBuffer(*m_device, m_buffer, nullptr);
}


VkDeviceSize VltBuffer::length() const
{
	return m_info.size;
}

const VltBufferCreateInfo& VltBuffer::info() const
{
	return m_info;
}

const VltBufferSliceHandle& VltBuffer::slice()
{
	return m_slice;
}

VltBufferSliceHandle VltBuffer::slice(VkDeviceSize offset, VkDeviceSize length)
{
	VltBufferSliceHandle result;
	result.buffer = m_slice.buffer;
	result.offset = m_slice.offset + offset;
	result.length = length;
	result.mapPtr = mapPtr(offset);
	return result;
}

void* VltBuffer::mapPtr(VkDeviceSize offset) const
{
	return m_memory.mapPtr(offset);
}


void VltBuffer::createBuffer(const VkBufferCreateInfo& info)
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

VltBufferSlice::VltBufferSlice(const RcPtr<VltBuffer>& buffer,
							   VkDeviceSize            offset,
							   VkDeviceSize            length) :
	m_buffer(buffer),
	m_offset(offset),
	m_length(length)
{
}

VltBufferSlice::VltBufferSlice(const RcPtr<VltBuffer>& buffer) :
	VltBufferSlice(buffer, 0, buffer->length())
{
}

VltBufferSlice::VltBufferSlice() :
	VltBufferSlice(nullptr, 0, 0)
{
}

VltBufferSlice::~VltBufferSlice()
{
}


bool VltBufferSlice::defined() const
{
	return m_buffer != nullptr;
}

RcPtr<VltBuffer> VltBufferSlice::buffer() const
{
	return m_buffer;
}

VltBufferSliceHandle VltBufferSlice::getHandle() const
{
	return m_buffer->slice(m_offset, m_length);
}

VkDeviceSize VltBufferSlice::offset() const
{
	return m_offset;
}

VkDeviceSize VltBufferSlice::length() const
{
	return m_length;
}

void* VltBufferSlice::mapPtr(VkDeviceSize offset) const
{
	return m_buffer != nullptr ? m_buffer->mapPtr(m_offset + offset) : nullptr;
}


///

VltBufferView::VltBufferView()
{
}

VltBufferView::~VltBufferView()
{
}



}  // namespace vlt