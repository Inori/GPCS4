#include "sce_npcommon.h"


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs


static const SCE_EXPORT_FUNCTION g_pSceNpCommon_libSceNpCommon_FunctionTable[] =
{
	{ 0x8BC5265D34AAECDE, "sceNpCmpNpId", (void*)sceNpCmpNpId },
	SCE_FUNCTION_ENTRY_END
};

static const SCE_EXPORT_LIBRARY g_pSceNpCommon_LibTable[] =
{
	{ "libSceNpCommon", g_pSceNpCommon_libSceNpCommon_FunctionTable },
	SCE_LIBRARY_ENTRY_END
};

const SCE_EXPORT_MODULE g_ExpModuleSceNpCommon =
{
	"libSceNpCommon",
	g_pSceNpCommon_LibTable
};


