#include "sce_msgdialog.h"


// Note:
// The codebase is genarated using GenerateCode.py
// You may need to modify the code manually to fit development needs


static const SCE_EXPORT_FUNCTION g_pSceMsgDialog_libSceMsgDialog_FunctionTable[] =
{
	{ 0x2EBF28BC71FD97A0, "sceMsgDialogGetResult", (void*)sceMsgDialogGetResult },
	{ 0x096556EFC41CDDF2, "sceMsgDialogGetStatus", (void*)sceMsgDialogGetStatus },
	{ 0x943AB1698D546C4A, "sceMsgDialogInitialize", (void*)sceMsgDialogInitialize },
	{ 0x6F4E878740CF11A1, "sceMsgDialogOpen", (void*)sceMsgDialogOpen },
	{ 0x78FC3F92A6667A5A, "sceMsgDialogTerminate", (void*)sceMsgDialogTerminate },
	{ 0xE9F202DD72ADDA4D, "sceMsgDialogUpdateStatus", (void*)sceMsgDialogUpdateStatus },
	SCE_FUNCTION_ENTRY_END
};

static const SCE_EXPORT_LIBRARY g_pSceMsgDialog_LibTable[] =
{
	{ "libSceMsgDialog", g_pSceMsgDialog_libSceMsgDialog_FunctionTable },
	SCE_LIBRARY_ENTRY_END
};

const SCE_EXPORT_MODULE g_ExpModuleSceMsgDialog =
{
	"libSceMsgDialog",
	g_pSceMsgDialog_LibTable
};


