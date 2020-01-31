#include "GveInstance.h"
#include "GveVkLayers.h"
#include "GveSwapChain.h"

#include <iostream>

LOG_CHANNEL(Graphic.Gve.GveInstance);

namespace gve
{;


GveInstance::GveInstance(const std::vector<const char*>& requiredExtensions)
{
	createInstance(requiredExtensions);

#ifdef GVE_VALIDATION_LAYERS_ENABLE
	setupDebugMessenger();
#endif // GVE_VALIDATION_LAYERS_ENABLE

	enumPhysicalDevices();
}

GveInstance::~GveInstance()
{

#ifdef GVE_VALIDATION_LAYERS_ENABLE
	destroyDebugUtilsMessengerEXT(m_debugMessenger, nullptr);
#endif // GVE_VALIDATION_LAYERS_ENABLE

	vkDestroyInstance(m_instance, nullptr);
}

GveInstance::operator VkInstance() const
{
	return m_instance;
}

RcPtr<GvePhysicalDevice> GveInstance::pickPhysicalDevice(VkSurfaceKHR windowSurface)
{
	RcPtr<GvePhysicalDevice> phyDevice;
	do
	{
		if (!m_instance)
		{
			break;
		}

		for (const auto& device : m_phyDevices)
		{
			if (isDeviceSuitable(device, windowSurface))
			{
				phyDevice = device;
				break;
			}
		}

	} while (false);
	return phyDevice;
}

bool GveInstance::isDeviceSuitable(const RcPtr<GvePhysicalDevice>& device, VkSurfaceKHR windowSurface)
{
	bool swapChainAdequate = false;

	SwapChainSupportDetails swapChainSupport = GveSwapChain::querySwapChainSupport(*device, windowSurface);
	swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();

	const VkPhysicalDeviceFeatures& supportedFeatures = device->features().core.features;

	return swapChainAdequate && supportedFeatures.samplerAnisotropy;
}

void GveInstance::createInstance(const std::vector<const char*>& requiredExtensions)
{
	do 
	{

#ifdef GVE_VALIDATION_LAYERS_ENABLE
		if (!checkValidationLayerSupport())
		{
			break;
		}
#endif // GVE_VALIDATION_LAYERS_ENABLE
	
		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = GPCS4_APP_NAME;
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = GPCS4_APP_NAME;
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_1;

		VkInstanceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		std::vector<const char*> extensions(requiredExtensions);

#ifdef GVE_VALIDATION_LAYERS_ENABLE
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif // GVE_VALIDATION_LAYERS_ENABLE

		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();

#ifdef GVE_VALIDATION_LAYERS_ENABLE
		
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();

		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
		populateDebugMessengerCreateInfo(debugCreateInfo);
		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
		
#else  // GVE_VALIDATION_LAYERS_ENABLE

		createInfo.enabledLayerCount = 0;
		createInfo.pNext = nullptr;

#endif  // GVE_VALIDATION_LAYERS_ENABLE

		if (vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS) 
		{
			LOG_ERR("create vulkan instance failed.");
			break;
		}

	} while (false);
}

void GveInstance::setupDebugMessenger()
{
#ifdef GVE_VALIDATION_LAYERS_ENABLE
	do 
	{
		VkDebugUtilsMessengerCreateInfoEXT createInfo;
		populateDebugMessengerCreateInfo(createInfo);

		if (createDebugUtilsMessengerEXT(&createInfo, nullptr, &m_debugMessenger) != VK_SUCCESS)
		{
			LOG_WARN("create debug messenger failed.");
			break;
		}
	} while (false);

#else // GVE_VALIDATION_LAYERS_ENABLE
	return;
#endif // GVE_VALIDATION_LAYERS_ENABLE
}

bool GveInstance::checkValidationLayerSupport()
{
	bool support = true;
	do 
	{
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const char* layerName : validationLayers)
		{
			bool layerFound = false;

			for (const auto& layerProperties : availableLayers) 
			{
				if (strcmp(layerName, layerProperties.layerName) == 0) 
				{
					layerFound = true;
					break;
				}
			}

			if (!layerFound) 
			{
				support = false;
				break;
			}
		}
	} while (false);
	return support;
}

void GveInstance::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
{
	createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = debugCallback;
}

VkResult GveInstance::createDebugUtilsMessengerEXT(const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
{
	VkResult ret = VK_ERROR_EXTENSION_NOT_PRESENT;
	do 
	{
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_instance, "vkCreateDebugUtilsMessengerEXT");
		if (!func) 
		{
			break;
		}

		ret = func(m_instance, pCreateInfo, pAllocator, pDebugMessenger);

	} while (false);
	return ret;
}

void GveInstance::destroyDebugUtilsMessengerEXT(VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator)
{
	do 
	{
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_instance, "vkDestroyDebugUtilsMessengerEXT");
		if (!func)
		{
			break;
		}

		func(m_instance, debugMessenger, pAllocator);

	} while (false);
}

VKAPI_ATTR VkBool32 VKAPI_CALL GveInstance::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
	std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
	return VK_FALSE;
}

void GveInstance::enumPhysicalDevices()
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);

	LOG_ASSERT(deviceCount != 0, "0 physical device count.");

	std::vector<VkPhysicalDevice> vkDevices(deviceCount);
	vkEnumeratePhysicalDevices(m_instance, &deviceCount, vkDevices.data());

	m_phyDevices.reserve(deviceCount);
	for (auto& device : vkDevices)
	{
		m_phyDevices.push_back(new GvePhysicalDevice(this, device));
	}
}

RcPtr<gve::GveInstance> gveCreateInstance(const std::vector<const char*>& requiredExtensions)
{
	return new GveInstance(requiredExtensions);
}

}  // gve