#pragma once

#include "GveCommon.h"
#include "GveDevice.h"

namespace gve
{;


// Maybe need to update in the future.
struct GveRenderPassFormat
{
	VkFormat swapChainImageFormat;
};


class GveRenderPass
{
public:
	GveRenderPass(
		RcPtr<GveDevice>& device,
		GveRenderPassFormat& fmt);
	~GveRenderPass();

	VkRenderPass handle() const;

private:
	void createRenderPass(GveRenderPassFormat& fmt);

private:
	RcPtr<GveDevice> m_device;
	VkRenderPass m_renderPass;
};


} // namespace gve
