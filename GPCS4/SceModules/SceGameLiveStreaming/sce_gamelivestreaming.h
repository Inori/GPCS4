/*
 *    GPCS4
 *    
 *    This file implements:
 *    module: libSceGameLiveStreaming
 *        library: libSceGameLiveStreaming
 *    
 */

#pragma once

#include "sce_module_common.h"


extern const SCE_EXPORT_MODULE g_ExpModuleSceGameLiveStreaming;


// Note:
// The codebase is genarated using GenerateCode.py
// You may need to modify the code manually to fit development needs



//////////////////////////////////////////////////////////////////////////
// library: libSceGameLiveStreaming
//////////////////////////////////////////////////////////////////////////

int PS4API sceGameLiveStreamingEnableLiveStreaming(void);


int PS4API sceGameLiveStreamingInitialize(void);


int PS4API sceGameLiveStreamingPermitServerSideRecording(void);


int PS4API sceGameLiveStreamingSetGuardAreas(void);


int PS4API sceGameLiveStreamingTerminate(void);




