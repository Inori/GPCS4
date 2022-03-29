#include "VltLifetime.h"

namespace sce::vlt
{
	VltLifetimeTracker::VltLifetimeTracker()
	{
	}
	VltLifetimeTracker::~VltLifetimeTracker()
	{
	}

	void VltLifetimeTracker::notify()
	{
		for (const auto& resource : m_resources)
			resource.first->release(resource.second);

		m_notified = true;
	}

	void VltLifetimeTracker::reset()
	{
		// If this gets called without ever being submitted then
		// we should at least report the resources as unused
		if (!m_notified)
			this->notify();

		m_resources.clear();
	}
}  // namespace sce::vlt