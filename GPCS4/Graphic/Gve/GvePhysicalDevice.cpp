#include "GvePhysicalDevice.h"
#include "GveVkLayers.h"
#include <set>

namespace gve
{;


GvePhysicalDevice::GvePhysicalDevice(GveInstance* instance, VkPhysicalDevice device):
	m_instance(instance),
	m_device(device)
{

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

std::vector<VkQueueFamilyProperties> GvePhysicalDevice::getQueueFamilies()
{
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(m_device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(m_device, &queueFamilyCount, queueFamilies.data());

	return queueFamilies;
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

QueueFamilyIndices GvePhysicalDevice::getSuitableQueueIndices(VkSurfaceKHR presentSurface)
{
	QueueFamilyIndices indices;
	auto queueFamilies = getQueueFamilies();
	int i = 0;
	for (const auto& queueFamily : queueFamilies) 
	{
		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) 
		{
			indices.graphicsFamily = i;
		}

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(m_device, i, presentSurface, &presentSupport);

		if (queueFamily.queueCount > 0 && presentSupport)
		{
			indices.presentFamily = i;
		}

		if (indices.isComplete()) 
		{
			break;
		}

		i++;
	}
	return indices;
}

RcPtr<GveDevice> GvePhysicalDevice::createLogicalDevice(QueueFamilyIndices& indices,
	const std::vector<const char*>& deviceExtensions)
{
	RcPtr<GveDevice> createdDevice;
	do 
	{
		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

		float queuePriority = 1.0f;
		for (uint32_t queueFamily : uniqueQueueFamilies)
		{
			VkDeviceQueueCreateInfo queueCreateInfo = {};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueCreateInfos.push_back(queueCreateInfo);
		}

		VkPhysicalDeviceFeatures deviceFeatures = {};
		deviceFeatures.samplerAnisotropy = VK_TRUE;

		VkDeviceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		createInfo.pQueueCreateInfos = queueCreateInfos.data();

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

		createdDevice = new GveDevice(logicalDevice);
	} while (false);

	return createdDevice;
}

} // namespace gve