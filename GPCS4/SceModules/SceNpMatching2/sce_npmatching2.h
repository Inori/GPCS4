/*
 *    GPCS4
 *    
 *    This file implements:
 *    module: libSceNpMatching2
 *        library: libSceNpMatching2
 *    
 */

#pragma once

#include "sce_module_common.h"


extern const SCE_EXPORT_MODULE g_ExpModuleSceNpMatching2;


// Note:
// The codebase is genarated using GenerateCode.py
// You may need to modify the code manually to fit development needs



//////////////////////////////////////////////////////////////////////////
// library: libSceNpMatching2
//////////////////////////////////////////////////////////////////////////

int PS4API sceNpMatching2ContextStart(void);


int PS4API sceNpMatching2ContextStop(void);


int PS4API sceNpMatching2CreateContext(void);


int PS4API sceNpMatching2CreateJoinRoom(void);


int PS4API sceNpMatching2DestroyContext(void);


int PS4API sceNpMatching2GetRoomDataExternalList(void);


int PS4API sceNpMatching2GetRoomDataInternal(void);


int PS4API sceNpMatching2GetRoomMemberDataInternal(void);


int PS4API sceNpMatching2GetRoomMemberIdListLocal(void);


int PS4API sceNpMatching2GetServerId(void);


int PS4API sceNpMatching2GetWorldInfoList(void);


int PS4API sceNpMatching2GrantRoomOwner(void);


int PS4API sceNpMatching2Initialize(void);


int PS4API sceNpMatching2JoinRoom(void);


int PS4API sceNpMatching2KickoutRoomMember(void);


int PS4API sceNpMatching2LeaveRoom(void);


int PS4API sceNpMatching2RegisterContextCallback(void);


int PS4API sceNpMatching2RegisterRoomEventCallback(void);


int PS4API sceNpMatching2RegisterRoomMessageCallback(void);


int PS4API sceNpMatching2RegisterSignalingCallback(void);


int PS4API sceNpMatching2SearchRoom(void);


int PS4API sceNpMatching2SendRoomMessage(void);


int PS4API sceNpMatching2SetRoomDataExternal(void);


int PS4API sceNpMatching2SetRoomDataInternal(void);


int PS4API sceNpMatching2SetRoomMemberDataInternal(void);


int PS4API sceNpMatching2Terminate(void);




