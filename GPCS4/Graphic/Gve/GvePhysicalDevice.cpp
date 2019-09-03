#include "GvePhysicalDevice.h"
#include "GveVkLayers.h"
#include "GveDevice.h"
#include <set>

namespace gve
{;


GvePhysicalDevice::GvePhysicalDevice(GveInstance* instance, VkPhysicalDevice device):
	m_instance(instance),
	m_device(device)
{
	queryDeviceQueues();
}

GvePhysicalDevice::~GvePhysicalDevice()
{
	
}

GvePhysicalDevice::operator VkPhysicalDevice() const
{
	return m_device;
}

GveInstance* GvePhysicalDevice::getInstance() const
{
	return m_instance;
}

gve::GvePhysicalDeviceQueueFamilies GvePhysicalDevice::findQueueFamilies()
{
	uint32_t graphicsQueue = findQueueFamily(
		VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT,
		VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT);

	GvePhysicalDeviceQueueFamilies queueFamilies;
	queueFamilies.graphicsFamily = graphicsQueue;
	return queueFamilies;
}

void GvePhysicalDevice::queryDeviceQueues()
{
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(m_device, &queueFamilyCount, nullptr);

	m_queueFamilyProps.resize(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(m_device, &queueFamilyCount, m_queueFamilyProps.data());
}

std::vector<VkExtensionProperties> GvePhysicalDevice::getAvailableExtensions()
{
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(m_device, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(m_device, nullptr, &extensionCount, availableExtensions.data());

	return availableExtensions;
}

VkPhysicalDeviceFeatures GvePhysicalDevice::getFeatures()
{
	VkPhysicalDeviceFeatures supportedFeatures;
	vkGetPhysicalDeviceFeatures(m_device, &supportedFeatures);
	return supportedFeatures;
}

RcPtr<GveDevice> GvePhysicalDevice::createLogicalDevice(const std::vector<const char*>& deviceExtensions)
{
	RcPtr<GveDevice> createdDevice;
	do 
	{
		float queuePriority = 1.0f;
		GvePhysicalDeviceQueueFamilies queueFamilies = findQueueFamilies();
		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamilies.graphicsFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;

		VkPhysicalDeviceFeatures deviceFeatures = {};
		deviceFeatures.samplerAnisotropy = VK_TRUE;

		VkDeviceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

		createInfo.queueCreateInfoCount = 1;
		createInfo.pQueueCreateInfos = &queueCreateInfo;

		createInfo.pEnabledFeatures = &deviceFeatures;

		createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
		createInfo.ppEnabledExtensionNames = deviceExtensions.data();

#ifdef GVE_VALIDATION_LAYERS_ENABLE

		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();

#else  // GVE_VALIDATION_LAYERS_ENABLE

		createInfo.enabledLayerCount = 0;

#endif  // GVE_VALIDATION_LAYERS_ENABLE

		VkDevice logicalDevice = VK_NULL_HANDLE;
		if (vkCreateDevice(m_device, &createInfo, nullptr, &logicalDevice) != VK_SUCCESS)
		{
			break;
		}

		createdDevice = new GveDevice(logicalDevice, this);
	} while (false);

	return createdDevice;
}

uint32_t GvePhysicalDevice::findQueueFamily(VkQueueFlags mask, VkQueueFlags flags)
{
	uint32_t index = VK_QUEUE_FAMILY_IGNORED;

	for (uint32_t i = 0; i < m_queueFamilyProps.size(); i++)
	{
		if ((m_queueFamilyProps[i].queueFlags & mask) == flags)
		{
			index = i;
			break;
		}
	}

	return index;
}

} // namespace gve