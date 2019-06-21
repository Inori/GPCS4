#include "sce_invitationdialog.h"


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs


static const SCE_EXPORT_FUNCTION g_pSceInvitationDialog_libSceInvitationDialog_FunctionTable[] =
{
	{ 0x5EF039292E7AC1CB, "sceInvitationDialogInitialize", (void*)sceInvitationDialogInitialize },
	{ 0xD335341BEC2254B0, "sceInvitationDialogOpen", (void*)sceInvitationDialogOpen },
	{ 0x07A1D526D0D8C441, "sceInvitationDialogTerminate", (void*)sceInvitationDialogTerminate },
	{ 0xF7E83D88EABEEE48, "sceInvitationDialogUpdateStatus", (void*)sceInvitationDialogUpdateStatus },
	SCE_FUNCTION_ENTRY_END
};

static const SCE_EXPORT_LIBRARY g_pSceInvitationDialog_LibTable[] =
{
	{ "libSceInvitationDialog", g_pSceInvitationDialog_libSceInvitationDialog_FunctionTable },
	SCE_LIBRARY_ENTRY_END
};

const SCE_EXPORT_MODULE g_ExpModuleSceInvitationDialog =
{
	"libSceInvitationDialog",
	g_pSceInvitationDialog_LibTable
};


