#pragma once

#include "SceCommon.h"
#include "SceResource.h"
#include "UtilSync.h"
#include "Violet/VltRc.h"

#include <map>
#include <variant>

namespace sce
{
	namespace vlt
	{
		class VltContext;
	}  // namespace vlt


	/**
	 * \brief Global resource tracker.
	 * 
	 * Use to query vulkan object by Gnm resource memory.
	 * It's thread safe.
	 * 
	 */
	class SceResourceTracker
	{
		// use std::greater as the compare function
		// so that we can find the nearest smaller element 
		// with a single lower_bound call
		using SceResourceMap = std::map<void*, SceResource, std::greater<void*>>;

	public:
		SceResourceTracker();
		~SceResourceTracker();

		/**
		 * \brief Track a sce resource type.
		 */
		template <class ResType>
		std::pair<SceResourceMap::iterator, bool>
		track(ResType&& arg)
		{
			std::lock_guard<util::sync::Spinlock> guard(m_lock);

			void* cpuMem = arg.cpuMemory();
			return m_resources.emplace(cpuMem, std::forward<ResType>(arg));
		}

		/**
		 * \brief Find resource object by memory pointer
		 * 
		 * The memory is not limited to the start address of a object,
		 * it can be any address 
		 * from start to end(not included) within the object memory.
		 */
		SceResource* find(void* mem);

		/**
		 * \brief Apple pending transforms
		 */
		void flush(vlt::VltContext* context);

		/**
		 * \brief Clear all information in the tracker
		 */
		void reset();
		
	private:
		util::sync::Spinlock m_lock;
		SceResourceMap       m_resources;
	};
}  // namespace sce