#include "sce_screenshot.h"


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs


static const SCE_EXPORT_FUNCTION g_pSceScreenShot_libSceScreenShot_FunctionTable[] =
{
	{ 0xB4861FD16E554E2F, "sceScreenShotDisable", (void*)sceScreenShotDisable },
	{ 0xDB1C54B6E0BF4731, "sceScreenShotEnable", (void*)sceScreenShotEnable },
	{ 0xef7590e098f49c92, "sceScreenShotSetOverlayImageWithOrigin", (void*)sceScreenShotSetOverlayImageWithOrigin },
	SCE_FUNCTION_ENTRY_END
};

static const SCE_EXPORT_LIBRARY g_pSceScreenShot_LibTable[] =
{
	{ "libSceScreenShot", g_pSceScreenShot_libSceScreenShot_FunctionTable },
	SCE_LIBRARY_ENTRY_END
};

const SCE_EXPORT_MODULE g_ExpModuleSceScreenShot =
{
	"libSceScreenShot",
	g_pSceScreenShot_LibTable
};


