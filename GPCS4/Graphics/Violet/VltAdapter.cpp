#include "VltAdapter.h"
#include "VltDevice.h"
#include "VltInstance.h"

#include <ios>
#include <unordered_set>

LOG_CHANNEL(Graphic.Violet);

namespace sce::vlt
{
	VltAdapter::VltAdapter(VkPhysicalDevice handle) :
		m_handle(handle)
	{
		this->initHeapAllocInfo();
		this->queryExtensions();
		this->queryDeviceInfo();
		this->queryDeviceFeatures();
		this->queryDeviceQueues();

		m_hasMemoryBudget = m_deviceExtensions.supports(VK_EXT_MEMORY_BUDGET_EXTENSION_NAME);
	}

	VltAdapter::~VltAdapter()
	{
	}

	VltAdapterMemoryInfo VltAdapter::getMemoryHeapInfo() const
	{
		VkPhysicalDeviceMemoryBudgetPropertiesEXT memBudget = {};
		memBudget.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_BUDGET_PROPERTIES_EXT;
		memBudget.pNext = nullptr;

		VkPhysicalDeviceMemoryProperties2 memProps = {};
		memProps.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_PROPERTIES_2;
		memProps.pNext = m_hasMemoryBudget ? &memBudget : nullptr;

		vkGetPhysicalDeviceMemoryProperties2(m_handle, &memProps);

		VltAdapterMemoryInfo info = {};
		info.heapCount            = memProps.memoryProperties.memoryHeapCount;

		for (uint32_t i = 0; i < info.heapCount; i++)
		{
			info.heaps[i].heapFlags = memProps.memoryProperties.memoryHeaps[i].flags;

			if (m_hasMemoryBudget)
			{
				info.heaps[i].memoryBudget    = memBudget.heapBudget[i];
				info.heaps[i].memoryAllocated = memBudget.heapUsage[i];
			}
			else
			{
				info.heaps[i].memoryBudget    = memProps.memoryProperties.memoryHeaps[i].size;
				info.heaps[i].memoryAllocated = m_heapAlloc[i].load();
			}
		}

		return info;
	}

	VkPhysicalDeviceMemoryProperties VltAdapter::memoryProperties() const
	{
		VkPhysicalDeviceMemoryProperties memoryProperties;
		vkGetPhysicalDeviceMemoryProperties(m_handle, &memoryProperties);
		return memoryProperties;
	}

	VkFormatProperties VltAdapter::formatProperties(VkFormat format) const
	{
		VkFormatProperties formatProperties;
		vkGetPhysicalDeviceFormatProperties(m_handle, format, &formatProperties);
		return formatProperties;
	}

	VkResult VltAdapter::imageFormatProperties(
		VkFormat                 format,
		VkImageType              type,
		VkImageTiling            tiling,
		VkImageUsageFlags        usage,
		VkImageCreateFlags       flags,
		VkImageFormatProperties& properties) const
	{
		return vkGetPhysicalDeviceImageFormatProperties(
			m_handle, format, type, tiling, usage, flags, &properties);
	}

	VltAdapterQueueIndices VltAdapter::findQueueFamilies() const
	{
		uint32_t graphicsQueue = findQueueFamily(
			VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT,
			VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT);

		uint32_t computeQueue = findQueueFamily(
			VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT,
			VK_QUEUE_COMPUTE_BIT);

		if (computeQueue == VK_QUEUE_FAMILY_IGNORED)
			computeQueue = graphicsQueue;

		uint32_t transferQueue = findQueueFamily(
			VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT,
			VK_QUEUE_TRANSFER_BIT);

		if (transferQueue == VK_QUEUE_FAMILY_IGNORED)
			transferQueue = computeQueue;

		VltAdapterQueueIndices queues;
		queues.graphics = graphicsQueue;
		queues.compute  = computeQueue;
		queues.transfer = transferQueue;
		return queues;
	}

