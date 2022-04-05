#include "VltPipeLayout.h"

#include "VltDescriptor.h"
#include "VltDevice.h"

namespace sce::vlt
{

	VltDescriptorSlotMapping::VltDescriptorSlotMapping()
	{
	}
	VltDescriptorSlotMapping::~VltDescriptorSlotMapping()
	{
	}

	void VltDescriptorSlotMapping::defineSlot(
		VkShaderStageFlagBits  stage,
		const VltResourceSlot& desc)
	{
		uint32_t bindingId = this->getBindingId(desc.slot);

		if (bindingId != InvalidBinding)
		{
			m_descriptorSlots[bindingId].stages |= stage;
			m_descriptorSlots[bindingId].access |= desc.access;
		}
		else
		{
			DxvkDescriptorSlot slotInfo;
			slotInfo.slot   = desc.slot;
			slotInfo.type   = desc.type;
			slotInfo.view   = desc.view;
			slotInfo.stages = stage;
			slotInfo.access = desc.access;
			m_descriptorSlots.push_back(slotInfo);
		}
	}

	void VltDescriptorSlotMapping::definePushConstRange(
		VkShaderStageFlagBits stage,
		uint32_t              offset,
		uint32_t              size)
	{
		m_pushConstRange.stageFlags |= stage;
		m_pushConstRange.size = std::max(
			m_pushConstRange.size, offset + size);
	}

	uint32_t VltDescriptorSlotMapping::getBindingId(uint32_t slot) const
	{
		// This won't win a performance competition, but the number
		// of bindings used by a shader is usually much smaller than
		// the number of resource slots available to the system.
		for (uint32_t i = 0; i < m_descriptorSlots.size(); i++)
		{
			if (m_descriptorSlots[i].slot == slot)
				return i;
		}

		return InvalidBinding;
	}

	void VltDescriptorSlotMapping::makeDescriptorsDynamic(
		uint32_t uniformBuffers,
		uint32_t storageBuffers)
	{
		if (this->countDescriptors(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER) <= uniformBuffers)
			this->replaceDescriptors(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC);
	}

	uint32_t VltDescriptorSlotMapping::countDescriptors(
		VkDescriptorType type) const
	{
		uint32_t count = 0;

		for (const auto& slot : m_descriptorSlots)
			count += slot.type == type ? 1 : 0;

		return count;
	}

	void VltDescriptorSlotMapping::replaceDescriptors(
		VkDescriptorType oldType,
		VkDescriptorType newType)
	{
		for (auto& slot : m_descriptorSlots)
		{
			if (slot.type == oldType)
				slot.type = newType;
		}
	}

