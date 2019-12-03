#pragma once
#include <stdint.h>

struct SceErrorDialogParam
{
	uint32_t size;
	uint32_t errorCode;
	uint32_t userId;
	uint32_t reserved;
};

enum SceErrorDialogStatus
{
	SCE_ERROR_DIALOG_STATUS_NONE = 0,
	SCE_ERROR_DIALOG_STATUS_INITIALIZED = 1,
	SCE_ERROR_DIALOG_STATUS_RUNNING = 2,
	SCE_ERROR_DIALOG_STATUS_FINISHED = 3
};