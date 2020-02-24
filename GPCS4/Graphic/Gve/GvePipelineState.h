#pragma once

#include "GveCommon.h"
#include "UtilFlag.h"
#include "GveLimit.h"
#include "GveUtil.h"
#include "GveBuffer.h"

#include <array>
#include <iostream>

namespace gve
{;

// Structures defined here are aiming to be stored in file 
// to index pipeline cache for future use.
// A "GvXyzInfo" class stands for a "VkPipelineXyzStateCreateInfo"
// during pipeline creation.


//////////////////////////////////////////////////////////////////////////
//Graphics pipeline state
//////////////////////////////////////////////////////////////////////////
class GveVertexAttribute 
{

public:

	GveVertexAttribute() = default;

	GveVertexAttribute(
		uint32_t location,
		uint32_t binding,
		VkFormat format,
		uint32_t offset):
		m_location(uint32_t(location)),
		m_binding(uint32_t(binding)),
		m_format(uint32_t(format)),
		m_offset(uint32_t(offset)),
		m_reserved(0)
		{}

    uint32_t location() const
	{
		return m_location;
	}

	uint32_t binding() const
	{
		return m_binding;
	}

	VkFormat format() const
	{
		return VkFormat(m_format);
	}

	uint32_t offset() const
	{
		return m_offset;
	}

	VkVertexInputAttributeDescription description() const
	{
		VkVertexInputAttributeDescription result;
		result.location = m_location;
		result.binding  = m_binding;
		result.format   = VkFormat(m_format);
		result.offset   = m_offset;
		return result;
	}

private:
	uint32_t m_location		: 5;
	uint32_t m_binding		: 5;
	uint32_t m_format		: 7;
	uint32_t m_offset		: 11;
	uint32_t m_reserved		: 4;
};


class GveVertexBinding 
{

public:

	GveVertexBinding() = default;

	GveVertexBinding(
		uint32_t          binding,
		uint32_t          stride,
		VkVertexInputRate inputRate,
		uint32_t          divisor) :
		m_binding(uint32_t(binding)),
		m_stride(uint32_t(stride)),
		m_inputRate(uint32_t(inputRate)),
		m_reserved(0),
		m_divisor(divisor)
	{
	}

    uint32_t binding() const
	{
		return m_binding;
	}

	uint32_t stride() const
	{
		return m_stride;
	}

	VkVertexInputRate inputRate() const
	{
		return VkVertexInputRate(m_inputRate);
	}

	uint32_t divisor() const
	{
		return m_divisor;
	}

	void setStride(uint32_t stride)
	{
		m_stride = stride;
	}

	VkVertexInputBindingDescription description() const
	{
		VkVertexInputBindingDescription result;
		result.binding   = m_binding;
		result.stride    = m_stride;
		result.inputRate = VkVertexInputRate(m_inputRate);
		return result;
	}

private:
	uint32_t m_binding		: 5;
	uint32_t m_stride		: 12;
	uint32_t m_inputRate	: 1;
	uint32_t m_reserved		: 14;
	uint32_t m_divisor;
};


// Vertex input state
class GveVertexInputInfo
{
public:
	GveVertexInputInfo() = default;

	void addBinding(const GveVertexBinding& binding);

	void addAttribute(const GveVertexAttribute& attr);

	VkPipelineVertexInputStateCreateInfo state(
		std::vector<VkVertexInputBindingDescription>&   bindings,
		std::vector<VkVertexInputAttributeDescription>& attributes) const;

private:
	
	std::array<GveVertexBinding, MaxNumVertexBindings>     m_bindings;
	std::array<GveVertexAttribute, MaxNumVertexAttributes> m_attributes;
	uint8_t                                                m_bindingCount   = 0;
	uint8_t                                                m_attributeCount = 0;
};


// Input assembly state.
class GveInputAssemblyInfo 
{

public:

	GveInputAssemblyInfo() = default;

