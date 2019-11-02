#include "GveDevice.h"
#include "GveCommandBuffer.h"
#include "GveDescriptor.h"
#include "GveBuffer.h"
#include "GveImage.h"


namespace gve
{;


GveDevice::GveDevice(VkDevice device, const RcPtr<GvePhysicalDevice>& phyDevice):
	m_device(device),
	m_phyDevice(phyDevice)
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

RcPtr<gve::GvePhysicalDevice> GveDevice::physicalDevice() const
{
	return m_phyDevice;
}

GveDeviceQueueSet GveDevice::queues() const
{
	return m_queues;
}

RcPtr<gve::GveRenderPass> GveDevice::createRenderPass(GveRenderPassFormat& format)
{
	return new GveRenderPass(this, format);
}

RcPtr<gve::GveFrameBuffer> GveDevice::createFrameBuffer(VkRenderPass renderPass, VkImageView imageView, VkExtent2D& extent)
{
	return new GveFrameBuffer(this, renderPass, imageView, extent);
}

RcPtr<gve::GveCommandBuffer> GveDevice::createCommandBuffer()
{
	return new GveCommandBuffer(this);
}

RcPtr<gve::GveContex> GveDevice::createContext(const GveContextParam& param)
{
	return new GveContex(this, param);
}

RcPtr<gve::GveDescriptorPool> GveDevice::createDescriptorPool()
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