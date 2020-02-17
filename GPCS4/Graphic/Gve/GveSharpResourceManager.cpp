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
	gc();
}

RcPtr<gve::GveBuffer> GveSharpResourceManager::createIndexBuffer(
	const GveBufferCreateInfo& info,
	VkMemoryPropertyFlags      memoryType,
	const void*                address)
{
	uint64_t key = reinterpret_cast<uint64_t>(address);
	return createBuffer(info, memoryType, key);
}

RcPtr<gve::GveBuffer> GveSharpResourceManager::getIndexBuffer(const void* address)
{
	uint64_t key = reinterpret_cast<uint64_t>(address);
	auto     iter = m_buffers.find(key);
	return iter != m_buffers.end() ? iter->second : nullptr;
}

void GveSharpResourceManager::releaseIndexBuffer(const void* address)
{
	uint64_t key = reinterpret_cast<uint64_t>(address);
	m_buffers.erase(key);
}

template <typename T>
uint64_t gve::GveSharpResourceManager::calculateKey(const T& sharp)
{
	// TODO:
	// Just hashing sharp as the key is not correct theoretically,
	// because the buffer content themselves could be different
	// while still using the same sharp.
	return algo::MurmurHash(&sharp, sizeof(T));
}

RcPtr<gve::GveBuffer> GveSharpResourceManager::createBuffer(
	const GveBufferCreateInfo& info,
	VkMemoryPropertyFlags      memoryType,
	uint64_t                   key)
{
	auto buffer = m_device->createBuffer(info, memoryType);
	m_buffers.emplace(key, buffer);
	return buffer;
}

RcPtr<gve::GveBuffer> GveSharpResourceManager::createBufferVsharp(
	const GveBufferCreateInfo& info,
	VkMemoryPropertyFlags      memoryType,
	const VSharpBuffer&        vsharp)
{
	uint64_t key = calculateKey(vsharp);
	return createBuffer(info, memoryType, key);
}

RcPtr<gve::GveBuffer> GveSharpResourceManager::getBufferVsharp(const VSharpBuffer& vsharp)
{
	uint64_t key  = calculateKey(vsharp);
	auto     iter = m_buffers.find(key);
	return iter != m_buffers.end() ? iter->second : nullptr;
}

void GveSharpResourceManager::releaseBufferVsharp(const VSharpBuffer& vsharp)
{
	uint64_t key = calculateKey(vsharp);
	m_buffers.erase(key);
}

RcPtr<gve::GveImage> GveSharpResourceManager::createImageTsharp(const GveImageCreateInfo& info, VkMemoryPropertyFlags memoryType, const TSharpBuffer& tsharp)
{
	uint64_t key   = calculateKey(tsharp);
	auto     image = m_device->createImage(info, memoryType);
	m_images.emplace(key, image);
	return image;
}

RcPtr<gve::GveImage> GveSharpResourceManager::getImageTsharp(const TSharpBuffer& tsharp)
{
	uint64_t key  = calculateKey(tsharp);
	auto     iter = m_images.find(key);
	return iter != m_images.end() ? iter->second : nullptr;
}

void GveSharpResourceManager::releaseImageTsharp(const TSharpBuffer& tsharp)
{
	uint64_t key = calculateKey(tsharp);
	m_images.erase(key);
}

RcPtr<gve::GveImageView> GveSharpResourceManager::createImageViewTsharp(
	const RcPtr<GveImage>&        image,
	const GveImageViewCreateInfo& createInfo,
	const TSharpBuffer&           tsharp)
{
	uint64_t key       = calculateKey(tsharp);
	auto     imageView = m_device->createImageView(image, createInfo);
	m_imageViews.emplace(key, imageView);
	return imageView;
}

RcPtr<gve::GveImageView> GveSharpResourceManager::getImageViewTsharp(const TSharpBuffer& tsharp)
{
	uint64_t key  = calculateKey(tsharp);
	auto     iter = m_imageViews.find(key);
	return iter != m_imageViews.end() ? iter->second : nullptr;
}

void GveSharpResourceManager::releaseImageViewTsharp(const TSharpBuffer& tsharp)
{
	uint64_t key = calculateKey(tsharp);
	m_imageViews.erase(key);
}

RcPtr<gve::GveSampler> GveSharpResourceManager::createSamplerSsharp(
	const GveSamplerCreateInfo& info,
	const SSharpBuffer&         ssharp)
{
	uint64_t key     = calculateKey(ssharp);
	auto     sampler = m_device->createSampler(info);
	m_samplers.emplace(key, sampler);
	return sampler;
}

RcPtr<gve::GveSampler> GveSharpResourceManager::getSamplerSsharp(const SSharpBuffer& ssharp)
{
	uint64_t key  = calculateKey(ssharp);
	auto     iter = m_samplers.find(key);
	return iter != m_samplers.end() ? iter->second : nullptr;
}

void GveSharpResourceManager::releaseSamplerSsharp(const SSharpBuffer& ssharp)
{
	uint64_t key = calculateKey(ssharp);
	m_samplers.erase(key);
}

void GveSharpResourceManager::gc()
{
	m_buffers.clear();
	m_imageViews.clear();
	m_images.clear();
	m_samplers.clear();
}

}  // namespace gve