	// clang-format off
	bool VltAdapter::checkFeatureSupport(const VltDeviceFeatures& required) const {
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
			&& (m_deviceFeatures.vk11.storageBuffer16BitAccess
                    || !required.vk11.storageBuffer16BitAccess)
            && (m_deviceFeatures.vk11.uniformAndStorageBuffer16BitAccess
                    || !required.vk11.uniformAndStorageBuffer16BitAccess)
            && (m_deviceFeatures.vk11.storagePushConstant16
                    || !required.vk11.storagePushConstant16)
            && (m_deviceFeatures.vk11.storageInputOutput16
                    || !required.vk11.storageInputOutput16)
            && (m_deviceFeatures.vk11.multiview
                    || !required.vk11.multiview)
            && (m_deviceFeatures.vk11.multiviewGeometryShader
                    || !required.vk11.multiviewGeometryShader)
            && (m_deviceFeatures.vk11.multiviewTessellationShader
                    || !required.vk11.multiviewTessellationShader)
            && (m_deviceFeatures.vk11.variablePointersStorageBuffer
                    || !required.vk11.variablePointersStorageBuffer)
            && (m_deviceFeatures.vk11.variablePointers
                    || !required.vk11.variablePointers)
            && (m_deviceFeatures.vk11.protectedMemory
                    || !required.vk11.protectedMemory)
            && (m_deviceFeatures.vk11.samplerYcbcrConversion
                    || !required.vk11.samplerYcbcrConversion)
            && (m_deviceFeatures.vk11.shaderDrawParameters
                    || !required.vk11.shaderDrawParameters)
			&& (m_deviceFeatures.vk12.samplerMirrorClampToEdge
                    || !required.vk12.samplerMirrorClampToEdge)
            && (m_deviceFeatures.vk12.drawIndirectCount
                    || !required.vk12.drawIndirectCount)
            && (m_deviceFeatures.vk12.storageBuffer8BitAccess
                    || !required.vk12.storageBuffer8BitAccess)
            && (m_deviceFeatures.vk12.uniformAndStorageBuffer8BitAccess
                    || !required.vk12.uniformAndStorageBuffer8BitAccess)
            && (m_deviceFeatures.vk12.storagePushConstant8
                    || !required.vk12.storagePushConstant8)
            && (m_deviceFeatures.vk12.shaderBufferInt64Atomics
                    || !required.vk12.shaderBufferInt64Atomics)
            && (m_deviceFeatures.vk12.shaderSharedInt64Atomics
                    || !required.vk12.shaderSharedInt64Atomics)
            && (m_deviceFeatures.vk12.shaderFloat16
                    || !required.vk12.shaderFloat16)
            && (m_deviceFeatures.vk12.shaderInt8
                    || !required.vk12.shaderInt8)
            && (m_deviceFeatures.vk12.descriptorIndexing
                    || !required.vk12.descriptorIndexing)
            && (m_deviceFeatures.vk12.shaderInputAttachmentArrayDynamicIndexing
                    || !required.vk12.shaderInputAttachmentArrayDynamicIndexing)
            && (m_deviceFeatures.vk12.shaderUniformTexelBufferArrayDynamicIndexing
                    || !required.vk12.shaderUniformTexelBufferArrayDynamicIndexing)
            && (m_deviceFeatures.vk12.shaderStorageTexelBufferArrayDynamicIndexing
                    || !required.vk12.shaderStorageTexelBufferArrayDynamicIndexing)
            && (m_deviceFeatures.vk12.shaderUniformBufferArrayNonUniformIndexing
                    || !required.vk12.shaderUniformBufferArrayNonUniformIndexing)
            && (m_deviceFeatures.vk12.shaderSampledImageArrayNonUniformIndexing
                    || !required.vk12.shaderSampledImageArrayNonUniformIndexing)
            && (m_deviceFeatures.vk12.shaderStorageBufferArrayNonUniformIndexing
                    || !required.vk12.shaderStorageBufferArrayNonUniformIndexing)
            && (m_deviceFeatures.vk12.shaderStorageImageArrayNonUniformIndexing
                    || !required.vk12.shaderStorageImageArrayNonUniformIndexing)
            && (m_deviceFeatures.vk12.shaderInputAttachmentArrayNonUniformIndexing
                    || !required.vk12.shaderInputAttachmentArrayNonUniformIndexing)
            && (m_deviceFeatures.vk12.shaderUniformTexelBufferArrayNonUniformIndexing
                    || !required.vk12.shaderUniformTexelBufferArrayNonUniformIndexing)
            && (m_deviceFeatures.vk12.shaderStorageTexelBufferArrayNonUniformIndexing
                    || !required.vk12.shaderStorageTexelBufferArrayNonUniformIndexing)
            && (m_deviceFeatures.vk12.descriptorBindingUniformBufferUpdateAfterBind
                    || !required.vk12.descriptorBindingUniformBufferUpdateAfterBind)
            && (m_deviceFeatures.vk12.descriptorBindingSampledImageUpdateAfterBind
                    || !required.vk12.descriptorBindingSampledImageUpdateAfterBind)
            && (m_deviceFeatures.vk12.descriptorBindingStorageImageUpdateAfterBind
                    || !required.vk12.descriptorBindingStorageImageUpdateAfterBind)
            && (m_deviceFeatures.vk12.descriptorBindingStorageBufferUpdateAfterBind
                    || !required.vk12.descriptorBindingStorageBufferUpdateAfterBind)
            && (m_deviceFeatures.vk12.descriptorBindingUniformTexelBufferUpdateAfterBind
                    || !required.vk12.descriptorBindingUniformTexelBufferUpdateAfterBind)
            && (m_deviceFeatures.vk12.descriptorBindingStorageTexelBufferUpdateAfterBind
                    || !required.vk12.descriptorBindingStorageTexelBufferUpdateAfterBind)
            && (m_deviceFeatures.vk12.descriptorBindingUpdateUnusedWhilePending
                    || !required.vk12.descriptorBindingUpdateUnusedWhilePending)
            && (m_deviceFeatures.vk12.descriptorBindingPartiallyBound
                    || !required.vk12.descriptorBindingPartiallyBound)
            && (m_deviceFeatures.vk12.descriptorBindingVariableDescriptorCount
                    || !required.vk12.descriptorBindingVariableDescriptorCount)
            && (m_deviceFeatures.vk12.runtimeDescriptorArray
                    || !required.vk12.runtimeDescriptorArray)
            && (m_deviceFeatures.vk12.samplerFilterMinmax
                    || !required.vk12.samplerFilterMinmax)
            && (m_deviceFeatures.vk12.scalarBlockLayout
                    || !required.vk12.scalarBlockLayout)
            && (m_deviceFeatures.vk12.imagelessFramebuffer
                    || !required.vk12.imagelessFramebuffer)
            && (m_deviceFeatures.vk12.uniformBufferStandardLayout
                    || !required.vk12.uniformBufferStandardLayout)
            && (m_deviceFeatures.vk12.shaderSubgroupExtendedTypes
                    || !required.vk12.shaderSubgroupExtendedTypes)
            && (m_deviceFeatures.vk12.separateDepthStencilLayouts
                    || !required.vk12.separateDepthStencilLayouts)
            && (m_deviceFeatures.vk12.hostQueryReset
                    || !required.vk12.hostQueryReset)
            && (m_deviceFeatures.vk12.timelineSemaphore
                    || !required.vk12.timelineSemaphore)
            && (m_deviceFeatures.vk12.bufferDeviceAddress
                    || !required.vk12.bufferDeviceAddress)
            && (m_deviceFeatures.vk12.bufferDeviceAddressCaptureReplay
                    || !required.vk12.bufferDeviceAddressCaptureReplay)
            && (m_deviceFeatures.vk12.bufferDeviceAddressMultiDevice
                    || !required.vk12.bufferDeviceAddressMultiDevice)
            && (m_deviceFeatures.vk12.vulkanMemoryModel
                    || !required.vk12.vulkanMemoryModel)
            && (m_deviceFeatures.vk12.vulkanMemoryModelDeviceScope
                    || !required.vk12.vulkanMemoryModelDeviceScope)
            && (m_deviceFeatures.vk12.vulkanMemoryModelAvailabilityVisibilityChains
                    || !required.vk12.vulkanMemoryModelAvailabilityVisibilityChains)
            && (m_deviceFeatures.vk12.shaderOutputViewportIndex
                    || !required.vk12.shaderOutputViewportIndex)
            && (m_deviceFeatures.vk12.shaderOutputLayer
                    || !required.vk12.shaderOutputLayer)
            && (m_deviceFeatures.vk12.subgroupBroadcastDynamicId
                    || !required.vk12.subgroupBroadcastDynamicId)
			&& (m_deviceFeatures.vk13.robustImageAccess
					|| !required.vk13.robustImageAccess)
			&& (m_deviceFeatures.vk13.inlineUniformBlock
					|| !required.vk13.inlineUniformBlock)
			&& (m_deviceFeatures.vk13.descriptorBindingInlineUniformBlockUpdateAfterBind
					|| !required.vk13.descriptorBindingInlineUniformBlockUpdateAfterBind)
			&& (m_deviceFeatures.vk13.pipelineCreationCacheControl
					|| !required.vk13.pipelineCreationCacheControl)
			&& (m_deviceFeatures.vk13.privateData
					|| !required.vk13.privateData)
			&& (m_deviceFeatures.vk13.shaderDemoteToHelperInvocation
					|| !required.vk13.shaderDemoteToHelperInvocation)
			&& (m_deviceFeatures.vk13.shaderTerminateInvocation
					|| !required.vk13.shaderTerminateInvocation)
			&& (m_deviceFeatures.vk13.subgroupSizeControl
					|| !required.vk13.subgroupSizeControl)
			&& (m_deviceFeatures.vk13.computeFullSubgroups
					|| !required.vk13.computeFullSubgroups)
			&& (m_deviceFeatures.vk13.synchronization2
					|| !required.vk13.synchronization2)
			&& (m_deviceFeatures.vk13.textureCompressionASTC_HDR
					|| !required.vk13.textureCompressionASTC_HDR)
			&& (m_deviceFeatures.vk13.shaderZeroInitializeWorkgroupMemory
					|| !required.vk13.shaderZeroInitializeWorkgroupMemory)
			&& (m_deviceFeatures.vk13.dynamicRendering
					|| !required.vk13.dynamicRendering)
			&& (m_deviceFeatures.vk13.shaderIntegerDotProduct
					|| !required.vk13.shaderIntegerDotProduct)
			&& (m_deviceFeatures.vk13.maintenance4
					|| !required.vk13.maintenance4)
			&& (m_deviceFeatures.ext4444Formats.formatA4R4G4B4
					|| !required.ext4444Formats.formatA4R4G4B4)
			&& (m_deviceFeatures.ext4444Formats.formatA4B4G4R4
					|| !required.ext4444Formats.formatA4B4G4R4)
			&& (m_deviceFeatures.extCustomBorderColor.customBorderColors
					|| !required.extCustomBorderColor.customBorderColors)
			&& (m_deviceFeatures.extCustomBorderColor.customBorderColorWithoutFormat
					|| !required.extCustomBorderColor.customBorderColorWithoutFormat)
			&& (m_deviceFeatures.extDepthClipEnable.depthClipEnable
					|| !required.extDepthClipEnable.depthClipEnable)
			&& (m_deviceFeatures.extExtendedDynamicState.extendedDynamicState
					|| !required.extExtendedDynamicState.extendedDynamicState)
			&& (m_deviceFeatures.extHostQueryReset.hostQueryReset
					|| !required.extHostQueryReset.hostQueryReset)
			&& (m_deviceFeatures.extMemoryPriority.memoryPriority
					|| !required.extMemoryPriority.memoryPriority)
			&& (m_deviceFeatures.extRobustness2.robustBufferAccess2
					|| !required.extRobustness2.robustBufferAccess2)
			&& (m_deviceFeatures.extRobustness2.robustImageAccess2
					|| !required.extRobustness2.robustImageAccess2)
			&& (m_deviceFeatures.extRobustness2.nullDescriptor
					|| !required.extRobustness2.nullDescriptor)
			&& (m_deviceFeatures.extTransformFeedback.transformFeedback
					|| !required.extTransformFeedback.transformFeedback)
			&& (m_deviceFeatures.extVertexAttributeDivisor.vertexAttributeInstanceRateDivisor
					|| !required.extVertexAttributeDivisor.vertexAttributeInstanceRateDivisor)
			&& (m_deviceFeatures.extVertexAttributeDivisor.vertexAttributeInstanceRateZeroDivisor
					|| !required.extVertexAttributeDivisor.vertexAttributeInstanceRateZeroDivisor);
	}
	// clang-format on

