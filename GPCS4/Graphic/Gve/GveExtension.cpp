#include "GveExtension.h"

LOG_CHANNEL(Graphic.Gve.GveExtension);

namespace gve
{;


GveNameSet::GveNameSet() { }
GveNameSet::~GveNameSet() { }


void GveNameSet::add(const char* pName) 
{
	m_names.insert({ pName, 1u });
}


void GveNameSet::merge(const GveNameSet& names) 
{
	for (const auto& pair : names.m_names)
	{
		m_names.insert(pair);
	}
}


uint32_t GveNameSet::supports(const char* pName) const 
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


bool GveNameSet::enableExtensions(
	uint32_t          numExtensions,
	GveExt**         ppExtensions,
	GveNameSet&       nameSet) const 
{
	bool allRequiredEnabled = true;

	for (uint32_t i = 0; i < numExtensions; i++) 
	{
		GveExt* ext = ppExtensions[i];

		if (ext->mode() == GveExtMode::Disabled)
		{
			continue;
		}
			
		uint32_t revision = supports(ext->name());

		if (revision) 
		{
			if (ext->mode() != GveExtMode::Passive)
			{
				nameSet.add(ext->name());
			}
			ext->enable(revision);
		}
		else if (ext->mode() == GveExtMode::Required) 
		{
			LOG_WARN("Required Vulkan extension %s not supported", ext->name());
			allRequiredEnabled = false;
			continue;
		}
	}

	return allRequiredEnabled;
}


GveNameList GveNameSet::toNameList() const 
{
	GveNameList nameList;
	for (const auto& pair : m_names)
	{
		nameList.add(pair.first.c_str());
	}
	return nameList;
}


GveNameSet GveNameSet::enumInstanceLayers() 
{
	GveNameSet nameSet;
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


GveNameSet GveNameSet::enumInstanceExtensions() 
{
	GveNameSet nameSet;
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


GveNameSet GveNameSet::enumDeviceExtensions(VkPhysicalDevice device) 
{
	GveNameSet nameSet;
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


}  // namespace gve