	GveInputAssemblyInfo(
		VkPrimitiveTopology primitiveTopology,
		VkBool32            primitiveRestart,
		uint32_t            patchVertexCount) :
		m_primitiveTopology(uint16_t(primitiveTopology)),
		m_primitiveRestart(uint16_t(primitiveRestart)),
		m_patchVertexCount(uint16_t(patchVertexCount)),
		m_reserved(0)
	{
	}

    VkPrimitiveTopology primitiveTopology() const
	{
		return m_primitiveTopology <= VK_PRIMITIVE_TOPOLOGY_PATCH_LIST
				   ? VkPrimitiveTopology(m_primitiveTopology)
				   : VK_PRIMITIVE_TOPOLOGY_MAX_ENUM;
	}

	VkBool32 primitiveRestart() const
	{
		return VkBool32(m_primitiveRestart);
	}

	uint32_t patchVertexCount() const
	{
		return m_patchVertexCount;
	}

	VkPipelineInputAssemblyStateCreateInfo state() const
	{
		VkPipelineInputAssemblyStateCreateInfo state = {};
		state.sType                                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		state.pNext                                  = nullptr;
		state.flags                                  = 0;
		state.topology                               = VkPrimitiveTopology(m_primitiveTopology);
		state.primitiveRestartEnable                 = m_primitiveRestart;
		return state;
	}

private:
	uint16_t m_primitiveTopology : 4;
	uint16_t m_primitiveRestart : 1;
	uint16_t m_patchVertexCount : 6;
	uint16_t m_reserved : 5;
};


// Rasterization state
class GveRasterizationInfo 
{

public:

	GveRasterizationInfo() = default;

	GveRasterizationInfo(
		VkBool32           depthClipEnable,
		VkBool32           depthBiasEnable,
		VkPolygonMode      polygonMode,
		VkCullModeFlags    cullMode,
		VkFrontFace        frontFace,
		VkSampleCountFlags sampleCount) :
		m_depthClipEnable(uint32_t(depthClipEnable)),
		m_depthBiasEnable(uint32_t(depthBiasEnable)),
		m_polygonMode(uint32_t(polygonMode)),
		m_cullMode(uint32_t(cullMode)),
		m_frontFace(uint32_t(frontFace)),
		m_sampleCount(uint32_t(sampleCount)),
		m_reserved(0)
	{
	}

    VkBool32 depthClipEnable() const
	{
		return VkBool32(m_depthClipEnable);
	}

	VkBool32 depthBiasEnable() const
	{
		return VkBool32(m_depthBiasEnable);
	}

	VkPolygonMode polygonMode() const
	{
		return VkPolygonMode(m_polygonMode);
	}

	VkCullModeFlags cullMode() const
	{
		return VkCullModeFlags(m_cullMode);
	}

	VkFrontFace frontFace() const
	{
		return VkFrontFace(m_frontFace);
	}

	VkSampleCountFlags sampleCount() const
	{
		return VkSampleCountFlags(m_sampleCount);
	}

	VkPipelineRasterizationStateCreateInfo state() const
	{
		VkPipelineRasterizationStateCreateInfo state = {};
		state.sType                                  = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		state.pNext                                  = nullptr;
		state.depthClampEnable                       = VkBool32(m_depthClipEnable);
		state.rasterizerDiscardEnable                = VkBool32(0);
		state.polygonMode                            = VkPolygonMode(m_polygonMode);
		state.cullMode                               = VkCullModeFlags(m_cullMode);
		state.frontFace                              = VkFrontFace(m_frontFace);
		state.depthBiasEnable                        = VkBool32(m_depthBiasEnable);
		state.depthBiasConstantFactor                = m_depthBiasSlopeFactor;
		state.depthBiasClamp                         = m_depthBiasClamp;
		state.depthBiasSlopeFactor                   = m_depthBiasSlopeFactor;
		state.lineWidth                              = m_lineWidth;
		return state;
	}

private:
	uint32_t m_depthClipEnable : 1;
	uint32_t m_depthBiasEnable : 1;
	uint32_t m_polygonMode : 2;
	uint32_t m_cullMode : 2;
	uint32_t m_frontFace : 1;
	uint32_t m_sampleCount : 5;
	uint32_t m_reserved : 20;
	// TODO:
	float m_depthBiasConstantFactor = 0.0;
	float m_depthBiasClamp          = 0.0;
	float m_depthBiasSlopeFactor    = 0.0;
	float m_lineWidth               = 1.0;
};


// Multisample state
class GveMultisampleInfo 
{
public:

