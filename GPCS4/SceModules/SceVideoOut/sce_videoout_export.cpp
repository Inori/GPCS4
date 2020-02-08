#include "sce_videoout.h"


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs


static const SCE_EXPORT_FUNCTION g_pSceVideoOut_libSceVideoOut_FunctionTable[] =
{
	{ 0xBAAB951F8FC3BBBF, "sceVideoOutClose", (void*)sceVideoOutClose },
	{ 0xEA43E78F9D53EB66, "sceVideoOutGetResolutionStatus", (void*)sceVideoOutGetResolutionStatus },
	{ 0x529DFA3D393AF3B1, "sceVideoOutOpen", (void*)sceVideoOutOpen },
	{ 0xC37058FAD0048906, "sceVideoOutRegisterBuffers", (void*)sceVideoOutRegisterBuffers },
	{ 0x9424C23A88116E4D, "sceVideoOutRegisterStereoBuffers", (void*)sceVideoOutRegisterStereoBuffers },
	{ 0x8BAFEC47DD56B7FE, "sceVideoOutSetBufferAttribute", (void*)sceVideoOutSetBufferAttribute },
	{ 0x0818AEE26084D430, "sceVideoOutSetFlipRate", (void*)sceVideoOutSetFlipRate },
	{ 0x1D7CE32BDC88DF49, "sceVideoOutAddFlipEvent", (void*)sceVideoOutAddFlipEvent },
	{ 0xA6FF42239542F91D, "sceVideoOutAdjustColor_", (void*)sceVideoOutAdjustColor_ },
	{ 0x0D886159B2527918, "sceVideoOutColorSettingsSetGamma_", (void*)sceVideoOutColorSettingsSetGamma_ },
	{ 0x3756C4A09E12470E, "sceVideoOutConfigureOutputMode_", (void*)sceVideoOutConfigureOutputMode_ },
	{ 0x90654B73786D404F, "sceVideoOutGetDeviceCapabilityInfo_", (void*)sceVideoOutGetDeviceCapabilityInfo_ },
	{ 0xAD651370A7645334, "sceVideoOutGetEventData", (void*)sceVideoOutGetEventData },
	{ 0x49B537770A7CD254, "sceVideoOutGetFlipStatus", (void*)sceVideoOutGetFlipStatus },
	{ 0xCE05E27C74FD12B6, "sceVideoOutIsFlipPending", (void*)sceVideoOutIsFlipPending },
	{ 0xA63903B20C658BA7, "sceVideoOutModeSetAny_", (void*)sceVideoOutModeSetAny_ },
	{ 0x313C71ACE09E4A28, "sceVideoOutSetWindowModeMargins", (void*)sceVideoOutSetWindowModeMargins },
	{ 0x20E7601E508653F9, "sceVideoOutSubmitChangeBufferAttribute", (void*)sceVideoOutSubmitChangeBufferAttribute },
	{ 0x538E8DC0E889A72B, "sceVideoOutSubmitFlip", (void*)sceVideoOutSubmitFlip },
	{ 0x8FA45A01495A2EFD, "sceVideoOutWaitVblank", (void*)sceVideoOutWaitVblank },
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


