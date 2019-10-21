#include "PsslBindingCalculator.h"

namespace pssl
{;


PsslResourceSlotMap::PsslResourceSlotMap()
{

}

PsslResourceSlotMap::~PsslResourceSlotMap()
{

}

void PsslResourceSlotMap::defineSlot(const PsslResourceSlot& slot)
{
	m_slots.push_back(slot);
}

uint32_t PsslResourceSlotMap::getBindingId(const PsslResourceSlot& slot) const
{
	uint32_t bindingId = InvalidBindingId;
	do 
	{
		uint32_t slotCount = m_slots.size();
		for (uint32_t i = 0; i != slotCount; ++i)
		{
			auto& curSlot = m_slots[i];
			if (curSlot.startRegister == slot.startRegister &&
				curSlot.stage == slot.stage)
			{
				bindingId = i;
				break;
			}
		}
	} while (false);
	return bindingId;
}

uint32_t PsslResourceSlotMap::getBindingId(VkShaderStageFlagBits stage, uint32_t startRegister) const
{
	PsslResourceSlot slot;
	slot.stage = stage;
	slot.startRegister = startRegister;
	return getBindingId(slot);
}

}