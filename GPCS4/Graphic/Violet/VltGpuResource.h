#pragma once

#include "VltCommon.h"

#include <atomic>

namespace vlt
{;
	

// Trace wether a resource is being used by GPU or not

class VltGpuResource : public RcObject
{
public:
	
	virtual ~VltGpuResource();

	bool busy() const;

	void acquire();

	void release();

private:
	std::atomic_uint32_t m_useCount = 0;
};

}  // namespace vlt