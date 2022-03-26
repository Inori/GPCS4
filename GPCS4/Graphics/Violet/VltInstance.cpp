#include "VltInstance.h"
#include "VltAdapter.h"
#include "VltDeviceFilter.h"

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

	Rc<VltAdapter> VltInstance::findAdapterByLuid(const void* luid) const
	{
		for (const auto& adapter : m_adapters)
		{
			const auto& props = adapter->devicePropertiesExt().coreDeviceId;

			if (props.deviceLUIDValid && !std::memcmp(luid, props.deviceLUID, VK_LUID_SIZE))
				return adapter;
		}

		return nullptr;
	}

	Rc<VltAdapter> VltInstance::findAdapterByDeviceId(uint16_t vendorId, uint16_t deviceId) const
	{
		for (const auto& adapter : m_adapters)
		{
			const auto& props = adapter->deviceProperties();

			if (props.vendorID == vendorId && props.deviceID == deviceId)
				return adapter;
		}

		return nullptr;
	}

	VkInstance VltInstance::createInstance()
	{
		VltInstanceExtensions insExtensions;

		std::vector<VltExt*> insExtensionList = { {
			&insExtensions.khrGetSurfaceCapabilities2,
			&insExtensions.khrSurface,
		} };

#ifdef GPCS4_DEBUG
		insExtensionList.push_back(&insExtensions.extDebugUtils);
#endif

		VltNameSet extensionsEnabled;
		VltNameSet extensionsAvailable = VltNameSet::enumInstanceExtensions();

		if (!extensionsAvailable.enableExtensions(
				insExtensionList.size(),
				insExtensionList.data(),
				extensionsEnabled))
			Logger::assert("DxvkInstance: Failed to create instance");

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
		appInfo.pEngineName        = "DXVK";
		appInfo.engineVersion      = VK_MAKE_VERSION(1, 10, 0);
		appInfo.apiVersion         = VK_MAKE_VERSION(1, 1, 0);

		VkInstanceCreateInfo info;
		info.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		info.pNext                   = nullptr;
		info.flags                   = 0;
		info.pApplicationInfo        = &appInfo;
		info.enabledLayerCount       = 0;
		info.ppEnabledLayerNames     = nullptr;
		info.enabledExtensionCount   = extensionNameList.count();
		info.ppEnabledExtensionNames = extensionNameList.names();

		VkInstance result = VK_NULL_HANDLE;
		VkResult   status = vkCreateInstance(&info, nullptr, &result);

		if (status != VK_SUCCESS)
			Logger::assert("DxvkInstance::createInstance: Failed to create Vulkan 1.1 instance");

		return result;
	}

	std::vector<Rc<VltAdapter>> VltInstance::queryAdapters()
	{
		uint32_t numAdapters = 0;
		if (vkEnumeratePhysicalDevices(m_instance, &numAdapters, nullptr) != VK_SUCCESS)
			Logger::assert("DxvkInstance::enumAdapters: Failed to enumerate adapters");

		std::vector<VkPhysicalDevice> adapters(numAdapters);
		if (vkEnumeratePhysicalDevices(m_instance, &numAdapters, adapters.data()) != VK_SUCCESS)
			Logger::assert("DxvkInstance::enumAdapters: Failed to enumerate adapters");

		std::vector<VkPhysicalDeviceProperties> deviceProperties(numAdapters);
		VltDeviceFilterFlags                   filterFlags = 0;

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
			LOG_WARN("DXVK: No adapters found. Please check your "
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