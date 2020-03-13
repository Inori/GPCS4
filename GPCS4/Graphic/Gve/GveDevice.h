#pragma once

#include "GveCommon.h"
#include "GveDeviceInfo.h"
#include "GveResourceObjects.h"
#include "GveRecycler.h"
#include "GveEnums.h"
#include "GveSubmissionQueue.h"


namespace gve
{;

struct GveRenderTargets;
struct GveImageCreateInfo;
struct GveImageViewCreateInfo;
struct GveSamplerCreateInfo;
struct GveSubmitInfo;
struct GvePresentInfo;

class GveCmdList;
class GveDescriptorPool;
class GveBuffer;
class GveImage;
class GveImageView;
class GveSampler;
class GveContext;
class GveFrameBuffer;
class GvePhysicalDevice;

struct GveDeviceQueue 
{
	VkQueue   queueHandle = VK_NULL_HANDLE;
	uint32_t  queueFamily = 0;
	uint32_t  queueIndex = 0;
};

struct GveDeviceQueueSet 
{
	GveDeviceQueue graphics;
	GveDeviceQueue compute;
	GveDeviceQueue transfer;
};

class GveDevice : public RcObject
{
	friend class GveContext;
	friend class GveDescriptorPoolTracker;
	friend class GveSubmissionQueue;

public:
	GveDevice(
		VkDevice                        device,
		const RcPtr<GvePhysicalDevice>& phyDevice);
	~GveDevice();

	operator VkDevice() const;

	RcPtr<GvePhysicalDevice> physicalDevice() const;

	GveDeviceQueueSet queues() const;

	const GveDeviceFeatures& features() const;

	VkPipelineStageFlags getShaderPipelineStages() const;

	RcPtr<GveFrameBuffer> createFrameBuffer(const GveRenderTargets& renderTargets);

	RcPtr<GveCmdList> createCmdList(GvePipelineType pipelineType);

	RcPtr<GveContext> createContext();

	RcPtr<GveDescriptorPool> createDescriptorPool();

	/// Resource creation methods.

	RcPtr<GveBuffer> createBuffer(const GveBufferCreateInfo& info, VkMemoryPropertyFlags memoryType);

	RcPtr<GveImage> createImage(const GveImageCreateInfo& info, VkMemoryPropertyFlags memoryType);

	RcPtr<GveImageView> createImageView(const RcPtr<GveImage>& image, const GveImageViewCreateInfo& createInfo);

	RcPtr<GveSampler> createSampler(const GveSamplerCreateInfo& info);

	void submitCommandList(const GveSubmitInfo& submission);

	void presentImage(const GvePresentInfo& presentation);

	bool hasDedicatedTransferQueue() const;

private:
	void recycleDescriptorPool(const RcPtr<GveDescriptorPool>& pool);
	void recycleCommandList(const RcPtr<GveCmdList>& cmdList);

private:
	void initQueues();
	GveDeviceQueue getQueue(uint32_t family, uint32_t index);

private:
	VkDevice                 m_device;
	RcPtr<GvePhysicalDevice> m_phyDevice;

	GveDeviceQueueSet m_queues;
	GveDeviceInfo     m_properties;

	GveMemoryAllocator m_memAllocator;
	GveResourceObjects m_resObjects;

	GveSubmissionQueue m_submissionQueue;

	GveRecycler<GveDescriptorPool, 16> m_recycledDescriptorPools;
	GveRecycler<GveCmdList, 16>        m_recycledCmdLists;
	
};

} // namespace gve
