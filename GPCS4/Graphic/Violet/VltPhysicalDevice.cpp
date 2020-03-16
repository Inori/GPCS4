#include "VltPhysicalDevice.h"
#include "VltVkLayers.h"
#include "VltDevice.h"

#include <set>
#include <array>

LOG_CHANNEL(Graphic.Violet.VltPhysicalDevice);

namespace vlt
{;


VltPhysicalDevice::VltPhysicalDevice(VltInstance* instance, VkPhysicalDevice device):
	m_instance(instance),
	m_device(device)
{
	queryExtensions();
	queryDeviceInfo();
	queryDeviceFeatures();
	queryDeviceQueues();
}

VltPhysicalDevice::~VltPhysicalDevice()
{
	
}

VltPhysicalDevice::operator VkPhysicalDevice() const
{
	return m_device;
}

VltInstance* VltPhysicalDevice::instance() const
{
	return m_instance;
}

VltPhysicalDeviceQueueFamilies VltPhysicalDevice::findQueueFamilies()
{
	// The default graphics queue should also support compute.
	uint32_t graphicsQueue = findQueueFamily(
		VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT,
		VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT);

	// Dedicated queue for compute
	uint32_t computeQueue = findQueueFamily(
		VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT,
		VK_QUEUE_COMPUTE_BIT);

	// Dedicated queue for transfer
	uint32_t transferQueue = findQueueFamily(
		VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT,
		VK_QUEUE_TRANSFER_BIT);

	// The hardware which GPCS4 runs on is supposed to be
	// more powerful than PS4, so here we only use dedicated compute queue for now.
	LOG_ASSERT(computeQueue != VK_QUEUE_FAMILY_IGNORED, "the GPU doesn't support dedicated compute queue.");

	if (transferQueue == VK_QUEUE_FAMILY_IGNORED)
	{
		transferQueue = graphicsQueue;
	}

	VltPhysicalDeviceQueueFamilies queueFamilies;
	queueFamilies.graphics = graphicsQueue;
	queueFamilies.compute  = computeQueue;
	queueFamilies.transfer = transferQueue;

	return queueFamilies;
}

void VltPhysicalDevice::queryExtensions()
{
	m_deviceExtensions = VltNameSet::enumDeviceExtensions(m_device);
}

void VltPhysicalDevice::queryDeviceInfo()
{
	m_deviceInfo = VltDeviceInfo();
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
	if (VltGpuVendor(m_deviceInfo.core.properties.vendorID) == VltGpuVendor::Nvidia) 
	{
		m_deviceInfo.core.properties.driverVersion = VK_MAKE_VERSION(
			VK_VERSION_MAJOR(m_deviceInfo.core.properties.driverVersion),
			VK_VERSION_MINOR(m_deviceInfo.core.properties.driverVersion >> 0) >> 2,
			VK_VERSION_PATCH(m_deviceInfo.core.properties.driverVersion >> 2) >> 4);
	}
}

void VltPhysicalDevice::queryDeviceFeatures()
{
	m_deviceFeatures = VltDeviceFeatures();
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

void VltPhysicalDevice::queryDeviceQueues()
{
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(m_device, &queueFamilyCount, nullptr);

	m_queueFamilyProps.resize(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(m_device, &queueFamilyCount, m_queueFamilyProps.data());
}

std::vector<VkExtensionProperties> VltPhysicalDevice::availableExtensions()
{
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(m_device, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(m_device, nullptr, &extensionCount, availableExtensions.data());

	return availableExtensions;
}

const VltDeviceFeatures& VltPhysicalDevice::features() const
{
	return m_deviceFeatures;
}

const VkPhysicalDeviceProperties& VltPhysicalDevice::deviceProperties() const
{
	return m_deviceInfo.core.properties;
}

const VltDeviceInfo& VltPhysicalDevice::devicePropertiesExt() const
{
	return m_deviceInfo;
}

VkPhysicalDeviceMemoryProperties VltPhysicalDevice::memoryProperties() const
{
	VkPhysicalDeviceMemoryProperties memoryProperties;
	vkGetPhysicalDeviceMemoryProperties(m_device, &memoryProperties);
	return memoryProperties;
}

bool VltPhysicalDevice::checkFeatureSupport(const VltDeviceFeatures& required) const
{
	return (m_deviceFeatures.core.features.robustBufferAccess
		|| !required.core.features.robustBufferAccess)
		&& (m_deviceFeatures.core.features.fullDrawIndexUint32
			|| !required.core.features.fullDrawIndexUint32)
		&& (m_deviceFeatures.core.features.imageCubeArray
			|| !required.core.features.imageCubeArray)
		&& (m_deviceFeatures.core.features.independentBlend
			|| !required.core.features.independentBlend)
		&& (m_deviceFeatures.core.features.geometryShader
			|| !required.core.features.geometryShader)
		&& (m_deviceFeatures.core.features.tessellationShader
			|| !required.core.features.tessellationShader)
		&& (m_deviceFeatures.core.features.sampleRateShading
			|| !required.core.features.sampleRateShading)
		&& (m_deviceFeatures.core.features.dualSrcBlend
			|| !required.core.features.dualSrcBlend)
		&& (m_deviceFeatures.core.features.logicOp
			|| !required.core.features.logicOp)
		&& (m_deviceFeatures.core.features.multiDrawIndirect
			|| !required.core.features.multiDrawIndirect)
		&& (m_deviceFeatures.core.features.drawIndirectFirstInstance
			|| !required.core.features.drawIndirectFirstInstance)
		&& (m_deviceFeatures.core.features.depthClamp
			|| !required.core.features.depthClamp)
		&& (m_deviceFeatures.core.features.depthBiasClamp
			|| !required.core.features.depthBiasClamp)
		&& (m_deviceFeatures.core.features.fillModeNonSolid
			|| !required.core.features.fillModeNonSolid)
		&& (m_deviceFeatures.core.features.depthBounds
			|| !required.core.features.depthBounds)
		&& (m_deviceFeatures.core.features.wideLines
			|| !required.core.features.wideLines)
		&& (m_deviceFeatures.core.features.largePoints
			|| !required.core.features.largePoints)
		&& (m_deviceFeatures.core.features.alphaToOne
			|| !required.core.features.alphaToOne)
		&& (m_deviceFeatures.core.features.multiViewport
			|| !required.core.features.multiViewport)
		&& (m_deviceFeatures.core.features.samplerAnisotropy
			|| !required.core.features.samplerAnisotropy)
		&& (m_deviceFeatures.core.features.textureCompressionETC2
			|| !required.core.features.textureCompressionETC2)
		&& (m_deviceFeatures.core.features.textureCompressionASTC_LDR
			|| !required.core.features.textureCompressionASTC_LDR)
		&& (m_deviceFeatures.core.features.textureCompressionBC
			|| !required.core.features.textureCompressionBC)
		&& (m_deviceFeatures.core.features.occlusionQueryPrecise
			|| !required.core.features.occlusionQueryPrecise)
		&& (m_deviceFeatures.core.features.pipelineStatisticsQuery
			|| !required.core.features.pipelineStatisticsQuery)
		&& (m_deviceFeatures.core.features.vertexPipelineStoresAndAtomics
			|| !required.core.features.vertexPipelineStoresAndAtomics)
		&& (m_deviceFeatures.core.features.fragmentStoresAndAtomics
			|| !required.core.features.fragmentStoresAndAtomics)
		&& (m_deviceFeatures.core.features.shaderTessellationAndGeometryPointSize
			|| !required.core.features.shaderTessellationAndGeometryPointSize)
		&& (m_deviceFeatures.core.features.shaderImageGatherExtended
			|| !required.core.features.shaderImageGatherExtended)
		&& (m_deviceFeatures.core.features.shaderStorageImageExtendedFormats
			|| !required.core.features.shaderStorageImageExtendedFormats)
		&& (m_deviceFeatures.core.features.shaderStorageImageMultisample
			|| !required.core.features.shaderStorageImageMultisample)
		&& (m_deviceFeatures.core.features.shaderStorageImageReadWithoutFormat
			|| !required.core.features.shaderStorageImageReadWithoutFormat)
		&& (m_deviceFeatures.core.features.shaderStorageImageWriteWithoutFormat
			|| !required.core.features.shaderStorageImageWriteWithoutFormat)
		&& (m_deviceFeatures.core.features.shaderUniformBufferArrayDynamicIndexing
			|| !required.core.features.shaderUniformBufferArrayDynamicIndexing)
		&& (m_deviceFeatures.core.features.shaderSampledImageArrayDynamicIndexing
			|| !required.core.features.shaderSampledImageArrayDynamicIndexing)
		&& (m_deviceFeatures.core.features.shaderStorageBufferArrayDynamicIndexing
			|| !required.core.features.shaderStorageBufferArrayDynamicIndexing)
		&& (m_deviceFeatures.core.features.shaderStorageImageArrayDynamicIndexing
			|| !required.core.features.shaderStorageImageArrayDynamicIndexing)
		&& (m_deviceFeatures.core.features.shaderClipDistance
			|| !required.core.features.shaderClipDistance)
		&& (m_deviceFeatures.core.features.shaderCullDistance
			|| !required.core.features.shaderCullDistance)
		&& (m_deviceFeatures.core.features.shaderFloat64
			|| !required.core.features.shaderFloat64)
		&& (m_deviceFeatures.core.features.shaderInt64
			|| !required.core.features.shaderInt64)
		&& (m_deviceFeatures.core.features.shaderInt16
			|| !required.core.features.shaderInt16)
		&& (m_deviceFeatures.core.features.shaderResourceResidency
			|| !required.core.features.shaderResourceResidency)
		&& (m_deviceFeatures.core.features.shaderResourceMinLod
			|| !required.core.features.shaderResourceMinLod)
		&& (m_deviceFeatures.core.features.sparseBinding
			|| !required.core.features.sparseBinding)
		&& (m_deviceFeatures.core.features.sparseResidencyBuffer
			|| !required.core.features.sparseResidencyBuffer)
		&& (m_deviceFeatures.core.features.sparseResidencyImage2D
			|| !required.core.features.sparseResidencyImage2D)
		&& (m_deviceFeatures.core.features.sparseResidencyImage3D
			|| !required.core.features.sparseResidencyImage3D)
		&& (m_deviceFeatures.core.features.sparseResidency2Samples
			|| !required.core.features.sparseResidency2Samples)
		&& (m_deviceFeatures.core.features.sparseResidency4Samples
			|| !required.core.features.sparseResidency4Samples)
		&& (m_deviceFeatures.core.features.sparseResidency8Samples
			|| !required.core.features.sparseResidency8Samples)
		&& (m_deviceFeatures.core.features.sparseResidency16Samples
			|| !required.core.features.sparseResidency16Samples)
		&& (m_deviceFeatures.core.features.sparseResidencyAliased
			|| !required.core.features.sparseResidencyAliased)
		&& (m_deviceFeatures.core.features.variableMultisampleRate
			|| !required.core.features.variableMultisampleRate)
		&& (m_deviceFeatures.core.features.inheritedQueries
			|| !required.core.features.inheritedQueries)
		&& (m_deviceFeatures.shaderDrawParameters.shaderDrawParameters
			|| !required.shaderDrawParameters.shaderDrawParameters)
		&& (m_deviceFeatures.extConditionalRendering.conditionalRendering
			|| !required.extConditionalRendering.conditionalRendering)
		&& (m_deviceFeatures.extDepthClipEnable.depthClipEnable
			|| !required.extDepthClipEnable.depthClipEnable)
		&& (m_deviceFeatures.extHostQueryReset.hostQueryReset
			|| !required.extHostQueryReset.hostQueryReset)
		&& (m_deviceFeatures.extMemoryPriority.memoryPriority
			|| !required.extMemoryPriority.memoryPriority)
		&& (m_deviceFeatures.extTransformFeedback.transformFeedback
			|| !required.extTransformFeedback.transformFeedback)
		&& (m_deviceFeatures.extVertexAttributeDivisor.vertexAttributeInstanceRateDivisor
			|| !required.extVertexAttributeDivisor.vertexAttributeInstanceRateDivisor)
		&& (m_deviceFeatures.extVertexAttributeDivisor.vertexAttributeInstanceRateZeroDivisor
			|| !required.extVertexAttributeDivisor.vertexAttributeInstanceRateZeroDivisor);
}

void VltPhysicalDevice::populateQueueCreateInfos(std::vector<VkDeviceQueueCreateInfo>& queueInfos)
{
	float                          queuePriority = 1.0f;
	VltPhysicalDeviceQueueFamilies queueFamilies = findQueueFamilies();

	VkDeviceQueueCreateInfo queueInfo = {};
	queueInfo.sType                   = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueInfo.queueFamilyIndex        = queueFamilies.graphics;
	queueInfo.queueCount              = 1;
	queueInfo.pQueuePriorities        = &queuePriority;
	queueInfos.push_back(queueInfo);

	if (queueFamilies.compute != queueFamilies.graphics)
	{
		// If compute family index differs, we need an additional queue create info for the compute queue
		VkDeviceQueueCreateInfo queueInfo = {};
		queueInfo.sType                   = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueInfo.queueFamilyIndex        = queueFamilies.compute;
		queueInfo.queueCount              = 1;
		queueInfo.pQueuePriorities        = &queuePriority;
		queueInfos.push_back(queueInfo);
	}

	if ((queueFamilies.transfer != queueFamilies.graphics) &&
		(queueFamilies.transfer != queueFamilies.compute))
	{
		// If compute family index differs, we need an additional queue create info for the compute queue
		VkDeviceQueueCreateInfo queueInfo = {};
		queueInfo.sType                   = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueInfo.queueFamilyIndex        = queueFamilies.transfer;
		queueInfo.queueCount              = 1;
		queueInfo.pQueuePriorities        = &queuePriority;
		queueInfos.push_back(queueInfo);
	}
}

RcPtr<vlt::VltDevice> VltPhysicalDevice::createLogicalDevice(const VltDeviceFeatures& features)
{
	RcPtr<VltDevice> createdDevice;
	do 
	{
		VltDeviceExtensions devExtensions;

		std::array<VltExt*, 3> devExtensionList = { {
		  &devExtensions.khrSwapchain,
		  &devExtensions.khrDedicatedAllocation,
		  &devExtensions.khrMaintenance1
		} };

		VltNameSet extensionsEnabled;

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
		VltNameList extensionNameList = extensionsEnabled.toNameList();

		std::vector<VkDeviceQueueCreateInfo> queueInfos;
		populateQueueCreateInfos(queueInfos);

		VkDeviceCreateInfo createInfo      = {};
		createInfo.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.pQueueCreateInfos       = queueInfos.data();
		createInfo.queueCreateInfoCount    = queueInfos.size();
		createInfo.pEnabledFeatures        = &features.core.features;
		createInfo.enabledExtensionCount   = extensionNameList.count();
		createInfo.ppEnabledExtensionNames = extensionNameList.names();

#ifdef VLT_VALIDATION_LAYERS_ENABLE

		createInfo.enabledLayerCount   = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();

#else  // VLT_VALIDATION_LAYERS_ENABLE

		createInfo.enabledLayerCount = 0;

#endif  // VLT_VALIDATION_LAYERS_ENABLE

		VkDevice logicalDevice = VK_NULL_HANDLE;
		if (vkCreateDevice(m_device, &createInfo, nullptr, &logicalDevice) != VK_SUCCESS)
		{
			break;
		}

		createdDevice = new VltDevice(logicalDevice, this);
	} while (false);

	return createdDevice;
}

uint32_t VltPhysicalDevice::findQueueFamily(VkQueueFlags mask, VkQueueFlags flags)
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


} // namespace vlt