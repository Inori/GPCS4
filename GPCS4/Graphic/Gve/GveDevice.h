#pragma once

#include "GveCommon.h"
#include "GvePhysicalDevice.h"
#include "GveRenderPass.h"
#include "GveFrameBuffer.h"
#include "GveContext.h"
#include "GveCommandBuffer.h"

namespace gve
{;

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

	RcPtr<GvePhysicalDevice> getPhysicalDevice();

	RcPtr<GveRenderPass> createRenderPass(GveRenderPassFormat& format);

	RcPtr<GveFrameBuffer> createFrameBuffer(VkRenderPass renderPass, VkImageView imageView, VkExtent2D& extent);

	RcPtr<GveCommandBuffer> createCommandBuffer();

	RcPtr<GveContex> createContext();

private:
	void initQueues();

private:
	VkDevice m_device;
	RcPtr<GvePhysicalDevice> m_phyDevice;
	GveDeviceQueueSet m_queues;
};

} // namespace gve
