#include "GnmSamplerCache.h"
#include "GnmMemoryMonitor.h"

#include "../Violet/VltSampler.h"

GnmSamplerCache::GnmSamplerCache(sce::SceGpuQueueDevice* device,
								 GnmMemoryMonitor*       monitor) :
	m_device(device),
	m_monitor(monitor)
{
}

GnmSamplerCache::~GnmSamplerCache()
{
}