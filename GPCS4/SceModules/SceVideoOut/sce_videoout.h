/*
 *    GPCS4
 *    
 *    This file implements:
 *    module: libSceVideoOut
 *        library: libSceVideoOut
 *    
 */

#pragma once

#include "sce_module_common.h"


extern const SCE_EXPORT_MODULE g_ExpModuleSceVideoOut;


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs



//////////////////////////////////////////////////////////////////////////
// library: libSceVideoOut
//////////////////////////////////////////////////////////////////////////

int PS4API sceVideoOutClose(void);


int PS4API sceVideoOutGetResolutionStatus(void);


int PS4API sceVideoOutOpen(void);


int PS4API sceVideoOutRegisterBuffers(void);


int PS4API sceVideoOutSetBufferAttribute(void);


int PS4API sceVideoOutSetFlipRate(void);


int PS4API sceVideoOutAddFlipEvent(void);


int PS4API sceVideoOutAdjustColor_(void);


int PS4API sceVideoOutColorSettingsSetGamma_(void);


int PS4API sceVideoOutConfigureOutputMode_(void);


int PS4API sceVideoOutGetDeviceCapabilityInfo_(void);


int PS4API sceVideoOutGetEventData(void);


int PS4API sceVideoOutGetFlipStatus(void);


int PS4API sceVideoOutIsFlipPending(void);


int PS4API sceVideoOutModeSetAny_(void);


int PS4API sceVideoOutSetWindowModeMargins(void);


int PS4API sceVideoOutSubmitChangeBufferAttribute(void);


int PS4API sceVideoOutSubmitFlip(void);


int PS4API sceVideoOutWaitVblank(void);

