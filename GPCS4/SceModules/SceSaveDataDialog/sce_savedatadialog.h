/*
 *    GPCS4
 *    
 *    This file implements:
 *    module: libSceSaveDataDialog
 *        library: libSceSaveDataDialog
 *    
 */

#pragma once

#include "sce_module_common.h"


extern const SCE_EXPORT_MODULE g_ExpModuleSceSaveDataDialog;


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs



//////////////////////////////////////////////////////////////////////////
// library: libSceSaveDataDialog
//////////////////////////////////////////////////////////////////////////

int PS4API sceSaveDataDialogInitialize(void);


int PS4API sceSaveDataDialogOpen(void);


int PS4API sceSaveDataDialogTerminate(void);


int PS4API sceSaveDataDialogUpdateStatus(void);


int PS4API sceSaveDataDialogGetResult(void);


int PS4API sceSaveDataDialogProgressBarSetValue(void);

