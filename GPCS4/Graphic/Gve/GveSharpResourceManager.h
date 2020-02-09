#pragma once

#include "GveCommon.h"
#include "GveMemory.h"

#include <unordered_map>

struct VSharpBuffer;
struct TSharpBuffer;
struct SSharpBuffer;

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

class GveSharpResourceManager
{
public:
	GveSharpResourceManager(GveDevice* device);

	~GveSharpResourceManager();

	/// Buffer

	RcPtr<GveBuffer> createOrGetIndexBuffer(
		const GveBufferCreateInfo& info,
		VkMemoryPropertyFlags memoryType,
		const void* address);

	void freeIndexBuffer(const void* address);

	/**
	 * \brief Create a buffer correspond to a V# buffer
	 *
	 * For this type of buffer, we don't know when it will be released,
	 * so we need to hold a reference count inside the GveResourceManager class
	 */
	RcPtr<GveBuffer> createOrGetBufferVsharp(
		const GveBufferCreateInfo&	info,
		VkMemoryPropertyFlags		memoryType,
		const VSharpBuffer&			vsharp);

	/**
	 * \brief Free a buffer
	 *
	 * Free a buffer created with createBufferVsharp
	 */
	void freeBufferVsharp(const VSharpBuffer& vsharp);

	/// Image

	/**
	 * \brief Create a image correspond to a T# buffer
	 *
	 * For this type of buffer, we don't know when it will be released,
	 * so we need to hold a reference count inside the GveResourceManager class
	 */
	RcPtr<GveImage> createOrGetImageTsharp(
		const GveImageCreateInfo&	info,
		VkMemoryPropertyFlags		memoryType,
		const TSharpBuffer&			tsharp);

	/**
	 * \brief Free a image
	 *
	 * Free a image created with createImageTsharp
	 */
	void freeImageTsharp(const TSharpBuffer& tsharp);

	/// Image View

	RcPtr<GveImageView> createOrGetImageViewTsharp(
		const RcPtr<GveImage>&            image,
		const GveImageViewCreateInfo&     createInfo,
		const TSharpBuffer&				  tsharp);

	void freeImageViewTsharp(const TSharpBuffer& tsharp);

	/// Sampler

	/**
	 * \brief Create a sampler correspond to a S# buffer
	 *
	 * For this type of sampler, we don't know when it will be released,
	 * so we need to hold a reference count inside the GveResourceManager class
	 */
	RcPtr<GveSampler> createOrGetSamplerSsharp(
		const GveSamplerCreateInfo&		info,
		const SSharpBuffer&				ssharp);

	/**
	 * \brief Free a sampler
	 *
	 * Free a sampler created with createSamplerSsharp
	 */
	void freeSamplerSsharp(const SSharpBuffer& ssharp);

	/**
	 * \brief Garbage collect
	 *
	 */
	void GC();

private:
	template <typename T>
	uint64_t calculateKey(const T& sharp);

	RcPtr<gve::GveBuffer> createOrGetBuffer(
		const GveBufferCreateInfo& info, 
		VkMemoryPropertyFlags memoryType, 
		uint64_t key);

private:
	GveDevice* m_device;

	std::unordered_map<uint64_t, RcPtr<GveBuffer>> m_buffers;
	std::unordered_map<uint64_t, RcPtr<GveImage>> m_images;
	std::unordered_map<uint64_t, RcPtr<GveImageView>> m_imageViews;
	std::unordered_map<uint64_t, RcPtr<GveSampler>> m_samplers;
};


}  // namespace gve
