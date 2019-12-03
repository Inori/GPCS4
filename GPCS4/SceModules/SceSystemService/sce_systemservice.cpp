#include "sce_systemservice.h"


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs



//////////////////////////////////////////////////////////////////////////
// library: libSceSystemService
//////////////////////////////////////////////////////////////////////////

int PS4API sceSystemServiceGetStatus(SceSystemServiceStatus* status)
{
	LOG_SCE_DUMMY_IMPL();
	status->eventNum = 0;
	status->isSystemUiOverlaid = false;
	status->isInBackgroundExecution = false;
	status->isCpuMode7CpuNormal = true;
	status->isGameLiveStreamingOnAir = false;
	status->isOutOfVrPlayArea = false;
	return SCE_OK;
}


int PS4API sceSystemServiceHideSplashScreen(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}

#define SERVICE_ERROR_PARAMETER 0x80A10003

// paramID
#define SERVICE_ID_LANG 1
#define SERVICE_ID_DATE_FORMAT 2
#define SERVICE_ID_TIME_FORMAT 3
#define SERVICE_ID_TIME_ZONE 4
#define SERVICE_ID_SUMMERTIME 5
#define SERVICE_ID_GAME_PARENTAL_LEVEL 7
#define SERVICE_ID_ENTER_BUTTON_ASSIGN 1000

// language
#define SETTING_LANG_JAPANESE 0
#define SETTING_LANG_ENGLISH_US 1
#define SETTING_LANG_FRENCH 2
#define SETTING_LANG_SPANISH 3
#define SETTING_LANG_GERMAN 4
#define SETTING_LANG_ITALIAN 5
#define SETTING_LANG_DUTCH 6
#define SETTING_LANG_PORTUGUESE_PT 7
#define SETTING_LANG_RUSSIAN 8
#define SETTING_LANG_KOREAN 9
#define SETTING_LANG_CHINESE_T 10
#define SETTING_LANG_CHINESE_S 11
#define SETTING_LANG_FINNISH 12
#define SETTING_LANG_SWEDISH 13
#define SETTING_LANG_DANISH 14
#define SETTING_LANG_NORWEGIAN 15
#define SETTING_LANG_POLISH 16
#define SETTING_LANG_PORTUGUESE_BR 17
#define SETTING_LANG_ENGLISH_GB 18
#define SETTING_LANG_TURKISH 19
#define SETTING_LANG_SPANISH_LA 20
#define SETTING_LANG_ARABIC 21
#define SETTING_LANG_FRENCH_CA 22

// date format
#define SETTING_DATE_FORMAT_YYYYMMDD 0
#define SETTING_DATE_FORMAT_DDMMYYYY 1
#define SETTING_DATE_FORMAT_MMDDYYYY 2

// time format
#define SETTING_TIME_FORMAT_12HOUR 0
#define SETTING_TIME_FORMAT_24HOUR 1

// time zone???

int PS4API sceSystemServiceParamGetInt(int paramID, int* value)
{
	LOG_SCE_TRACE("sceSystemServiceParamGetInt called");

	int returnVal = -1;

	switch (paramID)
	{
	case SERVICE_ID_LANG:
		*value = SETTING_LANG_ENGLISH_US;
		returnVal = SCE_OK;
		break;

	case SERVICE_ID_DATE_FORMAT:
		*value = SETTING_DATE_FORMAT_MMDDYYYY;
		returnVal = SCE_OK;
		break;

	case SERVICE_ID_TIME_FORMAT:
		*value = SETTING_TIME_FORMAT_12HOUR;
		returnVal = SCE_OK;
		break;

	case SERVICE_ID_TIME_ZONE:
		*value = 0;
		returnVal = SCE_OK;
		break;

		// Day-light savings
	case SERVICE_ID_SUMMERTIME:
		*value = 0;
		returnVal = SCE_OK;
		break;

	case SERVICE_ID_GAME_PARENTAL_LEVEL:
		// parental controls can be any of 
		// the following values

		// 0 -- which means off
		// 1
		// 2
		// 3
		// ...
		// 10
		// 11
		*value = 0;
		returnVal = SCE_OK;

	case SERVICE_ID_ENTER_BUTTON_ASSIGN:
		// 0 means the O button
		// 1 means the X button

		*value = 1;
		returnVal = SCE_OK;
		break;

	default:
		returnVal = SERVICE_ERROR_PARAMETER;
		break;

	};

	return returnVal;
}

#define SERVICE_ID_SYSTEM_NAME 6
#define SCE_SYSTEM_SERVICE_MAX_SYSTEM_NAME_LENGTH 65

// We do not have a stub for this yet
int sceSystemServiceParamGetString(int paramId, char *buf, size_t bufSize)
{
	LOG_SCE_TRACE("sceSystemServiceParamGetString");

	int returnVal = -1;

	if (paramId == SERVICE_ID_SYSTEM_NAME)
	{
		sprintf(buf, "dummySystemName");
		returnVal = SCE_OK;
	}

	else
	{
		returnVal = SERVICE_ERROR_PARAMETER;
	}

	return returnVal;
}


int PS4API sceSystemServiceReceiveEvent(SceSystemServiceEvent* Event)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceSystemServiceGetDisplaySafeAreaInfo(SceSystemServiceDisplaySafeAreaInfo* info)
{
	LOG_SCE_TRACE("sceSystemServiceGetDisplaySafeAreaInfo called");

	// default value
	info->ratio = 1.0f;

	return SCE_OK;
}


//////////////////////////////////////////////////////////////////////////
// library: libSceSystemServicePlatformPrivacy
//////////////////////////////////////////////////////////////////////////

int PS4API sceSystemServiceGetPlatformPrivacySetting(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}

