/*
 *    GPCS4
 *    
 *    This file implements:
 *    module: libSceHttp
 *        library: libSceHttp
 *    
 */

#pragma once

#include "sce_module_common.h"


extern const SCE_EXPORT_MODULE g_ExpModuleSceHttp;


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs



//////////////////////////////////////////////////////////////////////////
// library: libSceHttp
//////////////////////////////////////////////////////////////////////////

int PS4API sceHttpInit(void);


int PS4API sceHttpTerm(void);


int PS4API sceHttpAddRequestHeader(void);


int PS4API sceHttpCreateConnectionWithURL(void);


int PS4API sceHttpCreateRequestWithURL(void);


int PS4API sceHttpCreateTemplate(void);


int PS4API sceHttpDeleteConnection(void);


int PS4API sceHttpDeleteRequest(void);


int PS4API sceHttpDeleteTemplate(void);


int PS4API sceHttpGetResponseContentLength(void);


int PS4API sceHttpGetStatusCode(void);


int PS4API sceHttpReadData(void);


int PS4API sceHttpSendRequest(void);


int PS4API sceHttpSetConnectTimeOut(void);


int PS4API sceHttpSetRecvTimeOut(void);


int PS4API sceHttpSetRequestContentLength(void);


int PS4API sceHttpSetResolveRetry(void);


int PS4API sceHttpSetResolveTimeOut(void);


int PS4API sceHttpSetSendTimeOut(void);


int PS4API sceHttpUriEscape(void);




