#include "GnmCommandBufferDispatch.h"

GnmCommandBufferDispatch::GnmCommandBufferDispatch(const RcPtr<gve::GveDevice>& device,
	RcPtr<gve::GveContex>& context,
	gve::GveResourceManager* resMgr) :
	GnmCommandBuffer(device, context, resMgr)
{
}

GnmCommandBufferDispatch::~GnmCommandBufferDispatch()
{
}