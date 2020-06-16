#include "GnmCacheManager.h"

#include "../Violet/VltBuffer.h"
#include "../Violet/VltImage.h"
#include "../Violet/VltSampler.h"

GnmCacheManager::GnmCacheManager(sce::SceGpuQueueDevice* device,
								 vlt::VltContext*        context) :
	m_device(device),
	m_context(context),
	m_monitor(
		{ [this](const GnmMemoryRange& range) { onMemoryRead(range); },
		  [this](const GnmMemoryRange& range) { onMemoryWrite(range); } }),
	m_bufferCache(m_device, m_context, &m_monitor),
	m_textureCache(m_device, m_context, &m_monitor),
	m_samplerCache(m_device)
{
}

GnmCacheManager::~GnmCacheManager()
{
}

GnmBufferInstance* GnmCacheManager::grabBuffer(const GnmBufferCreateInfo& desc)
{
	return m_bufferCache.grabBuffer(desc);
}

GnmTextureInstance* GnmCacheManager::grabTexture(const GnmTextureCreateInfo& desc)
{
	return m_textureCache.grabTexture(desc);
}

GnmSammplerInstance* GnmCacheManager::grabSampler(const GnmSamplerCreateInfo& desc)
{
	return m_samplerCache.grabSampler(desc);
}

void GnmCacheManager::onMemoryRead(const GnmMemoryRange& range)
{
	m_bufferCache.flush(range);
	m_textureCache.flush(range);
}

void GnmCacheManager::onMemoryWrite(const GnmMemoryRange& range)
{
	m_bufferCache.invalidate(range);
	m_textureCache.invalidate(range);
}

