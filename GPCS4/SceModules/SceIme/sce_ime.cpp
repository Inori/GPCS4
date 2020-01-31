#include "sce_ime.h"
#include "sce_ime_error.h"

// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs

LOG_CHANNEL(SceModules.SceIme);

//////////////////////////////////////////////////////////////////////////
// library: libSceIme
//////////////////////////////////////////////////////////////////////////

int PS4API sceImeClose(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceImeKeyboardClose(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceImeKeyboardOpen(void)
{
	LOG_SCE_DUMMY_IMPL();
	return SCE_IME_ERROR_CONNECTION_FAILED;
}


int PS4API sceImeOpen(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceImeParamInit(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceImeSetCaret(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceImeSetText(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceImeUpdate(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}