	void VltAdapter::enableExtensions(const VltNameSet& extensions)
	{
		m_extraExtensions.merge(extensions);
	}

	void VltAdapter::getRequestFeatures(const VltDeviceExtensions& extensions,
										VltDeviceFeatures&         enabled)
	{
		const VltDeviceFeatures& supported = m_deviceFeatures;

		// Modify and add request features as development goes.

		// Create pNext chain for additional device features
		enabled.core.sType                       = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2_KHR;
		enabled.core.pNext                       = nullptr;
		enabled.core.features.geometryShader     = VK_TRUE;
		enabled.core.features.samplerAnisotropy  = supported.core.features.samplerAnisotropy;
		enabled.core.features.shaderFloat64      = VK_TRUE;
		enabled.core.features.shaderInt64        = VK_TRUE;
		enabled.core.features.tessellationShader = VK_TRUE;
		enabled.core.features.logicOp            = VK_TRUE;
		enabled.core.features.imageCubeArray     = VK_TRUE;

		enabled.vk11.sType                = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES;
		enabled.vk11.pNext                = std::exchange(enabled.core.pNext, &enabled.vk11);
		enabled.vk11.shaderDrawParameters = VK_TRUE;

		enabled.vk13.sType            = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES;
		enabled.vk13.pNext            = std::exchange(enabled.core.pNext, &enabled.vk13);
		enabled.vk13.synchronization2 = VK_TRUE;
		enabled.vk13.dynamicRendering = VK_TRUE;

		if (extensions.extMemoryPriority)
		{
			enabled.extMemoryPriority.sType          = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_PRIORITY_FEATURES_EXT;
			enabled.extMemoryPriority.pNext          = std::exchange(enabled.core.pNext, &enabled.extMemoryPriority);
			enabled.extMemoryPriority.memoryPriority = supported.extMemoryPriority.memoryPriority;
		}

		if (extensions.extDepthClipEnable)
		{
			enabled.extDepthClipEnable.sType           = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEPTH_CLIP_ENABLE_FEATURES_EXT;
			enabled.extDepthClipEnable.pNext           = std::exchange(enabled.core.pNext, &enabled.extDepthClipEnable);
			enabled.extDepthClipEnable.depthClipEnable = VK_TRUE;
		}

	}


