#include "GveSampler.h"
#include "GveDevice.h"

LOG_CHANNEL(Graphic.Gve.GveSampler);

namespace gve
{;

GveSampler::GveSampler(const RcPtr<GveDevice>& device, const GveSamplerCreateInfo& createInfo):
	m_device(device),
	m_info(createInfo)
{
	do 
	{
		// TODO:
		// Create sampler according to createInfo
		VkSamplerCreateInfo samplerInfo = {};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.magFilter = VK_FILTER_LINEAR;
		samplerInfo.minFilter = VK_FILTER_LINEAR;
		samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.anisotropyEnable = VK_TRUE;
		samplerInfo.maxAnisotropy = 16;
		samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		samplerInfo.unnormalizedCoordinates = VK_FALSE;
		samplerInfo.compareEnable = VK_FALSE;
		samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

		if (vkCreateSampler(*m_device, &samplerInfo, nullptr, &m_sampler) != VK_SUCCESS)
		{
			LOG_ERR("failed to create texture sampler!");
		}
	} while (false);
}

GveSampler::~GveSampler()
{
	vkDestroySampler(*m_device, m_sampler, nullptr);
}


VkSampler GveSampler::handle() const
{
	return m_sampler;
}


}  // namespace gve