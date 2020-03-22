/*
 *    GPCS4
 *    
 *    This file implements:
 *    module: libSceFiber
 *        library: libSceFiber
 *    
 */

#pragma once

#include "sce_module_common.h"


extern const SCE_EXPORT_MODULE g_ExpModuleSceFiber;


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs


typedef void PS4API (*SceFiberEntry)(uint64_t argOnInitialize, uint64_t argOnRun) PS4NORETURN;

struct SceFiberOptParam;

struct SceFiber {
    uint64_t opaque[32 - 7];
    void* fiber;
    const char* name;
    SceFiberEntry entry;
    uint64_t argOnInitialize;
    void* addrContext;
    uint64_t sizeContext;
    const SceFiberOptParam* option;
};


struct SceFiberInfo {
    uint64_t size;
    SceFiberEntry entry;
    uint64_t argOnInitialize;
    void* addrContext;
    uint64_t sizeContext;
    char name[32];
    uint64_t sizeContextMargin;
};

//////////////////////////////////////////////////////////////////////////
// library: libSceFiber
//////////////////////////////////////////////////////////////////////////

int PS4API _sceFiberInitializeImpl(SceFiber* fiber, const char* name, SceFiberEntry entry, uint64_t argOnInitialize,
    void* addrContext, uint64_t sizeContext);


int PS4API _sceFiberInitializeWithInternalOptionImpl(SceFiber* fiber, const char* name, SceFiberEntry entry, uint64_t argOnInitialize,
    void* addrContext, uint64_t sizeContext, const SceFiberOptParam* option);


int PS4API sceFiberFinalize(SceFiber* fiber);


int PS4API sceFiberReturnToThread(void);


int PS4API sceFiberRun(SceFiber* fiber, uint64_t argOnRunTo, uint64_t* argOnReturn);


int PS4API sceFiberSwitch(SceFiber* fiber, uint64_t argOnRunTo, uint64_t* argOnRun);


int PS4API sceFiberGetSelf(SceFiber** fiber);


int PS4API sceFiberStartContextSizeCheck(uint32_t flags);


int PS4API sceFiberStopContextSizeCheck(void);


int PS4API sceFiberGetInfo(SceFiber* fiber, SceFiberInfo* fiberInfo);


