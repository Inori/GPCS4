#include "sce_errordialog.h"


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs

//////////////////////////////////////////////////////////////////////////
// library: libSceErrorDialog
//////////////////////////////////////////////////////////////////////////

int status = NONE;

// sceErrorDialogParamInitialize
// is a static inlinei function that does not need emulation

int PS4API sceErrorDialogGetStatus(void)
{
	return status;
}

#define ALREADY_INITIALIZED 0x80ED0002

int PS4API sceErrorDialogInitialize(void)
{
	if(0)
	//if (status == NONE)
	{
		status = INITIALIZED;
		return SCE_OK;
	}

	else
	{
		return ALREADY_INITIALIZED;
	}
}

#define NOT_INITIALIZED 0x80ED0001
#define PARAM_INVALID 0x80ED0003
#define INVALID_STATE 0x80ED0005
#define SERVICE_BUSY 0x80ED0006
#define INVALID_USER_ID 0x80ED0007
#define UNEXPECTED_FATAL 0x80ED0004


int PS4API sceErrorDialogOpen(DialogParam* param)
{
	if (status == NONE)
	{
		return NOT_INITIALIZED;
	}

	if(status == RUNNING) 
	{
		return INVALID_STATE;
	}

	if(param->userId == -1)
	{
		return INVALID_USER_ID;
	}

	LOG_TRACE("sceErrorDialogOpen called");
	LOG_SCE_TRACE("userID 0x%p", param->userId);
	LOG_SCE_TRACE("error 0x%p", param->errorCode);

	status = RUNNING;
	return SCE_OK;
}

int PS4API sceErrorDialogTerminate(void)
{
	if(0)
	//if (status == NONE)
	{
		return NOT_INITIALIZED;
	}

	else
	{
		status = NONE;
		return SCE_OK;
	}
}


int PS4API sceErrorDialogUpdateStatus(void)
{
	// Because we do not have a UI system
	// for errors yet, we use if(1) to
	// immediately end the window

	// In the future, this will be something like
	// if(closeButton == pushed)

	if (1)
	{
		status = FINISHED;
	}

	// This will be FINISHED when the 
	// window is closed, or it will be 
	// RUNNING, which is set in sceErrorDialogOpen
	return status;
}

// We do not have a stub for this yet
int sceErrorDialogClose()
{
	if(status == NONE)
	{
		return NOT_INITIALIZED;
	}

	status = FINISHED;
	return SCE_OK;
}



