#include "sce_userservice.h"
#include "sce_userservice_error.h"

// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs



//////////////////////////////////////////////////////////////////////////
// library: libSceUserService
//////////////////////////////////////////////////////////////////////////

static bool init = false;

#define SCE_KERNEL_PRIO_FIFO_HIGHEST    256
#define SCE_KERNEL_PRIO_FIFO_LOWEST     767

int PS4API sceUserServiceInitialize(const SceUserServiceInitializeParams *initParams)
{
	LOG_FIXME("Not implemented");

	if (initParams->priority > SCE_KERNEL_PRIO_FIFO_LOWEST ||
		initParams->priority < SCE_KERNEL_PRIO_FIFO_HIGHEST)
	{
		return SCE_USER_SERVICE_ERROR_INVALID_ARGUMENT;
	}

	if (!init)
	{
		init = true;
		return SCE_OK;
	}

	else
	{
		return SCE_USER_SERVICE_ERROR_ALREADY_INITIALIZED;
	}
}


int PS4API sceUserServiceTerminate(void)
{
	LOG_FIXME("Not implemented");
	if(0)
	//if (!init)
	{
		return SCE_USER_SERVICE_ERROR_NOT_INITIALIZED;
	}

	else
	{
		init = false;
		return SCE_OK;
	}
	
}


int PS4API sceUserServiceGetInitialUser(SceUserServiceUserId *userId)
{
	if (!init)
	{
		return SCE_USER_SERVICE_ERROR_NOT_INITIALIZED;
	}

	else
	{
		*userId = SCE_DUMMY_USERID;
		return SCE_OK;
	}
}


int PS4API sceUserServiceGetUserName(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceUserServiceGetEvent(SceUserServiceEvent* event)
{
	LOG_SCE_TRACE("event %p", event);
	event->eventType = SCE_USER_SERVICE_EVENT_TYPE_LOGIN;
	event->userId = SCE_DUMMY_USERID;
	return SCE_USER_SERVICE_ERROR_NO_EVENT;
}


