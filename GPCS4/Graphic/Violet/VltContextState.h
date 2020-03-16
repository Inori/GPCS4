#pragma once

#include "VltCommon.h"
#include "VltFrameBuffer.h"
#include "VltGraphicsPipeline.h"


namespace vlt
{;
	
/**
 * \brief Graphics pipeline state flags
 *
 * Stores some information on which state
 * of the graphics and compute pipelines
 * has changed and/or needs to be updated.
 */
enum class VltContextFlag : uint32_t
{
	GpRenderPassBound,          ///< Render pass is currently bound
	GpCondActive,               ///< Conditional rendering is enabled
	GpXfbActive,                ///< Transform feedback is enabled
	GpClearRenderTargets,       ///< Render targets need to be cleared
	GpDirtyFramebuffer,         ///< Framebuffer binding is out of date
	GpDirtyPipeline,            ///< Graphics pipeline binding is out of date, shader resource layout changed
	GpDirtyPipelineState,       ///< Graphics PSOs changed, thus needs to be recompiled
	GpDirtyResources,           ///< Graphics pipeline resource bindings are out of date
	GpDirtyDescriptorBinding,   ///< Graphics descriptor set needs to be rebound
	GpDirtyVertexBuffers,       ///< Vertex buffer bindings are out of date
	GpDirtyIndexBuffer,         ///< Index buffer binding are out of date
	GpDirtyXfbBuffers,          ///< Transform feedback buffer bindings are out of date
	GpDirtyXfbCounters,         ///< Counter buffer values are dirty
	GpDirtyBlendConstants,      ///< Blend constants have changed
	GpDirtyDepthBias,           ///< Depth bias has changed
	GpDirtyDepthBounds,         ///< Depth bounds have changed
	GpDirtyStencilRef,          ///< Stencil reference has changed
	GpDirtyViewport,            ///< Viewport state has changed
	GpDirtyPredicate,           ///< Predicate has changed
	GpDynamicBlendConstants,    ///< Blend constants are dynamic
	GpDynamicDepthBias,         ///< Depth bias is dynamic
	GpDynamicDepthBounds,       ///< Depth bounds are dynamic
	GpDynamicStencilRef,        ///< Stencil reference is dynamic

	CpDirtyPipeline,            ///< Compute pipeline binding are out of date
	CpDirtyPipelineState,       ///< Compute pipeline needs to be recompiled
	CpDirtyResources,           ///< Compute pipeline resource bindings are out of date
	CpDirtyDescriptorBinding,   ///< Compute descriptor set needs to be rebound

	DirtyDrawBuffer,            ///< Indirect argument buffer is dirty
	DirtyPushConstants,         ///< Push constant data has changed
};

using VltContextFlags = Flags<VltContextFlag>;

//////////////////////////////////////////////////////////////////////////

struct VltOutputMergerState
{
	VltRenderPassOps      renderPassOps;
	VltRenderTargets      renderTargets;
	RcPtr<VltFrameBuffer> framebuffer = nullptr;

	std::array<VkClearValue, MaxNumRenderTargets> clearValues = {};
};

struct VltVertexInputState
{
	VltBufferSlice indexBuffer;
	VkIndexType    indexType = VK_INDEX_TYPE_UINT32;

	std::array<VltBufferSlice, MaxNumVertexBindings> vertexBuffers = {};
	std::array<uint32_t, MaxNumVertexBindings>       vertexStrides = {};
};

struct VltViewPortState
{
	uint32_t                                count;
	std::array<VkViewport, MaxNumViewports> viewports;
	std::array<VkRect2D, MaxNumViewports>   scissors;
};

struct VltDynamicState
{
	VltViewPortState vp;
};

//////////////////////////////////////////////////////////////////////////
struct VltGraphicsPipelineState
{
	VltGraphicsPipelineShaders   shaders;
	VltGraphicsPipelineStateInfo states;
	VltGraphicsPipeline*         pipeline = nullptr;
};

struct VltComputePipelineState
{
	VltComputePipelineStateInfo state;
};

//////////////////////////////////////////////////////////////////////////
struct VltContextState
{
	VltVertexInputState  vi;
	VltDynamicState      dy;
	VltOutputMergerState om;

	VltGraphicsPipelineState gp;
	VltComputePipelineState  cp;
};


}  // namespace vlt
