#pragma once

#include "VltCommon.h"
#include "VltBuffer.h"
#include <queue>

namespace vlt
{;

class VltDevice;


class VltStagingBufferAllocator : public RcObject
{
	const VkDeviceSize MaxBufferSize  = 1024 * 1024 * 32;  // 32MB
	const uint32_t     MaxBufferCount = 2;

public:
	VltStagingBufferAllocator(
		const RcPtr<VltDevice>& device);
	~VltStagingBufferAllocator();

	VltBufferSlice alloc(VkDeviceSize size, VkDeviceSize align);

	void trim();

private:
	RcPtr<VltBuffer> createBuffer(VkDeviceSize size);

private:
	RcPtr<VltDevice> m_device;

	RcPtr<VltBuffer> m_buffer;
	VkDeviceSize     m_offset = 0;

	std::queue<RcPtr<VltBuffer>> m_bufferCache;
};

}  // namespace vlt