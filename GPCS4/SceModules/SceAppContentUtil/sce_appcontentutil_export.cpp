#include "sce_appcontentutil.h"


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs


static const SCE_EXPORT_FUNCTION g_pSceAppContentUtil_libSceAppContent_FunctionTable[] =
{
	{ 0xEE0C61FB942E6E16, "sceAppContentAddcontEnqueueDownload", (void*)sceAppContentAddcontEnqueueDownload },
	{ 0x54036121672A61A9, "sceAppContentAddcontMount", (void*)sceAppContentAddcontMount },
	{ 0xDEB1D6695FF5282E, "sceAppContentAddcontUnmount", (void*)sceAppContentAddcontUnmount },
	{ 0xC6777C049CC0C669, "sceAppContentGetAddcontInfoList", (void*)sceAppContentGetAddcontInfoList },
	{ 0x47D940F363AB68DB, "sceAppContentInitialize", (void*)sceAppContentInitialize },
	{ 0xf7d6fcd88297a47e, "sceAppContentAppParamGetInt", (void*)sceAppContentAppParamGetInt },
	SCE_FUNCTION_ENTRY_END
};

static const SCE_EXPORT_LIBRARY g_pSceAppContentUtil_LibTable[] =
{
	{ "libSceAppContent", g_pSceAppContentUtil_libSceAppContent_FunctionTable },
	SCE_LIBRARY_ENTRY_END
};

const SCE_EXPORT_MODULE g_ExpModuleSceAppContentUtil =
{
	"libSceAppContentUtil",
	g_pSceAppContentUtil_LibTable
};