	Rc<VltDevice> VltAdapter::createDevice(
		const Rc<VltInstance>& instance)
	{
		VltDeviceExtensions devExtensions;

		std::array<VltExt*, 4> devExtensionList = { {
			&devExtensions.extMemoryBudget,
			&devExtensions.extMemoryPriority,
			&devExtensions.extDepthClipEnable,
			&devExtensions.khrSwapchain,
		} };

		VltNameSet extensionsEnabled;

		if (!m_deviceExtensions.enableExtensions(
				devExtensionList.size(),
				devExtensionList.data(),
				extensionsEnabled))
			Logger::exception("DxvkAdapter: request extensions not all supported.");

		// Enable additional extensions if necessary
		extensionsEnabled.merge(m_extraExtensions);
		VltNameList extensionNameList = extensionsEnabled.toNameList();

		VltDeviceFeatures requestFeatures = {};
		this->getRequestFeatures(devExtensions, requestFeatures);

		if (!checkFeatureSupport(requestFeatures))
		{
			Logger::exception("DxvkAdapter: request features not all supported.");
		}

		Logger::info(util::str::formatex("Device properties:"
										 "\n  Device name:     : ",
										 m_deviceInfo.core.properties.deviceName,
										 "\n  Driver version   : ",
										 VK_API_VERSION_MAJOR(m_deviceInfo.core.properties.driverVersion), ".",
										 VK_API_VERSION_MINOR(m_deviceInfo.core.properties.driverVersion), ".",
										 VK_API_VERSION_PATCH(m_deviceInfo.core.properties.driverVersion)));

		Logger::info("Enabled device extensions:");
		this->logNameList(extensionNameList);
		this->logFeatures(requestFeatures);

		// Create the requested queues
		float                                queuePriority = 1.0f;
		std::vector<VkDeviceQueueCreateInfo> queueInfos;

		std::unordered_set<uint32_t> queueFamiliySet;

		VltAdapterQueueIndices queueFamilies = findQueueFamilies();
		queueFamiliySet.insert(queueFamilies.graphics);
		queueFamiliySet.insert(queueFamilies.compute);
		queueFamiliySet.insert(queueFamilies.transfer);
		this->logQueueFamilies(queueFamilies);

		for (uint32_t family : queueFamiliySet)
		{
			VkDeviceQueueCreateInfo deviceQueue;
			deviceQueue.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			deviceQueue.pNext            = nullptr;
			deviceQueue.flags            = 0;
			deviceQueue.queueFamilyIndex = family;
			deviceQueue.queueCount       = 1;
			deviceQueue.pQueuePriorities = &queuePriority;
			queueInfos.push_back(deviceQueue);
		}

		VkDeviceCreateInfo info;
		info.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		info.pNext                   = requestFeatures.core.pNext;
		info.flags                   = 0;
		info.queueCreateInfoCount    = queueInfos.size();
		info.pQueueCreateInfos       = queueInfos.data();
		info.enabledLayerCount       = 0;
		info.ppEnabledLayerNames     = nullptr;
		info.enabledExtensionCount   = extensionNameList.count();
		info.ppEnabledExtensionNames = extensionNameList.names();
		info.pEnabledFeatures        = &requestFeatures.core.features;

		VkDevice device = VK_NULL_HANDLE;
		VkResult vr     = vkCreateDevice(m_handle, &info, nullptr, &device);

		if (vr != VK_SUCCESS)
			Logger::exception("DxvkAdapter: Failed to create device");

		Rc<VltDevice> result = new VltDevice(
			instance, this, device, devExtensions, requestFeatures);
		return result;
	}

