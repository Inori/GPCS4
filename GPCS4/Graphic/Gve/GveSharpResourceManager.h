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

	RcPtr<GveBuffer> createIndexBuffer(
		const GveBufferCreateInfo& info,
		VkMemoryPropertyFlags memoryType,
		const void* address);

	RcPtr<GveBuffer> getIndexBuffer(const void* address);

	void releaseIndexBuffer(const void* address);

	/**
	 * \brief Create a buffer correspond to a V# buffer
	 *
	 * For this type of buffer, we don't know when it will be released,
	 * so we need to hold a reference count inside the GveResourceManager class
	 */
	RcPtr<GveBuffer> createBufferVsharp(
		const GveBufferCreateInfo&	info,
		VkMemoryPropertyFlags		memoryType,
		const VSharpBuffer&			vsharp);

	RcPtr<GveBuffer> getBufferVsharp(const VSharpBuffer& vsharp);

	/**
	 * \brief Release a buffer
	 *
	 * Release a buffer created with createBufferVsharp
	 */
	void releaseBufferVsharp(const VSharpBuffer& vsharp);

	/// Image

	/**
	 * \brief Create a image correspond to a T# buffer
	 *
	 * For this type of buffer, we don't know when it will be released,
	 * so we need to hold a reference count inside the GveResourceManager class
	 */
	RcPtr<GveImage> createImageTsharp(
		const GveImageCreateInfo&	info,
		VkMemoryPropertyFlags		memoryType,
		const TSharpBuffer&			tsharp);

	RcPtr<GveImage> getImageTsharp(const TSharpBuffer& tsharp);

	/**
	 * \brief Release a image
	 *
	 * Release a image created with createImageTsharp
	 */
	void releaseImageTsharp(const TSharpBuffer& tsharp);

	/// Image View

	RcPtr<GveImageView> createImageViewTsharp(
		const RcPtr<GveImage>&            image,
		const GveImageViewCreateInfo&     createInfo,
		const TSharpBuffer&				  tsharp);

	RcPtr<GveImageView> getImageViewTsharp(const TSharpBuffer& tsharp);

	void releaseImageViewTsharp(const TSharpBuffer& tsharp);

	/// Sampler

	/**
	 * \brief Create a sampler correspond to a S# buffer
	 *
	 * For this type of sampler, we don't know when it will be released,
	 * so we need to hold a reference count inside the GveResourceManager class
	 */
	RcPtr<GveSampler> createSamplerSsharp(
		const GveSamplerCreateInfo&		info,
		const SSharpBuffer&				ssharp);

	RcPtr<GveSampler> getSamplerSsharp(const SSharpBuffer& ssharp);
	/**
	 * \brief Release a sampler
	 *
	 * Release a sampler created with createSamplerSsharp
	 */
	void releaseSamplerSsharp(const SSharpBuffer& ssharp);

	/**
	 * \brief Garbage collect
	 *
	 */
	void gc();

private:
	template <typename T>
	uint64_t calculateKey(const T& sharp);

	RcPtr<gve::GveBuffer> createBuffer(
		const GveBufferCreateInfo& info,
		VkMemoryPropertyFlags      memoryType,
		uint64_t                   key);

private:
	GveDevice* m_device;

	std::unordered_map<uint64_t, RcPtr<GveBuffer>> m_buffers;
	std::unordered_map<uint64_t, RcPtr<GveImage>> m_images;
	std::unordered_map<uint64_t, RcPtr<GveImageView>> m_imageViews;
	std::unordered_map<uint64_t, RcPtr<GveSampler>> m_samplers;
};


}  // namespace gve
