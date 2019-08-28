#pragma once

#include "GveCommon.h"

namespace gve
{;

class GveDevice : public RcObject
{
public:
	GveDevice(VkDevice device);
	~GveDevice();

private:
	VkDevice m_device;
};

} // namespace gve
