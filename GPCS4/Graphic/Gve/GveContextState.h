#pragma once

#include "GveCommon.h"

namespace gve
{;
	


/**
 * \brief Graphics pipeline state flags
 *
 * Stores some information on which state
 * of the graphics and compute pipelines
 * has changed and/or needs to be updated.
 */
enum class GveContextFlag : uint32_t
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

using GveContextFlags = Flags<GveContextFlag>;

//////////////////////////////////////////////////////////////////////////

struct GveOutputMergerState
{
	GveRenderTargets renderTargets;
	RcPtr<GveFrameBuffer> framebuffer = nullptr;
};

struct GveVertexInputState
{
	RcPtr<GveBuffer> indexBuffer;
	VkIndexType      indexType = VK_INDEX_TYPE_UINT32;

	std::array<RcPtr<GveBuffer>, MaxNumVertexBindings> vertexBuffers = { };
	std::array<uint32_t,         MaxNumVertexBindings> vertexStrides = { };
};

//////////////////////////////////////////////////////////////////////////
struct GveGraphicsPipelineState
{
	GveGraphicsPipelineShaders   shaders;
	GveGraphicsPipelineStateInfo states;
	GveGraphicsPipeline*         pipeline = nullptr;
};

struct GveComputePipelineState
{
	GveComputePipelineStateInfo state;
};

//////////////////////////////////////////////////////////////////////////
struct GveContextState
{
	GveVertexInputState		vi;
	GveOutputMergerState	om;

	GveGraphicsPipelineState	gp;
	GveComputePipelineState		cp;
};


}  // namespace gve
