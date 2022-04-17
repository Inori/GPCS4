#include "SceResourceTracker.h"

LOG_CHANNEL(Graphic.Sce.SceResourceTracker);

namespace sce
{

	SceResourceTracker::SceResourceTracker()
	{
	}

	SceResourceTracker::~SceResourceTracker()
	{
	}

	SceResource* SceResourceTracker::find(void* mem)
	{
		std::lock_guard<util::sync::Spinlock> guard(m_lock);

		SceResource* result = nullptr;
		auto iter = m_resources.lower_bound(mem);
		if (iter != m_resources.end())
		{
			// detect if mem is within a resource's memory range
			auto& res = iter->second;
			void*       start = res.cpuMemory();
			void*       end   = reinterpret_cast<uint8_t*>(start) + res.size();
			if (mem	>= start && mem < end)
			{
				result = &res;
			}
		}

		return result;
	}

	void SceResourceTracker::reset()
	{
		std::lock_guard<util::sync::Spinlock> guard(m_lock);

		m_resources.clear();
	}

}  // namespace sce