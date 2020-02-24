#include "GvePipelineState.h"

namespace gve
{;


void GveVertexInputInfo::addBinding(const GveVertexBinding& binding)
{
	m_bindings[m_bindingCount++] = binding;
}

void GveVertexInputInfo::addAttribute(const GveVertexAttribute& attr)
{
	m_attributes[m_attributeCount++] = attr;
}

VkPipelineVertexInputStateCreateInfo GveVertexInputInfo::state(
	std::vector<VkVertexInputBindingDescription>&   bindings,
	std::vector<VkVertexInputAttributeDescription>& attributes) const
{
	VkPipelineVertexInputStateCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	info.pNext = nullptr;

	// Bindings
	bindings.clear();
	bindings.resize(m_bindingCount);
	std::generate(bindings.begin(), bindings.end(), [n = 0, this]() mutable 
	{ 
		return m_bindings[n++].description();
	});
	info.vertexBindingDescriptionCount = bindings.size();
	info.pVertexBindingDescriptions = bindings.data();

	// Attributes
	attributes.clear();
	attributes.resize(m_attributeCount);
	std::generate(attributes.begin(), attributes.end(), [n = 0, this]() mutable 
	{
		return m_attributes[n++].description();
	});
	info.vertexAttributeDescriptionCount = attributes.size();
	info.pVertexAttributeDescriptions = attributes.data();

	return info;
}


void GveColorBlendInfo::addAttachment(const GveColorBlendAttachment& attachment)
{
	m_attachments[m_attachmentCount++] = attachment;
}

void GveColorBlendInfo::addSwizzle(GveColorBlendAttachmentSwizzle& swizzle)
{
	m_swizzles[m_swizzleCount++] = swizzle;
}

void GveColorBlendInfo::setBlendConstants(float constants[4])
{
	std::memcpy(m_blendConstants, constants, sizeof(float) * 4);
}

VkPipelineColorBlendStateCreateInfo GveColorBlendInfo::state(
	std::vector<VkPipelineColorBlendAttachmentState>& attachmentStates) const
{
	VkPipelineColorBlendStateCreateInfo state = {};
	state.sType                               = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	state.logicOpEnable                       = VkBool32(m_enableLogicOp);
	state.logicOp                             = VkLogicOp(m_logicOp);

	attachmentStates.clear();
	attachmentStates.resize(m_attachmentCount);
	std::generate(attachmentStates.begin(), attachmentStates.end(), [n = 0, this]() mutable
	{
		return m_attachments[n++].state();
	});

	state.attachmentCount = attachmentStates.size();
	state.pAttachments    = attachmentStates.data();

	state.blendConstants[0] = m_blendConstants[0];
	state.blendConstants[1] = m_blendConstants[1];
	state.blendConstants[2] = m_blendConstants[2];
	state.blendConstants[3] = m_blendConstants[3];
	return state;
}

void GveDynamicStateInfo::setViewportCount(uint8_t count)
{
	m_viewportCount = count;
}

VkPipelineViewportStateCreateInfo GveDynamicStateInfo::viewportState() const
{
	VkPipelineViewportStateCreateInfo vpInfo = {};
	vpInfo.sType                             = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	vpInfo.pNext                             = nullptr;
	vpInfo.flags                             = 0;
	vpInfo.viewportCount                     = m_viewportCount;
	vpInfo.pViewports                        = nullptr;
	vpInfo.scissorCount                      = m_viewportCount;
	vpInfo.pScissors                         = nullptr;
	return vpInfo;
}

VkPipelineDynamicStateCreateInfo GveDynamicStateInfo::state(std::vector<VkDynamicState>& dynStates) const
{
	VkPipelineDynamicStateCreateInfo state = {};

	dynStates.reserve(2);
	dynStates.push_back(VK_DYNAMIC_STATE_VIEWPORT);
	dynStates.push_back(VK_DYNAMIC_STATE_SCISSOR);

	state.sType             = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	state.pNext             = nullptr;
	state.flags             = 0;
	state.dynamicStateCount = dynStates.size();
	state.pDynamicStates    = dynStates.data();

	return state;
}


std::ostream& operator << (std::ostream& out, const GveGraphicsPipelineStateInfo& state)
{
	// TODO:
}

std::istream& operator >> (std::istream& in, GveGraphicsPipelineStateInfo& state)
{
	// TODO:
}



}  // namespace gve