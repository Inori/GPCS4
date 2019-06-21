#include "sce_rtc.h"


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs


static const SCE_EXPORT_FUNCTION g_pSceRtc_libSceRtc_FunctionTable[] =
{
	{ 0x3354EF16CB7F8EB3, "sceRtcConvertUtcToLocalTime", (void*)sceRtcConvertUtcToLocalTime },
	{ 0x64F0F560E288F8AC, "sceRtcGetCurrentClockLocalTime", (void*)sceRtcGetCurrentClockLocalTime },
	{ 0xCCEF542F7A8820D4, "sceRtcGetCurrentNetworkTick", (void*)sceRtcGetCurrentNetworkTick },
	{ 0x0B220AFE2E177604, "sceRtcGetDayOfWeek", (void*)sceRtcGetDayOfWeek },
	{ 0xF30FC7D7D8A9E3C2, "sceRtcGetTick", (void*)sceRtcGetTick },
	{ 0xB9E7A06BABF7194C, "sceRtcSetTick", (void*)sceRtcSetTick },
	SCE_FUNCTION_ENTRY_END
};

static const SCE_EXPORT_LIBRARY g_pSceRtc_LibTable[] =
{
	{ "libSceRtc", g_pSceRtc_libSceRtc_FunctionTable },
	SCE_LIBRARY_ENTRY_END
};

const SCE_EXPORT_MODULE g_ExpModuleSceRtc =
{
	"libSceRtc",
	g_pSceRtc_LibTable
};


