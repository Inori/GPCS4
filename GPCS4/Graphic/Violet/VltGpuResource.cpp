#include "VltGpuResource.h"

namespace vlt
{;

VltGpuResource::~VltGpuResource()
{
}

bool VltGpuResource::busy() const
{
	return m_useCount.load() != 0;
}

void VltGpuResource::acquire()
{
	++m_useCount;
}

void VltGpuResource::release()
{
	--m_useCount;
}

}  // namespace vlt