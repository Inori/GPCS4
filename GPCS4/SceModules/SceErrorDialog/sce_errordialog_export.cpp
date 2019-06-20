#include "sce_errordialog.h"


// Note:
// The codebase is genarated using GenerateCode.py
// You may need to modify the code manually to fit development needs


static const SCE_EXPORT_FUNCTION g_pSceErrorDialog_libSceErrorDialog_FunctionTable[] =
{
	{ 0xB7616F1D15F382A9, "sceErrorDialogGetStatus", (void*)sceErrorDialogGetStatus },
	{ 0x23CF0A0A19729D2B, "sceErrorDialogInitialize", (void*)sceErrorDialogInitialize },
	{ 0x336645FC294B8606, "sceErrorDialogOpen", (void*)sceErrorDialogOpen },
	{ 0xF570312B63CCC24F, "sceErrorDialogTerminate", (void*)sceErrorDialogTerminate },
	{ 0x596886BA1F577E04, "sceErrorDialogUpdateStatus", (void*)sceErrorDialogUpdateStatus },
	SCE_FUNCTION_ENTRY_END
};

static const SCE_EXPORT_LIBRARY g_pSceErrorDialog_LibTable[] =
{
	{ "libSceErrorDialog", g_pSceErrorDialog_libSceErrorDialog_FunctionTable },
	SCE_LIBRARY_ENTRY_END
};

const SCE_EXPORT_MODULE g_ExpModuleSceErrorDialog =
{
	"libSceErrorDialog",
	g_pSceErrorDialog_LibTable
};


