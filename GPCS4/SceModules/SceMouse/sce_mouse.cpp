#include "sce_mouse.h"


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs



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


int PS4API sceMouseOpen(void)
{
	LOG_SCE_DUMMY_IMPL();
	// it seems that mouse open must be successful even not connected a mouse,
	// not sure...
	return 0x123;
}


int PS4API sceMouseRead(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}