	GveMultisampleInfo() = default;

	GveMultisampleInfo(
		VkSampleCountFlags sampleCount,
		uint32_t           sampleMask,
		VkBool32           enableAlphaToCoverage) :
		m_sampleCount(uint16_t(sampleCount)),
		m_enableAlphaToCoverage(uint16_t(enableAlphaToCoverage)),
		m_reserved(0),
		m_sampleMask(uint16_t(sampleMask))
	{
	}
	
    VkSampleCountFlags sampleCount() const
	{
		return VkSampleCountFlags(m_sampleCount);
	}

	uint32_t sampleMask() const
	{
		return m_sampleMask;
	}

	VkBool32 enableAlphaToCoverage() const
	{
		return VkBool32(m_enableAlphaToCoverage);
	}

	void setSampleCount(VkSampleCountFlags sampleCount)
	{
		m_sampleCount = uint16_t(sampleCount);
	}

	VkPipelineMultisampleStateCreateInfo state(
		VkSampleCountFlagBits rsSampleCount,
		VkBool32              sampleShadingEnable,
		float                 minSampleShading) const
	{
		VkSampleCountFlagBits                sampleCount = m_sampleCount ? 
			VkSampleCountFlagBits(m_sampleCount) : rsSampleCount;
		VkPipelineMultisampleStateCreateInfo state       = {};
		state.sType                                      = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		state.pNext                                      = nullptr;
		state.flags                                      = 0;
		state.rasterizationSamples                       = sampleCount;
		state.sampleShadingEnable                        = sampleShadingEnable;
		state.minSampleShading                           = minSampleShading;
		// TODO:
		// Currently, set sample mask will result in a black screen.
		// To be fixed.
		// state.pSampleMask           = (VkSampleMask*)&m_sampleMask;
		state.alphaToCoverageEnable = VkBool32(m_enableAlphaToCoverage);
		state.alphaToOneEnable      = VK_FALSE;

		return state;
	}

private:
	uint16_t m_sampleCount : 5;
	uint16_t m_enableAlphaToCoverage : 1;
	uint16_t m_reserved : 10;
	uint16_t m_sampleMask;

};


class GveDepthStencilOp 
{

public:

	GveDepthStencilOp() = default;

	GveDepthStencilOp(VkStencilOpState state) :
		m_failOp(uint32_t(state.failOp)),
		m_passOp(uint32_t(state.passOp)),
		m_depthFailOp(uint32_t(state.depthFailOp)),
		m_compareOp(uint32_t(state.compareOp)),
		m_reserved(0),
		m_compareMask(uint32_t(state.compareMask)),
		m_writeMask(uint32_t(state.writeMask))
	{
	}

	VkStencilOpState state() const
	{
		VkStencilOpState result;
		result.failOp      = VkStencilOp(m_failOp);
		result.passOp      = VkStencilOp(m_passOp);
		result.depthFailOp = VkStencilOp(m_depthFailOp);
		result.compareOp   = VkCompareOp(m_compareOp);
		result.compareMask = m_compareMask;
		result.writeMask   = m_writeMask;
		result.reference   = 0;
		return result;
	}

private:
	uint32_t m_failOp : 3;
	uint32_t m_passOp : 3;
	uint32_t m_depthFailOp : 3;
	uint32_t m_compareOp : 3;
	uint32_t m_reserved : 4;
	uint32_t m_compareMask : 8;
	uint32_t m_writeMask : 8;
};

// Depth stencil state
class GveDepthStencilInfo 
{

public:

