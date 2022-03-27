#include "VltDeviceFilter.h"

namespace sce::vlt
{
	VltDeviceFilter::VltDeviceFilter(VltDeviceFilterFlags flags) :
		m_flags(flags)
	{
		// m_matchDeviceName = env::getEnvVar("DXVK_FILTER_DEVICE_NAME");

		if (m_matchDeviceName.size() != 0)
			m_flags.set(VltDeviceFilterFlag::MatchDeviceName);
	}

	VltDeviceFilter::~VltDeviceFilter()
	{
	}

	bool VltDeviceFilter::testAdapter(const VkPhysicalDeviceProperties& properties) const
	{
		if (properties.apiVersion < VK_MAKE_VERSION(1, 3, 0))
		{
			Logger::warn(util::str::formatex("Skipping Vulkan 1.3 adapter: ", properties.deviceName));
			return false;
		}

		if (m_flags.test(VltDeviceFilterFlag::MatchDeviceName))
		{
			if (std::string(properties.deviceName).find(m_matchDeviceName) == std::string::npos)
				return false;
		}

		if (m_flags.test(VltDeviceFilterFlag::SkipCpuDevices))
		{
			if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_CPU)
			{
				Logger::warn(util::str::format("Skipping CPU adapter: ", properties.deviceName));
				return false;
			}
		}

		return true;
	}
}  // namespace sce::vlt