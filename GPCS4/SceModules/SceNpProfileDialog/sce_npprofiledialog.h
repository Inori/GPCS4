/*
 *    GPCS4
 *    
 *    This file implements:
 *    module: libSceNpProfileDialog
 *        library: libSceNpProfileDialog
 *    
 */

#pragma once

#include "sce_module_common.h"


extern const SCE_EXPORT_MODULE g_ExpModuleSceNpProfileDialog;


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs



//////////////////////////////////////////////////////////////////////////
// library: libSceNpProfileDialog
//////////////////////////////////////////////////////////////////////////

int PS4API sceNpProfileDialogInitialize(void);


int PS4API sceNpProfileDialogOpen(void);


int PS4API sceNpProfileDialogTerminate(void);


int PS4API sceNpProfileDialogUpdateStatus(void);




