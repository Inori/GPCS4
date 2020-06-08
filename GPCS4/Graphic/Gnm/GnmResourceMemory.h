#pragma once

#include "GnmCommon.h"
#include "UtilFlag.h"

enum class GnmMemoryAttribute : uint32_t
{
	GpuRead,
	GpuWrite,
	CpuRead,
	CpuWrite
};

using GnmMemoryFlag = Flags<GnmMemoryAttribute>;


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
	GnmResourceMemory(void* start, uint32_t size, GnmMemoryFlag flag);
	~GnmResourceMemory();

	const GnmMemoryRange& range() const;

	GnmMemoryAccess access() const;

	void setAccess(GnmMemoryAccess access);

	bool forceUpdate();

	void reset();

private:
	GnmMemoryRange  m_range;
	GnmMemoryFlag   m_flag;
	GnmMemoryAccess m_access;
	uint32_t        m_accessCount;
};
