#include "sce_audioout.h"


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs


static const SCE_EXPORT_FUNCTION g_pSceAudioOut_libSceAudioOut_FunctionTable[] =
{
	{ 0xB35FFFB84F66045C, "sceAudioOutClose", (void*)sceAudioOutClose },
	{ 0x1AB43DB3822B35A4, "sceAudioOutGetPortState", (void*)sceAudioOutGetPortState },
	{ 0x25F10F5D5C6116A0, "sceAudioOutInit", (void*)sceAudioOutInit },
	{ 0x7A436FB13DB6AEC6, "sceAudioOutOpen", (void*)sceAudioOutOpen },
	{ 0x40E42D6DE0EAB13E, "sceAudioOutOutput", (void*)sceAudioOutOutput },
	{ 0xC373DD6924D2C061, "sceAudioOutOutputs", (void*)sceAudioOutOutputs },
	{ 0x6FEB8057CF489711, "sceAudioOutSetVolume", (void*)sceAudioOutSetVolume },
	{ 0x9f5e8a768c67be5d, "sceAudioOutInitIpmiGetSession", (void*)sceAudioOutInitIpmiGetSession },
	SCE_FUNCTION_ENTRY_END
};

static const SCE_EXPORT_LIBRARY g_pSceAudioOut_LibTable[] =
{
	{ "libSceAudioOut", g_pSceAudioOut_libSceAudioOut_FunctionTable },
	SCE_LIBRARY_ENTRY_END
};

const SCE_EXPORT_MODULE g_ExpModuleSceAudioOut =
{
	"libSceAudioOut",
	g_pSceAudioOut_LibTable
};


