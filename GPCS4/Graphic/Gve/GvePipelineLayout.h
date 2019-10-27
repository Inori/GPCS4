#pragma once

#include "GveCommon.h"
#include "../Pssl/PsslBindingCalculator.h"

namespace gve
{;

class GveDevice;


struct GveResourceSlot 
{
	// SGPR register slot, calculated using methods in PsslBindingCalculator.h
	// TODO: Need to take apiSlot into consideration?
	uint32_t           regSlot;  
	//Descriptor type (aka resource type)
	VkDescriptorType   type;	  
};

struct GveDescriptorSlot 
{
	GveResourceSlot resSlot;
	VkShaderStageFlags stage;
};


class GveDescriptorSlotMap
{
	constexpr static uint32_t InvalidBindingId = 0xFFFFFFFF;

public:
	GveDescriptorSlotMap();
	~GveDescriptorSlotMap();

	uint32_t bindingCount() const;

	const GveDescriptorSlot* bindingInfos() const;

	void defineSlot(const GveResourceSlot& slot, VkShaderStageFlags stage);

	uint32_t getBindingId(VkShaderStageFlagBits stage, uint32_t startRegister) const;


private:
	std::vector<GveDescriptorSlot> m_slots;
};



/**
 * \brief Shader interface
 *
 * Describes shader resource bindings
 * for a graphics or compute pipeline.
 */
class GvePipelineLayout : public RcObject 
{

public:

	GvePipelineLayout(
		const RcPtr<GveDevice>& device,
		const GveDescriptorSlotMap& slotMap,
		VkPipelineBindPoint pipelineBindPoint);

	~GvePipelineLayout();

	/**
	 * \brief Number of resource bindings
	 * \returns Resource binding count
	 */
	uint32_t bindingCount() const 
	{
		return m_bindingSlots.size();
	}

	/**
	 * \brief Resource binding info  
	 *
	 * \param [in] id Binding index
	 * \returns Resource binding info
	 */
	const GveDescriptorSlot& binding(uint32_t id) const 
	{
		return m_bindingSlots[id];
	}

	/**
	 * \brief Resource binding info
	 * \returns Resource binding info
	 */
	const GveDescriptorSlot* bindings() const 
	{
		return m_bindingSlots.data();
	}

	/**
	 * \brief Descriptor set layout handle
	 * \returns Descriptor set layout handle
	 */
	VkDescriptorSetLayout descriptorSetLayout() const 
	{
		return m_descriptorSetLayout;
	}

	/**
	 * \brief Pipeline layout handle
	 * \returns Pipeline layout handle
	 */
	VkPipelineLayout pipelineLayout() const 
	{
		return m_pipelineLayout;
	}


private:

	RcPtr<GveDevice> m_device;

	VkDescriptorSetLayout           m_descriptorSetLayout = VK_NULL_HANDLE;
	VkPipelineLayout                m_pipelineLayout = VK_NULL_HANDLE;

	std::vector<GveDescriptorSlot>  m_bindingSlots;
};


}  // namespace gve