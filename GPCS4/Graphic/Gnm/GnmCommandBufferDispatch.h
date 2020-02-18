#pragma once

#include "GnmCommandBuffer.h"


class GnmCommandBufferDispatch : GnmCommandBuffer
{
public:
	GnmCommandBufferDispatch(
		const RcPtr<gve::GveDevice>&             device,
		const RcPtr<gve::GveContext>&            context,
		const RcPtr<gve::GveSwapChain>&          swapchain,
		const std::shared_ptr<sce::SceVideoOut>& videoOut
	);

	virtual ~GnmCommandBufferDispatch();


private:
	
};


