/*
 *    GPCS4
 *    
 *    This file implements:
 *    module: libSceAudioOut
 *        library: libSceAudioOut
 *    
 */

#pragma once

#include "sce_module_common.h"


extern const SCE_EXPORT_MODULE g_ExpModuleSceAudioOut;


// Note:
// The codebase is genarated using GenerateCode.py
// You may need to modify the code manually to fit development needs



//////////////////////////////////////////////////////////////////////////
// library: libSceAudioOut
//////////////////////////////////////////////////////////////////////////

int PS4API sceAudioOutClose(void);


int PS4API sceAudioOutGetPortState(void);


int PS4API sceAudioOutInit(void);


int PS4API sceAudioOutOpen(void);


int PS4API sceAudioOutOutput(void);


int PS4API sceAudioOutOutputs(void);


int PS4API sceAudioOutSetVolume(void);




