#pragma once

#include "GveCommon.h"
#include <vector>

namespace gve
{;


class CGveInstance : public RcObject
{
public:
	CGveInstance(const std::vector<const char*>& requiredExtensions);
	~CGveInstance();

	operator VkInstance() const;

private:

	void createInstance(const std::vector<const char*>& requiredExtensions);
	void setupDebugMessenger();

	bool checkValidationLayerSupport();
	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

	VkResult createDebugUtilsMessengerEXT(const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkDebugUtilsMessengerEXT* pDebugMessenger);

	void DestroyDebugUtilsMessengerEXT(VkDebugUtilsMessengerEXT debugMessenger,
		const VkAllocationCallbacks* pAllocator);

	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, 
		VkDebugUtilsMessageTypeFlagsEXT messageType, 
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData);

private:
	VkInstance m_instance = VK_NULL_HANDLE;

#ifdef GVE_VALIDATION_LAYERS_ENABLE
	VkDebugUtilsMessengerEXT m_debugMessenger = VK_NULL_HANDLE;
#endif // GVE_VALIDATION_LAYERS_ENABLE
};


}  // gve



