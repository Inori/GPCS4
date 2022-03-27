#include "VltDevice.h"
#include "VltInstance.h"

namespace sce::vlt
{

	VltDevice::VltDevice(
		const Rc<VltInstance>&     instance,
		const Rc<VltAdapter>&      adapter,
		const VltDeviceExtensions& extensions,
		const VltDeviceFeatures&   features) :
		m_instance(instance),
		m_adapter(adapter),
		m_extensions(extensions),
		m_features(features),
		m_properties(adapter->devicePropertiesExt())
		//m_objects(this),
		//m_submissionQueue(this)
	{
		auto queueFamilies = m_adapter->findQueueFamilies();
		m_queues.graphics  = getQueue(queueFamilies.graphics, 0);
		m_queues.transfer  = getQueue(queueFamilies.transfer, 0);
	}

	VltDevice::~VltDevice()
	{
		// Wait for all pending Vulkan commands to be
		// executed before we destroy any resources.
		this->waitForIdle();

		// Stop workers explicitly in order to prevent
		// access to structures that are being destroyed.
		//m_objects.pipelineManager().stopWorkerThreads();
	}

	bool VltDevice::isUnifiedMemoryArchitecture() const
	{
		return m_adapter->isUnifiedMemoryArchitecture();
	}

	//DxvkFramebufferSize VltDevice::getDefaultFramebufferSize() const
	//{
	//	return DxvkFramebufferSize{
	//		m_properties.core.properties.limits.maxFramebufferWidth,
	//		m_properties.core.properties.limits.maxFramebufferHeight,
	//		m_properties.core.properties.limits.maxFramebufferLayers
	//	};
	//}

	VkPipelineStageFlags VltDevice::getShaderPipelineStages() const
	{
		VkPipelineStageFlags result = VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT | VK_PIPELINE_STAGE_VERTEX_SHADER_BIT | VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;

		if (m_features.core.features.geometryShader)
			result |= VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT;

		if (m_features.core.features.tessellationShader)
		{
			result |= VK_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT | VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT;
		}

		return result;
	}

	VltDeviceOptions VltDevice::options() const
	{
		VltDeviceOptions options;
		options.maxNumDynamicUniformBuffers = m_properties.core.properties.limits.maxDescriptorSetUniformBuffersDynamic;
		options.maxNumDynamicStorageBuffers = m_properties.core.properties.limits.maxDescriptorSetStorageBuffersDynamic;
		return options;
	}

	//Rc<DxvkCommandList> VltDevice::createCommandList()
	//{
	//	Rc<DxvkCommandList> cmdList = m_recycledCommandLists.retrieveObject();

	//	if (cmdList == nullptr)
	//		cmdList = new DxvkCommandList(this);

	//	return cmdList;
	//}

	//Rc<DxvkDescriptorPool> VltDevice::createDescriptorPool()
	//{
	//	Rc<DxvkDescriptorPool> pool = m_recycledDescriptorPools.retrieveObject();

	//	if (pool == nullptr)
	//		pool = new DxvkDescriptorPool(m_vkd);

	//	return pool;
	//}

	//Rc<DxvkContext> VltDevice::createContext()
	//{
	//	return new DxvkContext(this);
	//}

	//Rc<DxvkGpuEvent> VltDevice::createGpuEvent()
	//{
	//	return new DxvkGpuEvent(m_vkd);
	//}

	//Rc<DxvkGpuQuery> VltDevice::createGpuQuery(
	//	VkQueryType         type,
	//	VkQueryControlFlags flags,
	//	uint32_t            index)
	//{
	//	return new DxvkGpuQuery(m_vkd, type, flags, index);
	//}

	//Rc<DxvkFramebuffer> VltDevice::createFramebuffer(
	//	const DxvkFramebufferInfo& info)
	//{
	//	return new DxvkFramebuffer(m_vkd, info);
	//}

	//Rc<DxvkBuffer> VltDevice::createBuffer(
	//	const DxvkBufferCreateInfo& createInfo,
	//	VkMemoryPropertyFlags       memoryType)
	//{
	//	return new DxvkBuffer(this, createInfo, m_objects.memoryManager(), memoryType);
	//}

	//Rc<DxvkBufferView> VltDevice::createBufferView(
	//	const Rc<DxvkBuffer>&           buffer,
	//	const DxvkBufferViewCreateInfo& createInfo)
	//{
	//	return new DxvkBufferView(m_vkd, buffer, createInfo);
	//}

	//Rc<DxvkImage> VltDevice::createImage(
	//	const DxvkImageCreateInfo& createInfo,
	//	VkMemoryPropertyFlags      memoryType)
	//{
	//	return new DxvkImage(m_vkd, createInfo, m_objects.memoryManager(), memoryType);
	//}

	//Rc<DxvkImage> VltDevice::createImageFromVkImage(
	//	const DxvkImageCreateInfo& createInfo,
	//	VkImage                    image)
	//{
	//	return new DxvkImage(m_vkd, createInfo, image);
	//}

	//Rc<DxvkImageView> VltDevice::createImageView(
	//	const Rc<DxvkImage>&           image,
	//	const DxvkImageViewCreateInfo& createInfo)
	//{
	//	return new DxvkImageView(m_vkd, image, createInfo);
	//}

