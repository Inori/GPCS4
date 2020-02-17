#include "sce_savedatadialog.h"


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs


static const SCE_EXPORT_FUNCTION g_pSceSaveDataDialog_libSceSaveDataDialog_FunctionTable[] =
{
	{ 0xB3D7B7F98A519F3C, "sceSaveDataDialogInitialize", (void*)sceSaveDataDialogInitialize },
	{ 0xE2D3E1B0FE85A432, "sceSaveDataDialogOpen", (void*)sceSaveDataDialogOpen },
	{ 0x62E1F6140EDACEA4, "sceSaveDataDialogTerminate", (void*)sceSaveDataDialogTerminate },
	{ 0x28ADC1760D5158AD, "sceSaveDataDialogUpdateStatus", (void*)sceSaveDataDialogUpdateStatus },
	{ 0xc84889feaaabe828, "sceSaveDataDialogGetResult", (void*)sceSaveDataDialogGetResult },
	{ 0x85acb509f4e62f20, "sceSaveDataDialogProgressBarSetValue", (void*)sceSaveDataDialogProgressBarSetValue },
	SCE_FUNCTION_ENTRY_END
};

static const SCE_EXPORT_LIBRARY g_pSceSaveDataDialog_LibTable[] =
{
	{ "libSceSaveDataDialog", g_pSceSaveDataDialog_libSceSaveDataDialog_FunctionTable },
	SCE_LIBRARY_ENTRY_END
};

const SCE_EXPORT_MODULE g_ExpModuleSceSaveDataDialog =
{
	"libSceSaveDataDialog",
	g_pSceSaveDataDialog_LibTable
};


