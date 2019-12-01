#include "sce_errordialog.h"


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs

//////////////////////////////////////////////////////////////////////////
// library: libSceErrorDialog
//////////////////////////////////////////////////////////////////////////

DialogParam myError;
DialogStatus status = NONE;

#define ALREADY_INITIALIZED 0x80ED0002
#define NOT_INITIALIZED 0x80ED0001

//sceErrorDialogParamInitialize
// is a static inlinei function that does not need emulation

/*
static inline
void sceErrorDialogParamInitialize(SceErrorDialogParam * param)
{
	memset(param, 0x0, sizeof(SceErrorDialogParam));
	param->size = sizeof(SceErrorDialogParam);
	param->userId = SCE_USER_SERVICE_USER_ID_EVERYONE;
}
*/

DialogStatus PS4API sceErrorDialogGetStatus(void)
{
	return status;
}


int PS4API sceErrorDialogInitialize(void)
{
	if (status == NONE)
	{
		status = INITIALIZED;
		return SCE_OK;
	}

	else
	{
		return ALREADY_INITIALIZED;
	}
}


int PS4API sceErrorDialogOpen(DialogParam* param)
{
	// dont put \n at the end, LOG_SCE_TRACE
	// adds \n at the end automatically
	LOG_SCE_TRACE("SceError:");
	LOG_SCE_TRACE("   UserID: %d", myError.userId);
	LOG_SCE_TRACE("   Error: %d", myError.errorCode);
	return SCE_OK;
}


int PS4API sceErrorDialogTerminate(void)
{
	if (status != NONE)
	{
		status = NONE;
		return NOT_INITIALIZED;
	}

	else
	{
		return SCE_OK;
	}
}


DialogStatus PS4API sceErrorDialogUpdateStatus(void)
{
	return FINISHED;
}

/*
	int32_t sceErrorDialogClose()
	{
		if(status == NONE)
		{
			return NOT_INITIALIZED;
		}

		status = FINISHED;
		return SCE_OK;
	}
*/


