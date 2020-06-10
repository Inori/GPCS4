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

/**
 * \brief Texture create information
 *
 * We need input usage type to create proper
 * vulkan textures.
 */
struct GnmTextureCreateInfo
{
	const GnmTexture*    texture;
	VkPipelineStageFlags stages;
	uint8_t              usageType;  // ShaderInputUsageType
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

private:
	sce::SceGpuQueueDevice* m_device;
	GnmMemoryMonitor*       m_monitor;

	std::unordered_map<
		GnmMemoryRange,
		GnmTextureInstance,
		GnmMemoryHash> m_bufferMap;
};

