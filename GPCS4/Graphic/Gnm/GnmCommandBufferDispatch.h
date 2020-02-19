#pragma once

#include "GnmCommandBuffer.h"


class GnmCommandBufferDispatch : public GnmCommandBuffer
{
public:
	GnmCommandBufferDispatch(
		const sce::SceGpuQueueDevice& device,
		const RcPtr<gve::GveContext>& context
	);

	virtual ~GnmCommandBufferDispatch();


private:
	
};


