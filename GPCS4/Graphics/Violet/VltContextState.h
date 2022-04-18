#pragma once

#include "VltBuffer.h"
#include "VltCommon.h"
#include "VltCompute.h"
#include "VltConstantState.h"
#include "VltGraphics.h"
#include "VltLimit.h"
#include "VltFramebuffer.h"

namespace sce::vlt
{
	/**
     * \brief Graphics pipeline state flags
     * 
     * Stores some information on which state
     * of the graphics and compute pipelines
     * has changed and/or needs to be updated.
     */
	enum class VltContextFlag : uint32_t
	{
		GpRenderingActive,         ///< Dynamic rendering (render pass instance) has began
		GpCondActive,              ///< Conditional rendering is enabled
		GpXfbActive,               ///< Transform feedback is enabled
		GpClearRenderTargets,      ///< Render targets need to be cleared
		GpDirtyFramebuffer,        ///< Framebuffer binding is out of date
		GpDirtyPipeline,           ///< Graphics pipeline binding is out of date
		GpDirtyPipelineState,      ///< Graphics pipeline needs to be recompiled
		GpDirtyResources,          ///< Graphics pipeline resource bindings are out of date
		GpDirtyDescriptorBinding,  ///< Graphics descriptor set needs to be rebound
		GpDirtyVertexBuffers,      ///< Vertex buffer bindings are out of date
		GpDirtyIndexBuffer,        ///< Index buffer binding are out of date
		GpDirtyXfbBuffers,         ///< Transform feedback buffer bindings are out of date
		GpDirtyXfbCounters,        ///< Counter buffer values are dirty
		GpDirtyBlendConstants,     ///< Blend constants have changed
		GpDirtyDepthBias,          ///< Depth bias has changed
		GpDirtyDepthBounds,        ///< Depth bounds have changed
		GpDirtyStencilRef,         ///< Stencil reference has changed
		GpDirtyViewport,           ///< Viewport has changed
		GpDirtyScissor,            ///< Scissor has changed
		GpDirtyPredicate,          ///< Predicate has changed
		GpDynamicBlendConstants,   ///< Blend constants are dynamic
		GpDynamicDepthBias,        ///< Depth bias is dynamic
		GpDynamicDepthBounds,      ///< Depth bounds are dynamic
		GpDynamicStencilRef,       ///< Stencil reference is dynamic

		CpDirtyPipeline,           ///< Compute pipeline binding are out of date
		CpDirtyPipelineState,      ///< Compute pipeline needs to be recompiled
		CpDirtyResources,          ///< Compute pipeline resource bindings are out of date
		CpDirtyDescriptorBinding,  ///< Compute descriptor set needs to be rebound

		DirtyDrawBuffer,     ///< Indirect argument buffer is dirty
		DirtyPushConstants,  ///< Push constant data has changed
	};

	using VltContextFlags = util::Flags<VltContextFlag>;

	/**
     * \brief Barrier control flags
     * 
     * These flags specify what (not) to
     * synchronize implicitly.
     */
	enum class VltBarrierControl : uint32_t
	{
		IgnoreWriteAfterWrite = 1,
	};

	using VltBarrierControlFlags = util::Flags<VltBarrierControl>;

	struct VltIndirectDrawState
	{
		VltBufferSlice argBuffer;
		VltBufferSlice cntBuffer;
	};

	struct VltVertexInputState
	{
		VltBufferSlice indexBuffer;
		VkIndexType    indexType = VK_INDEX_TYPE_UINT32;

		std::array<VltBufferSlice, VltLimits::MaxNumVertexBindings> vertexBuffers = {};
		std::array<uint32_t, VltLimits::MaxNumVertexBindings>       vertexStrides = {};
	};

	struct VltViewportState
	{
		std::array<VkViewport, VltLimits::MaxNumViewports> viewports    = {};
		std::array<VkRect2D, VltLimits::MaxNumViewports>   scissorRects = {};
	};

	struct VltColorBlendState
	{
		std::array<VkClearValue, MaxNumRenderTargets> colorClearValues = {};
		VkClearValue                                  depthClearValue;
		VltRenderTargets                              renderTargets;
		Rc<VltFramebuffer>                            framebuffer = nullptr;
	};

	struct VltPushConstantState
	{
		uint8_t data[MaxPushConstantSize];
	};

	struct VltXfbState
	{
		std::array<VltBufferSlice, MaxNumXfbBuffers> buffers;
		std::array<VltBufferSlice, MaxNumXfbBuffers> counters;
	};

	struct VltGraphicsPipelineState
	{
		VltGraphicsPipelineShaders   shaders;
		VltGraphicsPipelineStateInfo state;
		VltGraphicsPipelineFlags     flags;
		VltGraphicsPipeline*         pipeline = nullptr;
	};

	struct VltComputePipelineState
	{
		VltComputePipelineShaders   shaders;
		VltComputePipelineStateInfo state;
		VltComputePipeline*         pipeline = nullptr;
	};

	struct VltDynamicState
	{
		VltBlendConstants blendConstants   = { 0.0f, 0.0f, 0.0f, 0.0f };
		VltDepthBias      depthBias        = { 0.0f, 0.0f, 0.0f };
		VltDepthBounds    depthBounds      = { false, 0.0f, 1.0f };
		uint32_t          stencilReference = 0;
	};

	struct VltCondRenderState
	{
		VltBufferSlice                 predicate;
		VkConditionalRenderingFlagsEXT flags;
	};

	/**
     * \brief Pipeline state
     * 
     * Stores all bound shaders, resources,
     * and constant pipeline state objects.
     */
	struct VltContextState
	{
		VltIndirectDrawState id;
		VltVertexInputState  vi;
		VltViewportState     vp;
		VltColorBlendState   cb;
		VltPushConstantState pc;
		VltXfbState          xfb;
		VltDynamicState      dyn;
		VltCondRenderState   cond;

		VltGraphicsPipelineState gp;
		VltComputePipelineState  cp;
	};

}  // namespace sce::vlt