	GveDepthStencilInfo() = default;

	GveDepthStencilInfo(
		VkBool32          enableDepthTest,
		VkBool32          enableDepthWrite,
		VkBool32          enableDepthBoundsTest,
		VkBool32          enableStencilTest,
		VkCompareOp       depthCompareOp,
		GveDepthStencilOp frontOp,
		GveDepthStencilOp backOp) :
		m_enableDepthTest(uint16_t(enableDepthTest)),
		m_enableDepthWrite(uint16_t(enableDepthWrite)),
		m_enableDepthBoundsTest(uint16_t(enableDepthBoundsTest)),
		m_enableStencilTest(uint16_t(enableStencilTest)),
		m_depthCompareOp(uint16_t(depthCompareOp)),
		m_reserved(0),
		m_frontOp(frontOp),
		m_backOp(backOp)
	{
	}

    VkBool32 enableDepthTest() const
	{
		return VkBool32(m_enableDepthTest);
	}

	VkBool32 enableDepthWrite() const
	{
		return VkBool32(m_enableDepthWrite);
	}

	VkBool32 enableDepthBoundsTest() const
	{
		return VkBool32(m_enableDepthBoundsTest);
	}

	VkBool32 enableStencilTest() const
	{
		return VkBool32(m_enableStencilTest);
	}

	VkCompareOp depthCompareOp() const
	{
		return VkCompareOp(m_depthCompareOp);
	}

	void setEnableDepthBoundsTest(VkBool32 enableDepthBoundsTest)
	{
		m_enableDepthBoundsTest = VkBool32(enableDepthBoundsTest);
	}

	VkPipelineDepthStencilStateCreateInfo state() const
	{
		VkPipelineDepthStencilStateCreateInfo state = {};
		state.sType                                 = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		state.pNext                                 = nullptr;

		state.depthTestEnable       = VkBool32(m_enableDepthTest);
		state.depthWriteEnable      = VkBool32(m_enableDepthWrite);
		state.depthCompareOp        = VkCompareOp(m_depthCompareOp);
		state.depthBoundsTestEnable = VkBool32(m_enableDepthBoundsTest);
		state.stencilTestEnable     = VkBool32(m_enableStencilTest);

		state.front = m_frontOp.state();
		state.back  = m_backOp.state();

		state.minDepthBounds = m_minDepthBounds;
		state.maxDepthBounds = m_maxDepthBounds;

		return state;
	}

private:
	uint16_t m_enableDepthTest : 1;
	uint16_t m_enableDepthWrite : 1;
	uint16_t m_enableDepthBoundsTest : 1;
	uint16_t m_enableStencilTest : 1;
	uint16_t m_depthCompareOp : 3;
	uint16_t m_reserved : 9;

	GveDepthStencilOp m_frontOp;
	GveDepthStencilOp m_backOp;
	// TODO:
	float m_minDepthBounds = 0.0;
	float m_maxDepthBounds = 0.0;
};


class GveColorBlendAttachment 
{

public:

	GveColorBlendAttachment() = default;

