#pragma once
#include "user_service_defs.h"

struct SceUserServiceInitializeParams 
{
	int priority;
};

enum SceUserServiceEventType 
{
	SCE_USER_SERVICE_EVENT_TYPE_LOGIN = 0,
	SCE_USER_SERVICE_EVENT_TYPE_LOGOUT
};

struct SceUserServiceEvent 
{
	SceUserServiceEventType eventType;
	SceUserServiceUserId userId;
};

struct SceUserServiceLoginUserIdList
{
	SceUserServiceUserId userId[SCE_USER_SERVICE_MAX_LOGIN_USERS];
};
