#include "sce_mouse.h"


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs

LOG_CHANNEL(SceModules.SceMouse);

//////////////////////////////////////////////////////////////////////////
// library: libSceMouse
//////////////////////////////////////////////////////////////////////////

int PS4API sceMouseClose(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceMouseInit(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceMouseOpen(SceUserServiceUserId userId, int32_t type, int32_t index, SceMouseOpenParam *pParam)
{
	LOG_SCE_DUMMY_IMPL();
	// it seems that mouse open must be successful even not connected a mouse,
	// not sure...
	return 0x123;
}


int PS4API sceMouseRead(int32_t handle, SceMouseData *pData, int32_t num)
{
	LOG_SCE_DUMMY_IMPL();
	uint32_t dataCount = 0;
	do 
	{
		if (num < 1)
		{
			break;
		}

		// This indicates that a mouse is not connected.
		pData[0].connected = false;
		dataCount = 1;
			
	} while (false);
	return dataCount;
}




