#pragma once

#include "GveCommon.h"

namespace gve
{;

class GveDevice;

// Maybe need to update in the future.
struct GveRenderPassFormat
{
	VkFormat colorFormat;
};


class GveRenderPass : public RcObject
{
public:
	GveRenderPass(
		const RcPtr<GveDevice>& device,
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
