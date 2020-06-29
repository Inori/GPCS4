#pragma once

#include "GnmCommon.h"
#include "GnmResourceMemory.h"

#include "UtilException.h"

#include <set>
#include <functional>

struct GnmMemoryCompare
{
	using is_transparent = void;

	bool operator()(GnmResourceMemory const& lhs, GnmResourceMemory const& rhs) const
	{
		// lRange and rRange is supposed to be not overlapped.
		auto lRange = lhs.range();
		auto rRange = rhs.range();
		return reinterpret_cast<uintptr_t>(lRange.start) < reinterpret_cast<uintptr_t>(rRange.start);
	}

	bool operator()(const void* const address, GnmResourceMemory const& block) const
	{
		auto range = block.range();
		return reinterpret_cast<uintptr_t>(address) >= reinterpret_cast<uintptr_t>(range.start) &&
			   reinterpret_cast<uintptr_t>(address) < reinterpret_cast<uintptr_t>(range.start) + range.size;
	}
};

struct GnmMemoryCallback
{
	using MemoryAccessFunction = std::function<void(const GnmResourceMemory& range)>;
	MemoryAccessFunction read;
	MemoryAccessFunction write;
};

class GnmMemoryMonitor
{
	using MemoryBlockSet = std::set<std::reference_wrapper<GnmResourceMemory>, GnmMemoryCompare>;

public:
	GnmMemoryMonitor(const GnmMemoryCallback& callback);
	~GnmMemoryMonitor();

	void traceMemory(GnmResourceMemory& block);

	void untraceMemory(GnmResourceMemory& block);

private:
	void install();

	void monitor(GnmResourceMemory& block);
	void neglect(GnmResourceMemory& block);

	GnmMemoryRange getMonitorRange(GnmMemoryRange& block);

	static UtilException::ExceptionAction
	exceptionCallbackStatic(UtilException::ExceptionRecord* record, void* param);

	UtilException::ExceptionAction
	exceptionCallback(UtilException::ExceptionRecord* record);

private:
	MemoryBlockSet    m_blockRecords;
	GnmMemoryCallback m_callback;
};

