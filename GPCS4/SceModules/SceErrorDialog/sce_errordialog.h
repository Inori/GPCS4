/*
 *    GPCS4
 *    
 *    This file implements:
 *    module: libSceErrorDialog
 *        library: libSceErrorDialog
 *    
 */

#pragma once

#include "sce_module_common.h"


extern const SCE_EXPORT_MODULE g_ExpModuleSceErrorDialog;


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs



//////////////////////////////////////////////////////////////////////////
// library: libSceErrorDialog
//////////////////////////////////////////////////////////////////////////

int PS4API sceErrorDialogGetStatus(void);


int PS4API sceErrorDialogInitialize(void);


int PS4API sceErrorDialogOpen(void);


int PS4API sceErrorDialogTerminate(void);


int PS4API sceErrorDialogUpdateStatus(void);




