#include "VltDevice.h"
#include "VltCmdList.h"
#include "VltDescriptor.h"
#include "VltBuffer.h"
#include "VltImage.h"
#include "VltSampler.h"
#include "VltContext.h"
#include "VltFrameBuffer.h"
#include "VltPhysicalDevice.h"
#include "VltPresenter.h"

#include "../Sce/SceVideoOut.h"

namespace vlt
{;


VltDevice::VltDevice(
	VkDevice                        device,
	const RcPtr<VltPhysicalDevice>& phyDevice) :
	m_device(device),
	m_phyDevice(phyDevice),
	m_properties(phyDevice->devicePropertiesExt()),
	m_memAllocator(this),
	m_resObjects(this),
	m_submissionQueue(this)
{
	initQueues();
}

VltDevice::~VltDevice()
{
	vkDestroyDevice(m_device, nullptr);
}

VltDevice::operator VkDevice() const
{
	return m_device;
}

RcPtr<VltPhysicalDevice> VltDevice::physicalDevice() const
{
	return m_phyDevice;
}

VltDeviceQueueSet VltDevice::queues() const
{
	return m_queues;
}

const VltDeviceFeatures& VltDevice::features() const
{
	return m_phyDevice->features();
}

VkPipelineStageFlags VltDevice::getShaderPipelineStages() const
{
	VkPipelineStageFlags result = 
		VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT | 
		VK_PIPELINE_STAGE_VERTEX_SHADER_BIT | 
		VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;

	auto& features = m_phyDevice->features();
	if (features.core.features.geometryShader)
	{
		result |= VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT;
	}
		
	if (features.core.features.tessellationShader)
	{
		result |= 
			VK_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT | 
			VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT;
	}

	return result;
}

RcPtr<VltFrameBuffer> VltDevice::createFrameBuffer(const VltRenderTargets& renderTargets)
{
	auto rpFormat = VltFrameBuffer::getRenderPassFormat(renderTargets);
	auto renderPass = m_resObjects.renderPassPool().getRenderPass(rpFormat);

	const VltFramebufferSize defaultSize = 
	{
		m_properties.core.properties.limits.maxFramebufferWidth,
		m_properties.core.properties.limits.maxFramebufferHeight,
		m_properties.core.properties.limits.maxFramebufferLayers
	};

	return new VltFrameBuffer(this, renderTargets, renderPass, defaultSize);
}

RcPtr<VltCmdList> VltDevice::createCmdList(VltPipelineType pipelineType)
{
	RcPtr<VltCmdList> cmdList = m_recycledCmdLists.retrieveObject();
	if (cmdList == nullptr)
	{
		cmdList = new VltCmdList(this, pipelineType);
	}
	return cmdList;
}

RcPtr<VltContext> VltDevice::createContext()
{
	return new VltContext(this);
}

RcPtr<VltDescriptorPool> VltDevice::createDescriptorPool()
{
	RcPtr<VltDescriptorPool> pool = m_recycledDescriptorPools.retrieveObject();
	if (pool == nullptr)
	{
		pool = new VltDescriptorPool(this);
	}
	return pool;
}

RcPtr<VltBuffer> VltDevice::createBuffer(const VltBufferCreateInfo& info, VkMemoryPropertyFlags memoryType)
{
	return new VltBuffer(this, info, &m_memAllocator, memoryType);
}

RcPtr<VltImage> VltDevice::createImage(const VltImageCreateInfo& info, VkMemoryPropertyFlags memoryType)
{
	return new VltImage(this, info, &m_memAllocator, memoryType);
}

RcPtr<VltImageView> VltDevice::createImageView(const RcPtr<VltImage>& image, const VltImageViewCreateInfo& createInfo)
{
	return new VltImageView(this, createInfo, image);
}

RcPtr<VltSampler> VltDevice::createSampler(const VltSamplerCreateInfo& info)
{
	return new VltSampler(this, info);
}

void VltDevice::submitCommandList(const VltSubmitInfo& submission)
{
	m_submissionQueue.submit(submission);
}

void VltDevice::presentImage(const VltPresentInfo& presentation)
{
	m_submissionQueue.present(presentation);
}

bool VltDevice::hasDedicatedTransferQueue() const
{
	return m_queues.transfer.queueFamily != m_queues.graphics.queueFamily;
}

void VltDevice::recycleDescriptorPool(const RcPtr<VltDescriptorPool>& pool)
{
	m_recycledDescriptorPools.returnObject(pool);
}

void VltDevice::recycleCommandList(const RcPtr<VltCmdList>& cmdList)
{
	m_recycledCmdLists.returnObject(cmdList);
}

void VltDevice::initQueues()
{
	VltPhysicalDeviceQueueFamilies families = m_phyDevice->findQueueFamilies();

	m_queues.graphics = getQueue(families.graphics, 0);
	m_queues.compute  = getQueue(families.compute, 0);
	m_queues.transfer = getQueue(families.transfer, 0);
}

VltDeviceQueue VltDevice::getQueue(uint32_t family, uint32_t index)
{
	VltDeviceQueue queue = {};
	queue.queueFamily    = family;
	queue.queueIndex     = index;
	vkGetDeviceQueue(m_device, queue.queueFamily, queue.queueIndex, &queue.queueHandle);
	return queue;
}

} // namespace vlt