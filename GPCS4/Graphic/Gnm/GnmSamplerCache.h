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





