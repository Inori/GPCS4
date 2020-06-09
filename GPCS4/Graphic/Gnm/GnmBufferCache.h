#pragma once

#include "GnmCommon.h"
#include "GnmResourceMemory.h"

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
	uint8_t              usageType;  // ShaderInputUsageType
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

private:
	sce::SceGpuQueueDevice* m_device;
	GnmMemoryMonitor*       m_monitor;
};

