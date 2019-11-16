#include "GveGpuResource.h"

namespace gve
{;

GveGpuResource::~GveGpuResource()
{
}

bool GveGpuResource::isInUse() const
{
	return m_useCount.load() != 0;
}

void GveGpuResource::acquire()
{
	++m_useCount;
}

void GveGpuResource::release()
{
	--m_useCount;
}

}  // namespace gve