#pragma once

#include "GveCommon.h"

#include <vector>
#include <optional>

namespace gve
{;

class GveInstance;
class GveDevice;

struct GvePhysicalDeviceQueueFamilies
{
	uint32_t graphicsFamily;
};


class GvePhysicalDevice : public RcObject
{
public:
	GvePhysicalDevice(GveInstance* instance, VkPhysicalDevice device);
	~GvePhysicalDevice();

	operator VkPhysicalDevice() const;

	GveInstance* getInstance() const;

	GvePhysicalDeviceQueueFamilies findQueueFamilies();

	std::vector<VkExtensionProperties> getAvailableExtensions();

	VkPhysicalDeviceFeatures getFeatures();

	RcPtr<GveDevice> createLogicalDevice(const std::vector<const char*>& deviceExtensions);

private:
	void queryDeviceQueues();
	uint32_t findQueueFamily(VkQueueFlags mask, VkQueueFlags flags);

private:
	GveInstance* m_instance;
	VkPhysicalDevice m_device;
	GvePhysicalDeviceQueueFamilies m_queueFamilies;
	std::vector<VkQueueFamilyProperties> m_queueFamilyProps;
};

} // namespace gve
