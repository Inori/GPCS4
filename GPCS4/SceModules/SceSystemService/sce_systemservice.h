/*
 *    GPCS4
 *    
 *    This file implements:
 *    module: libSceSystemService
 *        library: libSceSystemService
 *    
 */

#pragma once

#include "sce_module_common.h"


extern const SCE_EXPORT_MODULE g_ExpModuleSceSystemService;


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs



//////////////////////////////////////////////////////////////////////////
// library: libSceSystemService
//////////////////////////////////////////////////////////////////////////

int PS4API sceSystemServiceGetStatus(void);


int PS4API sceSystemServiceHideSplashScreen(void);


int PS4API sceSystemServiceParamGetInt(void);


int PS4API sceSystemServiceReceiveEvent(void);


int PS4API sceSystemServiceGetDisplaySafeAreaInfo(void);


//////////////////////////////////////////////////////////////////////////
// library: libSceSystemServicePlatformPrivacy
//////////////////////////////////////////////////////////////////////////

int PS4API sceSystemServiceGetPlatformPrivacySetting(void);

