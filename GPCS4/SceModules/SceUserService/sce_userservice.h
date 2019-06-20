/*
 *    GPCS4
 *    
 *    This file implements:
 *    module: libSceUserService
 *        library: libSceUserService
 *    
 */

#pragma once

#include "sce_module_common.h"


extern const SCE_EXPORT_MODULE g_ExpModuleSceUserService;


// Note:
// The codebase is genarated using GenerateCode.py
// You may need to modify the code manually to fit development needs



//////////////////////////////////////////////////////////////////////////
// library: libSceUserService
//////////////////////////////////////////////////////////////////////////

int PS4API sceUserServiceGetEvent(void);


int PS4API sceUserServiceGetInitialUser(void);


int PS4API sceUserServiceInitialize(void);


int PS4API sceUserServiceTerminate(void);




