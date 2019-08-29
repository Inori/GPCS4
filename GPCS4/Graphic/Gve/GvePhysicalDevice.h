#pragma once

#include "GveCommon.h"
#include "GveDevice.h"

#include <vector>
#include <optional>

namespace gve
{;

class GveInstance;

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
	GvePhysicalDevice(GveInstance* instance, VkPhysicalDevice device);
	~GvePhysicalDevice();

	operator VkPhysicalDevice() const;

	GveInstance* getInstance() const;

	std::vector<VkQueueFamilyProperties> getQueueFamilies();

	QueueFamilyIndices getSuitableQueueIndices(VkSurfaceKHR presentSurface);

	RcPtr<GveDevice> createLogicalDevice(QueueFamilyIndices& indices);

private:
	GveInstance* m_instance;
	VkPhysicalDevice m_device;
};

} // namespace gve