	void VltAdapter::notifyHeapMemoryAlloc(
		uint32_t     heap,
		VkDeviceSize bytes)
	{
		if (!m_hasMemoryBudget)
			m_heapAlloc[heap] += bytes;
	}

	void VltAdapter::notifyHeapMemoryFree(
		uint32_t     heap,
		VkDeviceSize bytes)
	{
		if (!m_hasMemoryBudget)
			m_heapAlloc[heap] -= bytes;
	}

	void VltAdapter::logAdapterInfo() const
	{
		VkPhysicalDeviceProperties       deviceInfo = this->deviceProperties();
		VkPhysicalDeviceMemoryProperties memoryInfo = this->memoryProperties();

		Logger::info(util::str::formatex(deviceInfo.deviceName, ":"));
		Logger::info(util::str::formatex("  Driver: ",
										 VK_VERSION_MAJOR(deviceInfo.driverVersion), ".",
										 VK_VERSION_MINOR(deviceInfo.driverVersion), ".",
										 VK_VERSION_PATCH(deviceInfo.driverVersion)));
		Logger::info(util::str::formatex("  Vulkan: ",
										 VK_VERSION_MAJOR(deviceInfo.apiVersion), ".",
										 VK_VERSION_MINOR(deviceInfo.apiVersion), ".",
										 VK_VERSION_PATCH(deviceInfo.apiVersion)));

		for (uint32_t i = 0; i < memoryInfo.memoryHeapCount; i++)
		{
			constexpr VkDeviceSize mib = 1024 * 1024;

			Logger::info(util::str::formatex("  Memory Heap[", i, "]: "));
			Logger::info(util::str::formatex("    Size: ", memoryInfo.memoryHeaps[i].size / mib, " MiB"));
			Logger::info(util::str::formatex("    Flags: ", "0x", std::hex, memoryInfo.memoryHeaps[i].flags));

			for (uint32_t j = 0; j < memoryInfo.memoryTypeCount; j++)
			{
				if (memoryInfo.memoryTypes[j].heapIndex == i)
				{
					Logger::info(util::str::formatex(
						"    Memory Type[", j, "]: ",
						"Property Flags = ", "0x", std::hex, memoryInfo.memoryTypes[j].propertyFlags));
				}
			}
		}
	}

	bool VltAdapter::isUnifiedMemoryArchitecture() const
	{
		auto memory = this->memoryProperties();
		bool result = true;

		for (uint32_t i = 0; i < memory.memoryHeapCount; i++)
			result &= memory.memoryHeaps[i].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT;

		return result;
	}

	void VltAdapter::initHeapAllocInfo()
	{
		for (uint32_t i = 0; i < m_heapAlloc.size(); i++)
			m_heapAlloc[i] = 0;
	}

	void VltAdapter::queryExtensions()
	{
		m_deviceExtensions = VltNameSet::enumDeviceExtensions(m_handle);
	}

