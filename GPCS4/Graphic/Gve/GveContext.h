#pragma once

#include "GveCommon.h"

namespace gve
{;

class GveDevice;

// This is our render context.
// Just like GfxContext in PS4, one GveContex should be bound to one display buffer.

class GveContex
{
public:
	GveContex(RcPtr<GveDevice>& device);
	~GveContex();

private:
	RcPtr<GveDevice> m_device;
};


} // namespace gve
