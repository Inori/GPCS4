#include "sce_npmanager.h"


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs


static const SCE_EXPORT_FUNCTION g_pSceNpManager_libSceNpManager_FunctionTable[] =
{
	{ 0x3B32AF4EF8376585, "sceNpAbortRequest", (void*)sceNpAbortRequest },
	{ 0xDD997C05E3D387D6, "sceNpCheckCallback", (void*)sceNpCheckCallback },
	{ 0xDABB059A519695E4, "sceNpCheckNpAvailability", (void*)sceNpCheckNpAvailability },
	{ 0xAFA33260992BCB3F, "sceNpCheckPlus", (void*)sceNpCheckPlus },
	{ 0x7A2A8C0ADF54B212, "sceNpCreateAsyncRequest", (void*)sceNpCreateAsyncRequest },
	{ 0x4BB4139FBD8FAC3C, "sceNpDeleteRequest", (void*)sceNpDeleteRequest },
	{ 0xA7FA3BE029E83736, "sceNpGetNpId", (void*)sceNpGetNpId },
	{ 0x5C39DC5D02095129, "sceNpGetOnlineId", (void*)sceNpGetOnlineId },
	{ 0x8A5C0B338CCE9AEE, "sceNpGetParentalControlInfo", (void*)sceNpGetParentalControlInfo },
	{ 0x19AC6BA7711663F3, "sceNpNotifyPlusFeature", (void*)sceNpNotifyPlusFeature },
	{ 0xBAA70F24B58BD3C3, "sceNpPollAsync", (void*)sceNpPollAsync },
	{ 0xB8526968A341023E, "sceNpRegisterGamePresenceCallback", (void*)sceNpRegisterGamePresenceCallback },
	{ 0x870E4A36A0007A5B, "sceNpRegisterNpReachabilityStateCallback", (void*)sceNpRegisterNpReachabilityStateCallback },
	{ 0x1889880A787E6E80, "sceNpRegisterPlusEventCallback", (void*)sceNpRegisterPlusEventCallback },
	{ 0x55F45298F9A3F10F, "sceNpRegisterStateCallback", (void*)sceNpRegisterStateCallback },
	{ 0x036090DE4812A294, "sceNpSetContentRestriction", (void*)sceNpSetContentRestriction },
	{ 0x11CEB7CB9F65F6DC, "sceNpSetNpTitleId", (void*)sceNpSetNpTitleId },
	{ 0x71120B004BE7FBD3, "sceNpUnregisterNpReachabilityStateCallback", (void*)sceNpUnregisterNpReachabilityStateCallback },
	{ 0xC558AA25D0E02A5D, "sceNpUnregisterPlusEventCallback", (void*)sceNpUnregisterPlusEventCallback },
	{ 0x9A38D35E1F8D1D66, "sceNpUnregisterStateCallback", (void*)sceNpUnregisterStateCallback },
	{ 0x1A92D00CD28809A7, "sceNpCreateRequest", (void*)sceNpCreateRequest },
	{ 0x253FADD346B74F10, "sceNpGetAccountCountryA", (void*)sceNpGetAccountCountryA },
	{ 0xAB733B5F304A0B7B, "sceNpGetAccountDateOfBirthA", (void*)sceNpGetAccountDateOfBirthA },
	{ 0xADB9276948E9A96A, "sceNpGetAccountIdA", (void*)sceNpGetAccountIdA },
	{ 0x4CF31B808C6FA20D, "sceNpGetAccountLanguageA", (void*)sceNpGetAccountLanguageA },
	SCE_FUNCTION_ENTRY_END
};

static const SCE_EXPORT_LIBRARY g_pSceNpManager_LibTable[] =
{
	{ "libSceNpManager", g_pSceNpManager_libSceNpManager_FunctionTable },
	SCE_LIBRARY_ENTRY_END
};

const SCE_EXPORT_MODULE g_ExpModuleSceNpManager =
{
	"libSceNpManager",
	g_pSceNpManager_LibTable
};


