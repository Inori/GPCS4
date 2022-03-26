#pragma once

#include "VltCommon.h"

namespace sce::vlt
{
	
  /**
   * \brief Device filter flags
   * 
   * The device filter flags specify which device
   * properties are considered when testing adapters.
   * If no flags are set, all devices pass the test.
   */
	enum class VltDeviceFilterFlag
	{
		MatchDeviceName = 0,
		SkipCpuDevices  = 1,
	};

	using VltDeviceFilterFlags = util::Flags<VltDeviceFilterFlag>;

	/**
   * \brief DXVK device filter
   * 
   * Used to select specific Vulkan devices to use
   * with DXVK. This may be useful for games which
   * do not offer an option to select the correct
   * device.
   */
	class VltDeviceFilter
	{

	public:
		VltDeviceFilter(VltDeviceFilterFlags flags);
		~VltDeviceFilter();

		/**
     * \brief Tests an adapter
     * 
     * \param [in] properties Adapter properties
     * \returns \c true if the test passes
     */
		bool testAdapter(
			const VkPhysicalDeviceProperties& properties) const;

	private:
		VltDeviceFilterFlags m_flags;

		std::string m_matchDeviceName;
	};

}  // namespace sce::vlt