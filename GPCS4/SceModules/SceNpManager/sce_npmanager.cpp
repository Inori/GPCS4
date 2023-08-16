#include "sce_npmanager.h"
#include <cstring>


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs

LOG_CHANNEL(SceModules.SceNpManager);

//////////////////////////////////////////////////////////////////////////
// library: libSceNpManager
//////////////////////////////////////////////////////////////////////////

int PS4API sceNpAbortRequest(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceNpCheckCallback(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceNpCheckNpAvailability(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceNpCheckNpAvailabilityA(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceNpCheckPlus(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceNpCreateAsyncRequest(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceNpDeleteRequest(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceNpGetNpId(SceUserServiceUserId userId, SceNpId* npId)
{
	LOG_SCE_DUMMY_IMPL();
	LOG_SCE_TRACE("userId %x", userId);

	LOG_ASSERT(std::strlen(GPCS4_APP_NAME) < SCE_NP_ONLINEID_MAX_LENGTH, "app name too long.");
	std::memset(npId, 0, sizeof(SceNpId));
	std::strcpy(npId->handle.data, GPCS4_APP_NAME);
	npId->handle.term = '\0';

	return SCE_OK;
}


int PS4API sceNpGetOnlineId(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceNpGetParentalControlInfo(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceNpNotifyPlusFeature(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceNpPollAsync(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceNpRegisterGamePresenceCallback(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceNpRegisterNpReachabilityStateCallback(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceNpRegisterPlusEventCallback(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceNpRegisterStateCallback(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceNpSetContentRestriction(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceNpSetNpTitleId(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceNpUnregisterNpReachabilityStateCallback(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceNpUnregisterPlusEventCallback(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceNpUnregisterStateCallback(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceNpCreateRequest(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceNpGetAccountCountryA(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceNpGetAccountDateOfBirthA(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceNpGetAccountIdA(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceNpGetAccountLanguageA(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}




