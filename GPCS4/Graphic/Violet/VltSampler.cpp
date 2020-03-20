#include "VltSampler.h"
#include "VltDevice.h"

LOG_CHANNEL(Graphic.Violet.VltSampler);

namespace vlt
{;

VltSampler::VltSampler(
	const RcPtr<VltDevice>&     device,
	const VltSamplerCreateInfo& info) :
	m_device(device),
	m_info(info)
{
	do 
	{
		VkSamplerCreateInfo samplerInfo;
		samplerInfo.sType                   = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.pNext                   = nullptr;
		samplerInfo.flags                   = 0;
		samplerInfo.magFilter               = info.magFilter;
		samplerInfo.minFilter               = info.minFilter;
		samplerInfo.mipmapMode              = info.mipmapMode;
		samplerInfo.addressModeU            = info.addressModeU;
		samplerInfo.addressModeV            = info.addressModeV;
		samplerInfo.addressModeW            = info.addressModeW;
		samplerInfo.mipLodBias              = info.mipmapLodBias;
		samplerInfo.anisotropyEnable        = info.useAnisotropy;
		samplerInfo.maxAnisotropy           = info.maxAnisotropy;
		samplerInfo.compareEnable           = info.compareToDepth;
		samplerInfo.compareOp               = info.compareOp;
		samplerInfo.minLod                  = info.mipmapLodMin;
		samplerInfo.maxLod                  = info.mipmapLodMax;
		samplerInfo.borderColor             = info.borderColor;
		samplerInfo.unnormalizedCoordinates = info.usePixelCoord;

		if (vkCreateSampler(*m_device, &samplerInfo, nullptr, &m_sampler) != VK_SUCCESS)
		{
			LOG_ERR("failed to create texture sampler!");
		}
	} while (false);
}

VltSampler::~VltSampler()
{
	vkDestroySampler(*m_device, m_sampler, nullptr);
}


VkSampler VltSampler::handle() const
{
	return m_sampler;
}


}  // namespace vlt