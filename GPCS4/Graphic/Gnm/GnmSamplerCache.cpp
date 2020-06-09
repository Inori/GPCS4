#include "GnmSamplerCache.h"

#include "../Violet/VltSampler.h"

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