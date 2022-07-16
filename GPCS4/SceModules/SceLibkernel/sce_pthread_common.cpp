#include "sce_pthread_common.h"
#include "sce_libkernel.h"
#include "Emulator/TLSHandler.h"

LOG_CHANNEL(SceModules.SceLibkernel.pthreadcommon);

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
		TLSManager* tlsMgr = TLSManager::instance();
		tlsMgr->notifyThreadExit();

	} while (false);
	if (param)
	{
		delete param;
	}
	return ret;
}
