#pragma once

#include <inttypes.h>

//E type for UserID
typedef int32_t SceUserServiceUserId;

//E Maximum login users at the same time
#define SCE_USER_SERVICE_MAX_LOGIN_USERS (4)

//E Maximum length of user name
#define SCE_USER_SERVICE_MAX_USER_NAME_LENGTH (16)

//E An invalid user ID
#define SCE_USER_SERVICE_USER_ID_INVALID (-1)

//E User ID specified when using a device that cannot be allocated to a specific user
#define SCE_USER_SERVICE_USER_ID_SYSTEM (0xFF)

//E User ID specified when operation of Common Dialog is not limited to specific user.
#define SCE_USER_SERVICE_USER_ID_EVERYONE (0xFE)

