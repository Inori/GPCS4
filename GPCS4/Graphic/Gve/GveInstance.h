#pragma once

#include "GveCommon.h"
#include "GvePhysicalDevice.h"

#include <vector>

namespace gve
{;

class GveInstance : public RcObject
{
public:
	GveInstance(const std::vector<const char*>& requiredExtensions);
	~GveInstance();

	VkInstance instance();

	uint32_t physicalDeviceCount() const;

	RcPtr<GvePhysicalDevice> getPhysicalDevice(uint32_t index);

private:

	void createInstance(const std::vector<const char*>& requiredExtensions);
	void setupDebugMessenger();

	bool checkValidationLayerSupport();
	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

	VkResult createDebugUtilsMessengerEXT(const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkDebugUtilsMessengerEXT* pDebugMessenger);

	void destroyDebugUtilsMessengerEXT(VkDebugUtilsMessengerEXT debugMessenger,
		const VkAllocationCallbacks* pAllocator);

	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, 
		VkDebugUtilsMessageTypeFlagsEXT messageType, 
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData);

private:

	void enumPhysicalDevices();

private:
	VkInstance m_instance = VK_NULL_HANDLE;

	std::vector<RcPtr<GvePhysicalDevice>> m_phyDevices;

#ifdef GVE_VALIDATION_LAYERS_ENABLE
	VkDebugUtilsMessengerEXT m_debugMessenger = VK_NULL_HANDLE;
#endif // GVE_VALIDATION_LAYERS_ENABLE
};


}  // gve



