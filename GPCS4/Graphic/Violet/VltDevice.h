#pragma once

#include "VltCommon.h"
#include "VltDeviceInfo.h"
#include "VltResourceObjects.h"
#include "VltRecycler.h"
#include "VltEnums.h"
#include "VltSubmissionQueue.h"


namespace vlt
{;

struct VltRenderTargets;
struct VltImageCreateInfo;
struct VltImageViewCreateInfo;
struct VltSamplerCreateInfo;
struct VltSubmitInfo;
struct VltPresentInfo;

class VltCmdList;
class VltDescriptorPool;
class VltBuffer;
class VltImage;
class VltImageView;
class VltSampler;
class VltContext;
class VltFrameBuffer;
class VltPhysicalDevice;

struct VltDeviceQueue 
{
	VkQueue   queueHandle = VK_NULL_HANDLE;
	uint32_t  queueFamily = 0;
	uint32_t  queueIndex = 0;
};

struct VltDeviceQueueSet 
{
	VltDeviceQueue graphics;
	VltDeviceQueue compute;
	VltDeviceQueue transfer;
};

class VltDevice : public RcObject
{
	friend class VltContext;
	friend class VltDescriptorPoolTracker;
	friend class VltSubmissionQueue;

public:
	VltDevice(
		VkDevice                        device,
		const RcPtr<VltPhysicalDevice>& phyDevice);
	~VltDevice();

	operator VkDevice() const;

	RcPtr<VltPhysicalDevice> physicalDevice() const;

	VltDeviceQueueSet queues() const;

	const VltDeviceFeatures& features() const;

	VkPipelineStageFlags getShaderPipelineStages() const;

	RcPtr<VltFrameBuffer> createFrameBuffer(const VltRenderTargets& renderTargets);

	RcPtr<VltCmdList> createCmdList(VltPipelineType pipelineType);

	RcPtr<VltContext> createContext();

	RcPtr<VltDescriptorPool> createDescriptorPool();

	/// Resource creation methods.

	RcPtr<VltBuffer> createBuffer(const VltBufferCreateInfo& info, VkMemoryPropertyFlags memoryType);

	RcPtr<VltImage> createImage(const VltImageCreateInfo& info, VkMemoryPropertyFlags memoryType);

	RcPtr<VltImageView> createImageView(const RcPtr<VltImage>& image, const VltImageViewCreateInfo& createInfo);

	RcPtr<VltSampler> createSampler(const VltSamplerCreateInfo& info);

	void submitCommandList(const VltSubmitInfo& submission);

	void presentImage(const VltPresentInfo& presentation);

	bool hasDedicatedTransferQueue() const;

private:
	void recycleDescriptorPool(const RcPtr<VltDescriptorPool>& pool);
	void recycleCommandList(const RcPtr<VltCmdList>& cmdList);

private:
	void initQueues();
	VltDeviceQueue getQueue(uint32_t family, uint32_t index);

private:
	VkDevice                 m_device;
	RcPtr<VltPhysicalDevice> m_phyDevice;

	VltDeviceQueueSet m_queues;
	VltDeviceInfo     m_properties;

	VltMemoryAllocator m_memAllocator;
	VltResourceObjects m_resObjects;

	VltSubmissionQueue m_submissionQueue;

	VltRecycler<VltDescriptorPool, 16> m_recycledDescriptorPools;
	VltRecycler<VltCmdList, 16>        m_recycledCmdLists;
	
};

} // namespace vlt
