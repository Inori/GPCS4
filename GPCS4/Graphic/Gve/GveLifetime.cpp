#include "GveLifetime.h"

namespace gve
{;
	
GveLifetimeTracker::GveLifetimeTracker()
{

}

GveLifetimeTracker::~GveLifetimeTracker()
{

}

void GveLifetimeTracker::trackResource(RcPtr<GveGpuResource>&& rc)
{
	rc->acquire();
	m_resources.emplace_back(std::move(rc));
}

void GveLifetimeTracker::reset()
{
	for (auto& res : m_resources)
	{
		res->release();
	}
	m_resources.clear();
}

}  // namespace gve