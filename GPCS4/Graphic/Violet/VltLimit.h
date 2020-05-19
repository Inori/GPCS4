#pragma once

#include "VltCommon.h"

namespace vlt 
{;

enum VltLimits : size_t 
{
	MaxNumRenderTargets = 8,
	MaxNumVertexAttributes = 32,
	MaxNumVertexBindings = 32,
	MaxNumXfbBuffers = 4,
	MaxNumXfbStreams = 4,
	MaxNumViewports = 16,
	MaxNumResourceSlots = 1216,
	MaxNumActiveBindings = 128,
	MaxNumQueuedCommandBuffers = 12,
	MaxNumQueryCountPerPool = 128,
	MaxNumSpecConstants = 12,
	MaxUniformBufferSize = 65536,
	MaxVertexBindingStride = 2048,
	MaxPushConstantSize = 128,
};

}