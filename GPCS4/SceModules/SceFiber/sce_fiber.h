/*
 *    GPCS4
 *    
 *    This file implements:
 *    module: libSceFiber
 *        library: libSceFiber
 *    
 */

#pragma once

#include "sce_module_common.h"


extern const SCE_EXPORT_MODULE g_ExpModuleSceFiber;


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs



//////////////////////////////////////////////////////////////////////////
// library: libSceFiber
//////////////////////////////////////////////////////////////////////////

int PS4API _sceFiberInitializeImpl(void);


int PS4API sceFiberFinalize(void);


int PS4API sceFiberReturnToThread(void);


int PS4API sceFiberRun(void);


int PS4API sceFiberSwitch(void);


int PS4API sceFiberGetSelf(void);




