#pragma once

#include "GnmCommon.h"

namespace sce
{
	class SceDepthRenderTarget;

	namespace vlt
	{
		class VltDevice;
	}  // namespace vlt

	namespace Gnm
	{
		class DepthRenderTarget;


		class GnmResourceFactory
		{
		public:
			GnmResourceFactory(vlt::VltDevice* device);
			~GnmResourceFactory();

			void createDepthImage(
				const DepthRenderTarget* depthTarget,
				SceDepthRenderTarget&    depthImage);

		private:
			vlt::VltDevice* m_device;
		};


	}  // namespace Gnm
}  // namespace sce