	VltPipelineLayout::VltPipelineLayout(
		VltDevice*                      device,
		const VltDescriptorSlotMapping& slotMapping,
		VkPipelineBindPoint             pipelineBindPoint) :
		m_device(device),
		m_pushConstRange(slotMapping.pushConstRange()),
		m_bindingSlots(slotMapping.bindingCount())
	{

		auto bindingCount = slotMapping.bindingCount();
		auto bindingInfos = slotMapping.bindingInfos();

		for (uint32_t i = 0; i < bindingCount; i++)
			m_bindingSlots[i] = bindingInfos[i];

		std::vector<VkDescriptorSetLayoutBinding>       bindings(bindingCount);
		std::vector<VkDescriptorUpdateTemplateEntryKHR> tEntries(bindingCount);

		for (uint32_t i = 0; i < bindingCount; i++)
		{
			bindings[i].binding            = i;
			bindings[i].descriptorType     = bindingInfos[i].type;
			bindings[i].descriptorCount    = 1;
			bindings[i].stageFlags         = bindingInfos[i].stages;
			bindings[i].pImmutableSamplers = nullptr;

			tEntries[i].dstBinding      = i;
			tEntries[i].dstArrayElement = 0;
			tEntries[i].descriptorCount = 1;
			tEntries[i].descriptorType  = bindingInfos[i].type;
			tEntries[i].offset          = sizeof(VltDescriptorInfo) * i;
			tEntries[i].stride          = 0;

			if (bindingInfos[i].type == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC)
				m_dynamicSlots.push_back(i);

			m_descriptorTypes.set(bindingInfos[i].type);
		}

		// Create descriptor set layout. We do not need to
		// create one if there are no active resource bindings.
		if (bindingCount > 0)
		{
			VkDescriptorSetLayoutCreateInfo dsetInfo;
			dsetInfo.sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
			dsetInfo.pNext        = nullptr;
			dsetInfo.flags        = 0;
			dsetInfo.bindingCount = bindings.size();
			dsetInfo.pBindings    = bindings.data();

			if (vkCreateDescriptorSetLayout(m_device->handle(),
											&dsetInfo, nullptr, &m_descriptorSetLayout) != VK_SUCCESS)
				Logger::exception("DxvkPipelineLayout: Failed to create descriptor set layout");
		}

		// Create pipeline layout with the given descriptor set layout
		VkPipelineLayoutCreateInfo pipeInfo;
		pipeInfo.sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipeInfo.pNext                  = nullptr;
		pipeInfo.flags                  = 0;
		pipeInfo.setLayoutCount         = bindingCount > 0 ? 1 : 0;
		pipeInfo.pSetLayouts            = &m_descriptorSetLayout;
		pipeInfo.pushConstantRangeCount = 0;
		pipeInfo.pPushConstantRanges    = nullptr;

		if (m_pushConstRange.size)
		{
			pipeInfo.pushConstantRangeCount = 1;
			pipeInfo.pPushConstantRanges    = &m_pushConstRange;
		}

		if (vkCreatePipelineLayout(m_device->handle(),
								   &pipeInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS)
		{
			vkDestroyDescriptorSetLayout(m_device->handle(), m_descriptorSetLayout, nullptr);
			Logger::exception("DxvkPipelineLayout: Failed to create pipeline layout");
		}

		// Create descriptor update template. If there are no active
		// resource bindings, there won't be any descriptors to update.
		if (bindingCount > 0)
		{
			VkDescriptorUpdateTemplateCreateInfoKHR templateInfo;
			templateInfo.sType                      = VK_STRUCTURE_TYPE_DESCRIPTOR_UPDATE_TEMPLATE_CREATE_INFO_KHR;
			templateInfo.pNext                      = nullptr;
			templateInfo.flags                      = 0;
			templateInfo.descriptorUpdateEntryCount = tEntries.size();
			templateInfo.pDescriptorUpdateEntries   = tEntries.data();
			templateInfo.templateType               = VK_DESCRIPTOR_UPDATE_TEMPLATE_TYPE_DESCRIPTOR_SET_KHR;
			templateInfo.descriptorSetLayout        = m_descriptorSetLayout;
			templateInfo.pipelineBindPoint          = pipelineBindPoint;
			templateInfo.pipelineLayout             = m_pipelineLayout;
			templateInfo.set                        = 0;

			if (vkCreateDescriptorUpdateTemplate(
					m_device->handle(), &templateInfo, nullptr, &m_descriptorTemplate) != VK_SUCCESS)
			{
				vkDestroyDescriptorSetLayout(m_device->handle(), m_descriptorSetLayout, nullptr);
				vkDestroyPipelineLayout(m_device->handle(), m_pipelineLayout, nullptr);
				Logger::exception("DxvkPipelineLayout: Failed to create descriptor update template");
			}
		}
	}

	VltPipelineLayout::~VltPipelineLayout()
	{
		vkDestroyDescriptorUpdateTemplate(
			m_device->handle(), m_descriptorTemplate, nullptr);

		vkDestroyPipelineLayout(
			m_device->handle(), m_pipelineLayout, nullptr);

		vkDestroyDescriptorSetLayout(
			m_device->handle(), m_descriptorSetLayout, nullptr);
	}
}  // namespace sce::vlt