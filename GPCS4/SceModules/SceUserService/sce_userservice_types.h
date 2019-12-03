#pragma once


typedef struct UserServiceInit {
	int priority;
} UserServiceInit;

typedef enum UserEventType {
	USER_EVENT_TYPE_LOGIN = 0,
	USER_EVENT_TYPE_LOGOUT
} UserEventType;

typedef struct SceUserServiceEvent {
	UserEventType eventType;
	SceUserServiceUserId userId;
} SceUserServiceEvent;
