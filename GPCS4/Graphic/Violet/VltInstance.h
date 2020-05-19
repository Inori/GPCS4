#pragma once

#include "VltCommon.h"
#include "VltPhysicalDevice.h"

#include <vector>

namespace vlt
{;

class VltInstance : public RcObject
{
public:
	VltInstance(const std::vector<const char*>& requiredExtensions);

	~VltInstance();

	operator VkInstance() const;

	std::vector<RcPtr<VltPhysicalDevice>> enumPhysicalDevices();

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
	VkInstance m_instance = VK_NULL_HANDLE;

#ifdef VLT_VALIDATION_LAYERS_ENABLE
	VkDebugUtilsMessengerEXT m_debugMessenger = VK_NULL_HANDLE;
#endif // VLT_VALIDATION_LAYERS_ENABLE
};


RcPtr<VltInstance> violetCreateInstance(const std::vector<const char*>& requiredExtensions);


}  // vlt



