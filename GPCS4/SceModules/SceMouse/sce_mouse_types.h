#pragma once

#include "GPCS4Common.h"

struct SceMouseOpenParam 
{
	uint8_t behaviorFlag;
	uint8_t reserve[7];
};

struct SceMouseData
{
	uint64_t timestamp;
	bool connected;
	uint32_t buttons;
	int32_t xAxis;
	int32_t yAxis;
	int32_t wheel;
	int32_t tilt;
	uint8_t reserve[8];
};
