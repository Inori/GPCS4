#include "sce_pad.h"


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs


static const SCE_EXPORT_FUNCTION g_pScePad_libScePad_FunctionTable[] =
{
	{ 0xEA77207B9FA5E50B, "scePadClose", (void*)scePadClose },
	{ 0x8233FDFCA433A149, "scePadGetControllerInformation", (void*)scePadGetControllerInformation },
	{ 0xBB51911E9FA85A86, "scePadGetHandle", (void*)scePadGetHandle },
	{ 0x86FD65BA226BA903, "scePadInit", (void*)scePadInit },
	{ 0xC64D0071AACFDD5E, "scePadOpen", (void*)scePadOpen },
	{ 0x6277605EA41557B7, "scePadReadState", (void*)scePadReadState },
	{ 0x0EC703D62F475F5C, "scePadResetLightBar", (void*)scePadResetLightBar },
	{ 0x451E27A2F50410D6, "scePadSetLightBar", (void*)scePadSetLightBar },
	{ 0x72556F2F86439EDC, "scePadSetMotionSensorState", (void*)scePadSetMotionSensorState },
	{ 0xC8556739D1B1BD96, "scePadSetVibration", (void*)scePadSetVibration },
	{ 0xAB570735F1B270B2, "scePadRead", (void*)scePadRead },
	{ 0xbc32cca092dd7bc2, "scePadSetTiltCorrectionState", (void*)scePadSetTiltCorrectionState },
	SCE_FUNCTION_ENTRY_END
};

static const SCE_EXPORT_LIBRARY g_pScePad_LibTable[] =
{
	{ "libScePad", g_pScePad_libScePad_FunctionTable },
	SCE_LIBRARY_ENTRY_END
};

const SCE_EXPORT_MODULE g_ExpModuleScePad =
{
	"libScePad",
	g_pScePad_LibTable
};


