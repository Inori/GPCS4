#pragma once

#include "GveCommon.h"
#include "GveCommandBuffer.h"
#include "GveFrameBuffer.h"

namespace gve
{;

class GveDevice;

// This is our render context.
// Just like GfxContext in PS4, one GveContex should be bound to one display buffer.

class GveContex
{
public:
	GveContex(const RcPtr<GveDevice>& device);
	~GveContex();

private:
	RcPtr<GveDevice> m_device;
	RcPtr<GveCommandBuffer> m_command;
	RcPtr<GveFrameBuffer> m_frameBuffer;
};


} // namespace gve
