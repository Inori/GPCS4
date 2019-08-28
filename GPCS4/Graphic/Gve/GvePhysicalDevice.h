#pragma once

#include "GveCommon.h"
#include "GveDevice.h"

#include <vector>
#include <optional>

namespace gve
{;

struct QueueFamilyIndices 
{
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplete() 
	{
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};


class GvePhysicalDevice : public RcObject
{
public:
	GvePhysicalDevice(VkPhysicalDevice device);
	~GvePhysicalDevice();

	operator VkPhysicalDevice() const;

	std::vector<VkQueueFamilyProperties> getQueueFamilies();

	RcPtr<GveDevice> createLogicalDevice(uint32_t graphicsFamily, uint32_t presentFamily);

private:

	VkPhysicalDevice m_device;
};

} // namespace gve
