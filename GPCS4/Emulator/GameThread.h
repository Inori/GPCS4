#pragma once
#include "GPCS4Common.h"
#include "pthreads4w/pthread.h"
#include <memory>

class CGameThread
{
public:
	typedef void (PS4API *PFUNC_ExitFunction)();
	CGameThread(void* pFunc, void* pArg1, PFUNC_ExitFunction pExitFunc);  //mainly used for main thread
	~CGameThread() = default;

	bool Start();

	bool Join(void** ppRet);

	bool Cancel();
private:
	typedef void* (PS4API *PFUNC_GameTheadEntry)(void* pArg, PFUNC_ExitFunction pExit);

	struct PTHREAD_START_PARAM
	{
		CGameThread* pThis;
	};

private:
	static void* ThreadFunc(void* pArg);
	static void* RunGameThread(CGameThread* pThis);
	// PS4EXPORT makes it easy to be found in IDA
	PS4EXPORT static void* EntryStart(void* pFunc, void* pArg, PFUNC_ExitFunction pExitFunction);
private:
	pthread_t m_nTid;
	void* m_pFunc;
	void* m_pUserArg;
	PFUNC_ExitFunction m_pExitFunc;
	std::unique_ptr<PTHREAD_START_PARAM> m_pStartParam;
};

