#include "sce_systemservice.h"
#include "sce_systemservice_error.h"
#include "sce_system_param.h"

// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs

LOG_CHANNEL(SceModules.SceSystemService);

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


int PS4API sceSystemServiceParamGetInt(SceSystemServiceParamId paramId, int32_t *value)
{
	LOG_SCE_TRACE("paramId %d value %p", paramId, value);

	int ret       = SCE_SYSTEM_SERVICE_ERROR_PARAMETER;
	int32_t param = 0;
	do 
	{
		if (!value)
		{
			break;
		}

		if ((paramId != SCE_SYSTEM_SERVICE_PARAM_ID_ENTER_BUTTON_ASSIGN) &&
			(paramId < SCE_SYSTEM_SERVICE_PARAM_ID_LANG || paramId > SCE_SYSTEM_SERVICE_PARAM_ID_GAME_PARENTAL_LEVEL))
		{
			break;
		}

		switch (paramId)
		{
		case SCE_SYSTEM_SERVICE_PARAM_ID_LANG:
			param = SCE_SYSTEM_PARAM_LANG_CHINESE_T;
			break;
		case SCE_SYSTEM_SERVICE_PARAM_ID_DATE_FORMAT:
			param = SCE_SYSTEM_PARAM_DATE_FORMAT_DDMMYYYY;
			break;
		case SCE_SYSTEM_SERVICE_PARAM_ID_TIME_FORMAT:
			param = SCE_SYSTEM_PARAM_TIME_FORMAT_12HOUR;
			break;
		case SCE_SYSTEM_SERVICE_PARAM_ID_TIME_ZONE:
			// difference in minutes between local time and GMT
			// GMT+8
			param = 480;
			break;
		case SCE_SYSTEM_SERVICE_PARAM_ID_SUMMERTIME:
			// 1 in daylight savings time, 0 not in daylight savings time
			param = 1;
			break;
		case SCE_SYSTEM_SERVICE_PARAM_ID_SYSTEM_NAME:
			// for sceSystemServiceParamGetString, shouldn't be used here.
			break;
		case SCE_SYSTEM_SERVICE_PARAM_ID_GAME_PARENTAL_LEVEL:
			param = SCE_SYSTEM_PARAM_GAME_PARENTAL_OFF;
			break;
		case SCE_SYSTEM_SERVICE_PARAM_ID_ENTER_BUTTON_ASSIGN:
			param = SCE_SYSTEM_PARAM_ENTER_BUTTON_ASSIGN_CIRCLE;
			break;
		}

		*value = param;

		ret = SCE_OK;
	} while (false);
	return ret;
}


int PS4API sceSystemServiceReceiveEvent(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceSystemServiceGetDisplaySafeAreaInfo(SceSystemServiceDisplaySafeAreaInfo* info)
{
	LOG_SCE_TRACE("info %p", info);
	int ret = SCE_ERROR_UNKNOWN;
	do
	{
		if (!info)
		{
			ret = SCE_SYSTEM_SERVICE_ERROR_PARAMETER;
			break;
		}

		info->ratio = 1.0;

		ret = SCE_OK;
	} while (false);
	return ret;
}


//////////////////////////////////////////////////////////////////////////
// library: libSceSystemServicePlatformPrivacy
//////////////////////////////////////////////////////////////////////////

int PS4API sceSystemServiceGetPlatformPrivacySetting(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}

