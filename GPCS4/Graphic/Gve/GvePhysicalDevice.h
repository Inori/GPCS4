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

	/**
	 * \brief Supportred device features
	 *
	 * Queries the supported device features.
	 * \returns Device features
	 */
	const GveDeviceFeatures& features() const;

	/**
	 * \brief Physical device properties
	 *
	 * Returns a read-only reference to the core
	 * properties of the Vulkan physical device.
	 * \returns Physical device core properties
	 */
	const VkPhysicalDeviceProperties& deviceProperties() const;;

	/**
	 * \brief Memory properties
	 *
	 * Queries the memory types and memory heaps of
	 * the device. This is useful for memory allocators.
	 * \returns Device memory properties
	 */
	VkPhysicalDeviceMemoryProperties memoryProperties() const;

	RcPtr<GveDevice> createLogicalDevice(const std::vector<const char*>& deviceExtensions);

private:
	void queryExtensions();
	void queryDeviceInfo();
	void queryDeviceFeatures();
	void queryDeviceQueues();
	uint32_t findQueueFamily(VkQueueFlags mask, VkQueueFlags flags);

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
