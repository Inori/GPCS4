/*
 *    GPCS4
 *    
 *    This file implements:
 *    module: libSceNpManager
 *        library: libSceNpManager
 *    
 */

#pragma once

#include "sce_module_common.h"


extern const SCE_EXPORT_MODULE g_ExpModuleSceNpManager;


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs



//////////////////////////////////////////////////////////////////////////
// library: libSceNpManager
//////////////////////////////////////////////////////////////////////////

int PS4API sceNpAbortRequest(void);


int PS4API sceNpCheckCallback(void);


int PS4API sceNpCheckNpAvailability(void);


int PS4API sceNpCheckNpAvailabilityA(void);


int PS4API sceNpCheckPlus(void);


int PS4API sceNpCreateAsyncRequest(void);


int PS4API sceNpDeleteRequest(void);


int PS4API sceNpGetNpId(void);


int PS4API sceNpGetOnlineId(void);


int PS4API sceNpGetParentalControlInfo(void);


int PS4API sceNpNotifyPlusFeature(void);


int PS4API sceNpPollAsync(void);


int PS4API sceNpRegisterGamePresenceCallback(void);


int PS4API sceNpRegisterNpReachabilityStateCallback(void);


int PS4API sceNpRegisterPlusEventCallback(void);


int PS4API sceNpRegisterStateCallback(void);


int PS4API sceNpSetContentRestriction(void);


int PS4API sceNpSetNpTitleId(void);


int PS4API sceNpUnregisterNpReachabilityStateCallback(void);


int PS4API sceNpUnregisterPlusEventCallback(void);


int PS4API sceNpUnregisterStateCallback(void);


int PS4API sceNpCreateRequest(void);


int PS4API sceNpGetAccountCountryA(void);


int PS4API sceNpGetAccountDateOfBirthA(void);


int PS4API sceNpGetAccountIdA(void);


int PS4API sceNpGetAccountLanguageA(void);




