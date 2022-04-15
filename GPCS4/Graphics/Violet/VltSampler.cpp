#include "VltSampler.h"

#include "VltDevice.h"

namespace sce::vlt
{
	VltSampler::VltSampler(
		VltDevice*                  device,
		const VltSamplerCreateInfo& info) :
		m_device(device)
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
		samplerInfo.borderColor             = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
		samplerInfo.unnormalizedCoordinates = info.usePixelCoord;

		if (samplerInfo.addressModeU == VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER || samplerInfo.addressModeV == VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER || samplerInfo.addressModeW == VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER)
			samplerInfo.borderColor = getBorderColor(info.compareToDepth, info.borderColor);

		if (vkCreateSampler(m_device->handle(),
							&samplerInfo, nullptr, &m_sampler) != VK_SUCCESS)
			Logger::exception("DxvkSampler::DxvkSampler: Failed to create sampler");
	}

	VltSampler::~VltSampler()
	{
		vkDestroySampler(
			m_device->handle(), m_sampler, nullptr);
	}

	VkBorderColor VltSampler::getBorderColor(
		bool depthCompare, VkClearColorValue borderColor) const
	{
		static const std::array<std::pair<VkClearColorValue, VkBorderColor>, 3> s_borderColors = { {
			{ { { 0.0f, 0.0f, 0.0f, 0.0f } }, VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK },
			{ { { 0.0f, 0.0f, 0.0f, 1.0f } }, VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK },
			{ { { 1.0f, 1.0f, 1.0f, 1.0f } }, VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE },
		} };

		if (depthCompare)
		{
			// If we are a depth compare sampler:
			// Replicate the first index, only R matters.
			for (uint32_t i = 1; i < 4; i++)
				borderColor.float32[i] = borderColor.float32[0];
		}

		for (const auto& e : s_borderColors)
		{
			if (!std::memcmp(&e.first, &borderColor, sizeof(VkClearColorValue)))
				return e.second;
		}

		Logger::warn(util::str::formatex(
			"DXVK: No matching border color found for (",
			borderColor.float32[0], ",", borderColor.float32[1], ",",
			borderColor.float32[2], ",", borderColor.float32[3], ")"));
		return VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
	}
}  // namespace sce::vlt