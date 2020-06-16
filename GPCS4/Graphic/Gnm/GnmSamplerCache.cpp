#include "GnmSamplerCache.h"

#include "GnmSampler.h"

#include "../Violet/VltDevice.h"
#include "../Violet/VltSampler.h"
#include "../Sce/SceGpuQueue.h"
#include "Algorithm/MurmurHash2.h"

using namespace vlt;
using namespace sce;

GnmSamplerCache::GnmSamplerCache(sce::SceGpuQueueDevice* device) :
	m_device(device)
{
}

GnmSamplerCache::~GnmSamplerCache()
{
}

GnmSammplerInstance* GnmSamplerCache::grabSampler(const GnmSamplerCreateInfo& desc)
{
	// Since samplers do not have memory backend,
	// we use hash instead.
	uint64_t       hash          = algo::MurmurHash(desc.sampler->m_regs, sizeof(desc.sampler->m_regs));
	GnmMemoryRange range         = {};
	range.start                  = reinterpret_cast<void*>(bit::extract(hash, 63, 32));
	range.size                   = static_cast<uint32_t>(bit::extract(hash, 31, 0));

	GnmSammplerInstance* sampler = nullptr;

	auto iter = m_samplerMap.find(range);
	if (iter == m_samplerMap.end())
	{
		auto instance         = createSampler(desc);
		auto [iter, inserted] = m_samplerMap.emplace(std::make_pair(range, instance));
		sampler               = &iter->second;
	}
	else
	{
		sampler = &iter->second;
	}

	return sampler;
}

GnmSammplerInstance GnmSamplerCache::createSampler(const GnmSamplerCreateInfo& desc)
{
	// TODO:
	// Set create info according to desc.
	VltSamplerCreateInfo info = {};
	info.magFilter            = VK_FILTER_LINEAR;
	info.minFilter            = VK_FILTER_LINEAR;
	info.mipmapMode           = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	info.useAnisotropy        = VK_TRUE;
	info.maxAnisotropy        = 16;
	info.addressModeU         = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	info.addressModeV         = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	info.addressModeW         = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	info.compareToDepth       = VK_FALSE;
	info.compareOp            = VK_COMPARE_OP_ALWAYS;
	info.borderColor          = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	info.usePixelCoord        = VK_FALSE;
	auto sampler              = m_device->device->createSampler(info);

	GnmSammplerInstance instance = {};
	instance.sampler             = sampler;
	return instance;
}