	void VltAdapter::queryDeviceInfo()
	{
		m_deviceInfo            = VltDeviceInfo();
		m_deviceInfo.core.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
		m_deviceInfo.core.pNext = nullptr;

		// Query info now so that we have basic device properties available
		vkGetPhysicalDeviceProperties2(m_handle, &m_deviceInfo.core);

		m_deviceInfo.coreDeviceId.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ID_PROPERTIES;
		m_deviceInfo.coreDeviceId.pNext = std::exchange(m_deviceInfo.core.pNext, &m_deviceInfo.coreDeviceId);

		m_deviceInfo.coreSubgroup.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SUBGROUP_PROPERTIES;
		m_deviceInfo.coreSubgroup.pNext = std::exchange(m_deviceInfo.core.pNext, &m_deviceInfo.coreSubgroup);

		if (m_deviceExtensions.supports(VK_EXT_CONSERVATIVE_RASTERIZATION_EXTENSION_NAME))
		{
			m_deviceInfo.extConservativeRasterization.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CONSERVATIVE_RASTERIZATION_PROPERTIES_EXT;
			m_deviceInfo.extConservativeRasterization.pNext = std::exchange(m_deviceInfo.core.pNext, &m_deviceInfo.extConservativeRasterization);
		}

		if (m_deviceExtensions.supports(VK_EXT_CUSTOM_BORDER_COLOR_EXTENSION_NAME))
		{
			m_deviceInfo.extCustomBorderColor.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CUSTOM_BORDER_COLOR_PROPERTIES_EXT;
			m_deviceInfo.extCustomBorderColor.pNext = std::exchange(m_deviceInfo.core.pNext, &m_deviceInfo.extCustomBorderColor);
		}

		if (m_deviceExtensions.supports(VK_EXT_ROBUSTNESS_2_EXTENSION_NAME))
		{
			m_deviceInfo.extRobustness2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ROBUSTNESS_2_PROPERTIES_EXT;
			m_deviceInfo.extRobustness2.pNext = std::exchange(m_deviceInfo.core.pNext, &m_deviceInfo.extRobustness2);
		}

		if (m_deviceExtensions.supports(VK_EXT_TRANSFORM_FEEDBACK_EXTENSION_NAME))
		{
			m_deviceInfo.extTransformFeedback.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TRANSFORM_FEEDBACK_PROPERTIES_EXT;
			m_deviceInfo.extTransformFeedback.pNext = std::exchange(m_deviceInfo.core.pNext, &m_deviceInfo.extTransformFeedback);
		}

		if (m_deviceExtensions.supports(VK_EXT_VERTEX_ATTRIBUTE_DIVISOR_EXTENSION_NAME))
		{
			m_deviceInfo.extVertexAttributeDivisor.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VERTEX_ATTRIBUTE_DIVISOR_PROPERTIES_EXT;
			m_deviceInfo.extVertexAttributeDivisor.pNext = std::exchange(m_deviceInfo.core.pNext, &m_deviceInfo.extVertexAttributeDivisor);
		}

		if (m_deviceExtensions.supports(VK_KHR_DEPTH_STENCIL_RESOLVE_EXTENSION_NAME))
		{
			m_deviceInfo.khrDepthStencilResolve.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEPTH_STENCIL_RESOLVE_PROPERTIES_KHR;
			m_deviceInfo.khrDepthStencilResolve.pNext = std::exchange(m_deviceInfo.core.pNext, &m_deviceInfo.khrDepthStencilResolve);
		}

		if (m_deviceExtensions.supports(VK_KHR_DRIVER_PROPERTIES_EXTENSION_NAME))
		{
			m_deviceInfo.khrDeviceDriverProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DRIVER_PROPERTIES_KHR;
			m_deviceInfo.khrDeviceDriverProperties.pNext = std::exchange(m_deviceInfo.core.pNext, &m_deviceInfo.khrDeviceDriverProperties);
		}

		if (m_deviceExtensions.supports(VK_KHR_SHADER_FLOAT_CONTROLS_EXTENSION_NAME))
		{
			m_deviceInfo.khrShaderFloatControls.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FLOAT_CONTROLS_PROPERTIES_KHR;
			m_deviceInfo.khrShaderFloatControls.pNext = std::exchange(m_deviceInfo.core.pNext, &m_deviceInfo.khrShaderFloatControls);
		}

		// Query full device properties for all enabled extensions
		vkGetPhysicalDeviceProperties2(m_handle, &m_deviceInfo.core);

		// Some drivers reports the driver version in a slightly different format
		switch (m_deviceInfo.khrDeviceDriverProperties.driverID)
		{
		case VK_DRIVER_ID_NVIDIA_PROPRIETARY:
			m_deviceInfo.core.properties.driverVersion = VK_MAKE_VERSION(
				(m_deviceInfo.core.properties.driverVersion >> 22) & 0x3ff,
				(m_deviceInfo.core.properties.driverVersion >> 14) & 0x0ff,
				(m_deviceInfo.core.properties.driverVersion >> 6) & 0x0ff);
			break;

		case VK_DRIVER_ID_INTEL_PROPRIETARY_WINDOWS:
			m_deviceInfo.core.properties.driverVersion = VK_MAKE_VERSION(
				m_deviceInfo.core.properties.driverVersion >> 14,
				m_deviceInfo.core.properties.driverVersion & 0x3fff, 0);
			break;

		default:;
		}
	}

