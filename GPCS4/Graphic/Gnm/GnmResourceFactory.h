#pragma once

#include "GnmCommon.h"

#include <unordered_map>
#include <functional>

namespace sce
{;
struct SceGpuQueueDevice;
}  // namespace sce

namespace vlt
{;
class VltBuffer;
class VltImage;
class VltImageView;
class VltSampler;
}  // namespace vlt

struct GnmIndexBuffer;
class GnmBuffer;
class GnmTexture;
class GnmSampler;
class GnmRenderTarget;
class GnmDepthRenderTarget;


/**
 * \brief Buffer create information
 *
 * We need input usage type to create proper
 * vulkan buffers.
 */
struct GnmBufferCreateInfo
{
	const GnmBuffer*     buffer;
	VkPipelineStageFlags stages;
	uint8_t              usageType;  // ShaderInputUsageType
};

/**
 * \brief Texture create information
 *
 * We need input usage type to create proper
 * vulkan textures.
 */
struct GnmTextureCreateInfo
{
	const GnmTexture*    texture;
	VkPipelineStageFlags stages;
	uint8_t              usageType;  // ShaderInputUsageType
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
	RcPtr<vlt::VltImage>     image = nullptr;
	RcPtr<vlt::VltImageView> view  = nullptr;
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
	GnmResourceFactory(const sce::SceGpuQueueDevice* device);
	~GnmResourceFactory();

	/// Get or create resources.

	RcPtr<vlt::VltBuffer> grabIndex(
		const GnmIndexBuffer& desc,
		bool*                 create = nullptr);

	RcPtr<vlt::VltBuffer> grabBuffer(
		const GnmBufferCreateInfo& desc,
		bool*                      create = nullptr);

	GnmCombinedImageView grabImage(
		const GnmTextureCreateInfo& desc,
		bool*                       create = nullptr);

	GnmCombinedImageView grabRenderTarget(
		const GnmRenderTarget& desc,
		bool*                  create = nullptr);

	GnmCombinedImageView grabDepthRenderTarget(
		const GnmDepthRenderTarget& desc,
		bool*                       create = nullptr);

	RcPtr<vlt::VltSampler> grabSampler(
		const GnmSampler& desc,
		bool*             create = nullptr);

private:
	
	template <typename MapType>
	typename MapType::mapped_type grabResource(
		const GnmResourceEntry&                        entry,
		MapType&                                       map,
		std::function<typename MapType::mapped_type()> createFunc,
		bool*                                          create);

	void collectRenderTargets();

	RcPtr<vlt::VltBuffer> createIndex(const GnmIndexBuffer& desc);

	RcPtr<vlt::VltBuffer> createBuffer(const GnmBufferCreateInfo& desc);

	GnmCombinedImageView createImage(const GnmTextureCreateInfo& desc);

	GnmCombinedImageView createRenderTarget(const GnmRenderTarget& desc);

	GnmCombinedImageView createDepthRenderTarget(const GnmDepthRenderTarget& desc);

	RcPtr<vlt::VltSampler> createSampler(const GnmSampler& desc);

private:
	const sce::SceGpuQueueDevice* m_device;

	std::unordered_map<GnmResourceEntry, RcPtr<vlt::VltBuffer>, GnmResourceHash>  m_bufferMap;
	std::unordered_map<GnmResourceEntry, GnmCombinedImageView, GnmResourceHash>   m_imageMap;
	std::unordered_map<GnmResourceEntry, RcPtr<vlt::VltSampler>, GnmResourceHash> m_samplerMap;
};



