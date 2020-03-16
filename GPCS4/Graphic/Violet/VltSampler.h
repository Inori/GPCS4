#pragma once

#include "VltCommon.h"
#include "VltMemory.h"


namespace vlt
{;

class VltDevice;

struct VltSamplerCreateInfo
{
	// Texture filter propertoes
	VkFilter magFilter;
	VkFilter minFilter;

	// Mipmapping properties
	VkSamplerMipmapMode mipmapMode;
	float               mipmapLodBias;
	float               mipmapLodMin;
	float               mipmapLodMax;

	// Anisotropic filtering
	VkBool32 useAnisotropy;
	float    maxAnisotropy;

	// Address modes
	VkSamplerAddressMode addressModeU;
	VkSamplerAddressMode addressModeV;
	VkSamplerAddressMode addressModeW;

	// Compare op for shadow textures
	VkBool32    compareToDepth;
	VkCompareOp compareOp;

	// Texture border color
	VkBorderColor borderColor;

	// Enables unnormalized coordinates
	VkBool32 usePixelCoord;
};

class VltSampler : public RcObject
{
public:
	VltSampler(
		const RcPtr<VltDevice>&     device,
		const VltSamplerCreateInfo& info);
	~VltSampler();

	VkSampler handle() const;

private:
	RcPtr<VltDevice>     m_device;
	VltSamplerCreateInfo m_info;

	VkSampler m_sampler = VK_NULL_HANDLE;
};

}  // namespace vlt