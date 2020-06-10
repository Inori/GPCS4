#pragma once

#include "GnmCommon.h"
#include "GnmResourceMemory.h"

#include "UtilException.h"

#include <set>
#include <functional>

struct GnmMemoryCompare
{
	using is_transparent = void;
								
	bool operator()(GnmMemoryRange const& lhs, GnmMemoryRange const& rhs) const
	{
		// lRange and rRange is supposed to be not overlapped.
		return reinterpret_cast<uintptr_t>(lhs.start) < reinterpret_cast<uintptr_t>(lhs.start);
	}

	bool operator()(const void* const address, GnmMemoryRange const& range) const
	{
		return reinterpret_cast<uintptr_t>(address) >= reinterpret_cast<uintptr_t>(range.start) &&
			   reinterpret_cast<uintptr_t>(address) < reinterpret_cast<uintptr_t>(range.start) + range.size;
	}
};

struct GnmMemoryCallback
{
	using MemoryAccessFunction = std::function<void(const GnmMemoryRange& range)>;
	MemoryAccessFunction read;
	MemoryAccessFunction write;
};

class GnmMemoryMonitor
{
public:
	GnmMemoryMonitor(const GnmMemoryCallback& callback);
	~GnmMemoryMonitor();

	void traceMemory(GnmMemoryRange& block);

	void untraceMemory(GnmMemoryRange& block);

private:
	void install();

	static UtilException::ExceptionAction
	exceptionCallbackStatic(UtilException::ExceptionRecord* record, void* param);

	static UtilException::ExceptionAction
	exceptionCallback(UtilException::ExceptionRecord* record);

private:
	std::set<std::reference_wrapper<GnmMemoryRange>, GnmMemoryCompare> m_blockRecords;
	GnmMemoryCallback                                                  m_callback;
};

