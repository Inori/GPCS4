#include "sce_nptrophy.h"


// Note:
// The codebase is genarated using GenerateCode.py
// You may need to modify the code manually to fit development needs


static const SCE_EXPORT_FUNCTION g_pSceNpTrophy_libSceNpTrophy_FunctionTable[] =
{
	{ 0x5DB9236E86D99426, "sceNpTrophyCreateContext", (void*)sceNpTrophyCreateContext },
	{ 0xABB53AB440107FB7, "sceNpTrophyCreateHandle", (void*)sceNpTrophyCreateHandle },
	{ 0x1355ABC1DD3B2EBF, "sceNpTrophyDestroyContext", (void*)sceNpTrophyDestroyContext },
	{ 0x18D705E2889D6346, "sceNpTrophyDestroyHandle", (void*)sceNpTrophyDestroyHandle },
	{ 0x2C7B9298EDD22DDF, "sceNpTrophyGetTrophyUnlockState", (void*)sceNpTrophyGetTrophyUnlockState },
	{ 0x4C9080C6DA3D4845, "sceNpTrophyRegisterContext", (void*)sceNpTrophyRegisterContext },
	{ 0xDBCC6645415AA3AF, "sceNpTrophyUnlockTrophy", (void*)sceNpTrophyUnlockTrophy },
	SCE_FUNCTION_ENTRY_END
};

static const SCE_EXPORT_LIBRARY g_pSceNpTrophy_LibTable[] =
{
	{ "libSceNpTrophy", g_pSceNpTrophy_libSceNpTrophy_FunctionTable },
	SCE_LIBRARY_ENTRY_END
};

const SCE_EXPORT_MODULE g_ExpModuleSceNpTrophy =
{
	"libSceNpTrophy",
	g_pSceNpTrophy_LibTable
};