	GveColorBlendAttachment(
		VkBool32              blendEnable,
		VkBlendFactor         srcColorBlendFactor,
		VkBlendFactor         dstColorBlendFactor,
		VkBlendOp             colorBlendOp,
		VkBlendFactor         srcAlphaBlendFactor,
		VkBlendFactor         dstAlphaBlendFactor,
		VkBlendOp             alphaBlendOp,
		VkColorComponentFlags colorWriteMask) :
		m_blendEnable(uint32_t(blendEnable)),
		m_srcColorBlendFactor(uint32_t(srcColorBlendFactor)),
		m_dstColorBlendFactor(uint32_t(dstColorBlendFactor)),
		m_colorBlendOp(uint32_t(colorBlendOp)),
		m_srcAlphaBlendFactor(uint32_t(srcAlphaBlendFactor)),
		m_dstAlphaBlendFactor(uint32_t(dstAlphaBlendFactor)),
		m_alphaBlendOp(uint32_t(alphaBlendOp)),
		m_colorWriteMask(uint32_t(colorWriteMask)),
		m_reserved(0)
	{
	}

    VkBool32 blendEnable() const
	{
		return m_blendEnable;
	}

	VkBlendFactor srcColorBlendFactor() const
	{
		return VkBlendFactor(m_srcColorBlendFactor);
	}

	VkBlendFactor dstColorBlendFactor() const
	{
		return VkBlendFactor(m_dstColorBlendFactor);
	}

	VkBlendOp colorBlendOp() const
	{
		return VkBlendOp(m_colorBlendOp);
	}

	VkBlendFactor srcAlphaBlendFactor() const
	{
		return VkBlendFactor(m_srcAlphaBlendFactor);
	}

	VkBlendFactor dstAlphaBlendFactor() const
	{
		return VkBlendFactor(m_dstAlphaBlendFactor);
	}

	VkBlendOp alphaBlendOp() const
	{
		return VkBlendOp(m_alphaBlendOp);
	}

	VkColorComponentFlags colorWriteMask() const
	{
		return VkColorComponentFlags(m_colorWriteMask);
	}

	VkPipelineColorBlendAttachmentState state() const
	{
		VkPipelineColorBlendAttachmentState result;
		result.blendEnable         = VkBool32(m_blendEnable);
		result.srcColorBlendFactor = VkBlendFactor(m_srcColorBlendFactor);
		result.dstColorBlendFactor = VkBlendFactor(m_dstColorBlendFactor);
		result.colorBlendOp        = VkBlendOp(m_colorBlendOp);
		result.srcAlphaBlendFactor = VkBlendFactor(m_srcAlphaBlendFactor);
		result.dstAlphaBlendFactor = VkBlendFactor(m_dstAlphaBlendFactor);
		result.alphaBlendOp        = VkBlendOp(m_alphaBlendOp);
		result.colorWriteMask      = VkColorComponentFlags(m_colorWriteMask);
		return result;
	}

private:
	uint32_t m_blendEnable : 1;
	uint32_t m_srcColorBlendFactor : 5;
	uint32_t m_dstColorBlendFactor : 5;
	uint32_t m_colorBlendOp : 3;
	uint32_t m_srcAlphaBlendFactor : 5;
	uint32_t m_dstAlphaBlendFactor : 5;
	uint32_t m_alphaBlendOp : 3;
	uint32_t m_colorWriteMask : 4;
	uint32_t m_reserved : 1;
};

class GveColorBlendAttachmentSwizzle
{

public:
	GveColorBlendAttachmentSwizzle() = default;

	GveColorBlendAttachmentSwizzle(VkComponentMapping mapping) :
		m_r(util::getComponentIndex(mapping.r, 0)),
		m_g(util::getComponentIndex(mapping.g, 1)),
		m_b(util::getComponentIndex(mapping.b, 2)),
		m_a(util::getComponentIndex(mapping.a, 3))
	{
	}

	uint32_t rIndex() const
	{
		return m_r;
	}
	uint32_t gIndex() const
	{
		return m_g;
	}
	uint32_t bIndex() const
	{
		return m_b;
	}
	uint32_t aIndex() const
	{
		return m_a;
	}

	VkComponentMapping mapping() const
	{
		VkComponentMapping result;
		result.r = decodeSwizzle(m_r);
		result.g = decodeSwizzle(m_g);
		result.b = decodeSwizzle(m_b);
		result.a = decodeSwizzle(m_a);
		return result;
	}

private:
	uint8_t m_r : 2;
	uint8_t m_g : 2;
	uint8_t m_b : 2;
	uint8_t m_a : 2;

