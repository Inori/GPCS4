#include "sce_userservice.h"
#include "sce_userservice_error.h"

// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs



//////////////////////////////////////////////////////////////////////////
// library: libSceUserService
//////////////////////////////////////////////////////////////////////////

static bool g_init = false;

#define PRIO_FIFO_HIGHEST    256
#define PRIO_FIFO_LOWEST     767

int PS4API sceUserServiceInitialize(const UserServiceInit *initParams)
{
	LOG_SCE_DUMMY_IMPL();

	int returnVal = -1;

	if (initParams->priority > PRIO_FIFO_LOWEST ||
		initParams->priority < PRIO_FIFO_HIGHEST)
	{
		returnVal = USER_ERROR_INVALID_ARGUMENT;
	}

	else if (!g_init)
	{
		g_init = true;
		returnVal = SCE_OK;
	}

	else
	{
		returnVal = USER_ERROR_ALREADY_INITIALIZED;
	}

	return returnVal;
}


int PS4API sceUserServiceTerminate(void)
{
	LOG_SCE_DUMMY_IMPL();

	int returnVal = -1;

	if (!g_init)
	{
		returnVal = USER_ERROR_NOT_INITIALIZED;
	}

	else
	{
		g_init = false;
		returnVal = SCE_OK;
	}
	
	return returnVal;
}


int PS4API sceUserServiceGetInitialUser(SceUserServiceUserId *userId)
{
	LOG_SCE_DUMMY_IMPL();

	int returnVal = -1;

	if (!g_init)
	{
		returnVal = USER_ERROR_NOT_INITIALIZED;
	}

	else
	{
		*userId = SCE_DUMMY_USERID;
		returnVal = SCE_OK;
	}

	return returnVal;
}


int PS4API sceUserServiceGetUserName(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceUserServiceGetEvent(SceUserServiceEvent* event)
{
	LOG_SCE_TRACE("event %p", event);
	event->eventType = USER_EVENT_TYPE_LOGOUT;
	event->userId = SCE_DUMMY_USERID;
	return USER_ERROR_NO_EVENT;
}


