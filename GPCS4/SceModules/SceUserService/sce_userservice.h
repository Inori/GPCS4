/*
 *    GPCS4
 *    
 *    This file implements:
 *    module: libSceUserService
 *        library: libSceUserService
 *    
 */

#pragma once

#include "sce_module_common.h"
#include "sce_userservice_types.h"

extern const SCE_EXPORT_MODULE g_ExpModuleSceUserService;


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs



//////////////////////////////////////////////////////////////////////////
// library: libSceUserService
//////////////////////////////////////////////////////////////////////////

int PS4API sceUserServiceGetEvent(SceUserServiceEvent* event);


int PS4API sceUserServiceGetInitialUser(SceUserServiceUserId *userId);


int PS4API sceUserServiceInitialize(const SceUserServiceInitializeParams *initParams);


int PS4API sceUserServiceInitialize2( void );


int PS4API sceUserServiceGetLoginUserIdList(SceUserServiceLoginUserIdList* userIdList);


int PS4API sceUserServiceTerminate(void);


int PS4API sceUserServiceGetUserName(const SceUserServiceUserId userId, char* userName, const size_t size);




