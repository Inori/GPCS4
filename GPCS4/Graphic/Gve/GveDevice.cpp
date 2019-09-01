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

} // namespace gve