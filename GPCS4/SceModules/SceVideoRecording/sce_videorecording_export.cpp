#include "sce_videorecording.h"


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs


static const SCE_EXPORT_FUNCTION g_pSceVideoRecording_libSceVideoRecording_FunctionTable[] =
{
	{ 0x15CF2AC652883584, "sceVideoRecordingSetInfo", (void*)sceVideoRecordingSetInfo },
	SCE_FUNCTION_ENTRY_END
};

static const SCE_EXPORT_LIBRARY g_pSceVideoRecording_LibTable[] =
{
	{ "libSceVideoRecording", g_pSceVideoRecording_libSceVideoRecording_FunctionTable },
	SCE_LIBRARY_ENTRY_END
};

const SCE_EXPORT_MODULE g_ExpModuleSceVideoRecording =
{
	"libSceVideoRecording",
	g_pSceVideoRecording_LibTable
};


