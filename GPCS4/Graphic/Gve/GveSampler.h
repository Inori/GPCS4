#pragma once

#include "GveCommon.h"
#include "GveMemory.h"


namespace gve
{;

class GveDevice;

struct GveSamplerCreateInfo
{
	/// Texture filter propertoes
	VkFilter magFilter;
	VkFilter minFilter;

	/// Mipmapping properties
	VkSamplerMipmapMode mipmapMode;
	float               mipmapLodBias;
	float               mipmapLodMin;
	float               mipmapLodMax;

	/// Anisotropic filtering
	VkBool32 useAnisotropy;
	float    maxAnisotropy;

	/// Address modes
	VkSamplerAddressMode addressModeU;
	VkSamplerAddressMode addressModeV;
	VkSamplerAddressMode addressModeW;

	/// Compare op for shadow textures
	VkBool32    compareToDepth;
	VkCompareOp compareOp;

	/// Texture border color
	VkClearColorValue borderColor;

	/// Enables unnormalized coordinates
	VkBool32 usePixelCoord;
};

class GveSampler : public RcObject
{
public:
	GveSampler(const RcPtr<GveDevice>& device, const GveSamplerCreateInfo& createInfo);
	~GveSampler();

	VkSampler handle() const;

private:
	RcPtr<GveDevice> m_device;
	GveSamplerCreateInfo m_info;

	VkSampler m_sampler = VK_NULL_HANDLE;
};

}  // namespace gve