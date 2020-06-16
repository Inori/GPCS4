#pragma once

#include "GnmCommon.h"
#include "GnmResourceMemory.h"

#include <unordered_map>

namespace sce
{;
struct SceGpuQueueDevice;
}  // namespace sce

namespace vlt
{;
class VltImage;
class VltImageView;
}  // namespace vlt

class GnmMemoryMonitor;
class GnmTexture;
class GnmRenderTarget;
class GnmDepthRenderTarget;

/**
 * \brief Texture create information
 *
 * We need input usage type to create proper
 * vulkan textures.
 */
struct GnmTextureCreateInfo
{
	union
	{
		const GnmTexture*           texture;
		const GnmRenderTarget*      renderTarget;
		const GnmDepthRenderTarget* depthRenderTarget;
	};

	VkPipelineStageFlags stages;
	bool                 isWritable;
};

struct GnmTextureInstance
{
	RcPtr<vlt::VltImage>     image     = nullptr;
	RcPtr<vlt::VltImageView> view      = nullptr;
	uint32_t                 idleCount = 0;
	GnmResourceMemory        memory;
};

class GnmTextureCache
{
public:
	GnmTextureCache(sce::SceGpuQueueDevice* device,
					GnmMemoryMonitor*       monitor);
	~GnmTextureCache();

	GnmTextureInstance* grabTexture(const GnmTextureCreateInfo& desc);

	void flush(const GnmMemoryRange& range);

	void invalidate(const GnmMemoryRange& range);

private:
	void                collectRenderTargets();
	GnmMemoryRange      extractMemoryRange(const GnmTextureCreateInfo& desc);
	GnmTextureInstance  createTexture(const GnmTextureCreateInfo& desc);

private:
	sce::SceGpuQueueDevice* m_device;
	GnmMemoryMonitor*       m_monitor;

	std::unordered_map<
		GnmMemoryRange,
		GnmTextureInstance,
		GnmMemoryHash> m_textureMap;
};

