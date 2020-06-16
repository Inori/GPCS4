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
class VltSampler;
}  // namespace vlt

class GnmMemoryMonitor;
class GnmSampler;

struct GnmSamplerCreateInfo
{
	const GnmSampler* sampler;
};

struct GnmSammplerInstance
{
	RcPtr<vlt::VltSampler> sampler;
	uint32_t               idleCount = 0;
};

class GnmSamplerCache
{
public:
	GnmSamplerCache(const sce::SceGpuQueueDevice* device);
	~GnmSamplerCache();

	GnmSammplerInstance* grabSampler(const GnmSamplerCreateInfo& desc);

private:
	GnmSammplerInstance createSampler(const GnmSamplerCreateInfo& desc);

private:
	const sce::SceGpuQueueDevice* m_device;

	std::unordered_map<
		GnmMemoryRange,
		GnmSammplerInstance,
		GnmMemoryHash> m_samplerMap;
};





