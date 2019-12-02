#include "sce_msgdialog.h"


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs



//////////////////////////////////////////////////////////////////////////
// library: libSceMsgDialog
//////////////////////////////////////////////////////////////////////////

// same as sce_errordialog
static enum DialogStatus {
	NONE = 0,
	INITIALIZED = 1,
	RUNNING = 2,
	FINISHED = 3
};

static int status = NONE;

#define SCE_COMMON_DIALOG_ERROR_NOT_SYSTEM_INITIALIZED 0x80B80001
#define SCE_COMMON_DIALOG_ERROR_ALREADY_INITIALIZED 0x80B80004
#define SCE_COMMON_DIALOG_ERROR_BUSY 0x80B80008
#define SCE_COMMON_DIALOG_ERROR_NOT_FINISHED 0x80B80005
#define SCE_COMMON_DIALOG_ERROR_PARAM_INVALID 0x80B8000A
#define SCE_COMMON_DIALOG_ERROR_ARG_NULL 0x80B8000D
#define SCE_COMMON_DIALOG_ERROR_NOT_INITIALIZED 0x80B80003
#define SCE_COMMON_DIALOG_ERROR_INVALID_STATE 0x80B80006
#define SCE_COMMON_DIALOG_ERROR_ARG_NULL 0x80B8000D

int PS4API sceMsgDialogGetResult(SceMsgDialogResult* result)
{
	LOG_FIXME("Not implemented");

	if (status != FINISHED)
	{
		return SCE_COMMON_DIALOG_ERROR_NOT_FINISHED;
	}

	if (result == nullptr)
	{
		return SCE_COMMON_DIALOG_ERROR_ARG_NULL;
	}

	if ((uint32_t)result->reserved != 0)
	{
		return SCE_COMMON_DIALOG_ERROR_PARAM_INVALID;
	}

	return SCE_OK;
}


int PS4API sceMsgDialogGetStatus(void)
{
	return status;
}


int32_t PS4API sceMsgDialogInitialize(void)
{
	if(1)
	//if (status == NONE)
	{
		status = INITIALIZED;
		return SCE_OK;
	}

	else if (status == INITIALIZED)
	{
		return SCE_COMMON_DIALOG_ERROR_ALREADY_INITIALIZED;
	}

	// RUNNING OR FINISHED
	else
	{
		return SCE_COMMON_DIALOG_ERROR_BUSY;
	}
}


int PS4API sceMsgDialogOpen(const SceMsgDialogParam *param)
{
	// Do not know how to handle parameter
	// const SceMsgDialogParam *param

	LOG_FIXME("Not implemented");

	if (status == RUNNING || status == NONE)
	{
		return SCE_COMMON_DIALOG_ERROR_INVALID_STATE;
	}

	if (param == nullptr)
	{
		return SCE_COMMON_DIALOG_ERROR_ARG_NULL;
	}

	if (param->userId == -1)
	{
		return SCE_COMMON_DIALOG_ERROR_PARAM_INVALID;
	}

	// In the future, this will open a window
	// and print the error in that window,
	// rather than printing to a console

	status = RUNNING;
	return SCE_OK;
}


int PS4API sceMsgDialogTerminate(void)
{
	if(0)
	//if (status == NONE)
	{
		return SCE_COMMON_DIALOG_ERROR_NOT_INITIALIZED;
	}

	status = NONE;
	return SCE_OK;
}


int PS4API sceMsgDialogUpdateStatus(void)
{
	// Because we do not have a UI system
	// for messages yet, we use if(1) to
	// immediately end the window

	// In the future, this will be something like
	// if(closeButton == pushed)

	if (1)
	{
		status = FINISHED;
	}

	// This will be FINISHED when the 
	// window is closed, or it will be 
	// RUNNING, which is set in sceMsgDialogOpen
	return status;
}




