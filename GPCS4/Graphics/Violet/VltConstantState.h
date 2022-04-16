#pragma once

#include "VltCommon.h"
#include "VltLimit.h"

#include <array>

namespace sce::vlt
{

	/**
     * \brief Blend constants
     * 
     * Stores a blend factor
     * as an RGBA color value.
     */
	struct VltBlendConstants
	{
		float r, g, b, a;

		bool operator==(const VltBlendConstants& other) const
		{
			return this->r == other.r && this->g == other.g &&
				   this->b == other.b && this->a == other.a;
		}

		bool operator!=(const VltBlendConstants& other) const
		{
			return this->r != other.r || this->g != other.g ||
				   this->b != other.b || this->a != other.a;
		}
	};

	/**
     * \brief Depth bias
     * 
     * Stores depth bias values.
     */
	struct VltDepthBias
	{
		float depthBiasConstant;
		float depthBiasSlope;
		float depthBiasClamp;

		bool operator==(const VltDepthBias& other) const
		{
			return depthBiasConstant == other.depthBiasConstant &&
				   depthBiasSlope == other.depthBiasSlope &&
				   depthBiasClamp == other.depthBiasClamp;
		}

		bool operator!=(const VltDepthBias& other) const
		{
			return depthBiasConstant != other.depthBiasConstant ||
				   depthBiasSlope != other.depthBiasSlope ||
				   depthBiasClamp != other.depthBiasClamp;
		}
	};

	/**
     * \brief Depth bounds
     * 
     * Stores depth bounds values.
     */
	struct VltDepthBounds
	{
		VkBool32 enableDepthBounds;
		float    minDepthBounds;
		float    maxDepthBounds;

		bool operator==(const VltDepthBounds& other) const
		{
			return enableDepthBounds == other.enableDepthBounds &&
				   minDepthBounds == other.minDepthBounds &&
				   maxDepthBounds == other.maxDepthBounds;
		}

		bool operator!=(const VltDepthBounds& other) const
		{
			return enableDepthBounds != other.enableDepthBounds ||
				   minDepthBounds != other.minDepthBounds ||
				   maxDepthBounds != other.maxDepthBounds;
		}
	};

	/**
     * \brief Input assembly state
     * 
     * Stores the primitive topology and
     * whether or not primitive restart
     * is enabled.
     */
	struct VltInputAssemblyState
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
	struct VltRasterizerState
	{
		VkPolygonMode                      polygonMode;
		VkCullModeFlags                    cullMode;
		VkFrontFace                        frontFace;
		VkBool32                           depthClipEnable;
		VkBool32                           depthBiasEnable;
		VkSampleCountFlags                 sampleCount;
		VkConservativeRasterizationModeEXT conservativeMode;
	};

	/**
     * \brief Multisample state
     * 
     * Defines how to handle certain
     * aspects of multisampling.
     */
	struct VltMultisampleState
	{
		uint32_t sampleMask;
		VkBool32 enableAlphaToCoverage;
	};

	/**
     * \brief Depth-stencil state
     * 
     * Defines the depth test and stencil
     * operations for the graphics pipeline.
     */
	struct VltDepthStencilState
	{
		VkBool32         enableDepthTest;
		VkBool32         enableDepthWrite;
		VkBool32         enableStencilTest;
		VkCompareOp      depthCompareOp;
		VkStencilOpState stencilOpFront;
		VkStencilOpState stencilOpBack;
	};

	/**
     * \brief Logic op state
     * Defines a logic op.
     */
	struct VltLogicOpState
	{
		VkBool32  enableLogicOp;
		VkLogicOp logicOp;
	};

	/**
     * \brief Blend mode for a single attachment
     * 
     * Stores the blend state for a single color attachment.
     * Blend modes can be set separately for each attachment.
     */
	struct VltBlendMode
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
	struct VltVertexAttribute
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
	struct VltVertexBinding
	{
		uint32_t          binding;
		uint32_t          fetchRate;
		VkVertexInputRate inputRate;
	};

	/**
     * \brief Input layout
     * 
     * Stores the description of all active
     * vertex attributes and vertex bindings.
     */
	struct DxvkInputLayout
	{
		uint32_t numAttributes;
		uint32_t numBindings;

		std::array<VltVertexAttribute, VltLimits::MaxNumVertexAttributes> attributes;
		std::array<VltVertexBinding, VltLimits::MaxNumVertexBindings>     bindings;
	};

}  // namespace sce::vlt