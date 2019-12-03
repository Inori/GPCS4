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
#include "sce_audioout_types.h"
#include "sce_audioout_error.h"

extern const SCE_EXPORT_MODULE g_ExpModuleSceAudioOut;


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs



//////////////////////////////////////////////////////////////////////////
// library: libSceAudioOut
//////////////////////////////////////////////////////////////////////////

int PS4API sceAudioOutClose(int32_t handle);


int PS4API sceAudioOutGetPortState(int32_t handle, SceAudioOutPortState *state);


int PS4API sceAudioOutInit(void);


int PS4API sceAudioOutOpen(SceUserServiceUserId userId, int32_t type, int32_t index, uint32_t len, uint32_t freq, uint32_t param);


int PS4API sceAudioOutOutput(void);


int PS4API sceAudioOutOutputs(void);


int PS4API sceAudioOutSetVolume(int32_t handle, int32_t flag, int32_t *vol);




