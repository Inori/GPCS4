#pragma once

#include "VltCommon.h"
#include "VltResource.h"

namespace sce::vlt
{
	/**
     * \brief DXVK lifetime tracker
     * 
     * Maintains references to a set of resources. This is
     * used to guarantee that resources are not destroyed
     * or otherwise accessed in an unsafe manner until the
     * device has finished using them.
     */
	class VltLifetimeTracker
	{

	public:
		VltLifetimeTracker();
		~VltLifetimeTracker();

		/**
         * \brief Adds a resource to track
         * \param [in] rc The resource to track
         */
		template <VltAccess Access>
		void trackResource(Rc<VltResource>&& rc)
		{
			rc->acquire(Access);
			m_resources.emplace_back(std::move(rc), Access);
		}

		/**
         * \brief Releases resources
         *
         * Marks all tracked resources as unused.
         */
		void notify();

		/**
         * \brief Resets the command list
         * 
         * Called automatically by the device when
         * the command list has completed execution.
         */
		void reset();

	private:
		std::vector<std::pair<Rc<VltResource>, VltAccess>> m_resources;
		bool                                               m_notified = false;
	};

}  // namespace sce::vlt