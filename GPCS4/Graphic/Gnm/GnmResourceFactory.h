#pragma once

#include "GnmCommon.h"

#include <unordered_map>

namespace sce
{;
struct SceGpuQueueDevice;
}  // namespace sce

namespace gve
{;
class GveBuffer;
class GveImage;
class GveImageView;
class GveSampler;
}  // namespace gve

class GnmBuffer;
class GnmTexture;
class GnmSampler;
class GnmRenderTarget;
class GnmDepthRenderTarget;

/**
 * \brief Index Buffer Descriptor
 *
 * Since there's no dedicated Index Buffer Descriptor in Gnm,
 * we create one.
 */
struct GnmIndexBuffer
{
	const void* buffer = nullptr;
	uint32_t    count  = 0;
};

/**
 * \brief Combined image and image view.
 *
 * Gnm doesn't have explicit image view,
 * both image content and view information
 * are encoded in a single T#,
 * so it's convenient to have a combined presentation.
 * 
 */
struct GnmCombinedImageView
{
	RcPtr<gve::GveImage>     image = nullptr;
	RcPtr<gve::GveImageView> view  = nullptr;
};

/**
 * \brief Resource Entry
 *
 * This will be used for calculating hash value
 * to lookup resource
 */
struct GnmResourceEntry
{
	const void* memory;
	uint32_t    size;
};

struct GnmResourceHash
{
	std::size_t operator()(GnmResourceEntry const& entry) const noexcept
	{
		return reinterpret_cast<size_t>(entry.memory) << 32 | static_cast<size_t>(entry.size);
	}
};


class GnmResourceFactory
{
public:
	GnmResourceFactory(sce::SceGpuQueueDevice* device);
	~GnmResourceFactory();

	/// Get or create resources.

	RcPtr<gve::GveBuffer> grabIndex(const GnmIndexBuffer& desc);

	RcPtr<gve::GveBuffer> grabBuffer(const GnmBuffer& desc);

	GnmCombinedImageView grabImage(const GnmTexture& desc);

	GnmCombinedImageView grabRenderTarget(const GnmRenderTarget& desc);

	GnmCombinedImageView grabDepthRenderTarget(const GnmDepthRenderTarget& desc);

	RcPtr<gve::GveSampler> grabSampler(const GnmSampler& desc);

private:



private:
	sce::SceGpuQueueDevice* m_device;

	std::unordered_map<GnmResourceEntry, RcPtr<gve::GveBuffer>, GnmResourceHash>  m_bufferMap;
	std::unordered_map<GnmResourceEntry, GnmCombinedImageView, GnmResourceHash>   m_imageMap;
	std::unordered_map<GnmResourceEntry, RcPtr<gve::GveSampler>, GnmResourceHash> m_samplerMap;
};
