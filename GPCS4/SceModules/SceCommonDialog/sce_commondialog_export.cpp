#include "sce_commondialog.h"


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs


static const SCE_EXPORT_FUNCTION g_pSceCommonDialog_libSceCommonDialog_FunctionTable[] =
{
	{ 0xBA85292C6364CA09, "sceCommonDialogInitialize", (void*)sceCommonDialogInitialize },
	SCE_FUNCTION_ENTRY_END
};

static const SCE_EXPORT_LIBRARY g_pSceCommonDialog_LibTable[] =
{
	{ "libSceCommonDialog", g_pSceCommonDialog_libSceCommonDialog_FunctionTable },
	SCE_LIBRARY_ENTRY_END
};

const SCE_EXPORT_MODULE g_ExpModuleSceCommonDialog =
{
	"libSceCommonDialog",
	g_pSceCommonDialog_LibTable
};


