#pragma once

#include "GveCommon.h"

#include <atomic>

namespace gve
{;
	

// Trace wether a resource is being used by GPU or not

class GveGpuResource : public RcObject
{
public:
	
	virtual ~GveGpuResource();

	bool isInUse() const;

	void acquire();

	void release();

private:
	std::atomic_uint32_t m_useCount = 0;
};

}  // namespace gve