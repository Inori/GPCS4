#include "GameThread.h"

CGameThread::CGameThread(void* pFunc, void* pArg):
	m_pFunc(pFunc),
	m_pUserArg(pArg)
{

}

CGameThread::CGameThread(void* pFunc, void* pArg1, void* pArg2):
m_pFunc(pFunc),
m_pUserArg(pArg1),
m_pUserArg2(pArg2)
{

}

CGameThread::~CGameThread()
{
	
}

bool CGameThread::Start()
{
	bool bRet = false;
	do 
	{
		if (!m_pFunc)
		{
			break;
		}
		
		m_pStartParam = std::make_unique<PTHREAD_START_PARAM>();
		m_pStartParam->pThis = this;

		int nRet = pthread_create(&m_nTid, NULL, CGameThread::ThreadFunc, m_pStartParam.get());
		if (nRet != 0)
		{
			break;
		}

		bRet = true;
	} while (false);
	return bRet;
}

bool CGameThread::Join(void** ppRet)
{
	int nRet = pthread_join(m_nTid, ppRet);
	return (nRet == 0);
}

bool CGameThread::Cancel()
{
	int nRet = pthread_cancel(m_nTid);
	return (nRet == 0);
}

void* CGameThread::ThreadFunc(void* pArg)
{
	PTHREAD_START_PARAM* pParam = (PTHREAD_START_PARAM*)pArg;
	int nRet = -1;
	do 
	{
		if (!pParam)
		{
			break;
		}

		CGameThread* pThis = pParam->pThis;
		if (!pThis)
		{
			break;
		}
	
		void* pRet = RunGameThread(pThis);
	
		pthread_exit(pRet);

		nRet = 0;
	} while (false);
	return (void*)nRet;
}

void* CGameThread::RunGameThread(CGameThread* pThis)
{
	void* pRet = NULL;
	do 
	{
		void* pEntry = pThis->m_pFunc;
		if (!pEntry)
		{
			break;
		}

		if (pThis->m_pUserArg2 == NULL)  // likely
		{
			pRet = EntryOneParam(pEntry, pThis->m_pUserArg);
		}
		else
		{
			pRet = EntryTwoParam(pEntry, pThis->m_pUserArg, pThis->m_pUserArg2);
		}

	} while (false);
	return pRet;
}

void* CGameThread::EntryOneParam(void* pFunc, void* pArg)
{
	return ((PFUNC_GameTheadEntry)pFunc)(pArg);
}

void* CGameThread::EntryTwoParam(void* pFunc, void* pArg1, void* pArg2)
{
	return ((PFUNC_GameTheadEntryTwoParam)pFunc)(pArg1, pArg2);
}