	static VkComponentSwizzle decodeSwizzle(uint8_t swizzle)
	{
		return VkComponentSwizzle(uint32_t(swizzle) + uint32_t(VK_COMPONENT_SWIZZLE_R));
	}
};


// Color blend state, aka Output merger state
class GveColorBlendInfo
{
public:
	GveColorBlendInfo() = default;
	GveColorBlendInfo(
		VkBool32  logicOpEnable,
		VkLogicOp logicOp) :
		m_enableLogicOp(uint16_t(logicOpEnable)),
		m_logicOp(uint16_t(logicOp)),
		m_attachmentCount(0),
		m_swizzleCount(0),
		m_reserved(0)
	{
	}

	void addAttachment(const GveColorBlendAttachment& attachment);

	void addSwizzle(GveColorBlendAttachmentSwizzle& swizzle);

	void setBlendConstants(float constants[4]);

    VkBool32 enableLogicOp() const
	{
		return VkBool32(m_enableLogicOp);
	}

	VkLogicOp logicOp() const
	{
		return VkLogicOp(m_logicOp);
	}
	
	VkPipelineColorBlendStateCreateInfo state(
		std::vector<VkPipelineColorBlendAttachmentState>& attachmentStates) const;

private:
	uint16_t m_enableLogicOp	: 1;
	uint16_t m_logicOp			: 4;
	uint16_t m_attachmentCount	: 4;
	uint16_t m_swizzleCount		: 4;
	uint16_t m_reserved			: 3;

	std::array<GveColorBlendAttachment, MaxNumRenderTargets>        m_attachments;
	std::array<GveColorBlendAttachmentSwizzle, MaxNumRenderTargets> m_swizzles;

	float m_blendConstants[4] = { 0.0 };
};


// Specialization constant state
struct GveSpecConstantInfo
{
	uint32_t specConstants[MaxNumSpecConstants];
};


// Dynamic states, including:
// Viewport state
// 
class GveDynamicStateInfo
{
public:
	GveDynamicStateInfo() = default;

	void setViewportCount(uint8_t count);

	VkPipelineViewportStateCreateInfo viewportState() const;

	VkPipelineDynamicStateCreateInfo state(std::vector<VkDynamicState>& dynStates) const;

private:
	uint8_t m_viewportCount = 0;
};


//////////////////////////////////////////////////////////////////////////
//Compoute pipeline state
//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
// This is used to create graphics pipeline and pipeline cache.
struct PS4ALIGN(32) GveGraphicsPipelineStateInfo
{
	GveVertexInputInfo        vi;
	GveInputAssemblyInfo      ia;
	GveDynamicStateInfo       dy;  // TODO: Not sure if we can place a dynamic state here
	GveRasterizationInfo      rs;
	GveMultisampleInfo        ms;
	GveDepthStencilInfo       ds;
	GveColorBlendInfo         cb;
	GveSpecConstantInfo       sc;

	GveGraphicsPipelineStateInfo& operator=(const GveGraphicsPipelineStateInfo& other)
	{
		std::memcpy(this, &other, sizeof(*this));
		return *this;
	}

	bool operator==(const GveGraphicsPipelineStateInfo& other) const
	{
		return bit::bcmpeq(this, &other);
	}

	bool operator!=(const GveGraphicsPipelineStateInfo& other) const
	{
		return !bit::bcmpeq(this, &other);
	}

	friend std::ostream& operator << (std::ostream& out, const GveGraphicsPipelineStateInfo& state);
	friend std::istream& operator >> (std::istream& in, GveGraphicsPipelineStateInfo& state);
};


struct GveComputePipelineStateInfo
{
	// TODO:
	uint32_t placeHolder;
};


}
