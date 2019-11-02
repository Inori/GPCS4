#pragma once

#include "GveCommon.h"


namespace gve
{;

class GveDevice;
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
		VkImageView imageView,
		VkExtent2D& extent);
	~GveFrameBuffer();

	VkFramebuffer handle() const;

	VkExtent2D extent() const;

	VkRenderPass renderPassHandle();

private:
	bool createFrameBuffer(VkImageView imageView);

private:
	RcPtr<GveDevice> m_device;
	VkRenderPass m_renderPass;
	VkExtent2D m_extent;

	VkFramebuffer m_frameBuffer;
};



} // namespace gve
