#pragma once

#include "GnmCommandBuffer.h"


class GnmCommandBufferDispatch : GnmCommandBuffer
{
public:
	GnmCommandBufferDispatch(const RcPtr<gve::GveDevice>& device,
		RcPtr<gve::GveContex>& context);
	virtual ~GnmCommandBufferDispatch();


private:
	
};


