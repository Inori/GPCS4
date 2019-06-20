/*
 *    GPCS4
 *    
 *    This file implements:
 *    module: libSceNpCommerce
 *        library: libSceNpCommerce
 *    
 */

#pragma once

#include "sce_module_common.h"


extern const SCE_EXPORT_MODULE g_ExpModuleSceNpCommerce;


// Note:
// The codebase is genarated using GenerateCode.py
// You may need to modify the code manually to fit development needs



//////////////////////////////////////////////////////////////////////////
// library: libSceNpCommerce
//////////////////////////////////////////////////////////////////////////

int PS4API sceNpCommerceDialogGetResult(void);


int PS4API sceNpCommerceDialogInitialize(void);


int PS4API sceNpCommerceDialogOpen(void);


int PS4API sceNpCommerceDialogTerminate(void);


int PS4API sceNpCommerceDialogUpdateStatus(void);




