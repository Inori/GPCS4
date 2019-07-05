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


int PS4API sceSystemServiceParamGetInt(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceSystemServiceReceiveEvent(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceSystemServiceGetDisplaySafeAreaInfo(void)
{
	LOG_FIXME("Not implemented");
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

