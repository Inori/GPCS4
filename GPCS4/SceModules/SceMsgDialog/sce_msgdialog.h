/*
 *    GPCS4
 *    
 *    This file implements:
 *    module: libSceMsgDialog
 *        library: libSceMsgDialog
 *    
 */

#pragma once

#include "sce_module_common.h"
#include "sce_msgdialog_error.h"
#include "sce_msgdialog_types.h"


extern const SCE_EXPORT_MODULE g_ExpModuleSceMsgDialog;


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs



//////////////////////////////////////////////////////////////////////////
// library: libSceMsgDialog
//////////////////////////////////////////////////////////////////////////

int PS4API sceMsgDialogGetResult(SceMsgDialogResult* result);


int PS4API sceMsgDialogGetStatus(void);


int PS4API sceMsgDialogInitialize(void);


int PS4API sceMsgDialogOpen(const SceMsgDialogParam *param);


int PS4API sceMsgDialogTerminate(void);


int PS4API sceMsgDialogUpdateStatus(void);




