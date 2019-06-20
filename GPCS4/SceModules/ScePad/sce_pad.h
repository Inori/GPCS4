/*
 *    GPCS4
 *    
 *    This file implements:
 *    module: libScePad
 *        library: libScePad
 *    
 */

#pragma once

#include "sce_module_common.h"


extern const SCE_EXPORT_MODULE g_ExpModuleScePad;


// Note:
// The codebase is genarated using GenerateCode.py
// You may need to modify the code manually to fit development needs



//////////////////////////////////////////////////////////////////////////
// library: libScePad
//////////////////////////////////////////////////////////////////////////

int PS4API scePadClose(void);


int PS4API scePadGetControllerInformation(void);


int PS4API scePadInit(void);


int PS4API scePadOpen(void);


int PS4API scePadReadState(void);


int PS4API scePadResetLightBar(void);


int PS4API scePadSetLightBar(void);


int PS4API scePadSetVibration(void);




