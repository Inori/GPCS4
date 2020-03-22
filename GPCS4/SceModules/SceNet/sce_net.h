/*
 *    GPCS4
 *    
 *    This file implements:
 *    module: libSceNet
 *        library: libSceNet
 *    
 */

#pragma once

#include "sce_module_common.h"


extern const SCE_EXPORT_MODULE g_ExpModuleSceNet;


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs



//////////////////////////////////////////////////////////////////////////
// library: libSceNet
//////////////////////////////////////////////////////////////////////////

int PS4API sceNetAccept(void);


int PS4API sceNetBind(void);


int PS4API sceNetConnect(void);


int PS4API sceNetEpollAbort(void);


int PS4API sceNetEpollControl(void);


int PS4API sceNetEpollCreate(void);


int PS4API sceNetEpollDestroy(void);


int PS4API sceNetEpollWait(void);


int PS4API sceNetErrnoLoc(void);


int PS4API sceNetGetsockopt(void);


int PS4API sceNetHtonl(void);


int PS4API sceNetHtons(void);


int PS4API sceNetInit(void);


int PS4API sceNetListen(void);


int PS4API sceNetNtohs(void);


int PS4API sceNetPoolCreate(const char *name, int size, int flags);


int PS4API sceNetPoolDestroy(void);


int PS4API sceNetRecv(void);


int PS4API sceNetRecvfrom(void);


int PS4API sceNetSend(void);


int PS4API sceNetSendto(void);


int PS4API sceNetSetsockopt(void);


int PS4API sceNetShutdown(void);


int PS4API sceNetSocket(void);


int PS4API sceNetSocketAbort(void);


int PS4API sceNetSocketClose(void);


int PS4API sceNetTerm(void);


int PS4API sceNetGetMacAddress(void);


int PS4API sceNetResolverCreate(void);


int PS4API sceNetResolverDestroy(void);


int PS4API sceNetResolverStartNtoa(void);




