#pragma once
#include "GPCS4Common.h"
#include <string>
#include <mutex>
#include <condition_variable>
#include "sce_kernel_types.h"


class CSceEventFlag
{
public:
	CSceEventFlag(const std::string& name, uint attr, uint64 initPattern);
	~CSceEventFlag();

	int Set(uint64 bitPattern);

	int Wait(uint64 bitPattern, uint mode, uint64* pResultPat, SceKernelUseconds* pTimeout);

	int Poll(uint64 bitPattern, uint mode, uint64* pResultPat);

	int Clear(uint64 bitPattern);

	int Cancel(uint64 setPattern, int* pNumWaitThreads);

private:
	bool IsConditionMet(uint mode, uint64 bitPattern);
	bool IsSingleMode();

private:
	uint m_attr;
	std::string m_name;
	uint64 m_bitPattern;
	std::mutex m_mutex;
	std::condition_variable m_cond;
	bool m_anyWaiting;
};

