#include "GveDevice.h"
#include "GveCmdList.h"
#include "GveDescriptor.h"
#include "GveBuffer.h"
#include "GveImage.h"
#include "GveSampler.h"
#include "GveContext.h"
#include "GveFrameBuffer.h"
#include "GvePhysicalDevice.h"
#include "GvePresenter.h"

#include "../Sce/SceVideoOut.h"

namespace gve
{;


GveDevice::GveDevice(
	VkDevice                        device,
	const RcPtr<GvePhysicalDevice>& phyDevice) :
	m_device(device),
	m_phyDevice(phyDevice),
	m_properties(phyDevice->devicePropertiesExt()),
	m_memAllocator(this),
	m_resObjects(this),
	m_submissionQueue(this)
{
	initQueues();
}

GveDevice::~GveDevice()
{
	vkDestroyDevice(m_device, nullptr);
}

GveDevice::operator VkDevice() const
{
	return m_device;
}

RcPtr<GvePhysicalDevice> GveDevice::physicalDevice() const
{
	return m_phyDevice;
}

GveDeviceQueueSet GveDevice::queues() const
{
	return m_queues;
}

const GveDeviceFeatures& GveDevice::features() const
{
	return m_phyDevice->features();
}

VkPipelineStageFlags GveDevice::getShaderPipelineStages() const
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

RcPtr<GveFrameBuffer> GveDevice::createFrameBuffer(const GveRenderTargets& renderTargets)
{
	auto rpFormat = GveFrameBuffer::getRenderPassFormat(renderTargets);
	auto renderPass = m_resObjects.renderPassPool().getRenderPass(rpFormat);

	const GveFramebufferSize defaultSize = 
	{
		m_properties.core.properties.limits.maxFramebufferWidth,
		m_properties.core.properties.limits.maxFramebufferHeight,
		m_properties.core.properties.limits.maxFramebufferLayers
	};

	return new GveFrameBuffer(this, renderTargets, renderPass, defaultSize);
}

RcPtr<GveCmdList> GveDevice::createCmdList(GvePipelineType pipelineType)
{
	RcPtr<GveCmdList> cmdList = m_recycledCmdLists.retrieveObject();
	if (cmdList == nullptr)
	{
		cmdList = new GveCmdList(this, pipelineType);
	}
	return cmdList;
}

RcPtr<GveContext> GveDevice::createContext()
{
	return new GveContext(this);
}

RcPtr<GveDescriptorPool> GveDevice::createDescriptorPool()
{
	RcPtr<GveDescriptorPool> pool = m_recycledDescriptorPools.retrieveObject();
	if (pool == nullptr)
	{
		pool = new GveDescriptorPool(this);
	}
	return pool;
}

RcPtr<GveBuffer> GveDevice::createBuffer(const GveBufferCreateInfo& info, VkMemoryPropertyFlags memoryType)
{
	return new GveBuffer(this, info, &m_memAllocator, memoryType);
}

RcPtr<GveImage> GveDevice::createImage(const GveImageCreateInfo& info, VkMemoryPropertyFlags memoryType)
{
	return new GveImage(this, info, &m_memAllocator, memoryType);
}

RcPtr<GveImageView> GveDevice::createImageView(const RcPtr<GveImage>& image, const GveImageViewCreateInfo& createInfo)
{
	return new GveImageView(this, createInfo, image);
}

RcPtr<GveSampler> GveDevice::createSampler(const GveSamplerCreateInfo& info)
{
	return new GveSampler(this, info);
}

void GveDevice::submitCommandList(const GveSubmitInfo& submission)
{
	m_submissionQueue.submit(submission);
}

void GveDevice::presentImage(const GvePresentInfo& presentation)
{
	m_submissionQueue.present(presentation);
}

bool GveDevice::hasDedicatedTransferQueue() const
{
	return m_queues.transfer.queueFamily != m_queues.graphics.queueFamily;
}

void GveDevice::recycleDescriptorPool(const RcPtr<GveDescriptorPool>& pool)
{
	m_recycledDescriptorPools.returnObject(pool);
}

void GveDevice::recycleCommandList(const RcPtr<GveCmdList>& cmdList)
{
	m_recycledCmdLists.returnObject(cmdList);
}

void GveDevice::initQueues()
{
	GvePhysicalDeviceQueueFamilies families = m_phyDevice->findQueueFamilies();

	m_queues.graphics = getQueue(families.graphics, 0);
	m_queues.compute  = getQueue(families.compute, 0);
	m_queues.transfer = getQueue(families.transfer, 0);
}

GveDeviceQueue GveDevice::getQueue(uint32_t family, uint32_t index)
{
	GveDeviceQueue queue = {};
	queue.queueFamily    = family;
	queue.queueIndex     = index;
	vkGetDeviceQueue(m_device, queue.queueFamily, queue.queueIndex, &queue.queueHandle);
	return queue;
}

} // namespace gve