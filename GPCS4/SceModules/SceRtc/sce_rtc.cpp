#include "sce_rtc.h"


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs



//////////////////////////////////////////////////////////////////////////
// library: libSceRtc
//////////////////////////////////////////////////////////////////////////

int PS4API sceRtcConvertUtcToLocalTime(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceRtcGetCurrentClockLocalTime(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceRtcGetCurrentNetworkTick(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceRtcGetDayOfWeek(int day, int month, int year)
{
	LOG_SCE_TRACE("day %d month %m year %y", day, month, year);
	int dayIndex = (day += month < 3 ? year-- : year - 2, 23 * month / 9 + day + 4 + year / 4 - year / 100 + year / 400) % 7;
	return dayIndex;
}


int PS4API sceRtcGetTick(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceRtcSetTick(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceRtcGetCurrentClock(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceRtcGetCurrentTick(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}




