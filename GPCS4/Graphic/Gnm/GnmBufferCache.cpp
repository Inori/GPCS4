#include "GnmBufferCache.h"

#include "../Violet/VltBuffer.h"

GnmBufferCache::GnmBufferCache(sce::SceGpuQueueDevice* device,
							   GnmMemoryMonitor*       monitor) :
	m_device(device),
	m_monitor(monitor)
{
}

GnmBufferCache::~GnmBufferCache()
{
}