#pragma once

#include "GnmCommon.h"

enum class GnmMemoryAccess : uint32_t
{
	None,  // Haven't been accessed yet.
	Read,
	Write
};

struct GnmMemoryRange
{
	void* start = nullptr;
	void* end   = nullptr;
};

class GnmResourceMemory
{
	constexpr static uint32_t ForceUpdateCount = 10;

public:
	GnmResourceMemory(void* start, uint32_t size);
	~GnmResourceMemory();

	const GnmMemoryRange& range() const;

	GnmMemoryAccess access() const;

	void setAccess(GnmMemoryAccess access);

	bool forceUpdate();

	void reset();

private:
	GnmMemoryRange  m_range;
	GnmMemoryAccess m_access;
	uint32_t        m_accessCount;
};
