#include "sce_sysmodule.h"
#include "Emulator/SceModuleSystem.h"

// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs

LOG_CHANNEL(SceModules.SceSysmodule);

struct MODULE_ID_NAME_ENTRY
{
	uint16_t id;
	const char* name;
};

// generated using ModuleDefineToName.py
// TODO:
// some names in this map is not correct
// we should fix this
MODULE_ID_NAME_ENTRY g_moduleIdNameMap[] = 
{
	{ SCE_SYSMODULE_FIBER, "libSceFiber" },
	{ SCE_SYSMODULE_ULT, "libSceUlt" },
	{ SCE_SYSMODULE_NGS2, "libSceNgs2" },
	{ SCE_SYSMODULE_XML, "libSceXml" },
	{ SCE_SYSMODULE_NP_UTILITY, "libSceNpUtility" },
	{ SCE_SYSMODULE_VOICE, "libSceVoice" },
	{ SCE_SYSMODULE_VOICEQOS, "libSceVoiceqos" },
	{ SCE_SYSMODULE_NP_MATCHING2, "libSceNpMatching2" },
	{ SCE_SYSMODULE_NP_SCORE_RANKING, "libSceNpScoreRanking" },
	{ SCE_SYSMODULE_RUDP, "libSceRudp" },
	{ SCE_SYSMODULE_NP_TUS, "libSceNpTus" },
	{ SCE_SYSMODULE_FACE, "libSceFace" },
	{ SCE_SYSMODULE_SMART, "libSceSmart" },
	{ SCE_SYSMODULE_GAME_LIVE_STREAMING, "libSceGameLiveStreaming" },
	{ SCE_SYSMODULE_COMPANION_UTIL, "libSceCompanionUtil" },
	{ SCE_SYSMODULE_PLAYGO, "libScePlaygo" },
	{ SCE_SYSMODULE_FONT, "libSceFont" },
	{ SCE_SYSMODULE_VIDEO_RECORDING, "libSceVideoRecording" },
	{ SCE_SYSMODULE_S3DCONVERSION, "libSceS3dconversion" },
	{ SCE_SYSMODULE_AUDIODEC, "libSceAudiodec" },
	{ SCE_SYSMODULE_JPEG_DEC, "libSceJpegDec" },
	{ SCE_SYSMODULE_JPEG_ENC, "libSceJpegEnc" },
	{ SCE_SYSMODULE_PNG_DEC, "libScePngDec" },
	{ SCE_SYSMODULE_PNG_ENC, "libScePngEnc" },
	{ SCE_SYSMODULE_VIDEODEC, "libSceVideodec" },
	{ SCE_SYSMODULE_MOVE, "libSceMove" },
	{ SCE_SYSMODULE_PAD_TRACKER, "libScePadTracker" },
	{ SCE_SYSMODULE_DEPTH, "libSceDepth" },
	{ SCE_SYSMODULE_HAND, "libSceHand" },
	{ SCE_SYSMODULE_LIBIME, "libSceLibime" },
	{ SCE_SYSMODULE_IME_DIALOG, "libSceImeDialog" },
	{ SCE_SYSMODULE_NP_PARTY, "libSceNpParty" },
	{ SCE_SYSMODULE_FONT_FT, "libSceFontFt" },
	{ SCE_SYSMODULE_FREETYPE_OT, "libSceFreetypeOt" },
	{ SCE_SYSMODULE_FREETYPE_OL, "libSceFreetypeOl" },
	{ SCE_SYSMODULE_FREETYPE_OPT_OL, "libSceFreetypeOptOl" },
	{ SCE_SYSMODULE_SCREEN_SHOT, "libSceScreenShot" },
	{ SCE_SYSMODULE_NP_AUTH, "libSceNpAuth" },
	{ SCE_SYSMODULE_SULPHA, "libSceSulpha" },
	{ SCE_SYSMODULE_SAVE_DATA_DIALOG, "libSceSaveDataDialog" },
	{ SCE_SYSMODULE_INVITATION_DIALOG, "libSceInvitationDialog" },
	{ SCE_SYSMODULE_DEBUG_KEYBOARD, "libSceDebugKeyboard" },
	{ SCE_SYSMODULE_MESSAGE_DIALOG, "libSceMsgDialog" },
	{ SCE_SYSMODULE_AV_PLAYER, "libSceAvPlayer" },
	{ SCE_SYSMODULE_CONTENT_EXPORT, "libSceContentExport" },
	{ SCE_SYSMODULE_AUDIO_3D, "libSceAudio3d" },
	{ SCE_SYSMODULE_NP_COMMERCE, "libSceNpCommerce" },
	{ SCE_SYSMODULE_MOUSE, "libSceMouse" },
	{ SCE_SYSMODULE_COMPANION_HTTPD, "libSceCompanionHttpd" },
	{ SCE_SYSMODULE_WEB_BROWSER_DIALOG, "libSceWebBrowserDialog" },
	{ SCE_SYSMODULE_ERROR_DIALOG, "libSceErrorDialog" },
	{ SCE_SYSMODULE_NP_TROPHY, "libSceNpTrophy" },
	{ SCE_SYSMODULE_NP_SNS_FACEBOOK, "libSceNpSnsFacebook" },
	{ SCE_SYSMODULE_MOVE_TRACKER, "libSceMoveTracker" },
	{ SCE_SYSMODULE_NP_PROFILE_DIALOG, "libSceNpProfileDialog" },
	{ SCE_SYSMODULE_NP_FRIEND_LIST_DIALOG, "libSceNpFriendListDialog" },
	{ SCE_SYSMODULE_APP_CONTENT, "libSceAppContent" },
	{ SCE_SYSMODULE_NP_SIGNALING, "libSceNpSignaling" },
	{ SCE_SYSMODULE_REMOTE_PLAY, "libSceRemotePlay" },
	{ SCE_SYSMODULE_USBD, "libSceUsbd" },
	{ SCE_SYSMODULE_GAME_CUSTOM_DATA_DIALOG, "libSceGameCustomDataDialog" },
	{ SCE_SYSMODULE_M4AAC_ENC, "libSceM4aacEnc" },
	{ SCE_SYSMODULE_AUDIODEC_CPU, "libSceAudiodecCpu" },
	{ SCE_SYSMODULE_ZLIB, "libSceZlib" },
	{ SCE_SYSMODULE_CONTENT_SEARCH, "libSceContentSearch" },
	{ SCE_SYSMODULE_DECI4H, "libSceDeci4h" },
	{ SCE_SYSMODULE_HEAD_TRACKER, "libSceHeadTracker" },
	{ SCE_SYSMODULE_SYSTEM_GESTURE, "libSceSystemGesture" },
	{ SCE_SYSMODULE_VIDEODEC2, "libSceVideodec2" },
	{ SCE_SYSMODULE_AT9_ENC, "libSceAt9Enc" },
	{ SCE_SYSMODULE_CONVERT_KEYCODE, "libSceConvertKeycode" },
	{ SCE_SYSMODULE_SHARE_PLAY, "libSceSharePlay" },
	{ SCE_SYSMODULE_HMD, "libSceHmd" },
	{ SCE_SYSMODULE_FACE_TRACKER, "libSceFaceTracker" },
	{ SCE_SYSMODULE_HAND_TRACKER, "libSceHandTracker" },
	{ SCE_SYSMODULE_AUDIODEC_CPU_HEVAG, "libSceAudiodecCpuHevag" },
	{ SCE_SYSMODULE_LOGIN_DIALOG, "libSceLoginDialog" },
	{ SCE_SYSMODULE_LOGIN_SERVICE, "libSceLoginService" },
	{ SCE_SYSMODULE_SIGNIN_DIALOG, "libSceSigninDialog" },
	{ SCE_SYSMODULE_JSON2, "libSceJson2" },
	{ SCE_SYSMODULE_AUDIO_LATENCY_ESTIMATION, "libSceAudioLatencyEstimation" },
	{ SCE_SYSMODULE_HMD_SETUP_DIALOG, "libSceHmdSetupDialog" },
	{ SCE_SYSMODULE_VR_TRACKER, "libSceVrTracker" },
	{ SCE_SYSMODULE_CONTENT_DELETE, "libSceContentDelete" },
	{ SCE_SYSMODULE_IME_BACKEND, "libSceImeBackend" },
	{ SCE_SYSMODULE_NET_CTL_AP_DIALOG, "libSceNetCtlApDialog" },
	{ SCE_SYSMODULE_PLAYGO_DIALOG, "libScePlaygoDialog" },
	{ SCE_SYSMODULE_SOCIAL_SCREEN, "libSceSocialScreen" },
	{ SCE_SYSMODULE_EDIT_MP4, "libSceEditMp4" },
	{ SCE_SYSMODULE_TEXT_TO_SPEECH, "libSceTextToSpeech" },
	{ SCE_SYSMODULE_BLUETOOTH_HID, "libSceBluetoothHid" },
	{ SCE_SYSMODULE_VR_SERVICE_DIALOG, "libSceVrServiceDialog" },
	{ SCE_SYSMODULE_JOB_MANAGER, "libSceJobManager" },
	{ SCE_SYSMODULE_SOCIAL_SCREEN_DIALOG, "libSceSocialScreenDialog" },
	{ SCE_SYSMODULE_NP_TOOLKIT2, "libSceNpToolkit2" },
	{0, NULL}
};


