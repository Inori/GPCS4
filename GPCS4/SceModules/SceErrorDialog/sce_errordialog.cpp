#include "sce_errordialog.h"


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs

//////////////////////////////////////////////////////////////////////////
// library: libSceErrorDialog
//////////////////////////////////////////////////////////////////////////

// same as sce_msgdialog

static int g_status = SCE_ERROR_DIALOG_STATUS_NONE;

// sceErrorDialogParamInitialize
// is a static inlinei function that does not need emulation

int PS4API sceErrorDialogGetStatus(void)
{
	LOG_SCE_TRACE("sceErrorDialogGetStatus called");

	return g_status;
}

#define ALREADY_INITIALIZED 0x80ED0002

int PS4API sceErrorDialogInitialize(void)
{
	LOG_SCE_TRACE("sceErrorDialogInitialize called");

	int returnVal = -1;

	if (g_status == SCE_ERROR_DIALOG_STATUS_NONE)
	{
		g_status = SCE_ERROR_DIALOG_STATUS_INITIALIZED;
		returnVal = SCE_OK;
	}

	else
	{
		returnVal = ALREADY_INITIALIZED;
	}

	return returnVal;
}

#define SCE_ERROR_DIALOG_ERROR_NOT_INITIALIZED 0x80ED0001
#define SCE_ERROR_DIALOG_ERROR_PARAM_INVALID 0x80ED0003
#define SCE_ERROR_DIALOG_ERROR_INVALID_STATE 0x80ED0005
#define SCE_ERROR_DIALOG_ERROR_SERVICE_BUSY 0x80ED0006
#define SCE_ERROR_DIALOG_ERROR_INVALID_USER_ID 0x80ED0007
#define SCE_ERROR_DIALOG_ERROR_UNEXPECTED_FATAL 0x80ED0004


int PS4API sceErrorDialogOpen(SceErrorDialogParam* param)
{
	LOG_SCE_TRACE("sceErrorDialogOpen called");

	int returnVal = -1;

	if (g_status == SCE_ERROR_DIALOG_STATUS_NONE)
	{
		returnVal = SCE_ERROR_DIALOG_ERROR_NOT_INITIALIZED;
	}

	else if (g_status == SCE_ERROR_DIALOG_STATUS_RUNNING)
	{
		returnVal = SCE_ERROR_DIALOG_ERROR_INVALID_STATE;
	}

	else if (param->userId == -1)
	{
		returnVal = SCE_ERROR_DIALOG_ERROR_INVALID_USER_ID;
	}

	else
	{
		// In the future, this will open a window
		// and print the error in that window,
		// rather than printing to a console

		LOG_TRACE("userID 0x%p", param->userId);
		LOG_TRACE("error 0x%p", param->errorCode);

		g_status = SCE_ERROR_DIALOG_STATUS_RUNNING;
		returnVal = SCE_OK;
	}

	return returnVal;
}

int PS4API sceErrorDialogTerminate(void)
{
	LOG_SCE_TRACE("sceErrorDialogTerminate called")

	int returnVal = -1;

	if (g_status == SCE_ERROR_DIALOG_STATUS_NONE)
	{
		returnVal = SCE_ERROR_DIALOG_ERROR_NOT_INITIALIZED;
	}

	else
	{
		g_status = SCE_ERROR_DIALOG_STATUS_NONE;
		returnVal = SCE_OK;
	}

	return returnVal;
}


int PS4API sceErrorDialogUpdateStatus(void)
{
	LOG_SCE_TRACE("sceErrorDialogUpdateStatus called");

	// Because we do not have a UI system
	// for errors yet, we use if(1) to
	// immediately end the window

	// In the future, this will be something like
	// if(closeButton == pushed)

	if (1)
	{
		g_status = SCE_ERROR_DIALOG_STATUS_FINISHED;
	}

	// This will be SCE_ERROR_DIALOG_STATUS_FINISHED when the 
	// window is closed, or it will be 
	// SCE_ERROR_DIALOG_STATUS_RUNNING, which is set in sceErrorDialogOpen
	return g_status;
}

// We do not have a stub for this yet
int sceErrorDialogClose()
{
	LOG_SCE_TRACE("sceErrorDialogClose called");

	// In the future, this function 
	// will close the error window that
	// was opened by sceErrorDialogOpen

	int returnVal = -1;

	if (g_status == SCE_ERROR_DIALOG_STATUS_NONE)
	{
		returnVal = SCE_ERROR_DIALOG_ERROR_NOT_INITIALIZED;
	}

	else
	{
		g_status = SCE_ERROR_DIALOG_STATUS_FINISHED;
		returnVal = SCE_OK;
	}

	return returnVal;
}



