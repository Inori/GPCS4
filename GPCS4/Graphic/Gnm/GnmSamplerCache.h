#pragma once

#include "GnmCommon.h"
#include "GnmResourceMemory.h"

namespace sce
{;
struct SceGpuQueueDevice;
}  // namespace sce

namespace vlt
{;
class VltSampler;
}  // namespace vlt

class GnmMemoryMonitor;
class GnmSampler;

struct GnmSammplerInstance
{
	RcPtr<vlt::VltSampler> sampler   = nullptr;
	uint32_t               idleCount = 0;
};

class GnmSamplerCache
{
public:
	GnmSamplerCache(sce::SceGpuQueueDevice* device,
					GnmMemoryMonitor*       monitor);
	~GnmSamplerCache();

private:
	sce::SceGpuQueueDevice* m_device;
	GnmMemoryMonitor*       m_monitor;
};





