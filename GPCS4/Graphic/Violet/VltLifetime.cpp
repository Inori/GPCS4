#include "VltLifetime.h"

namespace vlt
{;
	
VltLifetimeTracker::VltLifetimeTracker()
{

}

VltLifetimeTracker::~VltLifetimeTracker()
{

}

void VltLifetimeTracker::trackResource(RcPtr<VltGpuResource>&& rc)
{
	rc->acquire();
	m_resources.emplace_back(std::move(rc));
}

void VltLifetimeTracker::reset()
{
	for (auto& res : m_resources)
	{
		res->release();
	}
	m_resources.clear();
}

}  // namespace vlt