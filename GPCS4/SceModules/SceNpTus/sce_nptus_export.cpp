#include "sce_nptus.h"


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs


static const SCE_EXPORT_FUNCTION g_pSceNpTus_libSceNpTus_FunctionTable[] =
{
	{ 0x4365261DD2B4E1CF, "sceNpTusAddAndGetVariableAsync", (void*)sceNpTusAddAndGetVariableAsync },
	{ 0x04890C9947CD2963, "sceNpTusCreateNpTitleCtx", (void*)sceNpTusCreateNpTitleCtx },
	{ 0xDDB876681BEF9AF3, "sceNpTusCreateRequest", (void*)sceNpTusCreateRequest },
	{ 0x1F7BAAEF1D2C64E2, "sceNpTusDeleteNpTitleCtx", (void*)sceNpTusDeleteNpTitleCtx },
	{ 0x09C207E347584BCF, "sceNpTusDeleteRequest", (void*)sceNpTusDeleteRequest },
	{ 0x6DC3C1DAB9E142AA, "sceNpTusGetMultiSlotVariableAsync", (void*)sceNpTusGetMultiSlotVariableAsync },
	{ 0xB7B6FA766A503622, "sceNpTusPollAsync", (void*)sceNpTusPollAsync },
	{ 0xE49F4618C96E7716, "sceNpTusSetMultiSlotVariableAsync", (void*)sceNpTusSetMultiSlotVariableAsync },
	{ 0xC507D1E758B891C9, "sceNpTusTryAndSetVariableAsync", (void*)sceNpTusTryAndSetVariableAsync },
	{ 0x941B6B93EEE5935E, "sceNpTssCreateNpTitleCtxA", (void*)sceNpTssCreateNpTitleCtxA },
	{ 0xFD2511F94A0B4BA7, "sceNpTssGetData", (void*)sceNpTssGetData },
	SCE_FUNCTION_ENTRY_END
};

static const SCE_EXPORT_LIBRARY g_pSceNpTus_LibTable[] =
{
	{ "libSceNpTus", g_pSceNpTus_libSceNpTus_FunctionTable },
	SCE_LIBRARY_ENTRY_END
};

const SCE_EXPORT_MODULE g_ExpModuleSceNpTus =
{
	"libSceNpTus",
	g_pSceNpTus_LibTable
};


