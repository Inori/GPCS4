/*
 *    GPCS4
 *    
 *    This file implements:
 *    module: libSceRudp
 *        library: libSceRudp
 *    
 */

#pragma once

#include "sce_module_common.h"


extern const SCE_EXPORT_MODULE g_ExpModuleSceRudp;


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs



//////////////////////////////////////////////////////////////////////////
// library: libSceRudp
//////////////////////////////////////////////////////////////////////////

int PS4API sceRudpBind(void);


int PS4API sceRudpCreateContext(void);


int PS4API sceRudpEnd(void);


int PS4API sceRudpGetNumberOfPacketsToRead(void);


int PS4API sceRudpGetOption(void);


int PS4API sceRudpGetSizeWritable(void);


int PS4API sceRudpInit(void);


int PS4API sceRudpInitiate(void);


int PS4API sceRudpNetReceived(void);


int PS4API sceRudpPollControl(void);


int PS4API sceRudpPollDestroy(void);


int PS4API sceRudpPollWait(void);


int PS4API sceRudpProcessEvents(void);


int PS4API sceRudpRead(void);


int PS4API sceRudpSetEventHandler(void);


int PS4API sceRudpSetOption(void);


int PS4API sceRudpTerminate(void);


int PS4API sceRudpWrite(void);




