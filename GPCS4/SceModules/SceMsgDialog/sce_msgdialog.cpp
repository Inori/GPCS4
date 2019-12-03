#include "sce_msgdialog.h"


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs



//////////////////////////////////////////////////////////////////////////
// library: libSceMsgDialog
//////////////////////////////////////////////////////////////////////////

static int g_status = SCE_COMMON_DIALOG_STATUS_NONE;

int PS4API sceMsgDialogGetResult(SceMsgDialogResult* result)
{
	LOG_SCE_DUMMY_IMPL();

	int returnVal = -1;

	if (g_status != SCE_COMMON_DIALOG_STATUS_FINISHED)
	{
		returnVal = MSG_ERROR_NOT_FINISHED;
	}

	else if (result == nullptr)
	{
		returnVal = MSG_ERROR_ARG_NULL;
	}

	else if ((uint32_t)result->reserved != 0)
	{
		returnVal = MSG_ERROR_PARAM_INVALID;
	}

	else
	{
		returnVal = SCE_OK;
	}

	return returnVal;
}


int PS4API sceMsgDialogGetStatus(void)
{
	LOG_SCE_DUMMY_IMPL();

	return g_status;
}


int32_t PS4API sceMsgDialogInitialize(void)
{
	LOG_SCE_DUMMY_IMPL();

	int returnVal = -1;

	if (g_status == SCE_COMMON_DIALOG_STATUS_NONE)
	{
		g_status = SCE_COMMON_DIALOG_STATUS_INITIALIZED;
		returnVal = SCE_OK;
	}

	else if (g_status == SCE_COMMON_DIALOG_STATUS_INITIALIZED)
	{
		returnVal = MSG_ERROR_ALREADY_INITIALIZED;
	}

	// RUNNING OR FINISHED
	else
	{
		returnVal = MSG_ERROR_BUSY;
	}

	return returnVal;
}


int PS4API sceMsgDialogOpen(const MsgInfo *param)
{
	// MsgInfo structure is not correct right now

	LOG_FIXME("Not implemented");

	int returnVal = -1;

	if (g_status == SCE_COMMON_DIALOG_STATUS_RUNNING || g_status == SCE_COMMON_DIALOG_STATUS_NONE)
	{
		returnVal = MSG_ERROR_INVALID_STATE;
	}

	else if (param == nullptr)
	{
		returnVal = MSG_ERROR_ARG_NULL;
	}

	else if (param->userId == -1)
	{
		returnVal = MSG_ERROR_PARAM_INVALID;
	}

	else
	{
		// This does not print anything,
		// because I cannot yet confirm if
		// the MsgInfo structure is correct

		g_status = SCE_COMMON_DIALOG_STATUS_RUNNING;
		returnVal = SCE_OK;
	}

	return returnVal;
}


int PS4API sceMsgDialogTerminate(void)
{
	LOG_SCE_DUMMY_IMPL();

	int returnVal = -1;

	if (g_status == SCE_COMMON_DIALOG_STATUS_NONE)
	{
		returnVal = MSG_ERROR_NOT_INITIALIZED;
	}

	else
	{
		g_status = SCE_COMMON_DIALOG_STATUS_NONE;
		returnVal = SCE_OK;
	}

	return returnVal;
}


int PS4API sceMsgDialogUpdateStatus(void)
{
	LOG_SCE_DUMMY_IMPL();

	// Because we do not have a UI system
	// for messages yet, we use if(1) to
	// immediately end the window

	// In the future, this will be something like
	// if(closeButton == pushed)

	if (1)
	{
		g_status = SCE_COMMON_DIALOG_STATUS_FINISHED;
	}

	// This will be FINISHED when the 
	// window is closed, or it will be 
	// RUNNING, which is set in sceMsgDialogOpen
	return g_status;
}




