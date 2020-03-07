#pragma once

#include "GnmCommon.h"

#include <unordered_map>
#include <functional>

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
	VkIndexType type   = VK_INDEX_TYPE_UINT16;
	uint32_t    count  = 0;
	uint32_t    size   = 0;
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
 * 
 * TODO:
 * Currently I only use buffer address and size as the key,
 * but this may not fit some situations,
 * another thing is that, we can't use the entire sharp buffer 
 * descriptor (T# V# S#) as the key, because the game could change some
 * fields while keep the buffer size and address unchanged.
 */
struct GnmResourceEntry
{
	const void* memory;
	uint32_t    size;

	bool operator==(const GnmResourceEntry& other) const
	{
		return memory == other.memory && size == other.size;
	}
};

struct GnmResourceHash
{
	std::size_t operator()(GnmResourceEntry const& entry) const noexcept
	{
		static_assert(sizeof(size_t) == sizeof(uint64_t), "size_t not 64 bit.");
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
	
	template <typename MapType>
	typename MapType::mapped_type grabResource(
		const GnmResourceEntry&                        entry,
		MapType&                                       map,
		std::function<typename MapType::mapped_type()> create);

	void collectRenderTargets();

	RcPtr<gve::GveBuffer> createIndex(const GnmIndexBuffer& desc);

	RcPtr<gve::GveBuffer> createBuffer(const GnmBuffer& desc);

	GnmCombinedImageView createImage(const GnmTexture& desc);

	GnmCombinedImageView createRenderTarget(const GnmRenderTarget& desc);

	GnmCombinedImageView createDepthRenderTarget(const GnmDepthRenderTarget& desc);

	RcPtr<gve::GveSampler> createSampler(const GnmSampler& desc);

private:
	sce::SceGpuQueueDevice* m_device;

	std::unordered_map<GnmResourceEntry, RcPtr<gve::GveBuffer>, GnmResourceHash>  m_bufferMap;
	std::unordered_map<GnmResourceEntry, GnmCombinedImageView, GnmResourceHash>   m_imageMap;
	std::unordered_map<GnmResourceEntry, RcPtr<gve::GveSampler>, GnmResourceHash> m_samplerMap;
};



