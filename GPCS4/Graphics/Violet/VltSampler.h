#pragma once

#include "VltCommon.h"
#include "VltResource.h"

namespace sce::vlt
{
	class VltDevice;

	/**
     * \brief Sampler properties
     */
	struct VltSamplerCreateInfo
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

	/**
     * \brief Sampler
     * 
     * Manages a sampler object that can be bound to
     * a pipeline. Sampler objects provide parameters
     * for texture lookups within a shader.
     */
	class VltSampler : public VltResource
	{

	public:
		VltSampler(
			VltDevice*                  device,
			const VltSamplerCreateInfo& info);
		~VltSampler();

		/**
         * \brief Sampler handle
         * \returns Sampler handle
         */
		VkSampler handle() const
		{
			return m_sampler;
		}

	private:
		VkBorderColor getBorderColor(
			bool depthCompare, VkClearColorValue borderColor) const;

	private:
		VltDevice* m_device;
		VkSampler  m_sampler = VK_NULL_HANDLE;
	};
}  // namespace sce::vlt