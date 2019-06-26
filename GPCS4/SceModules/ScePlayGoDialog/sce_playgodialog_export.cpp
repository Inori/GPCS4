#include "sce_playgodialog.h"


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs


static const SCE_EXPORT_FUNCTION g_pScePlayGoDialog_libScePlayGoDialog_FunctionTable[] =
{
	{ 0x90777BDAE92A6F1C, "scePlayGoDialogOpen", (void*)scePlayGoDialogOpen },
	SCE_FUNCTION_ENTRY_END
};

static const SCE_EXPORT_LIBRARY g_pScePlayGoDialog_LibTable[] =
{
	{ "libScePlayGoDialog", g_pScePlayGoDialog_libScePlayGoDialog_FunctionTable },
	SCE_LIBRARY_ENTRY_END
};

const SCE_EXPORT_MODULE g_ExpModuleScePlayGoDialog =
{
	"libScePlayGoDialog",
	g_pScePlayGoDialog_LibTable
};


