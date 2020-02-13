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

extern const SCE_EXPORT_MODULE g_ExpModuleSceAudioOut;


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs

#define SCE_AUDIO_OUT_PARAM_FORMAT_S16_MONO     0
#define SCE_AUDIO_OUT_PARAM_FORMAT_S16_STEREO   1
#define SCE_AUDIO_OUT_PARAM_FORMAT_S16_8CH      2
#define SCE_AUDIO_OUT_PARAM_FORMAT_FLOAT_MONO   3
#define SCE_AUDIO_OUT_PARAM_FORMAT_FLOAT_STEREO 4
#define SCE_AUDIO_OUT_PARAM_FORMAT_FLOAT_8CH    5

#define SCE_AUDIO_OUT_PARAM_FORMAT_S16_8CH_STD   6
#define SCE_AUDIO_OUT_PARAM_FORMAT_FLOAT_8CH_STD 7

#define SCE_AUDIO_OUT_PARAM_FORMAT_MASK  0x000000FF
#define SCE_AUDIO_OUT_PARAM_FORMAT_SHIFT 0

#define SCE_AUDIO_OUT_ERROR_TRANS_EVENT -2144993262
//////////////////////////////////////////////////////////////////////////
// library: libSceAudioOut
//////////////////////////////////////////////////////////////////////////

int32_t PS4API sceAudioOutClose(int32_t handle);


int PS4API sceAudioOutGetPortState(int32_t handle, SceAudioOutPortState *state);


int PS4API sceAudioOutInit(void);


int32_t PS4API sceAudioOutOpen(SceUserServiceUserId userId, int32_t type, int32_t index, uint32_t len, uint32_t freq, uint32_t param);


int32_t PS4API sceAudioOutOutput(int32_t handle, const void *p);


int PS4API sceAudioOutOutputs(void);


int PS4API sceAudioOutSetVolume(void);


