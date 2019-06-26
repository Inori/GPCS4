/*
 *    GPCS4
 *    
 *    This file implements:
 *    module: libSceNpWebApi
 *        library: libSceNpWebApi
 *    
 */

#pragma once

#include "sce_module_common.h"


extern const SCE_EXPORT_MODULE g_ExpModuleSceNpWebApi;


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs



//////////////////////////////////////////////////////////////////////////
// library: libSceNpWebApi
//////////////////////////////////////////////////////////////////////////

int PS4API sceNpWebApiAbortRequest(void);


int PS4API sceNpWebApiAddMultipartPart(void);


int PS4API sceNpWebApiCreateContext(void);


int PS4API sceNpWebApiCreateMultipartRequest(void);


int PS4API sceNpWebApiCreateRequest(void);


int PS4API sceNpWebApiDeleteContext(void);


int PS4API sceNpWebApiDeleteRequest(void);


int PS4API sceNpWebApiGetMemoryPoolStats(void);


int PS4API sceNpWebApiInitialize(void);


int PS4API sceNpWebApiReadData(void);


int PS4API sceNpWebApiSendMultipartRequest2(void);


int PS4API sceNpWebApiSendRequest2(void);


int PS4API sceNpWebApiTerminate(void);


int PS4API sceNpWebApiCreateContextA(void);



