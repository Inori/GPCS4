#pragma once
#include "GPCS4Common.h"
#include "pthreads4w/pthread.h"
#include <memory>

class CGameThread
{
public:
	CGameThread(void* pFunc, void* pArg);
	CGameThread(void* pFunc, void* pArg1, void* pArg2);  //mainly used for main thread
	~CGameThread();

	bool Start();

	bool Join(void** ppRet);

	bool Cancel();
private:
	typedef void* (PS4API *PFUNC_GameTheadEntry)(void* pArg);
	typedef void* (PS4API *PFUNC_GameTheadEntryTwoParam)(void* pArg1, void* pArg2);

	struct PTHREAD_START_PARAM
	{
		CGameThread* pThis;
	};

private:
	static void* ThreadFunc(void* pArg);
	static void* RunGameThread(CGameThread* pThis);
	static void* EntryOneParam(void* pFunc, void* pArg);
	static void* EntryTwoParam(void* pFunc, void* pArg1, void* pArg2);
private:
	pthread_t m_nTid;
	void* m_pFunc;
	void* m_pUserArg;
	void* m_pUserArg2;
	std::unique_ptr<PTHREAD_START_PARAM> m_pStartParam;
};

