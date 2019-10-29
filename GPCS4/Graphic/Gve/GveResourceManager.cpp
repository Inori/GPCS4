#include "GveResourceManager.h"
#include "GveDevice.h"
#include "GveBuffer.h"
#include "GveImage.h"
#include "GveSampler.h"

namespace gve
{;

GveResourceManager::GveResourceManager(const RcPtr<GveDevice>& device):
	m_device(device),
	m_memAllocator(m_device.ptr())
{

}

GveResourceManager::~GveResourceManager()
{
}


RcPtr<GveBuffer> GveResourceManager::getBuffer(const GveBufferCreateInfo& info, VkMemoryPropertyFlags memoryType)
{
	RcPtr<GveBuffer> bufferPtr;
	void* address = info.buffer.getBaseAddress();
	auto iter = m_buffers.find(address);
	if (iter == m_buffers.end())
	{
		auto buffer = new GveBuffer(m_device, info, m_memAllocator, memoryType);
		auto pair = m_buffers.emplace(address, buffer);
		bufferPtr = pair.first->second;
	}
	else
	{
		bufferPtr = iter->second;
	}
	return bufferPtr;
}

void GveResourceManager::freeBuffer(const RcPtr<GveBuffer>& buffer)
{
	void* address = buffer->getGnmBuffer()->getBaseAddress();
	m_buffers.erase(address);
}

RcPtr<GveImage> GveResourceManager::getImage(const GveImageCreateInfo& info, VkMemoryPropertyFlags memoryType)
{
	RcPtr<GveImage> imagePtr;
	void* address = info.texture.getBaseAddress();
	auto iter = m_images.find(address);
	if (iter == m_images.end())
	{
		auto image = new GveImage(m_device, info, m_memAllocator, memoryType);
		auto pair = m_images.emplace(address, image);
		imagePtr = pair.first->second;
	}
	else
	{
		imagePtr = iter->second;
	}
	return imagePtr;
}

void GveResourceManager::freeImage(const RcPtr<GveImage>& image)
{
	void* address = image->getGnmTexture()->getBaseAddress();
	m_images.erase(address);
}

RcPtr<GveSampler> GveResourceManager::getSampler(const GveSamplerCreateInfo& info)
{
	RcPtr<GveSampler> samplerPtr;
	uint64_t key = info.sampler.m_regs[0] | info.sampler.m_regs[0] << 32;
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

void GveResourceManager::freeSampler(const RcPtr<GveSampler>& sampler)
{
	uint64_t key = sampler->getGnmSampler()->m_regs[0] | sampler->getGnmSampler()->m_regs[0] << 32;
	m_samplers.erase(key);
}

void GveResourceManager::GC()
{
	m_buffers.clear();
	m_images.clear();
}

}  // namespace gve