#include "GveSharpResourceManager.h"
#include "GveDevice.h"
#include "GveBuffer.h"
#include "GveImage.h"
#include "GveSampler.h"

#include "Algorithm/MurmurHash2.h"

#include "../Gnm/GnmSharpBuffer.h"

namespace gve
{;

GveSharpResourceManager::GveSharpResourceManager(GveDevice* device):
	m_device(device)
{

}

GveSharpResourceManager::~GveSharpResourceManager()
{
	GC();
}

RcPtr<gve::GveBuffer> GveSharpResourceManager::createOrGetIndexBuffer(const GveBufferCreateInfo& info, VkMemoryPropertyFlags memoryType, const void* address)
{
	uint64_t key = reinterpret_cast<uint64_t>(address);
	return createOrGetBuffer(info, memoryType, key);
}

void GveSharpResourceManager::freeIndexBuffer(const void* address)
{
	uint64_t key = reinterpret_cast<uint64_t>(address);
	m_buffers.erase(key);
}

template <typename T>
uint64_t gve::GveSharpResourceManager::calculateKey(const T& sharp)
{
	// TODO:
	// Just hashing vsharp as the key is not correct theoretically,
	// because the buffer content themselves could be different
	// while still using the same vsharp.
	return algo::MurmurHash(&sharp, sizeof(T));
}

RcPtr<gve::GveBuffer> GveSharpResourceManager::createOrGetBuffer(const GveBufferCreateInfo& info, VkMemoryPropertyFlags memoryType, uint64_t key)
{
	RcPtr<GveBuffer> bufferPtr;
	
	auto iter    = m_buffers.find(key);
	if (iter == m_buffers.end())
	{
		auto buffer = m_device->createBuffer(info, memoryType);
		auto pair   = m_buffers.emplace(key, buffer);
		bufferPtr   = pair.first->second;
	}
	else
	{
		bufferPtr = iter->second;
	}
	return bufferPtr;
}

RcPtr<gve::GveBuffer> GveSharpResourceManager::createOrGetBufferVsharp(const GveBufferCreateInfo& info, VkMemoryPropertyFlags memoryType, const VSharpBuffer& vsharp)
{
	uint64_t key = calculateKey(vsharp);
	return createOrGetBuffer(info, memoryType, key);
}



void GveSharpResourceManager::freeBufferVsharp(const VSharpBuffer& vsharp)
{
	uint64_t key = calculateKey(vsharp);
	m_buffers.erase(key);
}

RcPtr<gve::GveImage> GveSharpResourceManager::createOrGetImageTsharp(const GveImageCreateInfo& info, VkMemoryPropertyFlags memoryType, const TSharpBuffer& tsharp)
{
	RcPtr<GveImage> imagePtr;
	uint64_t key = calculateKey(tsharp);
	auto iter = m_images.find(key);
	if (iter == m_images.end())
	{
		auto image = m_device->createImage(info, memoryType);
		auto pair  = m_images.emplace(key, image);
		imagePtr   = pair.first->second;
	}
	else
	{
		imagePtr = iter->second;
	}
	return imagePtr;
}

void GveSharpResourceManager::freeImageTsharp(const TSharpBuffer& tsharp)
{
	uint64_t key = calculateKey(tsharp);
	m_images.erase(key);
}

RcPtr<gve::GveImageView> GveSharpResourceManager::createOrGetImageViewTsharp(const RcPtr<GveImage>& image, const GveImageViewCreateInfo& createInfo, const TSharpBuffer& tsharp)
{
	RcPtr<GveImageView> imageViewPtr;
	uint64_t key = calculateKey(tsharp);
	auto iter = m_imageViews.find(key);
	if (iter == m_imageViews.end())
	{
		auto imageView = m_device->createImageView(image, createInfo);
		auto pair      = m_imageViews.emplace(key, imageView);
		imageViewPtr   = pair.first->second;
	}
	else
	{
		imageViewPtr = iter->second;
	}
	return imageViewPtr;
}

void GveSharpResourceManager::freeImageViewTsharp(const TSharpBuffer& tsharp)
{
	uint64_t key = calculateKey(tsharp);
	m_imageViews.erase(key);
}

RcPtr<gve::GveSampler> GveSharpResourceManager::createOrGetSamplerSsharp(const GveSamplerCreateInfo& info, const SSharpBuffer& ssharp)
{
	RcPtr<GveSampler> samplerPtr;
	uint64_t key = calculateKey(ssharp);
	auto iter = m_samplers.find(key);
	if (iter == m_samplers.end())
	{
		auto sampler = m_device->createSampler(info);
		auto pair    = m_samplers.emplace(key, sampler);
		samplerPtr   = pair.first->second;
	}
	else
	{
		samplerPtr = iter->second;
	}
	return samplerPtr;
}

void GveSharpResourceManager::freeSamplerSsharp(const SSharpBuffer& ssharp)
{
	uint64_t key = calculateKey(ssharp);
	m_samplers.erase(key);
}

void GveSharpResourceManager::GC()
{
	m_buffers.clear();
	m_imageViews.clear();
	m_images.clear();
	m_samplers.clear();
}

}  // namespace gve