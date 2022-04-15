#pragma once

#include "GnmCommon.h"

namespace sce
{
	class SceBuffer;
	class SceDepthRenderTarget;

	namespace vlt
	{
		class VltDevice;
		struct VltBufferCreateInfo;
	}  // namespace vlt

	namespace Gnm
	{
		class Buffer;
		class DepthRenderTarget;


		class GnmResourceFactory
		{
		public:
			GnmResourceFactory(vlt::VltDevice* device);
			~GnmResourceFactory();

			bool createBuffer(
				const vlt::VltBufferCreateInfo& createinfo,
				VkMemoryPropertyFlags           memoryType,
				const Buffer*                   buffer,
				SceBuffer&                      sceBuffer);

			bool createDepthImage(
				const DepthRenderTarget* depthTarget,
				SceDepthRenderTarget&    depthImage);

		private:
			vlt::VltDevice* m_device;
		};


	}  // namespace Gnm
}  // namespace sce