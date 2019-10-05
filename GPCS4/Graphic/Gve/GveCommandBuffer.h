#pragma once

#include "GveCommon.h"
#include "GveDevice.h"

namespace gve
{;

class GveCommandBuffer : public RcObject
{
public:
	GveCommandBuffer(const RcPtr<GveDevice>& device);
	~GveCommandBuffer();

	void beginRecording();

	void endRecording();

private:
	bool initCommandBuffer();

private:
	RcPtr<GveDevice> m_device;
	VkCommandPool m_pool = VK_NULL_HANDLE;
	VkCommandBuffer m_cmd = VK_NULL_HANDLE;
};


}  // namespace gve
