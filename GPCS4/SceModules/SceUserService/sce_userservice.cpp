#include "sce_userservice.h"
#include "sce_userservice_error.h"

// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs



//////////////////////////////////////////////////////////////////////////
// library: libSceUserService
//////////////////////////////////////////////////////////////////////////


int PS4API sceUserServiceInitialize(const SceUserServiceInitializeParams *initParams)
{
	LOG_SCE_DUMMY_IMPL();
	return SCE_OK;
}


int PS4API sceUserServiceTerminate(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceUserServiceGetInitialUser(SceUserServiceUserId *userId)
{
	LOG_SCE_DUMMY_IMPL();
	*userId = SCE_DUMMY_USERID;
	return SCE_OK;
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


