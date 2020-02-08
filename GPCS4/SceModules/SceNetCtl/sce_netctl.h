/*
 *    GPCS4
 *    
 *    This file implements:
 *    module: libSceNetCtl
 *        library: libSceNetCtl
 *    
 */

#pragma once

#include "sce_module_common.h"


extern const SCE_EXPORT_MODULE g_ExpModuleSceNetCtl;


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs



//////////////////////////////////////////////////////////////////////////
// library: libSceNetCtl
//////////////////////////////////////////////////////////////////////////

int PS4API sceNetCtlInit(void);


int PS4API sceNetCtlCheckCallback(void);


int PS4API sceNetCtlGetResult(void);


int PS4API sceNetCtlRegisterCallback(void);


int PS4API sceNetCtlUnregisterCallback(void);


int PS4API sceNetCtlGetInfo(void);




