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

	int returnVal = -1;

	if (status != FINISHED)
	{
		returnVal = SCE_COMMON_DIALOG_ERROR_NOT_FINISHED;
	}

	else if (result == nullptr)
	{
		returnVal = SCE_COMMON_DIALOG_ERROR_ARG_NULL;
	}

	else if ((uint32_t)result->reserved != 0)
	{
		returnVal = SCE_COMMON_DIALOG_ERROR_PARAM_INVALID;
	}

	else
	{
		returnVal = SCE_OK;
	}

	return returnVal;
}


int PS4API sceMsgDialogGetStatus(void)
{
	LOG_SCE_TRACE("sceMsgDialogGetStatus called");

	return status;
}


int32_t PS4API sceMsgDialogInitialize(void)
{
	LOG_SCE_TRACE("sceMsgDialogInitialize called");

	int returnVal = -1;

	if (status == NONE)
	{
		status = INITIALIZED;
		returnVal = SCE_OK;
	}

	else if (status == INITIALIZED)
	{
		returnVal = SCE_COMMON_DIALOG_ERROR_ALREADY_INITIALIZED;
	}

	// RUNNING OR FINISHED
	else
	{
		returnVal = SCE_COMMON_DIALOG_ERROR_BUSY;
	}

	return returnVal;
}


int PS4API sceMsgDialogOpen(const SceMsgDialogParam *param)
{
	// Do not know how to handle parameter
	// const SceMsgDialogParam *param

	LOG_FIXME("Not fully implemented");

	int returnVal = -1;

	if (status == RUNNING || status == NONE)
	{
		returnVal = SCE_COMMON_DIALOG_ERROR_INVALID_STATE;
	}

	else if (param == nullptr)
	{
		returnVal = SCE_COMMON_DIALOG_ERROR_ARG_NULL;
	}

	else if (param->userId == -1)
	{
		returnVal = SCE_COMMON_DIALOG_ERROR_PARAM_INVALID;
	}

	else
	{
		// This does not print anything,
		// because I cannot yet confirm if
		// the SceMsgDialogParam structure is correct

		status = RUNNING;
		returnVal = SCE_OK;
	}

	return returnVal;
}


int PS4API sceMsgDialogTerminate(void)
{
	LOG_SCE_TRACE("sceMsgDialogTerminate called");

	int returnVal = -1;

	if (status == NONE)
	{
		returnVal = SCE_COMMON_DIALOG_ERROR_NOT_INITIALIZED;
	}

	else
	{
		status = NONE;
		returnVal = SCE_OK;
	}

	return returnVal;
}


int PS4API sceMsgDialogUpdateStatus(void)
{
	LOG_SCE_TRACE("sceMsgDialogUpdateStatus called");

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




