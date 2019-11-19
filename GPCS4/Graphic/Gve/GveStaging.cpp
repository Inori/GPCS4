#include "GveStaging.h"
#include "GveDevice.h"


namespace gve
{;

GveStagingBufferAllocator::GveStagingBufferAllocator(const RcPtr<GveDevice>& device):
	m_device(device)
{

}

GveStagingBufferAllocator::~GveStagingBufferAllocator()
{
}


GveBufferSlice GveStagingBufferAllocator::alloc(VkDeviceSize size, VkDeviceSize align)
{
	GveBufferSlice slice;
	do 
	{
		if (size > MaxBufferSize)
		{
			auto buffer = createBuffer(size);
			slice = GveBufferSlice::fromBuffer(buffer);
			break;
		}

		if (m_buffer == nullptr)
		{
			m_buffer = createBuffer(MaxBufferSize);
		}

		if (!m_buffer->isInUse())
		{
			m_offset = 0;
		}

		m_offset = ALIGN_ROUND(m_offset, align);

		// If this is true, m_buffer must be in use,
		// we need to find another buffer, either in cache or a new one
		if (m_offset + size > MaxBufferSize)  
		{
			m_offset = 0;

			if (m_cacheBuffers.size() < MaxBufferCount)
			{
				// Save the buffer being used, for future use.
				m_cacheBuffers.push(std::move(m_buffer));
			}

			if (!m_cacheBuffers.front()->isInUse())
			{
				m_buffer = std::move(m_cacheBuffers.front());
				m_cacheBuffers.pop();
			}
			else
			{
				m_buffer = createBuffer(size);
			}
		}

		slice = GveBufferSlice(m_buffer, m_offset, size);
		m_offset = ALIGN_ROUND(m_offset + size, align);

	} while (false);
	return slice;
}

void GveStagingBufferAllocator::trim()
{
	m_buffer = nullptr;

	while (!m_cacheBuffers.empty())
	{
		m_cacheBuffers.pop();
	}
}

RcPtr<GveBuffer> GveStagingBufferAllocator::createBuffer(VkDeviceSize size)
{
	GveBufferCreateInfo info;
	info.size = size;
	info.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

	//return m_device->createBuffer(info, 
	//	VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
}

}  // namespace gve