	//Rc<DxvkSampler> VltDevice::createSampler(
	//	const DxvkSamplerCreateInfo& createInfo)
	//{
	//	return new DxvkSampler(this, createInfo);
	//}

	//Rc<DxvkShader> VltDevice::createShader(
	//	VkShaderStageFlagBits     stage,
	//	uint32_t                  slotCount,
	//	const DxvkResourceSlot*   slotInfos,
	//	const DxvkInterfaceSlots& iface,
	//	const SpirvCodeBuffer&    code)
	//{
	//	return new DxvkShader(stage,
	//						  slotCount, slotInfos, iface, code,
	//						  DxvkShaderOptions(),
	//						  DxvkShaderConstData());
	//}

	//DxvkStatCounters VltDevice::getStatCounters()
	//{
	//	DxvkPipelineCount pipe = m_objects.pipelineManager().getPipelineCount();

	//	DxvkStatCounters result;
	//	result.setCtr(DxvkStatCounter::PipeCountGraphics, pipe.numGraphicsPipelines);
	//	result.setCtr(DxvkStatCounter::PipeCountCompute, pipe.numComputePipelines);
	//	result.setCtr(DxvkStatCounter::PipeCompilerBusy, m_objects.pipelineManager().isCompilingShaders());
	//	result.setCtr(DxvkStatCounter::GpuIdleTicks, m_submissionQueue.gpuIdleTicks());

	//	std::lock_guard<sync::Spinlock> lock(m_statLock);
	//	result.merge(m_statCounters);
	//	return result;
	//}

	//DxvkMemoryStats VltDevice::getMemoryStats(uint32_t heap)
	//{
	//	return m_objects.memoryManager().getMemoryStats(heap);
	//}

	//uint32_t VltDevice::getCurrentFrameId() const
	//{
	//	return m_statCounters.getCtr(DxvkStatCounter::QueuePresentCount);
	//}

	void VltDevice::initResources()
	{
		//m_objects.dummyResources().clearResources(this);
	}

	//void VltDevice::registerShader(const Rc<DxvkShader>& shader)
	//{
	//	m_objects.pipelineManager().registerShader(shader);
	//}

	//void VltDevice::presentImage(
	//	const Rc<vk::Presenter>& presenter,
	//	DxvkSubmitStatus*        status)
	//{
	//	status->result = VK_NOT_READY;

	//	DxvkPresentInfo presentInfo;
	//	presentInfo.presenter = presenter;
	//	m_submissionQueue.present(presentInfo, status);

	//	std::lock_guard<sync::Spinlock> statLock(m_statLock);
	//	m_statCounters.addCtr(DxvkStatCounter::QueuePresentCount, 1);
	//}

	//void VltDevice::submitCommandList(
	//	const Rc<DxvkCommandList>& commandList,
	//	VkSemaphore                waitSync,
	//	VkSemaphore                wakeSync)
	//{
	//	DxvkSubmitInfo submitInfo;
	//	submitInfo.cmdList  = commandList;
	//	submitInfo.waitSync = waitSync;
	//	submitInfo.wakeSync = wakeSync;
	//	m_submissionQueue.submit(submitInfo);

	//	std::lock_guard<sync::Spinlock> statLock(m_statLock);
	//	m_statCounters.merge(commandList->statCounters());
	//	m_statCounters.addCtr(DxvkStatCounter::QueueSubmitCount, 1);
	//}

	//VkResult VltDevice::waitForSubmission(DxvkSubmitStatus* status)
	//{
	//	VkResult result = status->result.load();

	//	if (result == VK_NOT_READY)
	//	{
	//		m_submissionQueue.synchronizeSubmission(status);
	//		result = status->result.load();
	//	}

	//	return result;
	//}

	//void VltDevice::waitForResource(const Rc<DxvkResource>& resource, DxvkAccess access)
	//{
	//	if (resource->isInUse(access))
	//	{
	//		auto t0 = dxvk::high_resolution_clock::now();

	//		m_submissionQueue.synchronizeUntil([resource, access]
	//										   { return !resource->isInUse(access); });

	//		auto t1 = dxvk::high_resolution_clock::now();
	//		auto us = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0);

	//		std::lock_guard<sync::Spinlock> lock(m_statLock);
	//		m_statCounters.addCtr(DxvkStatCounter::GpuSyncCount, 1);
	//		m_statCounters.addCtr(DxvkStatCounter::GpuSyncTicks, us.count());
	//	}
	//}

	void VltDevice::waitForIdle()
	{
		//this->lockSubmission();
		if (vkDeviceWaitIdle(m_device) != VK_SUCCESS)
			Logger::err("DxvkDevice: waitForIdle: Operation failed");
		//this->unlockSubmission();
	}

	//void VltDevice::recycleCommandList(const Rc<DxvkCommandList>& cmdList)
	//{
	//	m_recycledCommandLists.returnObject(cmdList);
	//}

	//void VltDevice::recycleDescriptorPool(const Rc<DxvkDescriptorPool>& pool)
	//{
	//	m_recycledDescriptorPools.returnObject(pool);
	//}

	VltDeviceQueue VltDevice::getQueue(
		uint32_t family,
		uint32_t index) const
	{
		VkQueue queue = VK_NULL_HANDLE;
		vkGetDeviceQueue(m_device, family, index, &queue);
		return VltDeviceQueue{ queue, family, index };
	}

}  // namespace sce::vlt