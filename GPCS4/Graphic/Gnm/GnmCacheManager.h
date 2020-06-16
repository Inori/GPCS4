#pragma once

#include "GnmMemoryMonitor.h"
#include "GnmBufferCache.h"
#include "GnmTextureCache.h"
#include "GnmSamplerCache.h"

#include <memory>

namespace sce
{;
struct SceGpuQueueDevice;
}  // namespace sce

namespace vlt
{;
class VltContext;
}  // namespace vlt


class GnmCacheManager
{
public:
	GnmCacheManager(sce::SceGpuQueueDevice* device,
					vlt::VltContext*        context);
	~GnmCacheManager();

	/**
	 * \brief Get or create a buffer object.
	 * 
	 * \param desc Buffer create information.
	 * \returns GnmBufferInstance*
	 */
	GnmBufferInstance* grabBuffer(const GnmBufferCreateInfo& desc);

	/**
	 * \brief Get or create a texture object.
	 * 
	 * \param desc Texture create information.
	 * \returns GnmBufferInstance*
	 */
	GnmTextureInstance* grabTexture(const GnmTextureCreateInfo& desc);

	/**
	 * \brief Get or create a sampler object.
	 * 
	 * \param desc Sampler create information.
	 * \returns GnmBufferInstance*
	 */
	GnmSammplerInstance* grabSampler(const GnmSamplerCreateInfo& desc);

private:
	void onMemoryRead(const GnmMemoryRange& range);

	void onMemoryWrite(const GnmMemoryRange& range);

private:
	sce::SceGpuQueueDevice* m_device;
	vlt::VltContext*        m_context;
	GnmMemoryMonitor        m_monitor;
	GnmBufferCache          m_bufferCache;
	GnmTextureCache         m_textureCache;
	GnmSamplerCache         m_samplerCache;
};

