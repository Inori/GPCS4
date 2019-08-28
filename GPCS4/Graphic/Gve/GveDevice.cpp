#include "GveDevice.h"

namespace gve
{;


GveDevice::GveDevice(VkDevice device):
	m_device(device)
{

}

GveDevice::~GveDevice()
{

}

GveDevice::operator VkDevice() const
{
	return m_device;
}

} // namespace gve