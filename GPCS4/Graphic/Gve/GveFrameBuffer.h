#pragma once

#include "GveCommon.h"
#include "GveDevice.h"

namespace gve
{;


class GveFrameBuffer
{
public:
	GveFrameBuffer(RcPtr<GveDevice>& device, 
		VkRenderPass renderPass,
		VkImageView swapChainImageView,
		VkExtent2D& swapChainExtent);
	~GveFrameBuffer();

	VkFramebuffer handle() const;

	VkRenderPass renderPassHandle();

private:
	bool createFrameBuffer(VkImageView imageView, VkExtent2D& extent);

private:
	VkFramebuffer m_frameBuffer;
	VkRenderPass m_renderPass;
	RcPtr<GveDevice> m_device;
};



} // namespace gve
