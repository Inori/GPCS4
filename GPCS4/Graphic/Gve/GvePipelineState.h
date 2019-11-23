#pragma once

#include "GveCommon.h"
#include "FlagHelper.h"
#include "GveLimit.h"
#include "GveBuffer.h"

#include <array>
#include <vector>

namespace gve
{;


//////////////////////////////////////////////////////////////////////////
//Graphics pipeline state
//////////////////////////////////////////////////////////////////////////
class GveVertexAttribute 
{

public:

	GveVertexAttribute() = default;

	GveVertexAttribute(
		uint32_t                        location,
		uint32_t                        binding,
		VkFormat                        format,
		uint32_t                        offset);

	uint32_t location() const;

	uint32_t binding() const;

	VkFormat format() const;

	uint32_t offset() const;

	VkVertexInputAttributeDescription description() const;

private:

	uint32_t m_location : 5;
	uint32_t m_binding : 5;
	uint32_t m_format : 7;
	uint32_t m_offset : 11;
	uint32_t m_reserved : 4;

};


class GveVertexBinding 
{

public:

	GveVertexBinding() = default;

	GveVertexBinding(
		uint32_t                        binding,
		uint32_t                        stride,
		VkVertexInputRate               inputRate,
		uint32_t                        divisor);

	uint32_t binding() const;

	uint32_t stride() const;

	VkVertexInputRate inputRate() const;

	uint32_t divisor() const;

	VkVertexInputBindingDescription description() const;

	void setStride(uint32_t stride);

private:

	uint32_t m_binding : 5;
	uint32_t m_stride : 12;
	uint32_t m_inputRate : 1;
	uint32_t m_reserved : 14;
	uint32_t m_divisor;
};


class GveVertexInputInfo
{
public:
	GveVertexInputInfo() = default;

	void addBinding(const GveVertexBinding& binding);

	void addAttribute(const GveVertexAttribute& attr);

	VkPipelineVertexInputStateCreateInfo state() const;

private:
	
	std::vector<GveVertexBinding> m_bindings;
	std::vector<GveVertexAttribute> m_attributes;
};



class GveInputAssemblyInfo 
{

public:

	GveInputAssemblyInfo() = default;

	GveInputAssemblyInfo(
		VkPrimitiveTopology primitiveTopology,
		VkBool32            primitiveRestart,
		uint32_t            patchVertexCount);

	VkPrimitiveTopology primitiveTopology() const;

	VkBool32 primitiveRestart() const;

	uint32_t patchVertexCount() const;

	VkPipelineInputAssemblyStateCreateInfo state() const;

private:

	uint16_t m_primitiveTopology : 4;
	uint16_t m_primitiveRestart : 1;
	uint16_t m_patchVertexCount : 6;
	uint16_t m_reserved : 5;

};


class GveRasterizationInfo 
{

public:

	GveRasterizationInfo() = default;

	GveRasterizationInfo(
		VkBool32              depthClipEnable,
		VkBool32              depthBiasEnable,
		VkPolygonMode         polygonMode,
		VkCullModeFlags       cullMode,
		VkFrontFace           frontFace,
		uint32_t              viewportCount,
		VkSampleCountFlags    sampleCount);

	VkBool32 depthClipEnable() const;

	VkBool32 depthBiasEnable() const;

	VkPolygonMode polygonMode() const;

	VkCullModeFlags cullMode() const;

	VkFrontFace frontFace() const;

	uint32_t viewportCount() const;

	VkSampleCountFlags sampleCount() const;

	void setViewportCount(uint32_t viewportCount);

	VkPipelineRasterizationStateCreateInfo state() const;

private:

	uint32_t m_depthClipEnable : 1;
	uint32_t m_depthBiasEnable : 1;
	uint32_t m_rasterizerDiscardEnable : 1;
	uint32_t m_polygonMode : 2;
	uint32_t m_cullMode : 2;
	uint32_t m_frontFace : 1;
	uint32_t m_viewportCount : 5;
	uint32_t m_sampleCount : 5;
	uint32_t m_reserved : 14;
	float m_depthBiasConstantFactor = 0.0;
	float m_depthBiasClamp = 0.0;
	float m_depthBiasSlopeFactor = 0.0;
	float m_lineWidth = 0.0;
};


class GveMultisampleInfo 
{

public:

	GveMultisampleInfo() = default;

	GveMultisampleInfo(
		VkSampleCountFlagBits rasterizationSamples,
		VkBool32              sampleShadingEnable,
		float                 minSampleShading,
		VkSampleMask          sampleMask,
		VkBool32              alphaToCoverageEnable,
		VkBool32              alphaToOneEnable);
	

