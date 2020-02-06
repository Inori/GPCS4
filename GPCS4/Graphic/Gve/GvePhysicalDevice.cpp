#include "GvePhysicalDevice.h"
#include "GveVkLayers.h"
#include "GveDevice.h"

#include <set>
#include <array>

LOG_CHANNEL(Graphic.Gve.GvePhysicalDevice);

namespace gve
{;


GvePhysicalDevice::GvePhysicalDevice(GveInstance* instance, VkPhysicalDevice device):
	m_instance(instance),
	m_device(device)
{
	queryExtensions();
	queryDeviceInfo();
	queryDeviceFeatures();
	queryDeviceQueues();
}

GvePhysicalDevice::~GvePhysicalDevice()
{
	
}

GvePhysicalDevice::operator VkPhysicalDevice() const
{
	return m_device;
}

GveInstance* GvePhysicalDevice::instance() const
{
	return m_instance;
}

GvePhysicalDeviceQueueFamilies GvePhysicalDevice::findQueueFamilies()
{
	uint32_t graphicsQueue = findQueueFamily(
		VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT,
		VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT);

	GvePhysicalDeviceQueueFamilies queueFamilies;
	queueFamilies.graphicsFamily = graphicsQueue;
	return queueFamilies;
}

void GvePhysicalDevice::queryExtensions()
{
	m_deviceExtensions = GveNameSet::enumDeviceExtensions(m_device);
}

void GvePhysicalDevice::queryDeviceInfo()
{
	m_deviceInfo = GveDeviceInfo();
	m_deviceInfo.core.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2_KHR;
	m_deviceInfo.core.pNext = nullptr;

	// Query info now so that we have basic device properties available
	vkGetPhysicalDeviceProperties2(m_device, &m_deviceInfo.core);

	//if (m_deviceInfo.core.properties.apiVersion >= VK_MAKE_VERSION(1, 1, 0)) 
	//{
	//	m_deviceInfo.coreDeviceId.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ID_PROPERTIES;
	//	m_deviceInfo.coreDeviceId.pNext = std::exchange(m_deviceInfo.core.pNext, &m_deviceInfo.coreDeviceId);

	//	m_deviceInfo.coreSubgroup.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SUBGROUP_PROPERTIES;
	//	m_deviceInfo.coreSubgroup.pNext = std::exchange(m_deviceInfo.core.pNext, &m_deviceInfo.coreSubgroup);
	//}

	//if (m_deviceExtensions.supports(VK_EXT_TRANSFORM_FEEDBACK_EXTENSION_NAME)) 
	//{
	//	m_deviceInfo.extTransformFeedback.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TRANSFORM_FEEDBACK_PROPERTIES_EXT;
	//	m_deviceInfo.extTransformFeedback.pNext = std::exchange(m_deviceInfo.core.pNext, &m_deviceInfo.extTransformFeedback);
	//}

	//if (m_deviceExtensions.supports(VK_EXT_VERTEX_ATTRIBUTE_DIVISOR_EXTENSION_NAME)) 
	//{
	//	m_deviceInfo.extVertexAttributeDivisor.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VERTEX_ATTRIBUTE_DIVISOR_PROPERTIES_EXT;
	//	m_deviceInfo.extVertexAttributeDivisor.pNext = std::exchange(m_deviceInfo.core.pNext, &m_deviceInfo.extVertexAttributeDivisor);
	//}

	//if (m_deviceExtensions.supports(VK_KHR_DEPTH_STENCIL_RESOLVE_EXTENSION_NAME)) 
	//{
	//	m_deviceInfo.khrDepthStencilResolve.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEPTH_STENCIL_RESOLVE_PROPERTIES_KHR;
	//	m_deviceInfo.khrDepthStencilResolve.pNext = std::exchange(m_deviceInfo.core.pNext, &m_deviceInfo.khrDepthStencilResolve);
	//}

	//if (m_deviceExtensions.supports(VK_KHR_DRIVER_PROPERTIES_EXTENSION_NAME)) 
	//{
	//	m_deviceInfo.khrDeviceDriverProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DRIVER_PROPERTIES_KHR;
	//	m_deviceInfo.khrDeviceDriverProperties.pNext = std::exchange(m_deviceInfo.core.pNext, &m_deviceInfo.khrDeviceDriverProperties);
	//}

	// Query full device properties for all enabled extensions
	//vkGetPhysicalDeviceProperties2(m_device, &m_deviceInfo.core);

	// Nvidia reports the driver version in a slightly different format
	if (GveGpuVendor(m_deviceInfo.core.properties.vendorID) == GveGpuVendor::Nvidia) 
	{
		m_deviceInfo.core.properties.driverVersion = VK_MAKE_VERSION(
			VK_VERSION_MAJOR(m_deviceInfo.core.properties.driverVersion),
			VK_VERSION_MINOR(m_deviceInfo.core.properties.driverVersion >> 0) >> 2,
			VK_VERSION_PATCH(m_deviceInfo.core.properties.driverVersion >> 2) >> 4);
	}
}

void GvePhysicalDevice::queryDeviceFeatures()
{
	m_deviceFeatures = GveDeviceFeatures();
	m_deviceFeatures.core.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2_KHR;
	m_deviceFeatures.core.pNext = nullptr;

	//if (m_deviceExtensions.supports(VK_EXT_CONDITIONAL_RENDERING_EXTENSION_NAME)) 
	//{
	//	m_deviceFeatures.extConditionalRendering.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CONDITIONAL_RENDERING_FEATURES_EXT;
	//	m_deviceFeatures.extConditionalRendering.pNext = std::exchange(m_deviceFeatures.core.pNext, &m_deviceFeatures.extConditionalRendering);
	//}

	//if (m_deviceExtensions.supports(VK_EXT_DEPTH_CLIP_ENABLE_EXTENSION_NAME)) 
	//{
	//	m_deviceFeatures.extDepthClipEnable.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEPTH_CLIP_ENABLE_FEATURES_EXT;
	//	m_deviceFeatures.extDepthClipEnable.pNext = std::exchange(m_deviceFeatures.core.pNext, &m_deviceFeatures.extDepthClipEnable);
	//}

	//if (m_deviceExtensions.supports(VK_EXT_HOST_QUERY_RESET_EXTENSION_NAME)) 
	//{
	//	m_deviceFeatures.extHostQueryReset.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_HOST_QUERY_RESET_FEATURES_EXT;
	//	m_deviceFeatures.extHostQueryReset.pNext = std::exchange(m_deviceFeatures.core.pNext, &m_deviceFeatures.extHostQueryReset);
	//}

	if (m_deviceExtensions.supports(VK_EXT_MEMORY_PRIORITY_EXTENSION_NAME)) 
	{
		m_deviceFeatures.extMemoryPriority.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_PRIORITY_FEATURES_EXT;
		m_deviceFeatures.extMemoryPriority.pNext = std::exchange(m_deviceFeatures.core.pNext, &m_deviceFeatures.extMemoryPriority);
	}

	//if (m_deviceExtensions.supports(VK_EXT_SHADER_DEMOTE_TO_HELPER_INVOCATION_EXTENSION_NAME)) 
	//{
	//	m_deviceFeatures.extShaderDemoteToHelperInvocation.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_DEMOTE_TO_HELPER_INVOCATION_FEATURES_EXT;
	//	m_deviceFeatures.extShaderDemoteToHelperInvocation.pNext = std::exchange(m_deviceFeatures.core.pNext, &m_deviceFeatures.extShaderDemoteToHelperInvocation);
	//}

	//if (m_deviceExtensions.supports(VK_EXT_TRANSFORM_FEEDBACK_EXTENSION_NAME)) 
	//{
	//	m_deviceFeatures.extTransformFeedback.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TRANSFORM_FEEDBACK_FEATURES_EXT;
	//	m_deviceFeatures.extTransformFeedback.pNext = std::exchange(m_deviceFeatures.core.pNext, &m_deviceFeatures.extTransformFeedback);
	//}

	//if (m_deviceExtensions.supports(VK_EXT_VERTEX_ATTRIBUTE_DIVISOR_EXTENSION_NAME) >= 3) 
	//{
	//	m_deviceFeatures.extVertexAttributeDivisor.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VERTEX_ATTRIBUTE_DIVISOR_FEATURES_EXT;
	//	m_deviceFeatures.extVertexAttributeDivisor.pNext = std::exchange(m_deviceFeatures.core.pNext, &m_deviceFeatures.extVertexAttributeDivisor);
	//}

	vkGetPhysicalDeviceFeatures2(m_device, &m_deviceFeatures.core);
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

const GveDeviceFeatures& GvePhysicalDevice::features() const
{
	return m_deviceFeatures;
}

const VkPhysicalDeviceProperties& GvePhysicalDevice::deviceProperties() const
{
	return m_deviceInfo.core.properties;
}

const GveDeviceInfo& GvePhysicalDevice::devicePropertiesExt() const
{
	return m_deviceInfo;
}

VkPhysicalDeviceMemoryProperties GvePhysicalDevice::memoryProperties() const
{
	VkPhysicalDeviceMemoryProperties memoryProperties;
	vkGetPhysicalDeviceMemoryProperties(m_device, &memoryProperties);
	return memoryProperties;
}

GveDeviceFeatures GvePhysicalDevice::getDeviceFeatures()
{
	GveDeviceFeatures supported = features();
	GveDeviceFeatures enabled = {};

	enabled.core.features.samplerAnisotropy = VK_TRUE;
	enabled.core.features.shaderInt64       = supported.core.features.shaderInt64;
	return enabled;
}

RcPtr<gve::GveDevice> GvePhysicalDevice::createLogicalDevice()
{
	RcPtr<GveDevice> createdDevice;
	do 
	{
		GveDeviceExtensions devExtensions;

		std::array<GveExt*, 3> devExtensionList = { {
		  &devExtensions.khrSwapchain,
		  &devExtensions.khrDedicatedAllocation,
		  &devExtensions.khrMaintenance1
		} };

		GveNameSet extensionsEnabled;

		if (!m_deviceExtensions.enableExtensions(
			devExtensionList.size(),
			devExtensionList.data(),
			extensionsEnabled))
		{
			LOG_ERR("Failed to create device");
			break;
		}
			
		// Enable additional extensions if necessary
		extensionsEnabled.merge(m_extraExtensions);
		GveNameList extensionNameList = extensionsEnabled.toNameList();

		// TODO:
		// We should check whether the present queue is the same as graphic queue.
		float queuePriority = 1.0f;
		GvePhysicalDeviceQueueFamilies queueFamilies = findQueueFamilies();
		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamilies.graphicsFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;

		GveDeviceFeatures features = getDeviceFeatures();

		VkDeviceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

		createInfo.queueCreateInfoCount = 1;
		createInfo.pQueueCreateInfos = &queueCreateInfo;
		createInfo.pEnabledFeatures = &features.core.features;
		createInfo.enabledExtensionCount = extensionNameList.count();
		createInfo.ppEnabledExtensionNames = extensionNameList.names();

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