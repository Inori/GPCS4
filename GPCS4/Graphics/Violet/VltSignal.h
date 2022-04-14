#pragma once

#include "VltCommon.h"
#include "UtilSync.h"

namespace sce::vlt
{
	/**
     * \brief Signal tracker
     */
	class VltSignalTracker
	{

	public:
		VltSignalTracker();
		~VltSignalTracker();

		/**
         * \brief Adds a signal to track
         *
         * \param [in] signal The signal
         * \param [in] value Target value
         */
		void add(const Rc<util::sync::Signal>& signal, uint64_t value);

		/**
         * \brief Notifies tracked signals
         */
		void notify();

		/**
         * \brief Resets signal tracker
         */
		void reset();

	private:
		std::vector<std::pair<Rc<util::sync::Signal>, uint64_t>> m_signals;
	};
}  // namespace sce::vlt