#pragma once
#include <stdint.h>

struct SceMsgDialogResult
{
	int32_t mode;
	int32_t result;
	int32_t buttonId;
	char reserved[32];
};

// This structure is not correct
struct MsgInfo
{
	int baseParam;
	size_t size;
	int mode;
	int : 32;
	void *userMsgParam;
	void *progBarParam;
	void *sysMsgParam;
	int userId;
	char reserved[40];
	int : 32;
};

enum SceCommonDialogStatus
{
	SCE_COMMON_DIALOG_STATUS_NONE = 0,
	SCE_COMMON_DIALOG_STATUS_INITIALIZED = 1,
	SCE_COMMON_DIALOG_STATUS_RUNNING = 2,
	SCE_COMMON_DIALOG_STATUS_FINISHED = 3
};