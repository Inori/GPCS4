#pragma once

#include "GveCommon.h"
#include "GveGpuResource.h"

#include <vector>

namespace gve
{;
	
class GveLifetimeTracker
{

public:

	GveLifetimeTracker();
	~GveLifetimeTracker();

	void trackResource(RcPtr<GveGpuResource>&& rc);

	void reset();

private:

	std::vector<RcPtr<GveGpuResource>> m_resources;

};


}  // namespace gve