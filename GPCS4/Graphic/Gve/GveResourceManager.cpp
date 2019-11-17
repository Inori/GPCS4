#include "GveResourceManager.h"
#include "GveDevice.h"
#include "GveBuffer.h"
#include "GveImage.h"
#include "GveSampler.h"

namespace gve
{;

GveResourceManager::GveResourceManager(GveDevice* device,
	GveMemoryAllocator* memAlloc):
	m_device(device),
	m_memAllocator(memAlloc)
{

}

GveResourceManager::~GveResourceManager()
{
	GC();
}

RcPtr<gve::GveBuffer> GveResourceManager::createBuffer(const GveBufferCreateInfo& info, VkMemoryPropertyFlags memoryType)
{
	return new GveBuffer(m_device, info, m_memAllocator, memoryType);
}

RcPtr<gve::GveBuffer> GveResourceManager::createBufferVsharp(const GveBufferCreateInfo& info, uint64_t key, VkMemoryPropertyFlags memoryType)
{
	RcPtr<GveBuffer> bufferPtr;
	auto iter = m_buffers.find(key);
	if (iter == m_buffers.end())
	{
		auto buffer = new GveBuffer(m_device, info, m_memAllocator, memoryType);
		auto pair = m_buffers.emplace(key, buffer);
		bufferPtr = pair.first->second;
	}
	else
	{
		bufferPtr = iter->second;
	}
	return bufferPtr;
}

void GveResourceManager::freeBufferVsharp(uint64_t key)
{
	m_buffers.erase(key);
}


RcPtr<gve::GveImage> GveResourceManager::createImage(const GveImageCreateInfo& info, VkMemoryPropertyFlags memoryType)
{
	return new GveImage(m_device, info, m_memAllocator, memoryType);
}

RcPtr<gve::GveImage> GveResourceManager::createImageTsharp(const GveImageCreateInfo& info, 
	uint64_t key, VkMemoryPropertyFlags memoryType)
{
	RcPtr<GveImage> imagePtr;
	auto iter = m_images.find(key);
	if (iter == m_images.end())
	{
		auto image = new GveImage(m_device, info, m_memAllocator, memoryType);
		auto pair = m_images.emplace(key, image);
		imagePtr = pair.first->second;
	}
	else
	{
		imagePtr = iter->second;
	}
	return imagePtr;
}

void GveResourceManager::freeImageTsharp(uint64_t key)
{
	m_images.erase(key);
}

RcPtr<GveImageView> GveResourceManager::createImageView(const RcPtr<GveImage>& image, const GveImageViewCreateInfo& createInfo)
{
	return new GveImageView(m_device, createInfo, image);
}

RcPtr<GveImageView> GveResourceManager::createImageViewTsharp(const RcPtr<GveImage>& image, uint64_t key, const GveImageViewCreateInfo& createInfo)
{
	RcPtr<GveImageView> imageViewPtr;
	auto iter = m_imageViews.find(key);
	if (iter == m_imageViews.end())
	{
		auto imageView = new GveImageView(m_device, createInfo, image);
		auto pair = m_imageViews.emplace(key, imageView);
		imageViewPtr = pair.first->second;
	}
	else
	{
		imageViewPtr = iter->second;
	}
	return imageViewPtr;
}

void GveResourceManager::freeImageViewTsharp(uint64_t key)
{
	m_imageViews.erase(key);
}

RcPtr<gve::GveSampler> GveResourceManager::createSampler(const GveSamplerCreateInfo& info)
{
	return new GveSampler(m_device, info);
}

RcPtr<gve::GveSampler> GveResourceManager::createSamplerSsharp(const GveSamplerCreateInfo& info, uint64_t key)
{
	RcPtr<GveSampler> samplerPtr;
	auto iter = m_samplers.find(key);
	if (iter == m_samplers.end())
	{
		auto image = new GveSampler(m_device, info);
		auto pair = m_samplers.emplace(key, image);
		samplerPtr = pair.first->second;
	}
	else
	{
		samplerPtr = iter->second;
	}
	return samplerPtr;
}

void GveResourceManager::freeSamplerSsharp(uint64_t key)
{
	m_samplers.erase(key);
}

void GveResourceManager::GC()
{
	m_buffers.clear();
	m_images.clear();
}

}  // namespace gve