/*
 *    GPCS4
 *    
 *    This file implements:
 *    module: libSceInvitationDialog
 *        library: libSceInvitationDialog
 *    
 */

#pragma once

#include "sce_module_common.h"


extern const SCE_EXPORT_MODULE g_ExpModuleSceInvitationDialog;


// Note:
// The codebase is genarated using GenerateCode.py
// You may need to modify the code manually to fit development needs



//////////////////////////////////////////////////////////////////////////
// library: libSceInvitationDialog
//////////////////////////////////////////////////////////////////////////

int PS4API sceInvitationDialogInitialize(void);


int PS4API sceInvitationDialogOpen(void);


int PS4API sceInvitationDialogTerminate(void);


int PS4API sceInvitationDialogUpdateStatus(void);




