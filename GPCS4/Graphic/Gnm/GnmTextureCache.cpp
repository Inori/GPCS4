#include "GnmTextureCache.h"

#include "../Violet/VltImage.h"

GnmTextureCache::GnmTextureCache(sce::SceGpuQueueDevice* device,
								 GnmMemoryMonitor*       monitor) :
	m_device(device),
	m_monitor(monitor)
{
}

GnmTextureCache::~GnmTextureCache()
{

}
