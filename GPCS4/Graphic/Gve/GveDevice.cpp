#include "GveDevice.h"

namespace gve
{;


GveDevice::GveDevice(VkDevice device, RcPtr<GvePhysicalDevice>& phyDevice):
	m_device(device),
	m_phyDevice(phyDevice)
{

}

GveDevice::~GveDevice()
{
	vkDestroyDevice(m_device, nullptr);
}

GveDevice::operator VkDevice() const
{
	return m_device;
}

RcPtr<gve::GvePhysicalDevice> GveDevice::getPhysicalDevice()
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

RcPtr<gve::GveContex> GveDevice::createContext()
{
	return new GveContex(this);
}

} // namespace gve