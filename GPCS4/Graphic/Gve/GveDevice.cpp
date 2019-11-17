#include "GveDevice.h"
#include "GveCommandBuffer.h"
#include "GveDescriptor.h"
#include "GveBuffer.h"
#include "GveImage.h"


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

RcPtr<GveCommandBuffer> GveDevice::createCommandBuffer()
{
	return new GveCommandBuffer(this);
}

RcPtr<GveContex> GveDevice::createContext(const GveContextParam& param)
{
	return new GveContex(this, param);
}

RcPtr<GveDescriptorPool> GveDevice::createDescriptorPool()
{
	return new GveDescriptorPool(this);
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