#include "sce_videoout.h"


// Note:
// The codebase is genarated using GenerateCode.py
// You may need to modify the code manually to fit development needs


static const SCE_EXPORT_FUNCTION g_pSceVideoOut_libSceVideoOut_FunctionTable[] =
{
	{ 0xBAAB951F8FC3BBBF, "sceVideoOutClose", (void*)sceVideoOutClose },
	{ 0xEA43E78F9D53EB66, "sceVideoOutGetResolutionStatus", (void*)sceVideoOutGetResolutionStatus },
	{ 0x529DFA3D393AF3B1, "sceVideoOutOpen", (void*)sceVideoOutOpen },
	{ 0xC37058FAD0048906, "sceVideoOutRegisterBuffers", (void*)sceVideoOutRegisterBuffers },
	{ 0x8BAFEC47DD56B7FE, "sceVideoOutSetBufferAttribute", (void*)sceVideoOutSetBufferAttribute },
	{ 0x0818AEE26084D430, "sceVideoOutSetFlipRate", (void*)sceVideoOutSetFlipRate },
	SCE_FUNCTION_ENTRY_END
};

static const SCE_EXPORT_LIBRARY g_pSceVideoOut_LibTable[] =
{
	{ "libSceVideoOut", g_pSceVideoOut_libSceVideoOut_FunctionTable },
	SCE_LIBRARY_ENTRY_END
};

const SCE_EXPORT_MODULE g_ExpModuleSceVideoOut =
{
	"libSceVideoOut",
	g_pSceVideoOut_LibTable
};


