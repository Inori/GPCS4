/*
 *    GPCS4
 *    
 *    This file implements:
 *    module: libSceAjm
 *        library: libSceAjm
 *    
 */

#pragma once

#include "sce_module_common.h"


extern const SCE_EXPORT_MODULE g_ExpModuleSceAjm;


// Note:
// The codebase is genarated using GenerateCode.py
// You may need to modify the code manually to fit development needs



//////////////////////////////////////////////////////////////////////////
// library: libSceAjm
//////////////////////////////////////////////////////////////////////////

int PS4API sceAjmBatchJobControlBufferRa(void);


int PS4API sceAjmBatchJobRunSplitBufferRa(void);


int PS4API sceAjmBatchStartBuffer(void);


int PS4API sceAjmBatchWait(void);


int PS4API sceAjmFinalize(void);


int PS4API sceAjmInitialize(void);


int PS4API sceAjmInstanceCreate(void);


int PS4API sceAjmInstanceDestroy(void);


int PS4API sceAjmModuleRegister(void);


int PS4API sceAjmModuleUnregister(void);




