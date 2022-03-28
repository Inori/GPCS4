#pragma once

#include "VltCommon.h"
#include "VltExtension.h"


namespace sce::vlt
{
	class VltAdapter;

	/**
	* \brief VLT instance
	* 
	* Manages a Vulkan instance and stores a list
	* of adapters. This also provides methods for
	* device creation.
	*/
	class VltInstance : public RcObject
	{

	public:
		VltInstance();
		virtual ~VltInstance();

		/**
		 * \brief Vulkan instance handle
		 * \returns The instance handle
		 */
		VkInstance handle()
		{
			return m_instance;
		}

		/**
		 * \brief Number of adapters
		 * 
		 * \returns The number of adapters
		 */
		size_t adapterCount()
		{
			return m_adapters.size();
		}

		/**
		 * \brief Retrieves an adapter
		 * 
		 * Note that the adapter does not hold
		 * a hard reference to the instance.
		 * \param [in] index Adapter index
		 * \returns The adapter, or \c nullptr.
		 */
		Rc<VltAdapter> enumAdapters(
			uint32_t index) const;


		/**
		 * \brief Enabled instance extensions
		 * \returns Enabled instance extensions
		 */
		const VltInstanceExtensions& extensions() const
		{
			return m_extensions;
		}

	private:
		std::vector<VltExt> getPlatformExtensions();

		VkInstance createInstance();

		std::vector<Rc<VltAdapter>> queryAdapters();

		static void logNameList(const VltNameList& names);

	private:
		VkInstance            m_instance = VK_NULL_HANDLE;
		VltInstanceExtensions m_extensions;

		std::vector<Rc<VltAdapter>> m_adapters;
	};


}  // namespace sce::vlt
