#include "sce_errordialog.h"


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs

//////////////////////////////////////////////////////////////////////////
// library: libSceErrorDialog
//////////////////////////////////////////////////////////////////////////

// same as sce_msgdialog
static enum DialogStatus 
{
	NONE = 0,
	INITIALIZED = 1,
	RUNNING = 2,
	FINISHED = 3
};

static int g_status = NONE;

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

	if (g_status == NONE)
	{
		g_status = INITIALIZED;
		returnVal = SCE_OK;
	}

	else
	{
		returnVal = ALREADY_INITIALIZED;
	}

	return returnVal;
}

#define NOT_INITIALIZED 0x80ED0001
#define PARAM_INVALID 0x80ED0003
#define INVALID_STATE 0x80ED0005
#define SERVICE_BUSY 0x80ED0006
#define INVALID_USER_ID 0x80ED0007
#define UNEXPECTED_FATAL 0x80ED0004


int PS4API sceErrorDialogOpen(DialogParam* param)
{
	LOG_SCE_TRACE("sceErrorDialogOpen called");

	int returnVal = -1;

	if (g_status == NONE)
	{
		returnVal = NOT_INITIALIZED;
	}

	else if (g_status == RUNNING)
	{
		returnVal = INVALID_STATE;
	}

	else if (param->userId == -1)
	{
		returnVal = INVALID_USER_ID;
	}

	else
	{
		// In the future, this will open a window
		// and print the error in that window,
		// rather than printing to a console

		LOG_TRACE("userID 0x%p", param->userId);
		LOG_TRACE("error 0x%p", param->errorCode);

		g_status = RUNNING;
		returnVal = SCE_OK;
	}

	return returnVal;
}

int PS4API sceErrorDialogTerminate(void)
{
	LOG_SCE_TRACE("sceErrorDialogTerminate called")

	int returnVal = -1;

	if (g_status == NONE)
	{
		returnVal = NOT_INITIALIZED;
	}

	else
	{
		g_status = NONE;
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
		g_status = FINISHED;
	}

	// This will be FINISHED when the 
	// window is closed, or it will be 
	// RUNNING, which is set in sceErrorDialogOpen
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

	if (g_status == NONE)
	{
		returnVal = NOT_INITIALIZED;
	}

	else
	{
		g_status = FINISHED;
		returnVal = SCE_OK;
	}

	return returnVal;
}



