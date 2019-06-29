#pragma once
#include "GPCS4Common.h"
#include <string>
#include <mutex>
#include <condition_variable>

// TODO:
// this is a very simple implementation
// thread queue feature is not supported yet

class CSceSemaphore
{
public:
	CSceSemaphore(const std::string& name, int initCount, int maxCount, uint mode);
	~CSceSemaphore();

	int Signal(int count);

	int Poll(int count);

	// microseconds
	int Wait(int count, uint* pTimeOut);

	int Cancel(int setCount, uint* pNumWaitThreads);


private:
	std::string m_name;
	std::mutex m_mutex;
	std::condition_variable m_cond;
	int m_count;
	int m_maxCount;
};


