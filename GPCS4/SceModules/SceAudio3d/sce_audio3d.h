/*
 *    GPCS4
 *    
 *    This file implements:
 *    module: libSceAudio3d
 *        library: libSceAudio3d
 *    
 */

#pragma once

#include "sce_module_common.h"


extern const SCE_EXPORT_MODULE g_ExpModuleSceAudio3d;


// Note:
// The codebase is genarated using GenerateCode.py
// You may need to modify the code manually to fit development needs



//////////////////////////////////////////////////////////////////////////
// library: libSceAudio3d
//////////////////////////////////////////////////////////////////////////

int PS4API sceAudio3dBedWrite(void);


int PS4API sceAudio3dGetDefaultOpenParameters(void);


int PS4API sceAudio3dInitialize(void);


int PS4API sceAudio3dObjectReserve(void);


int PS4API sceAudio3dObjectSetAttributes(void);


int PS4API sceAudio3dObjectUnreserve(void);


int PS4API sceAudio3dPortAdvance(void);


int PS4API sceAudio3dPortClose(void);


int PS4API sceAudio3dPortFlush(void);


int PS4API sceAudio3dPortGetQueueLevel(void);


int PS4API sceAudio3dPortOpen(void);


int PS4API sceAudio3dPortPush(void);


int PS4API sceAudio3dPortSetAttribute(void);


int PS4API sceAudio3dTerminate(void);




