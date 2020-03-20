#include "VltExtension.h"

LOG_CHANNEL(Graphic.Violet.VltExtension);

namespace vlt
{;


VltNameSet::VltNameSet() { }
VltNameSet::~VltNameSet() { }


void VltNameSet::add(const char* pName) 
{
	m_names.insert({ pName, 1u });
}


void VltNameSet::merge(const VltNameSet& names) 
{
	for (const auto& pair : names.m_names)
	{
		m_names.insert(pair);
	}
}


uint32_t VltNameSet::supports(const char* pName) const 
{
	uint32_t revision = 0;
	do 
	{
		auto entry = m_names.find(pName);

		if (entry == m_names.end())
		{
			break;
		}

		revision = entry->second != 0 ? entry->second : 1;
	} while (false);
	return revision;
}


bool VltNameSet::enableExtensions(
	uint32_t          numExtensions,
	VltExt**         ppExtensions,
	VltNameSet&       nameSet) const 
{
	bool allRequiredEnabled = true;

	for (uint32_t i = 0; i < numExtensions; i++) 
	{
		VltExt* ext = ppExtensions[i];

		if (ext->mode() == VltExtMode::Disabled)
		{
			continue;
		}
			
		uint32_t revision = supports(ext->name());

		if (revision) 
		{
			if (ext->mode() != VltExtMode::Passive)
			{
				nameSet.add(ext->name());
			}
			ext->enable(revision);
		}
		else if (ext->mode() == VltExtMode::Required) 
		{
			LOG_WARN("Required Vulkan extension %s not supported", ext->name());
			allRequiredEnabled = false;
			continue;
		}
	}

	return allRequiredEnabled;
}


VltNameList VltNameSet::toNameList() const 
{
	VltNameList nameList;
	for (const auto& pair : m_names)
	{
		nameList.add(pair.first.c_str());
	}
	return nameList;
}


VltNameSet VltNameSet::enumInstanceLayers() 
{
	VltNameSet nameSet;
	do 
	{
		uint32_t entryCount = 0;
		if (vkEnumerateInstanceLayerProperties(&entryCount, nullptr) != VK_SUCCESS)
		{
			break;
		}

		std::vector<VkLayerProperties> entries(entryCount);
		if (vkEnumerateInstanceLayerProperties(&entryCount, entries.data()) != VK_SUCCESS)
		{
			break;
		}

		for (uint32_t i = 0; i < entryCount; i++)
		{
			nameSet.m_names.insert({ entries[i].layerName, entries[i].specVersion });
		}
			
	} while (false);
	return nameSet;
}


VltNameSet VltNameSet::enumInstanceExtensions() 
{
	VltNameSet nameSet;
	do
	{
		uint32_t entryCount = 0;
		if (vkEnumerateInstanceExtensionProperties(nullptr, &entryCount, nullptr) != VK_SUCCESS)
		{
			break;
		}

		std::vector<VkExtensionProperties> entries(entryCount);
		if (vkEnumerateInstanceExtensionProperties(nullptr, &entryCount, entries.data()) != VK_SUCCESS)
		{
			break;
		}

		for (uint32_t i = 0; i < entryCount; i++)
		{
			nameSet.m_names.insert({ entries[i].extensionName, entries[i].specVersion });
		}
			
	} while (false);
	return nameSet;
}


VltNameSet VltNameSet::enumDeviceExtensions(VkPhysicalDevice device) 
{
	VltNameSet nameSet;
	do
	{
		uint32_t entryCount = 0;
		if (vkEnumerateDeviceExtensionProperties(device, nullptr, &entryCount, nullptr) != VK_SUCCESS)
		{
			break;
		}

		std::vector<VkExtensionProperties> entries(entryCount);
		if (vkEnumerateDeviceExtensionProperties(device, nullptr, &entryCount, entries.data()) != VK_SUCCESS)
		{
			break;
		}
		
		for (uint32_t i = 0; i < entryCount; i++)
		{
			nameSet.m_names.insert({ entries[i].extensionName, entries[i].specVersion });
		}

	} while (false);
	return nameSet;
}


}  // namespace vlt