#include "sce_ssl.h"


// Note:
// The codebase is genarated using GenerateCode.py
// You may need to modify the code manually to fit development needs


static const SCE_EXPORT_FUNCTION g_pSceSsl_libSceSsl_FunctionTable[] =
{
	{ 0x85DA551140C55B7B, "sceSslInit", (void*)sceSslInit },
	{ 0xD0AD7243A2EFFD87, "sceSslTerm", (void*)sceSslTerm },
	SCE_FUNCTION_ENTRY_END
};

static const SCE_EXPORT_LIBRARY g_pSceSsl_LibTable[] =
{
	{ "libSceSsl", g_pSceSsl_libSceSsl_FunctionTable },
	SCE_LIBRARY_ENTRY_END
};

const SCE_EXPORT_MODULE g_ExpModuleSceSsl =
{
	"libSceSsl",
	g_pSceSsl_LibTable
};


