#include "sce_npprofiledialog.h"


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs


static const SCE_EXPORT_FUNCTION g_pSceNpProfileDialog_libSceNpProfileDialog_FunctionTable[] =
{
	{ 0x2E0F8D084EA94F04, "sceNpProfileDialogInitialize", (void*)sceNpProfileDialogInitialize },
	{ 0xBA3F42CFB4E4D1C7, "sceNpProfileDialogOpen", (void*)sceNpProfileDialogOpen },
	{ 0xD12A7DBC9701D7FC, "sceNpProfileDialogTerminate", (void*)sceNpProfileDialogTerminate },
	{ 0x85A55913D1602AA1, "sceNpProfileDialogUpdateStatus", (void*)sceNpProfileDialogUpdateStatus },
	SCE_FUNCTION_ENTRY_END
};

static const SCE_EXPORT_LIBRARY g_pSceNpProfileDialog_LibTable[] =
{
	{ "libSceNpProfileDialog", g_pSceNpProfileDialog_libSceNpProfileDialog_FunctionTable },
	SCE_LIBRARY_ENTRY_END
};

const SCE_EXPORT_MODULE g_ExpModuleSceNpProfileDialog =
{
	"libSceNpProfileDialog",
	g_pSceNpProfileDialog_LibTable
};


