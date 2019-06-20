#include "sce_ime.h"


// Note:
// The codebase is genarated using GenerateCode.py
// You may need to modify the code manually to fit development needs


static const SCE_EXPORT_FUNCTION g_pSceIme_libSceIme_FunctionTable[] =
{
	{ 0x4E654FF0BCDC15C6, "sceImeClose", (void*)sceImeClose },
	{ 0x3CC55E85295F67DE, "sceImeKeyboardClose", (void*)sceImeKeyboardClose },
	{ 0x79A1578DF26FDF1B, "sceImeKeyboardOpen", (void*)sceImeKeyboardOpen },
	{ 0x44FC9DBFF26BD5B7, "sceImeOpen", (void*)sceImeOpen },
	{ 0x5A6603CDD0B81072, "sceImeParamInit", (void*)sceImeParamInit },
	{ 0x58BC5437658C8A6F, "sceImeSetCaret", (void*)sceImeSetCaret },
	{ 0x89E08DAD5AF329DE, "sceImeSetText", (void*)sceImeSetText },
	{ 0xFF81827D874D175B, "sceImeUpdate", (void*)sceImeUpdate },
	SCE_FUNCTION_ENTRY_END
};

static const SCE_EXPORT_LIBRARY g_pSceIme_LibTable[] =
{
	{ "libSceIme", g_pSceIme_libSceIme_FunctionTable },
	SCE_LIBRARY_ENTRY_END
};

const SCE_EXPORT_MODULE g_ExpModuleSceIme =
{
	"libSceIme",
	g_pSceIme_LibTable
};


