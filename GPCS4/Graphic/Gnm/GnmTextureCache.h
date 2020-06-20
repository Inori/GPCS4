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
class VltContext;
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
	bool                 isGpuWritable;
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
	GnmTextureCache(const sce::SceGpuQueueDevice* device,
					vlt::VltContext*              context,
					GnmMemoryMonitor*             monitor);
	~GnmTextureCache();

	GnmTextureInstance* grabTexture(const GnmTextureCreateInfo& desc);

	void flush(const GnmMemoryRange& range);

	void invalidate(const GnmMemoryRange& range);

	void sync();

private:
	void                collectRenderTargets();
	GnmMemoryRange      extractMemoryRange(const GnmTextureCreateInfo& desc);
	GnmTextureInstance  createTexture(const GnmTextureCreateInfo& desc);
	void                upload(GnmTextureInstance& texture);

private:
	const sce::SceGpuQueueDevice* m_device;
	vlt::VltContext*              m_context;
	GnmMemoryMonitor*             m_monitor;

	std::unordered_map<
		GnmMemoryRange,
		GnmTextureInstance,
		GnmMemoryHash> m_textureMap;
};

