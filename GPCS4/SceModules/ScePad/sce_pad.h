/*
 *    GPCS4
 *    
 *    This file implements:
 *    module: libScePad
 *        library: libScePad
 *    
 */

#pragma once

#include "sce_module_common.h"
#include "sce_pad_types.h"
#include "sce_pad_error.h"

extern const SCE_EXPORT_MODULE g_ExpModuleScePad;


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs



//////////////////////////////////////////////////////////////////////////
// library: libScePad
//////////////////////////////////////////////////////////////////////////

int PS4API scePadClose(int32_t handle);


int PS4API scePadGetControllerInformation(int32_t handle, ScePadControllerInformation* pInfo);


int PS4API scePadGetHandle(SceUserServiceUserId userId, int32_t type, int32_t index);


int PS4API scePadInit(void);


int PS4API scePadOpen(SceUserServiceUserId userId, int32_t type, int32_t index, const ScePadOpenParam* pParam);


int PS4API scePadReadState(int32_t handle, ScePadData *pData);


int PS4API scePadResetLightBar(int32_t handle);


int PS4API scePadSetLightBar(int32_t handle, const ScePadLightBarParam *pParam);


int PS4API scePadSetMotionSensorState(void);


int PS4API scePadSetVibration(int32_t handle, const ScePadVibrationParam *pParam);


int PS4API scePadRead(int32_t handle, ScePadData *pData, int32_t num);

int PS4API scePadSetTiltCorrectionState(int32_t handle, bool bEnable);




