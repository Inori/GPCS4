#include "VltInstance.h"

#include "VltAdapter.h"
#include "VltDeviceFilter.h"

#include "Platform/PlatVulkan.h"

#include <array>
#include <iostream>

LOG_CHANNEL(Graphic.Violet);

namespace sce::vlt
{
	VltInstance::VltInstance()
	{
		m_instance = this->createInstance();
		m_adapters = this->queryAdapters();
	}

	VltInstance::~VltInstance()
	{
		if (m_debugMessenger != VK_NULL_HANDLE)
		{
			DestroyDebugUtilsMessengerEXT(m_instance, m_debugMessenger, nullptr);
			m_debugMessenger = nullptr;
		}

		if (m_instance != VK_NULL_HANDLE)
		{
			vkDestroyInstance(m_instance, nullptr);
			m_instance = VK_NULL_HANDLE;
		}
	}

	Rc<VltAdapter> VltInstance::enumAdapters(uint32_t index) const
	{
		return index < m_adapters.size()
				   ? m_adapters[index]
				   : nullptr;
	}

	std::vector<VltExt> VltInstance::getPlatformExtensions()
	{
		std::vector<VltExt> result;

		auto platformExtensions = plat::vulkanGetRequiredInstanceExtensions();
		for (const auto& ext : platformExtensions)
		{
			result.emplace_back(ext, VltExtMode::Required);
		}

		return result;
	}

	VkInstance VltInstance::createInstance()
	{
		VltInstanceExtensions insExtensions;

		std::vector<VltExt*> insExtensionList = { {
			&insExtensions.khrGetSurfaceCapabilities2,
			&insExtensions.khrSurface,
		} };

		// merge platform extensions
		auto platformExtensions = getPlatformExtensions();
		for (auto& ext : platformExtensions)
		{
			insExtensionList.push_back(&ext);
		}

		std::vector<const char*> insLayers;

#ifdef VLT_VALIDATION_AND_DEBUG
		insExtensionList.push_back(&insExtensions.extDebugUtils);
		insLayers.push_back("VK_LAYER_KHRONOS_validation");

		VkDebugUtilsMessengerCreateInfoEXT debugInfo = {};
		debugInfo.sType                              = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		debugInfo.messageSeverity                    = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		debugInfo.messageType                        = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		debugInfo.pfnUserCallback                    = debugCallback;
		debugInfo.pUserData                          = this;

#endif

		VltNameSet extensionsEnabled;
		VltNameSet extensionsAvailable = VltNameSet::enumInstanceExtensions();

		if (!extensionsAvailable.enableExtensions(
				insExtensionList.size(),
				insExtensionList.data(),
				extensionsEnabled))
			Logger::exception("DxvkInstance: Failed to create instance");

		m_extensions = insExtensions;
		VltNameList extensionNameList = extensionsEnabled.toNameList();

		Logger::info("Enabled instance extensions:");
		this->logNameList(extensionNameList);

		std::string appName = GPCS4_APP_NAME;

		VkApplicationInfo appInfo;
		appInfo.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pNext              = nullptr;
		appInfo.pApplicationName   = appName.c_str();
		appInfo.applicationVersion = 0;
		appInfo.pEngineName        = "Violet";
		appInfo.engineVersion      = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion         = VK_MAKE_VERSION(1, 3, 0);

		VkInstanceCreateInfo info;
		info.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
#ifdef VLT_VALIDATION_AND_DEBUG
		info.pNext = &debugInfo;
#else
		info.pNext = nullptr;
#endif
		info.flags                   = 0;
		info.pApplicationInfo        = &appInfo;
		info.enabledLayerCount       = insLayers.size();
		info.ppEnabledLayerNames     = insLayers.data();
		info.enabledExtensionCount   = extensionNameList.count();
		info.ppEnabledExtensionNames = extensionNameList.names();

		VkInstance result = VK_NULL_HANDLE;
		VkResult   status = vkCreateInstance(&info, nullptr, &result);

		if (status != VK_SUCCESS)
			Logger::exception("DxvkInstance::createInstance: Failed to create Vulkan 1.3 instance");

#ifdef VLT_VALIDATION_AND_DEBUG
		status = CreateDebugUtilsMessengerEXT(result, &debugInfo, nullptr, &m_debugMessenger);
		if (status != VK_SUCCESS)
			Logger::exception("DxvkInstance::createInstance: Failed to create debug messenger.");
#endif  // VLT_VALIDATION_AND_DEBUG


		return result;
	}

