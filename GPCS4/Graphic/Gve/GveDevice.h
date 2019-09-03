#pragma once

#include "GveCommon.h"
#include "GvePhysicalDevice.h"
#include "GveRenderPass.h"
#include "GveFrameBuffer.h"
#include "GveContext.h"

namespace gve
{;

class GveDevice : public RcObject
{
public:
	GveDevice(VkDevice device, RcPtr<GvePhysicalDevice>& phyDevice);
	~GveDevice();

	operator VkDevice() const;

	RcPtr<GvePhysicalDevice> getPhysicalDevice();

	RcPtr<GveRenderPass> createRenderPass(GveRenderPassFormat& format);

	RcPtr<GveFrameBuffer> createFrameBuffer(VkRenderPass renderPass, VkImageView imageView, VkExtent2D& extent);

	RcPtr<GveCommandBuffer> createCommandBuffer();

	RcPtr<GveContex> createContext();

private:
	VkDevice m_device;
	RcPtr<GvePhysicalDevice> m_phyDevice;
};

} // namespace gve