//////////////////////////////////////////////////////////////////////////
// library: libSceSysmodule
//////////////////////////////////////////////////////////////////////////
 
const char* moduleNameFromId(uint16_t id)
{
	const char* modName = NULL;
	MODULE_ID_NAME_ENTRY* pEntry = g_moduleIdNameMap;
	while (pEntry->id != 0 && pEntry->name != NULL)
	{
		if (pEntry->id != id)
		{
			++pEntry;
			continue;
		}
		modName = pEntry->name;
		break;
	}
	return modName;
}


int PS4API sceSysmoduleIsLoaded(uint16_t id)
{
	//int ret = SCE_SYSMODULE_ERROR_UNLOADED;
	//do 
	//{
	//	const char* modName = moduleNameFromId(id);
	//	if (!modName)
	//	{
	//		LOG_WARN("specified invalid module id %x", id);
	//		ret = SCE_SYSMODULE_ERROR_INVALID_VALUE;
	//		break;
	//	}

	//	if (!CSceModuleSystem::GetInstance()->IsModuleLoaded(modName))
	//	{
	//		LOG_ERR("game try to load an unimplemented module %s", modName);
	//		ret = SCE_SYSMODULE_ERROR_UNLOADED;
	//		break;
	//	}
	//	
	//	ret = SCE_SYSMODULE_LOADED;
	//} while (false);
	//return ret;
	const char* modName = moduleNameFromId(id);
	LOG_SCE_TRACE("module name %s id %x", modName, id);
	return SCE_SYSMODULE_LOADED;
}


int PS4API sceSysmoduleLoadModule(uint16_t id)
{
	//LOG_SCE_TRACE("id %x", id);
	//int ret = SCE_SYSMODULE_ERROR_FATAL;
	//do 
	//{
	//	if (sceSysmoduleIsLoaded(id) != SCE_SYSMODULE_LOADED)
	//	{
	//		break;
	//	}
	//	ret = SCE_OK;
	//} while (false);

	//return ret;

	const char* modName = moduleNameFromId(id);
	LOG_SCE_TRACE("module name %s id %x", modName, id);
	return SCE_OK;
}


int PS4API sceSysmoduleUnloadModule(uint16_t id)
{
	//LOG_SCE_TRACE("id %x", id);
	//int ret = SCE_SYSMODULE_ERROR_FATAL;
	//do
	//{
	//	if (!sceSysmoduleIsLoaded(id))
	//	{
	//		break;
	//	}
	//	ret = SCE_OK;
	//} while (false);

	//return ret;
	const char* modName = moduleNameFromId(id);
	LOG_SCE_TRACE("module name %s id %x", modName, id);
	return SCE_OK;
}




