#include "sce_errordialog.h"


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs

//////////////////////////////////////////////////////////////////////////
// library: libSceErrorDialog
//////////////////////////////////////////////////////////////////////////

DialogParam myError;
int status = NONE;

#define ALREADY_INITIALIZED 0x80ED0002
#define NOT_INITIALIZED 0x80ED0001

// sceErrorDialogParamInitialize
// is a static inlinei function that does not need emulation

int PS4API sceErrorDialogGetStatus(void)
{
	return status;
}


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


int PS4API sceErrorDialogOpen(DialogParam* param)
{
	LOG_TRACE("sceErrorDialogOpen called");
	LOG_SCE_TRACE("userID 0x%p", myError.userId);
	LOG_SCE_TRACE("error 0x%p", myError.errorCode);
	return SCE_OK;
}


int PS4API sceErrorDialogTerminate(void)
{
	if(0)
	//if (status != NONE)
	{
		status = NONE;
		return NOT_INITIALIZED;
	}

	else
	{
		return SCE_OK;
	}
}


int PS4API sceErrorDialogUpdateStatus(void)
{
	// If this were to open an actual error window,
	// it would look more like this:

	// if window is still open, return RUNNING
	// if you hit the close button, return FINISHED

	return FINISHED;
}

/*
	int sceErrorDialogClose()
	{
		if(status == NONE)
		{
			return NOT_INITIALIZED;
		}

		status = FINISHED;
		return SCE_OK;
	}
*/


