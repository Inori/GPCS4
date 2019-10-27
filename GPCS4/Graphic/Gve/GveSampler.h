#pragma once

#include "GveCommon.h"
#include "GveMemory.h"
#include "../Gnm/GnmSampler.h"


namespace gve
{;

class GveDevice;

struct GveSamplerCreateInfo
{
	GnmSampler sampler;
};

class GveSampler : public RcObject
{
public:
	GveSampler(const RcPtr<GveDevice>& device, const GveSamplerCreateInfo& createInfo);
	~GveSampler();

	VkSampler handle() const;

	const GnmSampler* getGnmSampler() const;

private:
	RcPtr<GveDevice> m_device;
	GveSamplerCreateInfo m_info;

	VkSampler m_sampler = VK_NULL_HANDLE;
};

}  // namespace gve