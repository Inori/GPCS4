#pragma once
#include "GPCS4Common.h"
#include "pthreads4w/pthread.h"
#include <memory>

class CGameThread
{
public:
	typedef void (PS4API *PFUNC_ExitFunction)();
	CGameThread(void* pFunc, void* pArg1, PFUNC_ExitFunction pArg2);  //mainly used for main thread
	~CGameThread() = default;

	bool Start();

	bool Join(void** ppRet);

	bool Cancel();
private:
	typedef void* (PS4API *PFUNC_GameTheadEntry)(void* pArg1, PFUNC_ExitFunction pArg2);

	struct PTHREAD_START_PARAM
	{
		CGameThread* pThis;
	};

private:
	static void* ThreadFunc(void* pArg);
	static void* RunGameThread(CGameThread* pThis);
#ifdef GPCS4_WINDOWS
	__declspec(dllexport)
#endif // GPCS4_WINDOWS
	static void* EntryTwoParam(void* pFunc, void* pArg1, PFUNC_ExitFunction pExitFunction);
private:
	pthread_t m_nTid;
	void* m_pFunc;
	void* m_pUserArg;
	PFUNC_ExitFunction m_pUserArg2;
	std::unique_ptr<PTHREAD_START_PARAM> m_pStartParam;
};

