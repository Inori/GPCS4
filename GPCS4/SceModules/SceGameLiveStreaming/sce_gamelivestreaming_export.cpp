#include "sce_gamelivestreaming.h"


// Note:
// The codebase is genarated using GenerateCode.py
// You may need to modify the code manually to fit development needs


static const SCE_EXPORT_FUNCTION g_pSceGameLiveStreaming_libSceGameLiveStreaming_FunctionTable[] =
{
	{ 0x75633CD005F7F68E, "sceGameLiveStreamingEnableLiveStreaming", (void*)sceGameLiveStreamingEnableLiveStreaming },
	{ 0x92F604C369419DD9, "sceGameLiveStreamingInitialize", (void*)sceGameLiveStreamingInitialize },
	{ 0xFC41E753AF201315, "sceGameLiveStreamingPermitServerSideRecording", (void*)sceGameLiveStreamingPermitServerSideRecording },
	{ 0x1B0E92E28AA563B1, "sceGameLiveStreamingSetGuardAreas", (void*)sceGameLiveStreamingSetGuardAreas },
	{ 0xF722BA164F2628E4, "sceGameLiveStreamingTerminate", (void*)sceGameLiveStreamingTerminate },
	SCE_FUNCTION_ENTRY_END
};

static const SCE_EXPORT_LIBRARY g_pSceGameLiveStreaming_LibTable[] =
{
	{ "libSceGameLiveStreaming", g_pSceGameLiveStreaming_libSceGameLiveStreaming_FunctionTable },
	SCE_LIBRARY_ENTRY_END
};

const SCE_EXPORT_MODULE g_ExpModuleSceGameLiveStreaming =
{
	"libSceGameLiveStreaming",
	g_pSceGameLiveStreaming_LibTable
};


