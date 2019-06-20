/*
 *    GPCS4
 *    
 *    This file implements:
 *    module: libSceNpTus
 *        library: libSceNpTus
 *    
 */

#pragma once

#include "sce_module_common.h"


extern const SCE_EXPORT_MODULE g_ExpModuleSceNpTus;


// Note:
// The codebase is genarated using GenerateCode.py
// You may need to modify the code manually to fit development needs



//////////////////////////////////////////////////////////////////////////
// library: libSceNpTus
//////////////////////////////////////////////////////////////////////////

int PS4API sceNpTusAddAndGetVariableAsync(void);


int PS4API sceNpTusCreateNpTitleCtx(void);


int PS4API sceNpTusCreateRequest(void);


int PS4API sceNpTusDeleteNpTitleCtx(void);


int PS4API sceNpTusDeleteRequest(void);


int PS4API sceNpTusGetMultiSlotVariableAsync(void);


int PS4API sceNpTusPollAsync(void);


int PS4API sceNpTusSetMultiSlotVariableAsync(void);


int PS4API sceNpTusTryAndSetVariableAsync(void);




