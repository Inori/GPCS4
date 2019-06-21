/*
 *    GPCS4
 *    
 *    This file implements:
 *    module: libSceNpSignaling
 *        library: libSceNpSignaling
 *    
 */

#pragma once

#include "sce_module_common.h"


extern const SCE_EXPORT_MODULE g_ExpModuleSceNpSignaling;


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs



//////////////////////////////////////////////////////////////////////////
// library: libSceNpSignaling
//////////////////////////////////////////////////////////////////////////

int PS4API sceNpSignalingActivateConnection(void);


int PS4API sceNpSignalingCreateContext(void);


int PS4API sceNpSignalingDeleteContext(void);


int PS4API sceNpSignalingGetConnectionFromNpId(void);


int PS4API sceNpSignalingGetConnectionInfo(void);


int PS4API sceNpSignalingGetConnectionStatus(void);


int PS4API sceNpSignalingGetLocalNetInfo(void);


int PS4API sceNpSignalingInitialize(void);


int PS4API sceNpSignalingTerminate(void);


int PS4API sceNpSignalingTerminateConnection(void);




