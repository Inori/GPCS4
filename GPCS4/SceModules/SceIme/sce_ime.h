/*
 *    GPCS4
 *    
 *    This file implements:
 *    module: libSceIme
 *        library: libSceIme
 *    
 */

#pragma once

#include "sce_module_common.h"


extern const SCE_EXPORT_MODULE g_ExpModuleSceIme;


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs



//////////////////////////////////////////////////////////////////////////
// library: libSceIme
//////////////////////////////////////////////////////////////////////////

int PS4API sceImeClose(void);


int PS4API sceImeKeyboardClose(void);


int PS4API sceImeKeyboardOpen(void);


int PS4API sceImeOpen(void);


int PS4API sceImeParamInit(void);


int PS4API sceImeSetCaret(void);


int PS4API sceImeSetText(void);


int PS4API sceImeUpdate(void);




