/*
 *    GPCS4
 *    
 *    This file implements:
 *    module: libScePlayGo
 *        library: libScePlayGo
 *    
 */

#pragma once

#include "sce_module_common.h"


extern const SCE_EXPORT_MODULE g_ExpModuleScePlayGo;


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs



//////////////////////////////////////////////////////////////////////////
// library: libScePlayGo
//////////////////////////////////////////////////////////////////////////

int PS4API scePlayGoClose(void);


int PS4API scePlayGoGetLocus(void);


int PS4API scePlayGoGetProgress(void);


int PS4API scePlayGoInitialize(void);


int PS4API scePlayGoOpen(void);


int PS4API scePlayGoTerminate(void);




