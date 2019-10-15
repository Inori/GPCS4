#include "GveDevice.h"
#include "GveCommandBuffer.h"

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

RcPtr<gve::GveContex> GveDevice::createContext()
{
	return new GveContex(this);
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