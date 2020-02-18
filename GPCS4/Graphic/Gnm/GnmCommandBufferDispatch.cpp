#include "GnmCommandBufferDispatch.h"

#include "../Gve/GveDevice.h"
#include "../Gve/GveContext.h"
#include "../Gve/GveSwapChain.h"

GnmCommandBufferDispatch::GnmCommandBufferDispatch(
	const RcPtr<gve::GveDevice>&             device,
	const RcPtr<gve::GveContext>&            context,
	const RcPtr<gve::GveSwapChain>&          swapchain,
	const std::shared_ptr<sce::SceVideoOut>& videoOut) :
	GnmCommandBuffer(device, context, swapchain, videoOut)
{
}

GnmCommandBufferDispatch::~GnmCommandBufferDispatch()
{
}