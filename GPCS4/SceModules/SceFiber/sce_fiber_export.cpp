#include "sce_fiber.h"


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs


static const SCE_EXPORT_FUNCTION g_pSceFiber_libSceFiber_FunctionTable[] =
{
	{ 0x855603ECEBB6A424, "_sceFiberInitializeImpl", (void*)_sceFiberInitializeImpl },
	{ 0xEFE389229928F516, "_sceFiberInitializeWithInternalOptionImpl", (void*)_sceFiberInitializeWithInternalOptionImpl },
	{ 0x25E357E45FCDCD05, "sceFiberFinalize", (void*)sceFiberFinalize },
	{ 0x074657DA1C7D0CCC, "sceFiberReturnToThread", (void*)sceFiberReturnToThread },
	{ 0x6B42CBAD959A7343, "sceFiberRun", (void*)sceFiberRun },
	{ 0x3C54F64BFB49ED49, "sceFiberSwitch", (void*)sceFiberSwitch },
	{ 0xA7ECCB20E836EF35, "sceFiberGetSelf", (void*)sceFiberGetSelf },
	{ 0x2DCAADCBE40D5857, "sceFiberStartContextSizeCheck", (void*)sceFiberStartContextSizeCheck },
	{ 0x2A3E275CCA6733C6, "sceFiberStopContextSizeCheck", (void*)sceFiberStopContextSizeCheck },
	{ 0xBAAD98E41173D0F1, "sceFiberGetInfo", (void*)sceFiberGetInfo },
	SCE_FUNCTION_ENTRY_END
};

static const SCE_EXPORT_LIBRARY g_pSceFiber_LibTable[] =
{
	{ "libSceFiber", g_pSceFiber_libSceFiber_FunctionTable },
	SCE_LIBRARY_ENTRY_END
};

const SCE_EXPORT_MODULE g_ExpModuleSceFiber =
{
	"libSceFiber",
	g_pSceFiber_LibTable
};


