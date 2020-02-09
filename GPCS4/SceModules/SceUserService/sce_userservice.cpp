#include "sce_userservice.h"
#include "sce_userservice_error.h"

// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs

LOG_CHANNEL(SceModules.SceUserService);

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


int PS4API sceUserServiceGetUserName(const SceUserServiceUserId userId, char *userName, const size_t size)
{
	LOG_SCE_TRACE("userId %d userName %p size %d", userId, userName, size);
	int ret = SCE_ERROR_UNKNOWN;
	do 
	{
		if (!userName || ! size)
		{
			ret = SCE_USER_SERVICE_ERROR_INVALID_ARGUMENT;
			break;
		}

		uint32_t nameLen = strlen(GPCS4_APP_NAME);
		if (size <= nameLen)
		{
			ret = SCE_USER_SERVICE_ERROR_BUFFER_TOO_SHORT;
			break;
		}

		strcpy(userName, GPCS4_APP_NAME);

		ret = SCE_OK;
	} while (false);
	return ret;
}


int PS4API sceUserServiceGetEvent(SceUserServiceEvent* event)
{
	LOG_SCE_TRACE("event %p", event);
	event->eventType = SCE_USER_SERVICE_EVENT_TYPE_LOGIN;
	event->userId = SCE_DUMMY_USERID;
	return SCE_USER_SERVICE_ERROR_NO_EVENT;
}

int PS4API sceUserServiceGetLoginUserIdList(SceUserServiceLoginUserIdList* userIdList)
{
	LOG_SCE_TRACE("userIdList %p", userIdList);
	int ret = SCE_ERROR_UNKNOWN;
	do
	{
		if (!userIdList)
		{
			ret = SCE_USER_SERVICE_ERROR_INVALID_ARGUMENT;
			break;
		}

		userIdList->userId[0] = SCE_DUMMY_USERID;

		for (uint32_t i = 1; i != SCE_USER_SERVICE_MAX_LOGIN_USERS; ++i)
		{
			userIdList->userId[i] = SCE_USER_SERVICE_USER_ID_INVALID;
		}

		ret = SCE_OK;
	} while (false);
	return ret;
}

