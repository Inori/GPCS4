/*
 *    GPCS4
 *    
 *    This file implements:
 *    module: libSceMouse
 *        library: libSceMouse
 *    
 */

#pragma once

#include "sce_module_common.h"
#include "sce_mouse_types.h"

extern const SCE_EXPORT_MODULE g_ExpModuleSceMouse;


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs



//////////////////////////////////////////////////////////////////////////
// library: libSceMouse
//////////////////////////////////////////////////////////////////////////

int PS4API sceMouseClose(void);


int PS4API sceMouseInit(void);


int PS4API sceMouseOpen(SceUserServiceUserId userId, int32_t type, int32_t index, SceMouseOpenParam *pParam);


int PS4API sceMouseRead(int32_t handle, SceMouseData *pData, int32_t num);




