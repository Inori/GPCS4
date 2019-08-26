#pragma once

#include "GveCommon.h"

namespace gve
{;

class GveSwapChain
{
public:
	GveSwapChain(VkSurfaceKHR windowSurface);
	~GveSwapChain();

private:
	VkSurfaceKHR m_windowSurface;
};


}  // namespace gve
