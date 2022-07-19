#pragma once

#include "GnmCommon.h"
#include "GnmSamplerCache.h"
#include "Violet/VltDebugUtil.h"


#include <atomic>

namespace sce
{
	class SceBuffer;
	class SceTexture;
	class SceRenderTarget;
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
		class RenderTarget;
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

			bool createRenderTarget(
				const RenderTarget* target,
				SceRenderTarget&    targetImage);

			bool createSampler(
				const Sampler*            ssharp,
				vlt::Rc<vlt::VltSampler>& sampler);

		private:
			vlt::Rc<vlt::VltSampler> createSampler(
				const Sampler* ssharp);

			VkImageLayout optimizeLayout(VkImageUsageFlags usage);

			VkComponentMapping calculateSwizzle(DataFormat format);

		private:
			vlt::VltDevice*   m_device;
			vlt::VltDebugUtil m_debugUtil;
			GnmSamplerCache   m_samplerCache;

			static std::atomic_size_t s_objectId;
		};


	}  // namespace Gnm
}  // namespace sce