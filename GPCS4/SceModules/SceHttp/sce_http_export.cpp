#include "sce_http.h"


// Note:
// The codebase is genarated using GenerateCode.py
// You may need to modify the code manually to fit development needs


static const SCE_EXPORT_FUNCTION g_pSceHttp_libSceHttp_FunctionTable[] =
{
	{ 0x03D715314B44A786, "sceHttpInit", (void*)sceHttpInit },
	{ 0x224FCAA4B4E57FB4, "sceHttpTerm", (void*)sceHttpTerm },
	SCE_FUNCTION_ENTRY_END
};

static const SCE_EXPORT_LIBRARY g_pSceHttp_LibTable[] =
{
	{ "libSceHttp", g_pSceHttp_libSceHttp_FunctionTable },
	SCE_LIBRARY_ENTRY_END
};

const SCE_EXPORT_MODULE g_ExpModuleSceHttp =
{
	"libSceHttp",
	g_pSceHttp_LibTable
};