	VkSampleCountFlagBits sampleCount() const;

	VkSampleMask sampleMask() const;

	VkBool32 enableAlphaToCoverage() const;

	void setSampleCount(VkSampleCountFlagBits sampleCount);

	VkPipelineMultisampleStateCreateInfo state() const;
private:

	uint32_t m_rasterizationSamples : 8;
	uint32_t m_sampleShadingEnable : 1;
	uint32_t m_alphaToCoverageEnable : 1;
	uint32_t m_alphaToOneEnable : 1;
	uint32_t m_reserved : 21;
	uint32_t m_sampleMask;
	float m_minSampleShading;

};


class GveDepthStencilOp 
{

public:

	GveDepthStencilOp() = default;

	GveDepthStencilOp(VkStencilOpState state);

	VkStencilOpState state() const;

private:

	uint32_t m_failOp : 3;
	uint32_t m_passOp : 3;
	uint32_t m_depthFailOp : 3;
	uint32_t m_compareOp : 3;
	uint32_t m_reserved : 4;
	uint32_t m_compareMask : 8;
	uint32_t m_writeMask : 8;

};


class GveDepthStencilInfo 
{

public:

	GveDepthStencilInfo() = default;

	GveDepthStencilInfo(
		VkBool32 enableDepthTest,
		VkBool32 enableDepthWrite,
		VkBool32 enableDepthBoundsTest,
		VkBool32 enableStencilTest,
		VkCompareOp depthCompareOp,
		GveDepthStencilOp frontOp,
		GveDepthStencilOp backOp);

	VkBool32 enableDepthTest() const;

	VkBool32 enableDepthWrite() const;

	VkBool32 enableDepthBoundsTest() const;

	VkBool32 enableStencilTest() const;

	VkCompareOp depthCompareOp() const;

	void setEnableDepthBoundsTest(VkBool32 enableDepthBoundsTest);

	VkPipelineDepthStencilStateCreateInfo state() const;
private:

	uint16_t m_enableDepthTest : 1;
	uint16_t m_enableDepthWrite : 1;
	uint16_t m_enableDepthBoundsTest : 1;
	uint16_t m_enableStencilTest : 1;
	uint16_t m_depthCompareOp : 3;
	uint16_t m_reserved : 9;

	GveDepthStencilOp m_frontOp;
	GveDepthStencilOp m_backOp;

	float m_minDepthBounds = 0.0;
	float m_maxDepthBounds = 0.0;
};


class GveColorBlendAttachment 
{

public:

	GveColorBlendAttachment() = default;

	GveColorBlendAttachment(
		VkBool32                    blendEnable,
		VkBlendFactor               srcColorBlendFactor,
		VkBlendFactor               dstColorBlendFactor,
		VkBlendOp                   colorBlendOp,
		VkBlendFactor               srcAlphaBlendFactor,
		VkBlendFactor               dstAlphaBlendFactor,
		VkBlendOp                   alphaBlendOp,
		VkColorComponentFlags       colorWriteMask);

	VkBool32 blendEnable() const;

	VkBlendFactor srcColorBlendFactor() const;

	VkBlendFactor dstColorBlendFactor() const;

	VkBlendOp colorBlendOp() const;

	VkBlendFactor srcAlphaBlendFactor() const;

	VkBlendFactor dstAlphaBlendFactor() const;

	VkBlendOp alphaBlendOp() const;

	VkColorComponentFlags colorWriteMask() const;

	VkPipelineColorBlendAttachmentState state() const;

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


class GveColorBlendInfo
{
public:
	GveColorBlendInfo() = default;
	GveColorBlendInfo(
		VkBool32 logicOpEnable,
		VkLogicOp logicOp);

	void addAttachment(GveColorBlendAttachment attachment);

	void setBlendConstants(float constants[4]);

	VkPipelineColorBlendStateCreateInfo state() const;
private:
	uint32_t m_logicOp : 8;
	uint32_t m_logicOpEnable : 1;
	uint32_t m_reserved : 23;
	float m_blendConstants[4];
	std::vector<GveColorBlendAttachment> m_attachments;
};

//////////////////////////////////////////////////////////////////////////
//Compoute pipeline state
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////

struct GveGraphicsPipelineStateInfo
{
	GveVertexInputInfo        vi;
	GveInputAssemblyInfo ia;
	GveRasterizationInfo      rs;
	GveMultisampleInfo        ms;
	GveDepthStencilInfo       ds;
	GveColorBlendInfo         cb;
};


struct GveComputePipelineStateInfo
{
	uint32_t placeHolder;
};


}
