#include "VltStaging.h"
#include "VltDevice.h"



namespace vlt
{;

VltStagingBufferAllocator::VltStagingBufferAllocator(const RcPtr<VltDevice>& device):
	m_device(device)
{

}

VltStagingBufferAllocator::~VltStagingBufferAllocator()
{
}


VltBufferSlice VltStagingBufferAllocator::alloc(VkDeviceSize size, VkDeviceSize align)
{
	VltBufferSlice slice;
	do 
	{
		if (size > MaxBufferSize)
		{
			slice = VltBufferSlice(createBuffer(size));
			break;
		}

		if (m_buffer == nullptr)
		{
			m_buffer = createBuffer(MaxBufferSize);
		}

		if (!m_buffer->busy())
		{
			// If buffer not in use, we can safely go to start.
			m_offset = 0;
		}

		m_offset = ::util::align(m_offset, align);

		// If this is true, m_buffer must be in use,
		// we need to find another buffer, either in cache or a new one
		if (m_offset + size > MaxBufferSize)  
		{
			m_offset = 0;

			if (m_bufferCache.size() < MaxBufferCount)
			{
				// Save the buffer being used, for future use.
				m_bufferCache.push(std::move(m_buffer));
			}

			if (!m_bufferCache.front()->busy())
			{
				m_buffer = std::move(m_bufferCache.front());
				m_bufferCache.pop();
			}
			else
			{
				m_buffer = createBuffer(size);
			}
		}

		slice = VltBufferSlice(m_buffer, m_offset, size);
		m_offset = ::util::align(m_offset + size, align);

	} while (false);
	return slice;
}

void VltStagingBufferAllocator::trim()
{
	m_buffer = nullptr;

	while (!m_bufferCache.empty())
	{
		m_bufferCache.pop();
	}
}

RcPtr<VltBuffer> VltStagingBufferAllocator::createBuffer(VkDeviceSize size)
{
	VltBufferCreateInfo info;
	info.size   = size;
	info.usage  = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
	info.stages = VK_PIPELINE_STAGE_TRANSFER_BIT;
	info.access = VK_ACCESS_TRANSFER_READ_BIT;

	return m_device->createBuffer(
		info,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
}

}  // namespace vlt