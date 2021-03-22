#pragma once

#include "VltCommon.h"
#include "../Pssl/PsslBindingCalculator.h"

namespace vlt
{;

class VltDevice;


struct VltResourceSlot 
{
	// SGPR register slot, calculated using methods in PsslBindingCalculator.h
	// TODO: Need to take apiSlot into consideration?
	uint32_t           regSlot;  
	//Descriptor type (aka resource type)
	VkDescriptorType   type;	  
};

struct VltDescriptorSlot 
{
	VltResourceSlot resSlot;
	VkShaderStageFlags stage;
};


class VltDescriptorSlotMap
{
	constexpr static uint32_t InvalidBindingId = 0xFFFFFFFF;

public:
	VltDescriptorSlotMap();
	~VltDescriptorSlotMap();

	uint32_t bindingCount() const;

	const VltDescriptorSlot* bindingInfos() const;

	void defineSlot(const VltResourceSlot& slot, VkShaderStageFlags stage);

	uint32_t getBindingId(VkShaderStageFlagBits stage, uint32_t startRegister) const;


private:
	std::vector<VltDescriptorSlot> m_slots;
};



/**
 * \brief Shader interface
 *
 * Describes shader resource bindings
 * for a graphics or compute pipeline.
 */
class VltPipelineLayout : public RcObject 
{

public:

	VltPipelineLayout(
		const RcPtr<VltDevice>&     device,
		const VltDescriptorSlotMap& slotMap,
		VkPipelineBindPoint         pipelineBindPoint);

	~VltPipelineLayout();

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
	const VltDescriptorSlot& binding(uint32_t id) const 
	{
		return m_bindingSlots[id];
	}

	/**
	 * \brief Resource binding info
	 * \returns Resource binding info
	 */
	const VltDescriptorSlot* bindings() const 
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

	RcPtr<VltDevice> m_device;

	VkDescriptorSetLayout           m_descriptorSetLayout = VK_NULL_HANDLE;
	VkPipelineLayout                m_pipelineLayout = VK_NULL_HANDLE;

	std::vector<VltDescriptorSlot>  m_bindingSlots;
};


}  // namespace vlt