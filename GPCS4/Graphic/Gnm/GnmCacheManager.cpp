#include "GnmCacheManager.h"

#include "../Violet/VltBuffer.h"
#include "../Violet/VltImage.h"
#include "../Violet/VltSampler.h"

GnmCacheManager::GnmCacheManager(sce::SceGpuQueueDevice* device) :
	m_device(device),
	m_monitor(),
	m_bufferCache(m_device, &m_monitor),
	m_textureCache(m_device, &m_monitor),
	m_samplerCache(m_device, &m_monitor)
{
}

GnmCacheManager::~GnmCacheManager()
{
}

