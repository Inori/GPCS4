#pragma once

#include "GveCommon.h"
#include "GvePhysicalDevice.h"

namespace gve
{;

class GveDevice : public RcObject
{
public:
	GveDevice(VkDevice device, RcPtr<GvePhysicalDevice>& phyDevice);
	~GveDevice();

	operator VkDevice() const;

	RcPtr<GvePhysicalDevice> getPhysicalDevice();

private:
	VkDevice m_device;
	RcPtr<GvePhysicalDevice> m_phyDevice;
};

} // namespace gve
