/*
 *    GPCS4
 *    
 *    This file implements:
 *    module: libSceScreenShot
 *        library: libSceScreenShot
 *    
 */

#pragma once

#include "sce_module_common.h"


extern const SCE_EXPORT_MODULE g_ExpModuleSceScreenShot;


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs



//////////////////////////////////////////////////////////////////////////
// library: libSceScreenShot
//////////////////////////////////////////////////////////////////////////

int PS4API sceScreenShotDisable(void);


int PS4API sceScreenShotEnable(void);


int PS4API sceScreenShotSetOverlayImageWithOrigin(void);

