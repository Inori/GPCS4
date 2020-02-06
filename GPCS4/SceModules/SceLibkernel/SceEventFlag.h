#pragma once
#include "GPCS4Common.h"
#include <string>
#include <mutex>
#include <condition_variable>
#include "sce_kernel_types.h"


class CSceEventFlag
{
public:
	CSceEventFlag(const std::string& name, uint32_t attr, uint64_t initPattern);
	~CSceEventFlag();

	int Set(uint64_t bitPattern);

	int Wait(uint64_t bitPattern, uint32_t mode, uint64_t* pResultPat, SceKernelUseconds* pTimeout);

	int Poll(uint64_t bitPattern, uint32_t mode, uint64_t* pResultPat);

	int Clear(uint64_t bitPattern);

	int Cancel(uint64_t setPattern, int* pNumWaitThreads);

private:
	bool IsConditionMet(uint32_t mode, uint64_t bitPattern);
	bool IsSingleMode();

private:
	uint32_t m_attr;
	std::string m_name;
	uint64_t m_bitPattern;
	std::mutex m_mutex;
	std::condition_variable m_cond;
	bool m_anyWaiting;
};

