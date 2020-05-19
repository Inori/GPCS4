#pragma once

#include "VltCommon.h"
#include "VltExtension.h"
#include "VltDeviceInfo.h"

#include <vector>
#include <optional>

namespace vlt
{;

class VltInstance;
class VltDevice;

/**
 * \brief GPU vendors
 * Based on PCIe IDs.
 */
enum class VltGpuVendor : uint16_t
{
	Amd    = 0x1002,
	Nvidia = 0x10de,
	Intel  = 0x8086,
};

struct VltPhysicalDeviceQueueFamilies
{
	uint32_t graphics;
	uint32_t compute;
	uint32_t transfer;
};


class VltPhysicalDevice : public RcObject
{
public:
	VltPhysicalDevice(VltInstance* instance, VkPhysicalDevice device);
	~VltPhysicalDevice();

	operator VkPhysicalDevice() const;
	 
	VltInstance* instance() const;

	VltPhysicalDeviceQueueFamilies findQueueFamilies();

	std::vector<VkExtensionProperties> availableExtensions();

	const VltDeviceFeatures& features() const;

	const VkPhysicalDeviceProperties& deviceProperties() const;

	const VltDeviceInfo& devicePropertiesExt() const;

	VkPhysicalDeviceMemoryProperties memoryProperties() const;

	bool checkFeatureSupport(const VltDeviceFeatures& required) const;

	RcPtr<VltDevice> createLogicalDevice(const VltDeviceFeatures& features);

private:
	void queryExtensions();
	void queryDeviceInfo();
	void queryDeviceFeatures();
	void queryDeviceQueues();
	uint32_t findQueueFamily(VkQueueFlags mask, VkQueueFlags flags);
	
	void populateQueueCreateInfos(std::vector<VkDeviceQueueCreateInfo>& queueInfos);

private:
	VltInstance* m_instance;
	VkPhysicalDevice m_device;

	VltPhysicalDeviceQueueFamilies m_queueFamilies;
	std::vector<VkQueueFamilyProperties> m_queueFamilyProps;

	VltNameSet         m_extraExtensions;
	VltNameSet         m_deviceExtensions;
	VltDeviceInfo      m_deviceInfo;
	VltDeviceFeatures  m_deviceFeatures;
};

} // namespace vlt
