#pragma once
#include "user_service_defs.h"

typedef struct SceUserServiceInitializeParams {
	int priority;
} SceUserServiceInitializeParams;

typedef enum SceUserServiceEventType {
	SCE_USER_SERVICE_EVENT_TYPE_LOGIN = 0,
	SCE_USER_SERVICE_EVENT_TYPE_LOGOUT
} SceUserServiceEventType;

typedef struct SceUserServiceEvent {
	SceUserServiceEventType eventType;
	SceUserServiceUserId userId;
} SceUserServiceEvent;

typedef struct SceUserServiceLoginUserIdList
{
	SceUserServiceUserId userId[SCE_USER_SERVICE_MAX_LOGIN_USERS];
} SceUserServiceLoginUserIdList;
