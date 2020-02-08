/*
 *    GPCS4
 *    
 *    This file implements:
 *    module: libSceAppContentUtil
 *        library: libSceAppContent
 *    
 */

#pragma once

#include "sce_module_common.h"
#include "sce_appcontentutil_types.h"
#include "sce_appcontentutil_error.h"

extern const SCE_EXPORT_MODULE g_ExpModuleSceAppContentUtil;


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs



//////////////////////////////////////////////////////////////////////////
// library: libSceAppContent
//////////////////////////////////////////////////////////////////////////

int PS4API sceAppContentAddcontEnqueueDownload(void);


int PS4API sceAppContentAddcontMount(void);


int PS4API sceAppContentAddcontUnmount(void);


int PS4API sceAppContentGetAddcontInfoList(void);


int PS4API sceAppContentInitialize(const SceAppContentInitParam *initParam, SceAppContentBootParam *bootParam);


int PS4API sceAppContentAppParamGetInt(SceAppContentAppParamId paramId, int32_t *value);

