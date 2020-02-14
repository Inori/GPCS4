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
#include "sce_videoout_types.h"


extern const SCE_EXPORT_MODULE g_ExpModuleSceVideoOut;


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs



//////////////////////////////////////////////////////////////////////////
// library: libSceVideoOut
//////////////////////////////////////////////////////////////////////////

int PS4API sceVideoOutClose(int32_t handle);


int PS4API sceVideoOutGetResolutionStatus(int32_t handle, SceVideoOutResolutionStatus *status);


int PS4API sceVideoOutOpen(SceUserServiceUserId userId, int32_t type, int32_t index, const void *param);


int PS4API sceVideoOutRegisterBuffers(int32_t handle, int32_t startIndex, void * const *addresses, int32_t bufferNum, const SceVideoOutBufferAttribute *attribute);


int PS4API sceVideoOutRegisterStereoBuffers(int32_t handle, int32_t startIndex, const SceVideoOutStereoBuffers *buffers, int32_t bufferNum, const SceVideoOutBufferAttribute *attribute);


int PS4API sceVideoOutSetBufferAttribute(SceVideoOutBufferAttribute *attribute, uint32_t pixelFormat, uint32_t tilingMode, uint32_t aspectRatio,	uint32_t width, uint32_t height, uint32_t pitchInPixel);


int PS4API sceVideoOutSetFlipRate(int32_t handle, int32_t rate);


int PS4API sceVideoOutAddFlipEvent(void);


int PS4API sceVideoOutAdjustColor_(void);


int PS4API sceVideoOutColorSettingsSetGamma_(void);


int PS4API sceVideoOutConfigureOutputMode_(void);


int PS4API sceVideoOutGetDeviceCapabilityInfo_(void);


int PS4API sceVideoOutGetEventData(void);


int PS4API sceVideoOutGetFlipStatus(int32_t handle, SceVideoOutFlipStatus *status); 


int PS4API sceVideoOutIsFlipPending(void);


int PS4API sceVideoOutModeSetAny_(void);


int PS4API sceVideoOutSetWindowModeMargins(void);


int PS4API sceVideoOutSubmitChangeBufferAttribute(void);


int PS4API sceVideoOutSubmitFlip(void);


int PS4API sceVideoOutWaitVblank(void);

