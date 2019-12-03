/*
 *    GPCS4
 *    
 *    This file implements:
 *    module: libSceSystemService
 *        library: libSceSystemService
 *    
 */

#pragma once

#include "sce_module_common.h"
#include "sce_systemservice_types.h"

extern const SCE_EXPORT_MODULE g_ExpModuleSceSystemService;


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs



//////////////////////////////////////////////////////////////////////////
// library: libSceSystemService
//////////////////////////////////////////////////////////////////////////

struct SceSystemServiceDisplaySafeAreaInfo {
	float ratio;
	uint8_t reserved[];
};

enum SceSystemServiceEventType
{
	SCE_SYSTEM_SERVICE_EVENT_ON_RESUME = 0x10000000,
	SCE_SYSTEM_SERVICE_EVENT_GAME_LIVE_STREAMING_STATUS_UPDATE = 0x10000001,
	SCE_SYSTEM_SERVICE_EVENT_SESSION_INVITATION = 0x10000002,
	SCE_SYSTEM_SERVICE_EVENT_ENTITLEMENT_UPDATE = 0x10000003,
	SCE_SYSTEM_SERVICE_EVENT_GAME_CUSTOM_DATA = 0x10000004,
	SCE_SYSTEM_SERVICE_EVENT_LAUNCH_APP = 0x10000007,
	SCE_SYSTEM_SERVICE_EVENT_APP_LAUNCH_LINK = 0x10000008,
	SCE_SYSTEM_SERVICE_EVENT_ADDCONTENT_INSTALL = 0x10000009,
	SCE_SYSTEM_SERVICE_EVENT_RESET_VR_POSITION = 0x1000000a,
	SCE_SYSTEM_SERVICE_EVENT_JOIN_EVENT = 0x1000000b,
	SCE_SYSTEM_SERVICE_EVENT_PLAYGO_LOCUS_UPDATE = 0x1000000c,
	SCE_SYSTEM_SERVICE_EVENT_SERVICE_ENTITLEMENT_UPDATE = 0x1000000e,
	SCE_SYSTEM_SERVICE_EVENT_EYE_TO_EYE_DISTANCE_UPDATE = 0x1000000f,
	SCE_SYSTEM_SERVICE_EVENT_JOIN_MATCH_EVENT = 0x10000010,
	SCE_SYSTEM_SERVICE_EVENT_PLAY_TOGETHER_HOST_A = 0x10000011,
	SCE_SYSTEM_SERVICE_EVENT_WEBBROWSER_CLOSED = 0x10000012,
	SCE_SYSTEM_SERVICE_EVENT_CONTROLLER_SETTINGS_CLOSED = 0x10000013,
	SCE_SYSTEM_SERVICE_EVENT_JOIN_TEAM_ON_TEAM_MATCH_EVENT = 0x10000014,
	SCE_SYSTEM_SERVICE_EVENT_OPEN_SHARE_MENU = 0x30000000
};

struct SceSystemServiceEvent {
	SceSystemServiceEventType eventType;
	union {
		char param[8192];
		struct {
			uint32_t size;
			uint8_t arg[8188];
		} launchApp;
		struct {
			uint32_t size;
			uint8_t arg[2020];
		} appLaunchLink;
		struct {
			SceUserServiceUserId userId;
			uint32_t npServiceLabel;
			uint8_t reserved[8184];
		} serviceEntitlementUpdate;
		uint8_t	 reserved[8192];
	} data;
};

int PS4API sceSystemServiceGetStatus(SceSystemServiceStatus* status);


int PS4API sceSystemServiceHideSplashScreen(void);


int PS4API sceSystemServiceParamGetInt(int paramID, int* value);


int PS4API sceSystemServiceReceiveEvent(SceSystemServiceEvent* Event);


int PS4API sceSystemServiceGetDisplaySafeAreaInfo(SceSystemServiceDisplaySafeAreaInfo* info);


//////////////////////////////////////////////////////////////////////////
// library: libSceSystemServicePlatformPrivacy
//////////////////////////////////////////////////////////////////////////

int PS4API sceSystemServiceGetPlatformPrivacySetting(void);

