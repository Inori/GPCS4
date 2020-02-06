#pragma once

#include "GveCommon.h"
#include "GveExtension.h"
#include "GveDeviceInfo.h"

#include <vector>
#include <optional>

namespace gve
{;

class GveInstance;
class GveDevice;

/**
 * \brief GPU vendors
 * Based on PCIe IDs.
 */
enum class GveGpuVendor : uint16_t 
{
	Amd = 0x1002,
	Nvidia = 0x10de,
	Intel = 0x8086,
};

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

	GveInstance* instance() const;

	GvePhysicalDeviceQueueFamilies findQueueFamilies();

	std::vector<VkExtensionProperties> getAvailableExtensions();

	const GveDeviceFeatures& features() const;

	const VkPhysicalDeviceProperties& deviceProperties() const;

	const GveDeviceInfo& devicePropertiesExt() const;

	VkPhysicalDeviceMemoryProperties memoryProperties() const;

	RcPtr<GveDevice> createLogicalDevice();

private:
	void queryExtensions();
	void queryDeviceInfo();
	void queryDeviceFeatures();
	void queryDeviceQueues();
	uint32_t findQueueFamily(VkQueueFlags mask, VkQueueFlags flags);
	GveDeviceFeatures getDeviceFeatures();

private:
	GveInstance* m_instance;
	VkPhysicalDevice m_device;

	GvePhysicalDeviceQueueFamilies m_queueFamilies;
	std::vector<VkQueueFamilyProperties> m_queueFamilyProps;

	GveNameSet         m_extraExtensions;
	GveNameSet         m_deviceExtensions;
	GveDeviceInfo      m_deviceInfo;
	GveDeviceFeatures  m_deviceFeatures;
};

} // namespace gve
