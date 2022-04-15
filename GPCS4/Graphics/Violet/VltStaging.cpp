#include "VltStaging.h"

#include "VltDevice.h"

namespace sce::vlt
{
	VltStagingDataAlloc::VltStagingDataAlloc(VltDevice* device) :
		m_device(device)
	{
	}

	VltStagingDataAlloc::~VltStagingDataAlloc()
	{
	}

	VltBufferSlice VltStagingDataAlloc::alloc(VkDeviceSize size, VkDeviceSize align)
	{
		if (size > MaxBufferSize)
			return VltBufferSlice(createBuffer(size));

		if (m_buffer == nullptr)
			m_buffer = createBuffer(MaxBufferSize);

		if (!m_buffer->isInUse())
			m_offset = 0;

		m_offset = util::align(m_offset, align);

		if (m_offset + size > MaxBufferSize)
		{
			m_offset = 0;

			if (m_buffers.size() < MaxBufferCount)
				m_buffers.push(std::move(m_buffer));

			if (!m_buffers.front()->isInUse())
			{
				m_buffer = std::move(m_buffers.front());
				m_buffers.pop();
			}
			else
			{
				m_buffer = createBuffer(MaxBufferSize);
			}
		}

		VltBufferSlice slice(m_buffer, m_offset, size);
		m_offset = util::align(m_offset + size, align);
		return slice;
	}

	void VltStagingDataAlloc::trim()
	{
		m_buffer = nullptr;
		m_offset = 0;

		while (!m_buffers.empty())
			m_buffers.pop();
	}

	Rc<VltBuffer> VltStagingDataAlloc::createBuffer(VkDeviceSize size)
	{
		VltBufferCreateInfo info;
		info.size   = size;
		info.usage  = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		info.stages = VK_PIPELINE_STAGE_TRANSFER_BIT;
		info.access = VK_ACCESS_TRANSFER_READ_BIT;

		return m_device->createBuffer(info,
									  VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
									  VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	}
}  // namespace sce::vlt