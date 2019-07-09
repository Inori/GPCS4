#pragma once


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
