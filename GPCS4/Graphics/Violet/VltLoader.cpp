#include "VltLoader.h"

namespace sce::vlt::vk
{
	InstanceLoader::InstanceLoader(bool owned, VkInstance instance) :
		m_instance(instance), m_owned(owned)
	{
	}

	PFN_vkVoidFunction InstanceLoader::sym(const char* name) const
	{
		return vkGetInstanceProcAddr(m_instance, name);
	}

	DeviceLoader::DeviceLoader(bool owned, VkInstance instance, VkDevice device) :
		m_getDeviceProcAddr(reinterpret_cast<PFN_vkGetDeviceProcAddr>(
			vkGetInstanceProcAddr(instance, "vkGetDeviceProcAddr"))),
		m_device(device), m_owned(owned)
	{
	}

	PFN_vkVoidFunction DeviceLoader::sym(const char* name) const
	{
		return m_getDeviceProcAddr(m_device, name);
	}

	InstanceFn::InstanceFn(bool owned, VkInstance instance) :
		InstanceLoader(owned, instance)
	{
	}
	InstanceFn::~InstanceFn()
	{
		if (m_owned)
			vkDestroyInstance(m_instance, nullptr);
	}

	DeviceFn::DeviceFn(bool owned, VkInstance instance, VkDevice device) :
		DeviceLoader(owned, instance, device)
	{
	}
	DeviceFn::~DeviceFn()
	{
		if (m_owned)
			vkDestroyDevice(m_device, nullptr);
	}
}  // namespace sce::vlt