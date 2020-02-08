#include "GveDevice.h"
#include "GveCmdList.h"
#include "GveDescriptor.h"
#include "GveBuffer.h"
#include "GveImage.h"
#include "GveSampler.h"
#include "GveSwapChain.h"
#include "GveContext.h"
#include "GveFrameBuffer.h"
#include "GvePhysicalDevice.h"

#include "../Sce/SceVideoOut.h"

namespace gve
{;


GveDevice::GveDevice(VkDevice device, const RcPtr<GvePhysicalDevice>& phyDevice):
	m_device(device),
	m_phyDevice(phyDevice),
	m_properties(phyDevice->devicePropertiesExt()),
	m_memAllocator(this),
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

RcPtr<GveSwapChain> GveDevice::createSwapchain(std::shared_ptr<sce::SceVideoOut>& videoOut, uint32_t displayBufferCount)
{
	return new GveSwapChain(this, videoOut, displayBufferCount);
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

RcPtr<GveContext> GveDevice::createContext()
{
	return new GveContext(this);
}

RcPtr<GveDescriptorPool> GveDevice::createDescriptorPool()
{
	RcPtr<GveDescriptorPool> pool = m_recycledDescriptorPools.retrieveObject();

	if (pool == nullptr)
	{
		pool = new GveDescriptorPool(this);
	}

	return pool;
}

GveSharpResourceManager& GveDevice::getSharpResManager()
{
	return m_resObjects.getSharpResManager();
}

RcPtr<GveBuffer> GveDevice::createBuffer(const GveBufferCreateInfo& info, VkMemoryPropertyFlags memoryType)
{
	return new GveBuffer(this, info, &m_memAllocator, memoryType);
}

RcPtr<GveImage> GveDevice::createImage(const GveImageCreateInfo& info, VkMemoryPropertyFlags memoryType)
{
	return new GveImage(this, info, &m_memAllocator, memoryType);
}

RcPtr<GveImageView> GveDevice::createImageView(const RcPtr<GveImage>& image, const GveImageViewCreateInfo& createInfo)
{
	return new GveImageView(this, createInfo, image);
}

RcPtr<GveSampler> GveDevice::createSampler(const GveSamplerCreateInfo& info)
{
	return new GveSampler(this, info);
}

void GveDevice::recycleDescriptorPool(const RcPtr<GveDescriptorPool>& pool)
{
	m_recycledDescriptorPools.returnObject(pool);
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