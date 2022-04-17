#include "VltDevice.h"

#include "VltInstance.h"

#include "Sce/ScePresenter.h"
#include "SpirV/SpirvCodeBuffer.h"

using namespace sce::gcn;

namespace sce::vlt
{

	VltDevice::VltDevice(
		const Rc<VltInstance>&     instance,
		const Rc<VltAdapter>&      adapter,
		VkDevice                   device,
		const VltDeviceExtensions& extensions,
		const VltDeviceFeatures&   features) :
		m_device(device),
		m_instance(instance),
		m_adapter(adapter),
		m_extensions(extensions),
		m_features(features),
		m_properties(adapter->devicePropertiesExt()),
		m_objects(this),
		m_submissionQueue(this)
	{
		auto queueFamilies = m_adapter->findQueueFamilies();
		m_queues.graphics  = getQueue(queueFamilies.graphics, 0);
		m_queues.compute   = getQueue(queueFamilies.compute, 0);
		m_queues.transfer  = getQueue(queueFamilies.transfer, 0);
	}

	VltDevice::~VltDevice()
	{
		// Wait for all pending Vulkan commands to be
		// executed before we destroy any resources.
		this->waitForIdle();
	}

	bool VltDevice::isUnifiedMemoryArchitecture() const
	{
		return m_adapter->isUnifiedMemoryArchitecture();
	}

	VkPipelineStageFlags VltDevice::getShaderPipelineStages() const
	{
		VkPipelineStageFlags result = 
			VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT |
			VK_PIPELINE_STAGE_VERTEX_SHADER_BIT | 
			VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;

		if (m_features.core.features.geometryShader)
			result |= VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT;

		if (m_features.core.features.tessellationShader)
		{
			result |= 
				VK_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT | 
				VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT;
		}

		return result;
	}

	void VltDevice::waitForIdle()
	{
		if (vkDeviceWaitIdle(m_device) != VK_SUCCESS)
			Logger::err("DxvkDevice: waitForIdle: Operation failed");
	}

	VltDeviceQueue VltDevice::getQueue(
		uint32_t family,
		uint32_t index) const
	{
		VkQueue queue = VK_NULL_HANDLE;
		vkGetDeviceQueue(m_device, family, index, &queue);
		return VltDeviceQueue{ queue, family, index };
	}

	Rc<VltFramebuffer> VltDevice::createFramebuffer(
		const VltRenderTargets& renderTargets)
	{
		const VltFramebufferSize defaultSize = {
			m_properties.core.properties.limits.maxFramebufferWidth,
			m_properties.core.properties.limits.maxFramebufferHeight,
			m_properties.core.properties.limits.maxFramebufferLayers
		};

		return new VltFramebuffer(renderTargets, defaultSize);
	}

	Rc<VltBuffer> VltDevice::createBuffer(
		const VltBufferCreateInfo& createInfo, 
		VkMemoryPropertyFlags memoryType)
	{
		return new VltBuffer(this, createInfo, m_objects.memoryManager(), memoryType);
	}

	Rc<VltBufferView> VltDevice::createBufferView(
		const Rc<VltBuffer>& buffer, 
		const VltBufferViewCreateInfo& createInfo)
	{
		return new VltBufferView(this, buffer, createInfo);
	}

	Rc<VltImage> VltDevice::createImage(
		const VltImageCreateInfo& createInfo, 
		VkMemoryPropertyFlags memoryType)
	{
		return new VltImage(this, createInfo, m_objects.memoryManager(), memoryType);
	}

	Rc<VltImage> VltDevice::createImageFromVkImage(
		const VltImageCreateInfo& createInfo, 
		VkImage image)
	{
		return new VltImage(this, createInfo, image);
	}

	Rc<VltImageView> VltDevice::createImageView(
		const Rc<VltImage>& image, 
		const VltImageViewCreateInfo& createInfo)
	{
		return new VltImageView(this, image, createInfo);
	}

	Rc<VltSampler> VltDevice::createSampler(
		const VltSamplerCreateInfo& createInfo)
	{
		return new VltSampler(this, createInfo);
	}

	Rc<VltShader> VltDevice::createShader(
		VkShaderStageFlagBits    stage,
		uint32_t                 slotCount,
		const VltResourceSlot*   slotInfos,
		const VltInterfaceSlots& iface,
		const SpirvCodeBuffer&   code)
	{
		return new VltShader(stage,
							 std::vector<VltResourceSlot>(slotInfos, slotInfos + slotCount),
							 iface, code,
							 VltShaderOptions(),
							 VltShaderConstData());
	}

	Rc<VltCommandList> VltDevice::createCommandList()
	{
		Rc<VltCommandList> cmdList = m_recycledCommandLists.retrieveObject();

		if (cmdList == nullptr)
		{
			cmdList = new VltCommandList(this);
		}
		
		return cmdList;
	}

	Rc<VltDescriptorPool> VltDevice::createDescriptorPool()
	{
		Rc<VltDescriptorPool> pool = m_recycledDescriptorPools.retrieveObject();

		if (pool == nullptr)
			pool = new VltDescriptorPool(this);

		return pool;
	}

	Rc<VltContext> VltDevice::createContext()
	{
		return new VltContext(this);
	}
	
	Rc<VltGpuEvent> VltDevice::createGpuEvent()
	{
		auto handle = m_objects.eventPool().allocEvent();
		return new VltGpuEvent(this, handle);
	}

	void VltDevice::submitCommandList(
		const Rc<VltCommandList>& commandList,
		VkSemaphore               waitSync,
		VkSemaphore               wakeSync)
	{
		VltSubmitInfo submitInfo;
		submitInfo.cmdList  = commandList;
		submitInfo.waitSync = waitSync;
		submitInfo.wakeSync = wakeSync;
		m_submissionQueue.submit(submitInfo);
	}

	void VltDevice::presentImage(
		const Rc<sce::ScePresenter>& presenter)
	{
		VltPresentInfo presentInfo;
		presentInfo.presenter = presenter;
		m_submissionQueue.present(presentInfo);
	}

	void VltDevice::syncSubmission()
	{
		m_submissionQueue.synchronize();
	}

	void VltDevice::recycleCommandList(
		const Rc<VltCommandList>& cmdList)
	{
		m_recycledCommandLists.returnObject(cmdList);
	}

	void VltDevice::recycleDescriptorPool(
		const Rc<VltDescriptorPool>& pool)
	{
		m_recycledDescriptorPools.returnObject(pool);
	}




}  // namespace sce::vlt