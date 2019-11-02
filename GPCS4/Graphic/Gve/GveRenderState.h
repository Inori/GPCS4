#pragma once

#include "GveCommon.h"
#include "FlagHelper.h"
#include "GveLimit.h"
#include "GveBuffer.h"

#include <array>
#include <vector>

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
	GpDirtyPipeline,            ///< Graphics pipeline binding is out of date
	GpDirtyPipelineState,       ///< Graphics pipeline needs to be recompiled
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


  /**
   * \brief Blend constants
   *
   * Stores a blend factor
   * as an RGBA color value.
   */
struct GveBlendConstants 
{
	float r, g, b, a;

	bool operator == (const GveBlendConstants& other) const 
	{
		return this->r == other.r && this->g == other.g
			&& this->b == other.b && this->a == other.a;
	}

	bool operator != (const GveBlendConstants& other) const 
	{
		return this->r != other.r || this->g != other.g
			|| this->b != other.b || this->a != other.a;
	}
};


/**
 * \brief Depth bias
 *
 * Stores depth bias values.
 */
struct GveDepthBias 
{
	float               depthBiasConstant;
	float               depthBiasSlope;
	float               depthBiasClamp;

	bool operator == (const GveDepthBias& other) const 
	{
		return depthBiasConstant == other.depthBiasConstant
			&& depthBiasSlope == other.depthBiasSlope
			&& depthBiasClamp == other.depthBiasClamp;
	}

	bool operator != (const GveDepthBias& other) const 
	{
		return depthBiasConstant != other.depthBiasConstant
			|| depthBiasSlope != other.depthBiasSlope
			|| depthBiasClamp != other.depthBiasClamp;
	}
};


/**
 * \brief Depth bounds
 *
 * Stores depth bounds values.
 */
struct GveDepthBounds 
{
	VkBool32            enableDepthBounds;
	float               minDepthBounds;
	float               maxDepthBounds;

	bool operator == (const GveDepthBounds& other) const 
	{
		return enableDepthBounds == other.enableDepthBounds
			&& minDepthBounds == other.minDepthBounds
			&& maxDepthBounds == other.maxDepthBounds;
	}

	bool operator != (const GveDepthBounds& other) const 
	{
		return enableDepthBounds != other.enableDepthBounds
			|| minDepthBounds != other.minDepthBounds
			|| maxDepthBounds != other.maxDepthBounds;
	}
};


/**
 * \brief Input assembly state
 *
 * Stores the primitive topology and
 * whether or not primitive restart
 * is enabled.
 */
struct GveInputAssemblyState 
{
	VkPrimitiveTopology primitiveTopology;
	VkBool32            primitiveRestart;
	uint32_t            patchVertexCount;
};


/**
 * \brief Rasterizer state
 *
 * Stores the operating mode of the
 * rasterizer, including the depth bias.
 */
struct GveRasterizerState 
{
	VkPolygonMode       polygonMode;
	VkCullModeFlags     cullMode;
	VkFrontFace         frontFace;
	VkBool32            depthClampEnable;
	VkBool32            depthBiasEnable;
};


/**
 * \brief Multisample state
 *
 * Defines how to handle certain
 * aspects of multisampling.
 */
struct GveMultisampleState 
{
	uint32_t            sampleCount;
	VkBool32            sampleShadingEnable;
};


/**
 * \brief Depth-stencil state
 *
 * Defines the depth test and stencil
 * operations for the graphics pipeline.
 */
struct GveDepthStencilState 
{
	VkBool32            enableDepthTest;
	VkBool32            enableDepthWrite;
	VkBool32            enableStencilTest;
	VkCompareOp         depthCompareOp;
	VkStencilOpState    stencilOpFront;
	VkStencilOpState    stencilOpBack;
};


/**
 * \brief Logic op state
 * Defines a logic op.
 */
struct GveLogicOpState 
{
	VkBool32  enableLogicOp;
	VkLogicOp logicOp;
};


/**
 * \brief Blend control for a single attachment
 *
 * Stores the blend state for a single color attachment.
 * Blend modes can be set separately for each attachment.
 */
struct GveBlendControl
{
	VkBool32              enableBlending;
	VkBlendFactor         colorSrcFactor;
	VkBlendFactor         colorDstFactor;
	VkBlendOp             colorBlendOp;
	VkBlendFactor         alphaSrcFactor;
	VkBlendFactor         alphaDstFactor;
	VkBlendOp             alphaBlendOp;
	VkColorComponentFlags writeMask;
};


/**
 * \brief Vertex attribute description
 *
 * Stores information about a
 * single vertex attribute.
 */
struct GveVertexAttribute 
{
	uint32_t location;
	uint32_t binding;
	VkFormat format;
	uint32_t offset;
};


/**
 * \brief Vertex binding description
 *
 * Stores information about a
 * single vertex binding slot.
 */
struct GveVertexBinding 
{
	uint32_t          binding;
	uint32_t          fetchRate;
	VkVertexInputRate inputRate;
};


struct GveVertexInputState
{
	RcPtr<GveBuffer> indexBuffer;
	VkIndexType indexType = VK_INDEX_TYPE_UINT32;

	std::array<RcPtr<GveBuffer>, GveLimits::MaxNumVertexBindings> vertexBuffers = { };
	std::array<uint32_t, GveLimits::MaxNumVertexBindings> vertexStrides = { };

	VkPipelineVertexInputStateCreateInfo state;
};

/**
 * \brief Input layout
 *
 * Stores the description of all active
 * vertex attributes and vertex bindings.
 */
struct GveInputLayout 
{
	uint32_t numAttributes;
	uint32_t numBindings;

	std::array<GveVertexAttribute, GveLimits::MaxNumVertexAttributes> attributes;
	std::array<GveVertexBinding, GveLimits::MaxNumVertexBindings>   bindings;
};


struct GveDynamicState 
{
	GveBlendConstants  blendConstants = { 0.0f, 0.0f, 0.0f, 0.0f };
	GveDepthBias       depthBias = { 0.0f, 0.0f, 0.0f };
	GveDepthBounds     depthBounds = { false, 0.0f, 1.0f };
	uint32_t           stencilReference = 0;
};


/**
 * \brief Pipeline state
 *
 */
struct GveRenderState 
{
	GveVertexInputState vi;
	VkPipelineInputAssemblyStateCreateInfo ia;

	std::vector<VkViewport> viewports;
	std::vector<VkRect2D> scissors;

	VkPipelineRasterizationStateCreateInfo rs;
	VkPipelineMultisampleStateCreateInfo ms;

	VkPipelineColorBlendAttachmentState cba;
	VkPipelineColorBlendStateCreateInfo cb;
};


}