	void VltAdapter::queryDeviceFeatures()
	{
		m_deviceFeatures            = VltDeviceFeatures();
		m_deviceFeatures.core.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
		m_deviceFeatures.core.pNext = nullptr;

		m_deviceFeatures.vk11.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES;
		m_deviceFeatures.vk11.pNext = std::exchange(m_deviceFeatures.core.pNext, &m_deviceFeatures.vk11);
		m_deviceFeatures.vk12.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES;
		m_deviceFeatures.vk12.pNext = std::exchange(m_deviceFeatures.core.pNext, &m_deviceFeatures.vk12);
		m_deviceFeatures.vk13.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES;
		m_deviceFeatures.vk13.pNext = std::exchange(m_deviceFeatures.core.pNext, &m_deviceFeatures.vk13);

		if (m_deviceExtensions.supports(VK_EXT_4444_FORMATS_EXTENSION_NAME))
		{
			m_deviceFeatures.ext4444Formats.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_4444_FORMATS_FEATURES_EXT;
			m_deviceFeatures.ext4444Formats.pNext = std::exchange(m_deviceFeatures.core.pNext, &m_deviceFeatures.ext4444Formats);
		}

		if (m_deviceExtensions.supports(VK_EXT_CUSTOM_BORDER_COLOR_EXTENSION_NAME))
		{
			m_deviceFeatures.extCustomBorderColor.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_CUSTOM_BORDER_COLOR_FEATURES_EXT;
			m_deviceFeatures.extCustomBorderColor.pNext = std::exchange(m_deviceFeatures.core.pNext, &m_deviceFeatures.extCustomBorderColor);
		}

		if (m_deviceExtensions.supports(VK_EXT_DEPTH_CLIP_ENABLE_EXTENSION_NAME))
		{
			m_deviceFeatures.extDepthClipEnable.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEPTH_CLIP_ENABLE_FEATURES_EXT;
			m_deviceFeatures.extDepthClipEnable.pNext = std::exchange(m_deviceFeatures.core.pNext, &m_deviceFeatures.extDepthClipEnable);
		}

		if (m_deviceExtensions.supports(VK_EXT_EXTENDED_DYNAMIC_STATE_EXTENSION_NAME))
		{
			m_deviceFeatures.extExtendedDynamicState.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_FEATURES_EXT;
			m_deviceFeatures.extExtendedDynamicState.pNext = std::exchange(m_deviceFeatures.core.pNext, &m_deviceFeatures.extExtendedDynamicState);
		}

		if (m_deviceExtensions.supports(VK_EXT_HOST_QUERY_RESET_EXTENSION_NAME))
		{
			m_deviceFeatures.extHostQueryReset.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_HOST_QUERY_RESET_FEATURES_EXT;
			m_deviceFeatures.extHostQueryReset.pNext = std::exchange(m_deviceFeatures.core.pNext, &m_deviceFeatures.extHostQueryReset);
		}

		if (m_deviceExtensions.supports(VK_EXT_MEMORY_PRIORITY_EXTENSION_NAME))
		{
			m_deviceFeatures.extMemoryPriority.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_PRIORITY_FEATURES_EXT;
			m_deviceFeatures.extMemoryPriority.pNext = std::exchange(m_deviceFeatures.core.pNext, &m_deviceFeatures.extMemoryPriority);
		}

		if (m_deviceExtensions.supports(VK_EXT_ROBUSTNESS_2_EXTENSION_NAME))
		{
			m_deviceFeatures.extRobustness2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ROBUSTNESS_2_FEATURES_EXT;
			m_deviceFeatures.extRobustness2.pNext = std::exchange(m_deviceFeatures.core.pNext, &m_deviceFeatures.extRobustness2);
		}

		if (m_deviceExtensions.supports(VK_EXT_SHADER_DEMOTE_TO_HELPER_INVOCATION_EXTENSION_NAME))
		{
			m_deviceFeatures.extShaderDemoteToHelperInvocation.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_DEMOTE_TO_HELPER_INVOCATION_FEATURES_EXT;
			m_deviceFeatures.extShaderDemoteToHelperInvocation.pNext = std::exchange(m_deviceFeatures.core.pNext, &m_deviceFeatures.extShaderDemoteToHelperInvocation);
		}

		if (m_deviceExtensions.supports(VK_EXT_TRANSFORM_FEEDBACK_EXTENSION_NAME))
		{
			m_deviceFeatures.extTransformFeedback.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TRANSFORM_FEEDBACK_FEATURES_EXT;
			m_deviceFeatures.extTransformFeedback.pNext = std::exchange(m_deviceFeatures.core.pNext, &m_deviceFeatures.extTransformFeedback);
		}

		if (m_deviceExtensions.supports(VK_EXT_VERTEX_ATTRIBUTE_DIVISOR_EXTENSION_NAME) >= 3)
		{
			m_deviceFeatures.extVertexAttributeDivisor.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VERTEX_ATTRIBUTE_DIVISOR_FEATURES_EXT;
			m_deviceFeatures.extVertexAttributeDivisor.pNext = std::exchange(m_deviceFeatures.core.pNext, &m_deviceFeatures.extVertexAttributeDivisor);
		}

		if (m_deviceExtensions.supports(VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME))
		{
			m_deviceFeatures.khrBufferDeviceAddress.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_DEVICE_ADDRESS_FEATURES_KHR;
			m_deviceFeatures.khrBufferDeviceAddress.pNext = std::exchange(m_deviceFeatures.core.pNext, &m_deviceFeatures.khrBufferDeviceAddress);
		}

		vkGetPhysicalDeviceFeatures2(m_handle, &m_deviceFeatures.core);
	}

	void VltAdapter::queryDeviceQueues()
	{
		uint32_t numQueueFamilies = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(
			m_handle, &numQueueFamilies, nullptr);

		m_queueFamilies.resize(numQueueFamilies);
		vkGetPhysicalDeviceQueueFamilyProperties(
			m_handle, &numQueueFamilies, m_queueFamilies.data());
	}

	uint32_t VltAdapter::findQueueFamily(
		VkQueueFlags mask,
		VkQueueFlags flags) const
	{
		for (uint32_t i = 0; i < m_queueFamilies.size(); i++)
		{
			if ((m_queueFamilies[i].queueFlags & mask) == flags)
				return i;
		}

		return VK_QUEUE_FAMILY_IGNORED;
	}

	void VltAdapter::logNameList(const VltNameList& names)
	{
		for (uint32_t i = 0; i < names.count(); i++)
			Logger::info(util::str::formatex("  ", names.name(i)));
	}

