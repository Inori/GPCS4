#pragma once

#include "GnmCommon.h"
#include "GnmResourceMemory.h"

#include <unordered_map>

namespace sce
{;
struct SceGpuQueueDevice;
}  // namespace sce

namespace vlt
{;
class VltBuffer;
}  // namespace vlt

class GnmMemoryMonitor;
class GnmBuffer;


/**
 * \brief Buffer create information
 *
 * We need input usage type to create proper
 * vulkan buffers.
 */
struct GnmBufferCreateInfo
{
	const GnmBuffer*     buffer;
	VkPipelineStageFlags stages;
	VkBufferUsageFlags   usage;
	uint8_t              inputUsageType;  // ShaderInputUsageType
};


struct GnmBufferInstance
{
	RcPtr<vlt::VltBuffer> buffer    = nullptr;
	uint32_t              idleCount = 0;
	GnmResourceMemory     memory;
};


class GnmBufferCache
{
public:
	GnmBufferCache(sce::SceGpuQueueDevice* device,
				   GnmMemoryMonitor*       monitor);
	~GnmBufferCache();

	GnmBufferInstance grabBuffer(const GnmBufferCreateInfo& desc);

	void flush(const GnmMemoryRange& range);

	void invalidate(const GnmMemoryRange& range);

private:


	GnmBufferInstance createBuffer(const GnmBufferCreateInfo& desc);

private:
	sce::SceGpuQueueDevice* m_device;
	GnmMemoryMonitor*       m_monitor;

	std::unordered_map<
		GnmMemoryRange, GnmBufferInstance,
		GnmMemoryHash> m_bufferMap;
};

