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

private:


private:
	const RcPtr<GveDevice>& m_device;
	
};


}  // namespace gve
