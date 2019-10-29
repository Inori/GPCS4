#pragma once

#include "GveCommon.h"
#include "GveMemory.h"

#include <unordered_map>

namespace gve
{;

class GveDevice;
struct GveBufferCreateInfo;
class GveBuffer;
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
	GveResourceManager(const RcPtr<GveDevice>& device);
	~GveResourceManager();

	RcPtr<GveBuffer> getBuffer(const GveBufferCreateInfo& info, VkMemoryPropertyFlags memoryType);

	void freeBuffer(const RcPtr<GveBuffer>& buffer);

	RcPtr<GveImage> getImage(const GveImageCreateInfo& info, VkMemoryPropertyFlags memoryType);

	void freeImage(const RcPtr<GveImage>& image);

	RcPtr<GveSampler> getSampler(const GveSamplerCreateInfo& info);

	void freeSampler(const RcPtr<GveSampler>& sampler);

	void GC();

private:
	RcPtr<GveDevice> m_device;

	GveMemoryAllocator m_memAllocator;

	std::unordered_map<void*, RcPtr<GveBuffer>> m_buffers;
	std::unordered_map<void*, RcPtr<GveImage>> m_images;
	std::unordered_map<uint64_t, RcPtr<GveSampler>> m_samplers;
};


}  // namespace gve
