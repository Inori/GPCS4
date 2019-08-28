#include "GvePhysicalDevice.h"
#include "GveVkLayers.h"
#include <set>

namespace gve
{;

const std::vector<const char*> deviceExtensions = 
{
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

GvePhysicalDevice::GvePhysicalDevice(VkPhysicalDevice device):
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

std::vector<VkQueueFamilyProperties> GvePhysicalDevice::getQueueFamilies()
{
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(m_device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(m_device, &queueFamilyCount, queueFamilies.data());

	return queueFamilies;
}

RcPtr<GveDevice> GvePhysicalDevice::createLogicalDevice(uint32_t graphicsFamily, uint32_t presentFamily)
{
	RcPtr<GveDevice> createdDevice;
	do 
	{
		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<uint32_t> uniqueQueueFamilies = { graphicsFamily, presentFamily };

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