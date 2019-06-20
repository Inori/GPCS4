/*
 *    GPCS4
 *    
 *    This file implements:
 *    module: libSceSysmodule
 *        library: libSceSysmodule
 *    
 */

#pragma once

#include "sce_module_common.h"


extern const SCE_EXPORT_MODULE g_ExpModuleSceSysmodule;


// Note:
// The codebase is genarated using GenerateCode.py
// You may need to modify the code manually to fit development needs



//////////////////////////////////////////////////////////////////////////
// library: libSceSysmodule
//////////////////////////////////////////////////////////////////////////

int PS4API sceSysmoduleIsLoaded(void);


int PS4API sceSysmoduleLoadModule(void);


int PS4API sceSysmoduleUnloadModule(void);




