#pragma once

#include "GveCommon.h"
#include "GveMemory.h"

#include <unordered_map>

namespace gve
{;

class GveDevice;
struct GveBufferCreateInfo;
class GveBuffer;
struct GveImageViewCreateInfo;
class GveImageView;
struct GveImageCreateInfo;
class GveImage;
struct GveSamplerCreateInfo;
class GveSampler;

// Use to manage vulkan buffers and images.
//
// Unlike Vulkan or DirectX,
// PS4 Gnm doesn't have explicit APIs to allocate/free GPU resources.
// We need to create/destroy gpu resource while parsing command buffers.

class GveResourceManager
{
public:
	GveResourceManager(GveDevice* device,
		GveMemoryAllocator* memAlloc);
	~GveResourceManager();


	/// Buffer

	/**
	 * \brief Create a normal buffer
	 *
	 * ie. A staging buffer,
	 * For this type of buffer, we know exactly when it will be released,
	 * so we don't need to hold a reference.
	 */
	RcPtr<GveBuffer> createBuffer(const GveBufferCreateInfo& info, VkMemoryPropertyFlags memoryType);

	/**
	 * \brief Create a buffer correspond to a V# buffer
	 *
	 * For this type of buffer, we don't know when it will be released,
	 * so we need to hold a reference count inside the GveResourceManager class
	 */
	RcPtr<GveBuffer> createBufferVsharp(const GveBufferCreateInfo& info,
		uint64_t key, VkMemoryPropertyFlags memoryType);

	/**
	 * \brief Free a buffer
	 *
	 * Free a buffer created with createBufferVsharp
	 */
	void freeBufferVsharp(uint64_t key);

	/// Image

	/**
	 * \brief Create a normal image
	 *
	 * For this type of image, we know exactly when it will be released,
	 * so we don't need to hold a reference.
	 */
	RcPtr<GveImage> createImage(const GveImageCreateInfo& info, VkMemoryPropertyFlags memoryType);

	/**
	 * \brief Create a image correspond to a T# buffer
	 *
	 * For this type of buffer, we don't know when it will be released,
	 * so we need to hold a reference count inside the GveResourceManager class
	 */
	RcPtr<GveImage> createImageTsharp(const GveImageCreateInfo& info, 
		uint64_t key, VkMemoryPropertyFlags memoryType);

	/**
	 * \brief Free a image
	 *
	 * Free a image created with createImageTsharp
	 */
	void freeImageTsharp(uint64_t key);

	/// Image View

	RcPtr<GveImageView> createImageView(
		const RcPtr<GveImage>&            image,
		const GveImageViewCreateInfo&     createInfo);

	RcPtr<GveImageView> createImageViewTsharp(
		const RcPtr<GveImage>&            image,
		uint64_t						  key,
		const GveImageViewCreateInfo&     createInfo);

	void freeImageViewTsharp(uint64_t key);

	/// Sampler

	/**
	 * \brief Create a normal sampler
	 *
	 * For this type of sampler, we know exactly when it will be released,
	 * so we don't need to hold a reference.
	 */
	RcPtr<GveSampler> createSampler(const GveSamplerCreateInfo& info);

	/**
	 * \brief Create a sampler correspond to a S# buffer
	 *
	 * For this type of sampler, we don't know when it will be released,
	 * so we need to hold a reference count inside the GveResourceManager class
	 */
	RcPtr<GveSampler> createSamplerSsharp(const GveSamplerCreateInfo& info, uint64_t key);

	/**
	 * \brief Free a sampler
	 *
	 * Free a sampler created with createSamplerSsharp
	 */
	void freeSamplerSsharp(uint64_t key);

	/**
	 * \brief Garbage collect
	 *
	 */
	void GC();

private:
	GveDevice* m_device;

	GveMemoryAllocator* m_memAllocator;

	std::unordered_map<uint64_t, RcPtr<GveBuffer>> m_buffers;
	std::unordered_map<uint64_t, RcPtr<GveImage>> m_images;
	std::unordered_map<uint64_t, RcPtr<GveImageView>> m_imageViews;
	std::unordered_map<uint64_t, RcPtr<GveSampler>> m_samplers;
};


}  // namespace gve
