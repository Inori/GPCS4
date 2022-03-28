#include "VltInstance.h"

#include "VltAdapter.h"
#include "VltDeviceFilter.h"

#include "Platform/PlatVulkan.h"

#include <array>

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

		auto platformExtensions = getPlatformExtensions();
		for (auto& ext : platformExtensions)
		{
			insExtensionList.push_back(&ext);
		}

		std::vector<const char*> insLayers;

#ifdef VLT_VALIDATION_AND_DEBUG
		insExtensionList.push_back(&insExtensions.extDebugUtils);
		insLayers.push_back("VK_LAYER_KHRONOS_validation");
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
		info.pNext                   = nullptr;
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

}  // namespace sce::vlt