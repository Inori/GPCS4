#include "sce_audio3d.h"


// Note:
// The codebase is genarated using GenerateCode.py
// You may need to modify the code manually to fit development needs


static const SCE_EXPORT_FUNCTION g_pSceAudio3d_libSceAudio3d_FunctionTable[] =
{
	{ 0xF6D130134195D2AA, "sceAudio3dBedWrite", (void*)sceAudio3dBedWrite },
	{ 0x226FA33A86B95802, "sceAudio3dGetDefaultOpenParameters", (void*)sceAudio3dGetDefaultOpenParameters },
	{ 0x5260AF8D29AE648C, "sceAudio3dInitialize", (void*)sceAudio3dInitialize },
	{ 0x8CEDAD79CE1D2763, "sceAudio3dObjectReserve", (void*)sceAudio3dObjectReserve },
	{ 0xE2EC8737DAB865E5, "sceAudio3dObjectSetAttributes", (void*)sceAudio3dObjectSetAttributes },
	{ 0xD475F1A3FFB5A82C, "sceAudio3dObjectUnreserve", (void*)sceAudio3dObjectUnreserve },
	{ 0x970D2AADD4A366DF, "sceAudio3dPortAdvance", (void*)sceAudio3dPortAdvance },
	{ 0x3B256A39E54DB529, "sceAudio3dPortClose", (void*)sceAudio3dPortClose },
	{ 0x64E1ABC562E04331, "sceAudio3dPortFlush", (void*)sceAudio3dPortFlush },
	{ 0x61A6836C3C0AA453, "sceAudio3dPortGetQueueLevel", (void*)sceAudio3dPortGetQueueLevel },
	{ 0x5DE0C32B4C495900, "sceAudio3dPortOpen", (void*)sceAudio3dPortOpen },
	{ 0x54456167DA9DE196, "sceAudio3dPortPush", (void*)sceAudio3dPortPush },
	{ 0x62AF5B7D4434B898, "sceAudio3dPortSetAttribute", (void*)sceAudio3dPortSetAttribute },
	{ 0x596D534B68B3E727, "sceAudio3dTerminate", (void*)sceAudio3dTerminate },
	SCE_FUNCTION_ENTRY_END
};

static const SCE_EXPORT_LIBRARY g_pSceAudio3d_LibTable[] =
{
	{ "libSceAudio3d", g_pSceAudio3d_libSceAudio3d_FunctionTable },
	SCE_LIBRARY_ENTRY_END
};

const SCE_EXPORT_MODULE g_ExpModuleSceAudio3d =
{
	"libSceAudio3d",
	g_pSceAudio3d_LibTable
};


