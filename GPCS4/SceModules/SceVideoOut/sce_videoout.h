/*
 *    GPCS4
 *    
 *    This file implements:
 *    module: libSceVideoOut
 *        library: libSceVideoOut
 *    
 */

#pragma once

#include "sce_module_common.h"


extern const SCE_EXPORT_MODULE g_ExpModuleSceVideoOut;


// Note:
// The codebase is genarated using GenerateCode.py
// You may need to modify the code manually to fit development needs



//////////////////////////////////////////////////////////////////////////
// library: libSceVideoOut
//////////////////////////////////////////////////////////////////////////

int PS4API sceVideoOutClose(void);


int PS4API sceVideoOutGetResolutionStatus(void);


int PS4API sceVideoOutOpen(void);


int PS4API sceVideoOutRegisterBuffers(void);


int PS4API sceVideoOutSetBufferAttribute(void);


int PS4API sceVideoOutSetFlipRate(void);




