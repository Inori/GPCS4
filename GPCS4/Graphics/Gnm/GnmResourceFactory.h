#pragma once

#include "GnmCommon.h"

namespace sce
{
	class SceBuffer;
	class SceTexture;
	class SceDepthRenderTarget;
	class SceSampler;

	namespace vlt
	{
		class VltDevice;
		struct VltBufferCreateInfo;
	}  // namespace vlt

	namespace Gnm
	{
		class Buffer;
		class Texture;
		class Sampler;
		class DepthRenderTarget;
		
		struct GnmBufferCreateInfo
		{
			const Buffer*                   vsharp;
			VkImageUsageFlags               usage;
			VkPipelineStageFlags2           stage;
			VkAccessFlagBits2               access;
			VkMemoryPropertyFlags           memoryType;
		};

		struct GnmImageCreateInfo
		{
			const Texture*        tsharp;
			VkImageUsageFlags     usage;
			VkPipelineStageFlags2 stage;
			VkAccessFlagBits2     access;
			VkImageTiling         tiling;
			VkImageLayout         layout;
			VkMemoryPropertyFlags memoryType;
		};

		class GnmResourceFactory
		{
		public:
			GnmResourceFactory(vlt::VltDevice* device);
			~GnmResourceFactory();

			bool createBuffer(
				const GnmBufferCreateInfo& createInfo,
				SceBuffer&                 sceBuffer);

			bool createImage(
				const GnmImageCreateInfo& createInfo,
				SceTexture&               sceTexture);

			bool createDepthImage(
				const DepthRenderTarget* depthTarget,
				SceDepthRenderTarget&    depthImage);

			bool createSampler(
				const Sampler* ssharp,
				SceSampler&    sampler);

		private:
			vlt::VltDevice* m_device;
		};


	}  // namespace Gnm
}  // namespace sce