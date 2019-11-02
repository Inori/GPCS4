#pragma once

#include "GveCommon.h"
#include "GvePhysicalDevice.h"
#include "GveRenderPass.h"
#include "GveFrameBuffer.h"
#include "GveContext.h"
#include "GveMemory.h"

namespace gve
{;

class GveCommandBuffer;
class GveDescriptorPool;
class GveBuffer;
class GveImage;

struct GveDeviceQueue 
{
	VkQueue   queueHandle = VK_NULL_HANDLE;
	uint32_t  queueFamily = 0;
	uint32_t  queueIndex = 0;
};

struct GveDeviceQueueSet 
{
	GveDeviceQueue graphics;
};


class GveDevice : public RcObject
{
public:
	GveDevice(VkDevice device, const RcPtr<GvePhysicalDevice>& phyDevice);
	~GveDevice();

	operator VkDevice() const;

	RcPtr<GvePhysicalDevice> physicalDevice() const;

	GveDeviceQueueSet queues() const;

	RcPtr<GveRenderPass> createRenderPass(GveRenderPassFormat& format);

	RcPtr<GveFrameBuffer> createFrameBuffer(VkRenderPass renderPass, VkImageView imageView, VkExtent2D& extent);

	RcPtr<GveCommandBuffer> createCommandBuffer();

	RcPtr<GveContex> createContext(const GveContextParam& param);

	RcPtr<GveDescriptorPool> createDescriptorPool();

private:
	void initQueues();

private:
	VkDevice m_device;
	RcPtr<GvePhysicalDevice> m_phyDevice;
	GveDeviceQueueSet m_queues;
	
	
};

} // namespace gve
