#pragma once

typedef struct _SceSystemServiceStatus {
	int32_t	eventNum;
	bool isSystemUiOverlaid;
	bool isInBackgroundExecution;
	bool isCpuMode7CpuNormal;
	bool isGameLiveStreamingOnAir;
	bool isOutOfVrPlayArea;
	uint8_t reserved[125];
} SceSystemServiceStatus;
