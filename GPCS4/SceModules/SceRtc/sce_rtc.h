/*
 *    GPCS4
 *    
 *    This file implements:
 *    module: libSceRtc
 *        library: libSceRtc
 *    
 */

#pragma once

#include "sce_module_common.h"


extern const SCE_EXPORT_MODULE g_ExpModuleSceRtc;


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs



//////////////////////////////////////////////////////////////////////////
// library: libSceRtc
//////////////////////////////////////////////////////////////////////////

int PS4API sceRtcConvertUtcToLocalTime(void);


int PS4API sceRtcGetCurrentClockLocalTime(void);


int PS4API sceRtcGetCurrentNetworkTick(void);


int PS4API sceRtcGetDayOfWeek(int day, int month, int year);


int PS4API sceRtcGetTick(void);


int PS4API sceRtcSetTick(void);


int PS4API sceRtcGetCurrentClock(void);


int PS4API sceRtcGetCurrentTick(void);




