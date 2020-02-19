#include "GnmCommandBufferDispatch.h"

#include "../Gve/GveDevice.h"
#include "../Gve/GveContext.h"

GnmCommandBufferDispatch::GnmCommandBufferDispatch(
	const sce::SceGpuQueueDevice& device,
	const RcPtr<gve::GveContext>& context) :
	GnmCommandBuffer(device, context)
{
}

GnmCommandBufferDispatch::~GnmCommandBufferDispatch()
{
}