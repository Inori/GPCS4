#include "GveDevice.h"
#include "GveCmdList.h"
#include "GveDescriptor.h"
#include "GveBuffer.h"
#include "GveImage.h"
#include "GveSampler.h"


namespace gve
{;


GveDevice::GveDevice(VkDevice device, const RcPtr<GvePhysicalDevice>& phyDevice):
	m_device(device),
	m_phyDevice(phyDevice),
	m_properties(phyDevice->devicePropertiesExt()),
	m_resObjects(this)
{
	initQueues();
}

GveDevice::~GveDevice()
{
	vkDestroyDevice(m_device, nullptr);
}

GveDevice::operator VkDevice() const
{
	return m_device;
}

RcPtr<GvePhysicalDevice> GveDevice::physicalDevice() const
{
	return m_phyDevice;
}

GveDeviceQueueSet GveDevice::queues() const
{
	return m_queues;
}

RcPtr<GveFrameBuffer> GveDevice::createFrameBuffer(const GveRenderTargets& renderTargets)
{
	auto rpFormat = GveFrameBuffer::getRenderPassFormat(renderTargets);
	auto renderPass = m_resObjects.renderPassPool().getRenderPass(rpFormat);

	const GveFramebufferSize defaultSize = 
	{
		m_properties.core.properties.limits.maxFramebufferWidth,
		m_properties.core.properties.limits.maxFramebufferHeight,
		m_properties.core.properties.limits.maxFramebufferLayers
	};

	return new GveFrameBuffer(this, renderTargets, renderPass, defaultSize);
}

RcPtr<GveCmdList> GveDevice::createCmdList()
{
	return new GveCmdList(this);
}

RcPtr<GveContex> GveDevice::createContext()
{
	return new GveContex(this);
}

RcPtr<GveDescriptorPool> GveDevice::createDescriptorPool()
{
	return new GveDescriptorPool(this);
}

RcPtr<GveBuffer> GveDevice::createBuffer(const GveBufferCreateInfo& info, VkMemoryPropertyFlags memoryType)
{
	return m_resObjects.resourceManager().createBuffer(info, memoryType);
}

RcPtr<GveBuffer> GveDevice::createBufferVsharp(const GveBufferCreateInfo& info, VkMemoryPropertyFlags memoryType, uint64_t key)
{
	return m_resObjects.resourceManager().createBufferVsharp(info, memoryType, key);
}

void GveDevice::freeBufferVsharp(uint64_t key)
{
	return m_resObjects.resourceManager().freeBufferVsharp(key);
}

RcPtr<GveImage> GveDevice::createImage(const GveImageCreateInfo& info, VkMemoryPropertyFlags memoryType)
{
	return m_resObjects.resourceManager().createImage(info, memoryType);
}

RcPtr<GveImage> GveDevice::createImageTsharp(const GveImageCreateInfo& info, VkMemoryPropertyFlags memoryType, uint64_t key)
{
	return m_resObjects.resourceManager().createImageTsharp(info, memoryType, key);
}

void GveDevice::freeImageTsharp(uint64_t key)
{
	return m_resObjects.resourceManager().freeImageTsharp(key);
}

RcPtr<GveImageView> GveDevice::createImageView(const RcPtr<GveImage>& image, const GveImageViewCreateInfo& createInfo)
{
	return m_resObjects.resourceManager().createImageView(image, createInfo);
}

RcPtr<GveImageView> GveDevice::createImageViewTsharp(const RcPtr<GveImage>& image, const GveImageViewCreateInfo& createInfo, uint64_t key)
{
	return m_resObjects.resourceManager().createImageViewTsharp(image, createInfo, key);
}

void GveDevice::freeImageViewTsharp(uint64_t key)
{
	return m_resObjects.resourceManager().freeImageViewTsharp(key);
}

RcPtr<GveSampler> GveDevice::createSampler(const GveSamplerCreateInfo& info)
{
	return m_resObjects.resourceManager().createSampler(info);
}

RcPtr<GveSampler> GveDevice::createSamplerSsharp(const GveSamplerCreateInfo& info, uint64_t key)
{
	return m_resObjects.resourceManager().createSamplerSsharp(info, key);
}

void GveDevice::freeSamplerSsharp(uint64_t key)
{
	return m_resObjects.resourceManager().freeSamplerSsharp(key);
}

void GveDevice::GCSharpResource()
{
	return m_resObjects.resourceManager().GC();
}

void GveDevice::initQueues()
{
	GvePhysicalDeviceQueueFamilies families = m_phyDevice->findQueueFamilies();
	GveDeviceQueue& graphicQueue = m_queues.graphics;
	graphicQueue.queueFamily = families.graphicsFamily;
	graphicQueue.queueIndex = 0;
	vkGetDeviceQueue(m_device, families.graphicsFamily, graphicQueue.queueIndex, &graphicQueue.queueHandle);
}

} // namespace gve