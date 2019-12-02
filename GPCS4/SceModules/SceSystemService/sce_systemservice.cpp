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

#define SCE_SYSTEM_SERVICE_ERROR_PARAMETER 0x80A10003

// paramID
#define SCE_SYSTEM_SERVICE_PARAM_ID_LANG 1
#define SCE_SYSTEM_SERVICE_PARAM_ID_DATE_FORMAT 2
#define SCE_SYSTEM_SERVICE_PARAM_ID_TIME_FORMAT 3
#define SCE_SYSTEM_SERVICE_PARAM_ID_TIME_ZONE 4
#define SCE_SYSTEM_SERVICE_PARAM_ID_SUMMERTIME 5
#define SCE_SYSTEM_SERVICE_PARAM_ID_GAME_PARENTAL_LEVEL 7
#define SCE_SYSTEM_SERVICE_PARAM_ID_ENTER_BUTTON_ASSIGN 1000

// language
#define SCE_SYSTEM_PARAM_LANG_JAPANESE 0
#define SCE_SYSTEM_PARAM_LANG_ENGLISH_US 1
#define SCE_SYSTEM_PARAM_LANG_FRENCH 2
#define SCE_SYSTEM_PARAM_LANG_SPANISH 3
#define SCE_SYSTEM_PARAM_LANG_GERMAN 4
#define SCE_SYSTEM_PARAM_LANG_ITALIAN 5
#define SCE_SYSTEM_PARAM_LANG_DUTCH 6
#define SCE_SYSTEM_PARAM_LANG_PORTUGUESE_PT 7
#define SCE_SYSTEM_PARAM_LANG_RUSSIAN 8
#define SCE_SYSTEM_PARAM_LANG_KOREAN 9
#define SCE_SYSTEM_PARAM_LANG_CHINESE_T 10
#define SCE_SYSTEM_PARAM_LANG_CHINESE_S 11
#define SCE_SYSTEM_PARAM_LANG_FINNISH 12
#define SCE_SYSTEM_PARAM_LANG_SWEDISH 13
#define SCE_SYSTEM_PARAM_LANG_DANISH 14
#define SCE_SYSTEM_PARAM_LANG_NORWEGIAN 15
#define SCE_SYSTEM_PARAM_LANG_POLISH 16
#define SCE_SYSTEM_PARAM_LANG_PORTUGUESE_BR 17
#define SCE_SYSTEM_PARAM_LANG_ENGLISH_GB 18
#define SCE_SYSTEM_PARAM_LANG_TURKISH 19
#define SCE_SYSTEM_PARAM_LANG_SPANISH_LA 20
#define SCE_SYSTEM_PARAM_LANG_ARABIC 21
#define SCE_SYSTEM_PARAM_LANG_FRENCH_CA 22

// date format
#define SCE_SYSTEM_PARAM_DATE_FORMAT_YYYYMMDD 0
#define SCE_SYSTEM_PARAM_DATE_FORMAT_DDMMYYYY 1
#define SCE_SYSTEM_PARAM_DATE_FORMAT_MMDDYYYY 2

// time format
#define SCE_SYSTEM_PARAM_TIME_FORMAT_12HOUR 0
#define SCE_SYSTEM_PARAM_TIME_FORMAT_24HOUR 1

// time zone???

int PS4API sceSystemServiceParamGetInt(int paramID, int* value)
{
	LOG_SCE_TRACE("sceSystemServiceParamGetInt called");

	int returnVal = -1;

	if (paramID == SCE_SYSTEM_SERVICE_PARAM_ID_LANG)
	{
		*value = SCE_SYSTEM_PARAM_LANG_ENGLISH_US;
		returnVal = SCE_OK;
	}

	else if (paramID == SCE_SYSTEM_SERVICE_PARAM_ID_DATE_FORMAT)
	{
		*value = SCE_SYSTEM_PARAM_DATE_FORMAT_MMDDYYYY;
		returnVal = SCE_OK;
	}

	else if (paramID == SCE_SYSTEM_SERVICE_PARAM_ID_TIME_FORMAT)
	{
		*value = SCE_SYSTEM_PARAM_TIME_FORMAT_12HOUR;
		returnVal = SCE_OK;
	}

	else if (paramID == SCE_SYSTEM_SERVICE_PARAM_ID_TIME_ZONE)
	{
		*value = 0;
		returnVal = SCE_OK;
	}

	// Day-light savings
	else if (paramID == SCE_SYSTEM_SERVICE_PARAM_ID_SUMMERTIME)
	{
		*value = 0;
		returnVal = SCE_OK;
	}

	else if (paramID == SCE_SYSTEM_SERVICE_PARAM_ID_GAME_PARENTAL_LEVEL)
	{
		// parental controls can be any of the follwoing values
		// 0 -- which means off
		// 1
		// 2
		// 3
		// ...
		// 10
		// 11
		*value = 0;
		returnVal = SCE_OK;
	}

	else if (paramID == SCE_SYSTEM_SERVICE_PARAM_ID_ENTER_BUTTON_ASSIGN)
	{
		// 0 means the O button
		// 1 means the X button
		
		*value = 1;
		returnVal = SCE_OK;
	}

	else
	{
		 returnVal = SCE_SYSTEM_SERVICE_ERROR_PARAMETER;
	}

	return returnVal;
}

#define SCE_SYSTEM_SERVICE_PARAM_ID_SYSTEM_NAME 6
#define SCE_SYSTEM_SERVICE_MAX_SYSTEM_NAME_LENGTH 65

// We do not have a stub for this yet
int sceSystemServiceParamGetString(int paramId, char *buf, size_t bufSize)
{
	LOG_SCE_TRACE("sceSystemServiceParamGetString");

	int returnVal = -1;

	if (paramId == SCE_SYSTEM_SERVICE_PARAM_ID_SYSTEM_NAME)
	{
		sprintf(buf, "dummySystemName");
		returnVal = SCE_OK;
	}

	else
	{
		returnVal = SCE_SYSTEM_SERVICE_ERROR_PARAMETER;
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

