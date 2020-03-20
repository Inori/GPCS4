#pragma once

#include "VltCommon.h"
#include "VltGpuResource.h"

#include <vector>

namespace vlt
{;
	
class VltLifetimeTracker
{

public:

	VltLifetimeTracker();
	~VltLifetimeTracker();

	void trackResource(RcPtr<VltGpuResource>&& rc);

	void reset();

private:

	std::vector<RcPtr<VltGpuResource>> m_resources;

};


}  // namespace vlt