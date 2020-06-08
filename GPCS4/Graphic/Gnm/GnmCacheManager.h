#pragma once

#include "GnmMemoryMonitor.h"
#include "GnmBufferCache.h"
#include "GnmTextureCache.h"
#include "GnmSamplerCache.h"

namespace sce
{;
struct SceGpuQueueDevice;
}  // namespace sce


class GnmCacheManager
{
public:
	GnmCacheManager(sce::SceGpuQueueDevice* device);
	~GnmCacheManager();


private:
	sce::SceGpuQueueDevice* m_device;
	GnmMemoryMonitor        m_monitor;
	GnmBufferCache          m_bufferCache;
	GnmTextureCache         m_textureCache;
	GnmSamplerCache         m_samplerCache;
};

