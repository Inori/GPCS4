#include "sce_systemservice.h"


// Note:
// The codebase is genarated using GenerateCode.py
// You may need to modify the code manually to fit development needs


static const SCE_EXPORT_FUNCTION g_pSceSystemService_libSceSystemService_FunctionTable[] =
{
	{ 0xACFA3AB55F03F5B3, "sceSystemServiceGetStatus", (void*)sceSystemServiceGetStatus },
	{ 0x568E55F0A0300A69, "sceSystemServiceHideSplashScreen", (void*)sceSystemServiceHideSplashScreen },
	{ 0x7D9A38F2E9FB2CAE, "sceSystemServiceParamGetInt", (void*)sceSystemServiceParamGetInt },
	{ 0xEB9E8B3104AB83A5, "sceSystemServiceReceiveEvent", (void*)sceSystemServiceReceiveEvent },
	SCE_FUNCTION_ENTRY_END
};

static const SCE_EXPORT_LIBRARY g_pSceSystemService_LibTable[] =
{
	{ "libSceSystemService", g_pSceSystemService_libSceSystemService_FunctionTable },
	SCE_LIBRARY_ENTRY_END
};

const SCE_EXPORT_MODULE g_ExpModuleSceSystemService =
{
	"libSceSystemService",
	g_pSceSystemService_LibTable
};


