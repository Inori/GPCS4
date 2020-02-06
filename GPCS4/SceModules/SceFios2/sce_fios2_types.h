#pragma once

#include "GPCS4Common.h"



#define SCE_FIOS_HANDLE_INVALID 0

typedef int32_t SceFiosHandle;
typedef SceFiosHandle SceFiosOp;
typedef int64_t SceFiosTime;
typedef uint8_t SceFiosOpEvent;

typedef int (*SceFiosOpCallback)(void* pContext, SceFiosOp op, SceFiosOpEvent event, int err);

struct SceFiosOpAttr
{
	SceFiosTime deadline;
	SceFiosOpCallback pCallback;
	void* pCallbackContext;
	int32_t priority : 8;
	uint32_t opflags : 24;
	uint32_t userTag;
	void* userPtr;
	void* pReserved;
};