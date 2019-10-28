#include "GnmCommandBufferDispatch.h"

GnmCommandBufferDispatch::GnmCommandBufferDispatch(const RcPtr<gve::GveDevice>& device,
	RcPtr<gve::GveContex>& context) :
	GnmCommandBuffer(device, context)
{
}

GnmCommandBufferDispatch::~GnmCommandBufferDispatch()
{
}