#pragma once

#include "GveCommon.h"
#include "GveBuffer.h"
#include <queue>

namespace gve
{;

class GveDevice;
class GveBuffer;


class GveStagingBufferAllocator
{
	const VkDeviceSize MaxBufferSize = 1024 * 1024 * 32;  // 32MB
	const uint32_t MaxBufferCount = 2;

public:
	GveStagingBufferAllocator(const RcPtr<GveDevice>& device);
	~GveStagingBufferAllocator();

	GveBufferSlice alloc(VkDeviceSize size, VkDeviceSize align);

	void trim();

private:
	RcPtr<GveBuffer> createBuffer(VkDeviceSize size);

private:
	RcPtr<GveDevice> m_device;

	RcPtr<GveBuffer> m_buffer;
	VkDeviceSize     m_offset = 0;

	std::queue<RcPtr<GveBuffer>> m_cacheBuffers;
};

}  // namespace gve