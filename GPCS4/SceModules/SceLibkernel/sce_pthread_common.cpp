#include "sce_pthread_common.h"
#include "sce_libkernel.h"
#include "Emulator/TLSHandler.h"

MapSlot<pthread_t, decltype(isEmptyPthreadT)> g_threadSlot(SCE_THREAD_COUNT_MAX, isEmptyPthreadT);

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

		CTLSHandler::NotifyThreadCreate(tid);

		PFUNC_PS4_THREAD_ENTRY pSceEntry = (PFUNC_PS4_THREAD_ENTRY)param->entry;
		ret = pSceEntry(param->arg);

		CTLSHandler::NotifyThreadExit(tid);

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
