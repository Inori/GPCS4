#include "GnmResourceMemory.h"

GnmResourceMemory::GnmResourceMemory()
{
}

GnmResourceMemory::GnmResourceMemory(
	void*            start,
	uint32_t         size,
	GnmMemoryProtect prot) :
	m_protect(prot)
{
	m_range.start = start;
	m_range.size  = size;
}

GnmResourceMemory::GnmResourceMemory(
	const GnmMemoryRange& range,
	GnmMemoryProtect      prot) :
	m_range(range),
	m_protect(prot)
{
}

GnmResourceMemory::~GnmResourceMemory()
{
}

const GnmMemoryRange& GnmResourceMemory::range() const
{
	return m_range;
}

void GnmResourceMemory::setRange(const GnmMemoryRange& range)
{
	m_range = range;
}

GnmMemoryAccess GnmResourceMemory::access() const
{
	return m_access;
}

void GnmResourceMemory::setAccess(GnmMemoryAccess access)
{
	m_access = access;
	++m_accessCount;
}

GnmMemoryProtect GnmResourceMemory::protect() const
{
	return m_protect;
}

void GnmResourceMemory::setProtect(GnmMemoryProtect prot)
{
	m_protect = prot;
}

bool GnmResourceMemory::pendingSync() const
{
	return m_pendingSync;
}

void GnmResourceMemory::setPendingSync(bool sync)
{
	m_pendingSync = sync;
}

bool GnmResourceMemory::forceUpdate()
{
	return m_accessCount >= ForceUpdateCount;
}

void GnmResourceMemory::reset()
{
	m_access      = GnmMemoryAccess::None;
	m_accessCount = 0;
}
