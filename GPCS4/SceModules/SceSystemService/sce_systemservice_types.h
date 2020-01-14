#pragma once

typedef int32_t SceSystemServiceParamId;

#define SCE_SYSTEM_SERVICE_PARAM_ID_LANG (1)

#define SCE_SYSTEM_SERVICE_PARAM_ID_DATE_FORMAT (2)

#define SCE_SYSTEM_SERVICE_PARAM_ID_TIME_FORMAT (3)

#define SCE_SYSTEM_SERVICE_PARAM_ID_TIME_ZONE (4)

#define SCE_SYSTEM_SERVICE_PARAM_ID_SUMMERTIME (5)

#define SCE_SYSTEM_SERVICE_PARAM_ID_SYSTEM_NAME (6)

#define SCE_SYSTEM_SERVICE_PARAM_ID_GAME_PARENTAL_LEVEL (7)

#define SCE_SYSTEM_SERVICE_PARAM_ID_ENTER_BUTTON_ASSIGN (1000)


struct SceSystemServiceStatus 
{
	int32_t	eventNum;
	bool isSystemUiOverlaid;
	bool isInBackgroundExecution;
	bool isCpuMode7CpuNormal;
	bool isGameLiveStreamingOnAir;
	bool isOutOfVrPlayArea;
	uint8_t reserved[125];
};


struct SceSystemServiceDisplaySafeAreaInfo
{
	float ratio;
	uint8_t reserved[128];
};