	std::vector<Rc<VltAdapter>> VltInstance::queryAdapters()
	{
		uint32_t numAdapters = 0;
		if (vkEnumeratePhysicalDevices(m_instance, &numAdapters, nullptr) != VK_SUCCESS)
			Logger::exception("DxvkInstance::enumAdapters: Failed to enumerate adapters");

		std::vector<VkPhysicalDevice> adapters(numAdapters);
		if (vkEnumeratePhysicalDevices(m_instance, &numAdapters, adapters.data()) != VK_SUCCESS)
			Logger::exception("DxvkInstance::enumAdapters: Failed to enumerate adapters");

		std::vector<VkPhysicalDeviceProperties> deviceProperties(numAdapters);
		VltDeviceFilterFlags                    filterFlags = 0;

		for (uint32_t i = 0; i < numAdapters; i++)
		{
			vkGetPhysicalDeviceProperties(adapters[i], &deviceProperties[i]);

			if (deviceProperties[i].deviceType != VK_PHYSICAL_DEVICE_TYPE_CPU)
				filterFlags.set(VltDeviceFilterFlag::SkipCpuDevices);
		}

		VltDeviceFilter             filter(filterFlags);
		std::vector<Rc<VltAdapter>> result;

		for (uint32_t i = 0; i < numAdapters; i++)
		{
			if (filter.testAdapter(deviceProperties[i]))
				result.push_back(new VltAdapter(adapters[i]));
		}

		std::stable_sort(result.begin(), result.end(),
						 [](const Rc<VltAdapter>& a, const Rc<VltAdapter>& b) -> bool
						 {
							 static const std::array<VkPhysicalDeviceType, 3> deviceTypes = { {
								 VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU,
								 VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU,
								 VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU,
							 } };

							 uint32_t aRank = deviceTypes.size();
							 uint32_t bRank = deviceTypes.size();

							 for (uint32_t i = 0; i < std::min(aRank, bRank); i++)
							 {
								 if (a->deviceProperties().deviceType == deviceTypes[i])
									 aRank = i;
								 if (b->deviceProperties().deviceType == deviceTypes[i])
									 bRank = i;
							 }

							 return aRank < bRank;
						 });

		if (result.size() == 0)
		{
			Logger::exception("DXVK: No adapters found. Please check your "
							  "device filter settings and Vulkan setup.");
		}

		return result;
	}

	void VltInstance::logNameList(const VltNameList& names)
	{
		for (uint32_t i = 0; i < names.count(); i++)
			Logger::info(util::str::formatex("  ", names.name(i)));
	}

	VkResult VltInstance::CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
	{
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr)
		{
			return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
		}
		else
		{
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}

	void VltInstance::DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator)
	{
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr)
		{
			func(instance, debugMessenger, pAllocator);
		}
	}

	VKAPI_ATTR VkBool32 VKAPI_CALL VltInstance::debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT      messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT             messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void*                                       pUserData)
	{
		do 
		{
			// Filter out shader structured control flow information until we fix it.
			// See:
			// https://github.com/Inori/GPCS4/blob/shader_cfg/GPCS4/Graphics/Gcn/GcnCfgGenerator.h#L13
			if (std::strstr(pCallbackData->pMessage, "Selection must be structured"))
			{
				break;
			}

			if (std::strstr(pCallbackData->pMessage, "UNASSIGNED-DEBUG-PRINTF"))
			{
				break;
			}

			std::cerr << pCallbackData->pMessage << std::endl;

		} while (false);

		return VK_FALSE;
	}

}  // namespace sce::vlt