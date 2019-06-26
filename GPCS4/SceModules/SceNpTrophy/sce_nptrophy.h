/*
 *    GPCS4
 *    
 *    This file implements:
 *    module: libSceNpTrophy
 *        library: libSceNpTrophy
 *    
 */

#pragma once

#include "sce_module_common.h"


extern const SCE_EXPORT_MODULE g_ExpModuleSceNpTrophy;


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs



//////////////////////////////////////////////////////////////////////////
// library: libSceNpTrophy
//////////////////////////////////////////////////////////////////////////

int PS4API sceNpTrophyCreateContext(void);


int PS4API sceNpTrophyCreateHandle(void);


int PS4API sceNpTrophyDestroyContext(void);


int PS4API sceNpTrophyDestroyHandle(void);


int PS4API sceNpTrophyGetTrophyUnlockState(void);


int PS4API sceNpTrophyRegisterContext(void);


int PS4API sceNpTrophyUnlockTrophy(void);


int PS4API sceNpTrophyAbortHandle(void);

