#include "sce_pthread_common.h"
#include "sce_libkernel.h"
#include "Emulator/TLSHandler.h"

LOG_CHANNEL(SceModules.SceLibkernel.pthreadcommon);

MapSlot<pthread_t, isEmptyPthread, isEqualPthread> g_threadSlot(SCE_THREAD_COUNT_MAX);


bool isEmptyPthread(const pthread_t& pt)
{
	return pt.p == NULL && pt.x == 0;
}

bool isEqualPthread(const pthread_t& lhs, const pthread_t& rhs)
{
	return lhs.p == rhs.p && lhs.x == rhs.x;
}


void* newThreadWrapper(void* arg)
{
	void* ret = NULL;
	SCE_THREAD_PARAM* param = (SCE_THREAD_PARAM*)arg;
	do 
	{
		if (!param)
		{
			break;
		}

		ScePthread tid = scePthreadSelf();
		LOG_DEBUG("new sce thread created %d", tid);

		PFUNC_PS4_THREAD_ENTRY pSceEntry = (PFUNC_PS4_THREAD_ENTRY)param->entry;
		ret = pSceEntry(param->arg);

		// release tls data
		TLSManager* tlsMgr = TLSManager::GetInstance();
		tlsMgr->notifyThreadExit();

		// do clear
		pthread_t emptyPt = { 0 };
		g_threadSlot.SetItemAt(tid, emptyPt);

	} while (false);
	if (param)
	{
		delete param;
	}
	return ret;
}
