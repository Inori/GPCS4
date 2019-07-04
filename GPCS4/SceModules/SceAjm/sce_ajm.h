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
#include "sce_ajm_types.h"


extern const SCE_EXPORT_MODULE g_ExpModuleSceAjm;


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs



//////////////////////////////////////////////////////////////////////////
// library: libSceAjm
//////////////////////////////////////////////////////////////////////////

int PS4API sceAjmBatchJobControlBufferRa(void);


int PS4API sceAjmBatchJobRunSplitBufferRa(void);


int PS4API sceAjmBatchStartBuffer(void);


int PS4API sceAjmBatchWait(void);


int PS4API sceAjmFinalize(PS4UNUSED const SceAjmContextId uiContext);


int PS4API sceAjmInitialize(PS4UNUSED int64_t iReserved, SceAjmContextId * const pContext);


int PS4API sceAjmInstanceCreate(void);


int PS4API sceAjmInstanceDestroy(void);


int PS4API sceAjmModuleRegister(PS4UNUSED const SceAjmContextId uiContext, PS4UNUSED const SceAjmCodecType uiCodec, PS4UNUSED int64_t iReserved);


int PS4API sceAjmModuleUnregister(void);




