#include "sce_sysmodule.h"


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs


static const SCE_EXPORT_FUNCTION g_pSceSysmodule_libSceSysmodule_FunctionTable[] =
{
	{ 0x7CC3F934750E68C9, "sceSysmoduleIsLoaded", (void*)sceSysmoduleIsLoaded },
	{ 0x83C70CDFD11467AA, "sceSysmoduleLoadModule", (void*)sceSysmoduleLoadModule },
	{ 0x791D9B6450005344, "sceSysmoduleUnloadModule", (void*)sceSysmoduleUnloadModule },
	{ 0x847AC6A06A0D7FEB, "sceSysmoduleLoadModuleInternalWithArg", (void*)sceSysmoduleLoadModuleInternalWithArg },
	SCE_FUNCTION_ENTRY_END
};

static const SCE_EXPORT_LIBRARY g_pSceSysmodule_LibTable[] =
{
	{ "libSceSysmodule", g_pSceSysmodule_libSceSysmodule_FunctionTable },
	SCE_LIBRARY_ENTRY_END
};

const SCE_EXPORT_MODULE g_ExpModuleSceSysmodule =
{
	"libSceSysmodule",
	g_pSceSysmodule_LibTable
};


