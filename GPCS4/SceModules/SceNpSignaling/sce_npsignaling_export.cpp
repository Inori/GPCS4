#include "sce_npsignaling.h"


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs


static const SCE_EXPORT_FUNCTION g_pSceNpSignaling_libSceNpSignaling_FunctionTable[] =
{
	{ 0xD14BD315EA260143, "sceNpSignalingActivateConnection", (void*)sceNpSignalingActivateConnection },
	{ 0xE7262311D778B7C6, "sceNpSignalingCreateContext", (void*)sceNpSignalingCreateContext },
	{ 0x871F8B220FF59282, "sceNpSignalingDeleteContext", (void*)sceNpSignalingDeleteContext },
	{ 0x190D21AA6CE3D05E, "sceNpSignalingGetConnectionFromNpId", (void*)sceNpSignalingGetConnectionFromNpId },
	{ 0x00DDE1D040525FB0, "sceNpSignalingGetConnectionInfo", (void*)sceNpSignalingGetConnectionInfo },
	{ 0x6C3FC98B351BDC93, "sceNpSignalingGetConnectionStatus", (void*)sceNpSignalingGetConnectionStatus },
	{ 0x53C01032538505CF, "sceNpSignalingGetLocalNetInfo", (void*)sceNpSignalingGetLocalNetInfo },
	{ 0xDCA3AE0B84666595, "sceNpSignalingInitialize", (void*)sceNpSignalingInitialize },
	{ 0x34F870D145DA36B9, "sceNpSignalingTerminate", (void*)sceNpSignalingTerminate },
	{ 0x6F8A9A5CFCCC271A, "sceNpSignalingTerminateConnection", (void*)sceNpSignalingTerminateConnection },
	SCE_FUNCTION_ENTRY_END
};

static const SCE_EXPORT_LIBRARY g_pSceNpSignaling_LibTable[] =
{
	{ "libSceNpSignaling", g_pSceNpSignaling_libSceNpSignaling_FunctionTable },
	SCE_LIBRARY_ENTRY_END
};

const SCE_EXPORT_MODULE g_ExpModuleSceNpSignaling =
{
	"libSceNpSignaling",
	g_pSceNpSignaling_LibTable
};


