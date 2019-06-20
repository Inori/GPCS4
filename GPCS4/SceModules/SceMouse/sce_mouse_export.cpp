#include "sce_mouse.h"


// Note:
// The codebase is genarated using GenerateCode.py
// You may need to modify the code manually to fit development needs


static const SCE_EXPORT_FUNCTION g_pSceMouse_libSceMouse_FunctionTable[] =
{
	{ 0x7009D3D11C3F2305, "sceMouseClose", (void*)sceMouseClose },
	{ 0x42CD305AE96097B5, "sceMouseInit", (void*)sceMouseInit },
	{ 0x45AAB16487FA0EF1, "sceMouseOpen", (void*)sceMouseOpen },
	{ 0xC7CAA75EA87FB623, "sceMouseRead", (void*)sceMouseRead },
	SCE_FUNCTION_ENTRY_END
};

static const SCE_EXPORT_LIBRARY g_pSceMouse_LibTable[] =
{
	{ "libSceMouse", g_pSceMouse_libSceMouse_FunctionTable },
	SCE_LIBRARY_ENTRY_END
};

const SCE_EXPORT_MODULE g_ExpModuleSceMouse =
{
	"libSceMouse",
	g_pSceMouse_LibTable
};


