#pragma once

#include "GveCommon.h"
#include "GveDevice.h"

namespace gve
{;

class GveFrameBuffer;

struct GveRenderTarget
{
	RcPtr<GveFrameBuffer> frameBuffer;
};


class GveFrameBuffer : public RcObject
{
	friend class GveContex;
public:
	GveFrameBuffer(const RcPtr<GveDevice>& device, 
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