	void VltAdapter::logFeatures(const VltDeviceFeatures& features)
	{
		Logger::info(util::str::formatex("Device features:",
										 "\n  robustBufferAccess                     : ", features.core.features.robustBufferAccess ? "1" : "0",
										 "\n  fullDrawIndexUint32                    : ", features.core.features.fullDrawIndexUint32 ? "1" : "0",
										 "\n  imageCubeArray                         : ", features.core.features.imageCubeArray ? "1" : "0",
										 "\n  independentBlend                       : ", features.core.features.independentBlend ? "1" : "0",
										 "\n  geometryShader                         : ", features.core.features.geometryShader ? "1" : "0",
										 "\n  tessellationShader                     : ", features.core.features.tessellationShader ? "1" : "0",
										 "\n  sampleRateShading                      : ", features.core.features.sampleRateShading ? "1" : "0",
										 "\n  dualSrcBlend                           : ", features.core.features.dualSrcBlend ? "1" : "0",
										 "\n  logicOp                                : ", features.core.features.logicOp ? "1" : "0",
										 "\n  multiDrawIndirect                      : ", features.core.features.multiDrawIndirect ? "1" : "0",
										 "\n  drawIndirectFirstInstance              : ", features.core.features.drawIndirectFirstInstance ? "1" : "0",
										 "\n  depthClamp                             : ", features.core.features.depthClamp ? "1" : "0",
										 "\n  depthBiasClamp                         : ", features.core.features.depthBiasClamp ? "1" : "0",
										 "\n  fillModeNonSolid                       : ", features.core.features.fillModeNonSolid ? "1" : "0",
										 "\n  depthBounds                            : ", features.core.features.depthBounds ? "1" : "0",
										 "\n  multiViewport                          : ", features.core.features.multiViewport ? "1" : "0",
										 "\n  samplerAnisotropy                      : ", features.core.features.samplerAnisotropy ? "1" : "0",
										 "\n  textureCompressionBC                   : ", features.core.features.textureCompressionBC ? "1" : "0",
										 "\n  occlusionQueryPrecise                  : ", features.core.features.occlusionQueryPrecise ? "1" : "0",
										 "\n  pipelineStatisticsQuery                : ", features.core.features.pipelineStatisticsQuery ? "1" : "0",
										 "\n  vertexPipelineStoresAndAtomics         : ", features.core.features.vertexPipelineStoresAndAtomics ? "1" : "0",
										 "\n  fragmentStoresAndAtomics               : ", features.core.features.fragmentStoresAndAtomics ? "1" : "0",
										 "\n  shaderImageGatherExtended              : ", features.core.features.shaderImageGatherExtended ? "1" : "0",
										 "\n  shaderStorageImageExtendedFormats      : ", features.core.features.shaderStorageImageExtendedFormats ? "1" : "0",
										 "\n  shaderStorageImageReadWithoutFormat    : ", features.core.features.shaderStorageImageReadWithoutFormat ? "1" : "0",
										 "\n  shaderStorageImageWriteWithoutFormat   : ", features.core.features.shaderStorageImageWriteWithoutFormat ? "1" : "0",
										 "\n  shaderClipDistance                     : ", features.core.features.shaderClipDistance ? "1" : "0",
										 "\n  shaderCullDistance                     : ", features.core.features.shaderCullDistance ? "1" : "0",
										 "\n  shaderFloat64                          : ", features.core.features.shaderFloat64 ? "1" : "0",
										 "\n  shaderInt64                            : ", features.core.features.shaderInt64 ? "1" : "0",
										 "\n  variableMultisampleRate                : ", features.core.features.variableMultisampleRate ? "1" : "0",
										 "\n", VK_EXT_4444_FORMATS_EXTENSION_NAME,
										 "\n  formatA4R4G4B4                         : ", features.ext4444Formats.formatA4R4G4B4 ? "1" : "0",
										 "\n  formatA4B4G4R4                         : ", features.ext4444Formats.formatA4B4G4R4 ? "1" : "0",
										 "\n", VK_EXT_CUSTOM_BORDER_COLOR_EXTENSION_NAME,
										 "\n  customBorderColors                     : ", features.extCustomBorderColor.customBorderColors ? "1" : "0",
										 "\n  customBorderColorWithoutFormat         : ", features.extCustomBorderColor.customBorderColorWithoutFormat ? "1" : "0",
										 "\n", VK_EXT_DEPTH_CLIP_ENABLE_EXTENSION_NAME,
										 "\n  depthClipEnable                        : ", features.extDepthClipEnable.depthClipEnable ? "1" : "0",
										 "\n", VK_EXT_EXTENDED_DYNAMIC_STATE_EXTENSION_NAME,
										 "\n  extendedDynamicState                   : ", features.extExtendedDynamicState.extendedDynamicState ? "1" : "0",
										 "\n", VK_EXT_HOST_QUERY_RESET_EXTENSION_NAME,
										 "\n  hostQueryReset                         : ", features.extHostQueryReset.hostQueryReset ? "1" : "0",
										 "\n", VK_EXT_MEMORY_PRIORITY_EXTENSION_NAME,
										 "\n  memoryPriority                         : ", features.extMemoryPriority.memoryPriority ? "1" : "0",
										 "\n", VK_EXT_ROBUSTNESS_2_EXTENSION_NAME,
										 "\n  robustBufferAccess2                    : ", features.extRobustness2.robustBufferAccess2 ? "1" : "0",
										 "\n  robustImageAccess2                     : ", features.extRobustness2.robustImageAccess2 ? "1" : "0",
										 "\n  nullDescriptor                         : ", features.extRobustness2.nullDescriptor ? "1" : "0",
										 "\n", VK_EXT_SHADER_DEMOTE_TO_HELPER_INVOCATION_EXTENSION_NAME,
										 "\n  shaderDemoteToHelperInvocation         : ", features.extShaderDemoteToHelperInvocation.shaderDemoteToHelperInvocation ? "1" : "0",
										 "\n", VK_EXT_TRANSFORM_FEEDBACK_EXTENSION_NAME,
										 "\n  transformFeedback                      : ", features.extTransformFeedback.transformFeedback ? "1" : "0",
										 "\n  geometryStreams                        : ", features.extTransformFeedback.geometryStreams ? "1" : "0",
										 "\n", VK_EXT_VERTEX_ATTRIBUTE_DIVISOR_EXTENSION_NAME,
										 "\n  vertexAttributeInstanceRateDivisor     : ", features.extVertexAttributeDivisor.vertexAttributeInstanceRateDivisor ? "1" : "0",
										 "\n  vertexAttributeInstanceRateZeroDivisor : ", features.extVertexAttributeDivisor.vertexAttributeInstanceRateZeroDivisor ? "1" : "0",
										 "\n", VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME,
										 "\n  bufferDeviceAddress                    : ", features.khrBufferDeviceAddress.bufferDeviceAddress));
	}

	void VltAdapter::logQueueFamilies(const VltAdapterQueueIndices& queues)
	{
		Logger::info(util::str::formatex("Queue families:",
										 "\n  Graphics : ", queues.graphics,
										 "\n  Compute  : ", queues.compute,
										 "\n  Transfer : ", queues.transfer));
	}


}  // namespace sce::vlt