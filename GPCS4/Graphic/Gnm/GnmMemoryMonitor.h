#pragma once

#include "GnmCommon.h"
#include "GnmResourceMemory.h"

#include "UtilException.h"

#include <set>

struct GnmResourceMemoryCompare
{
	using is_transparent = void;  
								
	bool operator()(GnmResourceMemory const& lhs, GnmResourceMemory const& rhs) const
	{
		const auto& lRange = lhs.range();
		const auto&  rRange = rhs.range();
		// lRange and rRange is supposed to be not overlapped.
		return reinterpret_cast<uintptr_t>(lRange.start) < reinterpret_cast<uintptr_t>(rRange.start) &&
			   reinterpret_cast<uintptr_t>(lRange.end) < reinterpret_cast<uintptr_t>(rRange.end);
	}

	bool operator()(void* address, GnmResourceMemory const& block) const
	{
		const auto& range = block.range();
		return reinterpret_cast<uintptr_t>(address) >= reinterpret_cast<uintptr_t>(range.start) &&
			   reinterpret_cast<uintptr_t>(address) < reinterpret_cast<uintptr_t>(range.end);
	}
};

class GnmMemoryMonitor
{
public:
	GnmMemoryMonitor();
	~GnmMemoryMonitor();

	void traceMemory(GnmResourceMemory& block);

	void untraceMemory(GnmResourceMemory& block);

private:
	void                                  install();
	static UtilException::ExceptionAction exceptionCallbackStatic(UtilException::ExceptionRecord* record, void* param);
	static UtilException::ExceptionAction exceptionCallback(UtilException::ExceptionRecord* record);

private:
	std::set<std::reference_wrapper<GnmResourceMemory>, GnmResourceMemoryCompare> m_blockRecords;
};

