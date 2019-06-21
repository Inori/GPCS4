#include "sce_playgo.h"


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs


static const SCE_EXPORT_FUNCTION g_pScePlayGo_libScePlayGo_FunctionTable[] =
{
	{ 0x51CA352347650E2F, "scePlayGoClose", (void*)scePlayGoClose },
	{ 0xB962182C5924C2A9, "scePlayGoGetLocus", (void*)scePlayGoGetLocus },
	{ 0xFD125634C2B77C2F, "scePlayGoGetProgress", (void*)scePlayGoGetProgress },
	{ 0xB6CE8695938A46B1, "scePlayGoInitialize", (void*)scePlayGoInitialize },
	{ 0x3351A66B5A1CAC61, "scePlayGoOpen", (void*)scePlayGoOpen },
	{ 0x30F7B411E04633F1, "scePlayGoTerminate", (void*)scePlayGoTerminate },
	SCE_FUNCTION_ENTRY_END
};

static const SCE_EXPORT_LIBRARY g_pScePlayGo_LibTable[] =
{
	{ "libScePlayGo", g_pScePlayGo_libScePlayGo_FunctionTable },
	SCE_LIBRARY_ENTRY_END
};

const SCE_EXPORT_MODULE g_ExpModuleScePlayGo =
{
	"libScePlayGo",
	g_pScePlayGo_LibTable
};


