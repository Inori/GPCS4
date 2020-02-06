#include "GvePipelineLayout.h"
#include "GveDevice.h"

LOG_CHANNEL(Graphic.Gve.GvePipelineLayout);

namespace gve
{;


GveDescriptorSlotMap::GveDescriptorSlotMap()
{

}

GveDescriptorSlotMap::~GveDescriptorSlotMap()
{

}

uint32_t GveDescriptorSlotMap::bindingCount() const
{
	return m_slots.size();
}

const GveDescriptorSlot* GveDescriptorSlotMap::bindingInfos() const
{
	return m_slots.data();
}

void GveDescriptorSlotMap::defineSlot(const GveResourceSlot& slot, VkShaderStageFlags stage)
{
	m_slots.push_back({ slot, stage });
}

uint32_t GveDescriptorSlotMap::getBindingId(VkShaderStageFlagBits stage, uint32_t startRegister) const
{
	uint32_t bindingId = InvalidBindingId;
	do
	{
		uint32_t slotCount = m_slots.size();
		for (uint32_t i = 0; i != slotCount; ++i)
		{
			auto& curSlot = m_slots[i];
			if (curSlot.resSlot.regSlot == startRegister &&
				curSlot.stage == stage)
			{
				bindingId = i;
				break;
			}
		}
	} while (false);
	return bindingId;
}

///
GvePipelineLayout::GvePipelineLayout(
	const RcPtr<GveDevice>& device,
	const GveDescriptorSlotMap& slotMap,
	VkPipelineBindPoint pipelineBindPoint) :
	m_device(device),
	m_bindingSlots(slotMap.bindingCount())
{

	auto bindingCount = slotMap.bindingCount();
	auto bindingInfos = slotMap.bindingInfos();

	for (uint32_t i = 0; i < bindingCount; i++)
	{
		m_bindingSlots[i] = bindingInfos[i];
	}
		
	std::vector<VkDescriptorSetLayoutBinding> bindings(bindingCount);

	for (uint32_t i = 0; i < bindingCount; i++) 
	{
		bindings[i].binding = i;
		bindings[i].descriptorType = bindingInfos[i].resSlot.type;
		bindings[i].descriptorCount = 1;
		bindings[i].stageFlags = bindingInfos[i].stage;
		bindings[i].pImmutableSamplers = nullptr;
	}

	// Create descriptor set layout. We do not need to
	// create one if there are no active resource bindings.
	if (bindingCount > 0) 
	{
		VkDescriptorSetLayoutCreateInfo dsetInfo;
		dsetInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		dsetInfo.pNext = nullptr;
		dsetInfo.flags = 0;
		dsetInfo.bindingCount = bindings.size();
		dsetInfo.pBindings = bindings.data();

		if (vkCreateDescriptorSetLayout(*m_device, &dsetInfo, nullptr, &m_descriptorSetLayout) != VK_SUCCESS)
		{
			LOG_ERR("Failed to create descriptor set layout");
		}
	}

	// Create pipeline layout with the given descriptor set layout
	VkPipelineLayoutCreateInfo pipeInfo;
	pipeInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipeInfo.pNext = nullptr;
	pipeInfo.flags = 0;
	pipeInfo.setLayoutCount = bindingCount > 0 ? 1 : 0;
	pipeInfo.pSetLayouts = &m_descriptorSetLayout;
	pipeInfo.pushConstantRangeCount = 0;
	pipeInfo.pPushConstantRanges = nullptr;

	if (vkCreatePipelineLayout(*m_device, &pipeInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS) 
	{
		vkDestroyDescriptorSetLayout(*m_device, m_descriptorSetLayout, nullptr);
		LOG_ERR("Failed to create pipeline layout");
	}
}


GvePipelineLayout::~GvePipelineLayout() 
{
	vkDestroyPipelineLayout(*m_device, m_pipelineLayout, nullptr);
	vkDestroyDescriptorSetLayout(*m_device, m_descriptorSetLayout, nullptr);
}


}  // namespace gve