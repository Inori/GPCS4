#include "stdafx.h"
#include "Emu/System.h"
#include "Emu/Cell/PPUModule.h"

#include "sysPrxForUser.h"
#include "Emu/IdManager.h"
#include "Crypto/unedat.h"
#include "Crypto/unself.h"
#include "cellRtc.h"
#include "sceNp.h"
#include "cellSysutil.h"

LOG_CHANNEL(sceNp);


int sceNpScoreGetRankingByRangeAsync(int32_t transId, SceNpScoreBoardId boardId, SceNpScoreRankNumber startSerialRank, SceNpScoreRankData* rankArray,
	uint64_t rankArraySize, SceNpScoreComment* commentArray, uint64_t commentArraySize, void* infoArray, uint64_t infoArraySize,
	uint64_t arrayNum, CellRtcTick* lastSortDate, SceNpScoreRankNumber* totalRecord, int32_t prio, void* option)
{
	LOG_SCE_TRACE("(transId=%d, boardId=%d, startSerialRank=%d, rankArray=*0x%x, rankArraySize=%d, commentArray=*0x%x, commentArraySize=%d, infoArray=*0x%x, infoArraySize=%d, arrayNum=%d, lastSortDate=*0x%x, totalRecord=*0x%x, prio=%d, option=*0x%x)",
		transId, boardId, startSerialRank, rankArray, rankArraySize, commentArray, commentArraySize, infoArray, infoArraySize, arrayNum, lastSortDate, totalRecord, prio, option);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_score_manager>()->is_initialized)
		{
			ret = SCE_NP_COMMUNITY_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!arrayNum)
		{
			ret = SCE_NP_COMMUNITY_ERROR_INSUFFICIENT_ARGUMENT;
			break;
		}
	
		if (!startSerialRank || option) // option check at least until fw 4.71
		{
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

template<>
void fmt_class_string<SceNpError>::format(std::string& out, u64 arg)
{
	format_enum(out, arg, [](auto error)
	{
		switch (error)
		{
			STR_CASE(SCE_NP_ERROR_NOT_INITIALIZED);
			STR_CASE(SCE_NP_ERROR_ALREADY_INITIALIZED);
			STR_CASE(SCE_NP_ERROR_INVALID_ARGUMENT);
			STR_CASE(SCE_NP_ERROR_OUT_OF_MEMORY);
			STR_CASE(SCE_NP_ERROR_ID_NO_SPACE);
			STR_CASE(SCE_NP_ERROR_ID_NOT_FOUND);
			STR_CASE(SCE_NP_ERROR_SESSION_RUNNING);
			STR_CASE(SCE_NP_ERROR_LOGINID_ALREADY_EXISTS);
			STR_CASE(SCE_NP_ERROR_INVALID_TICKET_SIZE);
			STR_CASE(SCE_NP_ERROR_INVALID_STATE);
			STR_CASE(SCE_NP_ERROR_ABORTED);
			STR_CASE(SCE_NP_ERROR_OFFLINE);
			STR_CASE(SCE_NP_ERROR_VARIANT_ACCOUNT_ID);
			STR_CASE(SCE_NP_ERROR_GET_CLOCK);
			STR_CASE(SCE_NP_ERROR_INSUFFICIENT_BUFFER);
			STR_CASE(SCE_NP_ERROR_EXPIRED_TICKET);
			STR_CASE(SCE_NP_ERROR_TICKET_PARAM_NOT_FOUND);
			STR_CASE(SCE_NP_ERROR_UNSUPPORTED_TICKET_VERSION);
			STR_CASE(SCE_NP_ERROR_TICKET_STATUS_CODE_INVALID);
			STR_CASE(SCE_NP_ERROR_INVALID_TICKET_VERSION);
			STR_CASE(SCE_NP_ERROR_ALREADY_USED);
			STR_CASE(SCE_NP_ERROR_DIFFERENT_USER);
			STR_CASE(SCE_NP_ERROR_ALREADY_DONE);
			STR_CASE(SCE_NP_BASIC_ERROR_ALREADY_INITIALIZED);
			STR_CASE(SCE_NP_BASIC_ERROR_NOT_INITIALIZED);
			STR_CASE(SCE_NP_BASIC_ERROR_NOT_SUPPORTED);
			STR_CASE(SCE_NP_BASIC_ERROR_OUT_OF_MEMORY);
			STR_CASE(SCE_NP_BASIC_ERROR_INVALID_ARGUMENT);
			STR_CASE(SCE_NP_BASIC_ERROR_BAD_ID);
			STR_CASE(SCE_NP_BASIC_ERROR_IDS_DIFFER);
			STR_CASE(SCE_NP_BASIC_ERROR_PARSER_FAILED);
			STR_CASE(SCE_NP_BASIC_ERROR_TIMEOUT);
			STR_CASE(SCE_NP_BASIC_ERROR_NO_EVENT);
			STR_CASE(SCE_NP_BASIC_ERROR_EXCEEDS_MAX);
			STR_CASE(SCE_NP_BASIC_ERROR_INSUFFICIENT);
			STR_CASE(SCE_NP_BASIC_ERROR_NOT_REGISTERED);
			STR_CASE(SCE_NP_BASIC_ERROR_DATA_LOST);
			STR_CASE(SCE_NP_BASIC_ERROR_BUSY);
			STR_CASE(SCE_NP_BASIC_ERROR_STATUS);
			STR_CASE(SCE_NP_BASIC_ERROR_CANCEL);
			STR_CASE(SCE_NP_BASIC_ERROR_INVALID_MEMORY_CONTAINER);
			STR_CASE(SCE_NP_BASIC_ERROR_INVALID_DATA_ID);
			STR_CASE(SCE_NP_BASIC_ERROR_BROKEN_DATA);
			STR_CASE(SCE_NP_BASIC_ERROR_BLOCKLIST_ADD_FAILED);
			STR_CASE(SCE_NP_BASIC_ERROR_BLOCKLIST_IS_FULL);
			STR_CASE(SCE_NP_BASIC_ERROR_SEND_FAILED);
			STR_CASE(SCE_NP_BASIC_ERROR_NOT_CONNECTED);
			STR_CASE(SCE_NP_BASIC_ERROR_INSUFFICIENT_DISK_SPACE);
			STR_CASE(SCE_NP_BASIC_ERROR_INTERNAL_FAILURE);
			STR_CASE(SCE_NP_BASIC_ERROR_DOES_NOT_EXIST);
			STR_CASE(SCE_NP_BASIC_ERROR_INVALID);
			STR_CASE(SCE_NP_BASIC_ERROR_UNKNOWN);
			STR_CASE(SCE_NP_EXT_ERROR_CONTEXT_DOES_NOT_EXIST);
			STR_CASE(SCE_NP_EXT_ERROR_CONTEXT_ALREADY_EXISTS);
			STR_CASE(SCE_NP_EXT_ERROR_NO_CONTEXT);
			STR_CASE(SCE_NP_EXT_ERROR_NO_ORIGIN);
			STR_CASE(SCE_NP_UTIL_ERROR_INVALID_ARGUMENT);
			STR_CASE(SCE_NP_UTIL_ERROR_OUT_OF_MEMORY);
			STR_CASE(SCE_NP_UTIL_ERROR_INSUFFICIENT);
			STR_CASE(SCE_NP_UTIL_ERROR_PARSER_FAILED);
			STR_CASE(SCE_NP_UTIL_ERROR_INVALID_PROTOCOL_ID);
			STR_CASE(SCE_NP_UTIL_ERROR_INVALID_NP_ID);
			STR_CASE(SCE_NP_UTIL_ERROR_INVALID_NP_LOBBY_ID);
			STR_CASE(SCE_NP_UTIL_ERROR_INVALID_NP_ROOM_ID);
			STR_CASE(SCE_NP_UTIL_ERROR_INVALID_NP_ENV);
			STR_CASE(SCE_NP_UTIL_ERROR_INVALID_TITLEID);
			STR_CASE(SCE_NP_UTIL_ERROR_INVALID_CHARACTER);
			STR_CASE(SCE_NP_UTIL_ERROR_INVALID_ESCAPE_STRING);
			STR_CASE(SCE_NP_UTIL_ERROR_UNKNOWN_TYPE);
			STR_CASE(SCE_NP_UTIL_ERROR_UNKNOWN);
			STR_CASE(SCE_NP_UTIL_ERROR_NOT_MATCH);
			STR_CASE(SCE_NP_UTIL_ERROR_UNKNOWN_PLATFORM_TYPE);
			STR_CASE(SCE_NP_FRIENDLIST_ERROR_ALREADY_INITIALIZED);
			STR_CASE(SCE_NP_FRIENDLIST_ERROR_NOT_INITIALIZED);
			STR_CASE(SCE_NP_FRIENDLIST_ERROR_OUT_OF_MEMORY);
			STR_CASE(SCE_NP_FRIENDLIST_ERROR_INVALID_MEMORY_CONTAINER);
			STR_CASE(SCE_NP_FRIENDLIST_ERROR_INSUFFICIENT);
			STR_CASE(SCE_NP_FRIENDLIST_ERROR_CANCEL);
			STR_CASE(SCE_NP_FRIENDLIST_ERROR_STATUS);
			STR_CASE(SCE_NP_FRIENDLIST_ERROR_BUSY);
			STR_CASE(SCE_NP_FRIENDLIST_ERROR_INVALID_ARGUMENT);
			STR_CASE(SCE_NP_PROFILE_ERROR_ALREADY_INITIALIZED);
			STR_CASE(SCE_NP_PROFILE_ERROR_NOT_INITIALIZED);
			STR_CASE(SCE_NP_PROFILE_ERROR_OUT_OF_MEMORY);
			STR_CASE(SCE_NP_PROFILE_ERROR_NOT_SUPPORTED);
			STR_CASE(SCE_NP_PROFILE_ERROR_INSUFFICIENT);
			STR_CASE(SCE_NP_PROFILE_ERROR_CANCEL);
			STR_CASE(SCE_NP_PROFILE_ERROR_STATUS);
			STR_CASE(SCE_NP_PROFILE_ERROR_BUSY);
			STR_CASE(SCE_NP_PROFILE_ERROR_INVALID_ARGUMENT);
			STR_CASE(SCE_NP_PROFILE_ERROR_ABORT);
			STR_CASE(SCE_NP_COMMUNITY_ERROR_ALREADY_INITIALIZED);
			STR_CASE(SCE_NP_COMMUNITY_ERROR_NOT_INITIALIZED);
			STR_CASE(SCE_NP_COMMUNITY_ERROR_OUT_OF_MEMORY);
			STR_CASE(SCE_NP_COMMUNITY_ERROR_INVALID_ARGUMENT);
			STR_CASE(SCE_NP_COMMUNITY_ERROR_NO_TITLE_SET);
			STR_CASE(SCE_NP_COMMUNITY_ERROR_NO_LOGIN);
			STR_CASE(SCE_NP_COMMUNITY_ERROR_TOO_MANY_OBJECTS);
			STR_CASE(SCE_NP_COMMUNITY_ERROR_TRANSACTION_STILL_REFERENCED);
			STR_CASE(SCE_NP_COMMUNITY_ERROR_ABORTED);
			STR_CASE(SCE_NP_COMMUNITY_ERROR_NO_RESOURCE);
			STR_CASE(SCE_NP_COMMUNITY_ERROR_BAD_RESPONSE);
			STR_CASE(SCE_NP_COMMUNITY_ERROR_BODY_TOO_LARGE);
			STR_CASE(SCE_NP_COMMUNITY_ERROR_HTTP_SERVER);
			STR_CASE(SCE_NP_COMMUNITY_ERROR_INVALID_SIGNATURE);
			STR_CASE(SCE_NP_COMMUNITY_ERROR_TIMEOUT);
			STR_CASE(SCE_NP_COMMUNITY_ERROR_INSUFFICIENT_ARGUMENT);
			STR_CASE(SCE_NP_COMMUNITY_ERROR_UNKNOWN_TYPE);
			STR_CASE(SCE_NP_COMMUNITY_ERROR_INVALID_ID);
			STR_CASE(SCE_NP_COMMUNITY_ERROR_INVALID_ONLINE_ID);
			STR_CASE(SCE_NP_COMMUNITY_ERROR_INVALID_TICKET);
			STR_CASE(SCE_NP_COMMUNITY_ERROR_CLIENT_HANDLE_ALREADY_EXISTS);
			STR_CASE(SCE_NP_COMMUNITY_ERROR_INSUFFICIENT_BUFFER);
			STR_CASE(SCE_NP_COMMUNITY_ERROR_INVALID_TYPE);
			STR_CASE(SCE_NP_COMMUNITY_ERROR_TRANSACTION_ALREADY_END);
			STR_CASE(SCE_NP_COMMUNITY_ERROR_TRANSACTION_BEFORE_END);
			STR_CASE(SCE_NP_COMMUNITY_ERROR_BUSY_BY_ANOTEHR_TRANSACTION);
			STR_CASE(SCE_NP_COMMUNITY_ERROR_INVALID_ALIGNMENT);
			STR_CASE(SCE_NP_COMMUNITY_ERROR_TOO_MANY_NPID);
			STR_CASE(SCE_NP_COMMUNITY_ERROR_TOO_LARGE_RANGE);
			STR_CASE(SCE_NP_COMMUNITY_ERROR_INVALID_PARTITION);
			STR_CASE(SCE_NP_COMMUNITY_ERROR_TOO_MANY_SLOTID);
			STR_CASE(SCE_NP_COMMUNITY_SERVER_ERROR_BAD_REQUEST);
			STR_CASE(SCE_NP_COMMUNITY_SERVER_ERROR_INVALID_TICKET);
			STR_CASE(SCE_NP_COMMUNITY_SERVER_ERROR_INVALID_SIGNATURE);
			STR_CASE(SCE_NP_COMMUNITY_SERVER_ERROR_EXPIRED_TICKET);
			STR_CASE(SCE_NP_COMMUNITY_SERVER_ERROR_INVALID_NPID);
			STR_CASE(SCE_NP_COMMUNITY_SERVER_ERROR_FORBIDDEN);
			STR_CASE(SCE_NP_COMMUNITY_SERVER_ERROR_INTERNAL_SERVER_ERROR);
			STR_CASE(SCE_NP_COMMUNITY_SERVER_ERROR_VERSION_NOT_SUPPORTED);
			STR_CASE(SCE_NP_COMMUNITY_SERVER_ERROR_SERVICE_UNAVAILABLE);
			STR_CASE(SCE_NP_COMMUNITY_SERVER_ERROR_PLAYER_BANNED);
			STR_CASE(SCE_NP_COMMUNITY_SERVER_ERROR_CENSORED);
			STR_CASE(SCE_NP_COMMUNITY_SERVER_ERROR_RANKING_RECORD_FORBIDDEN);
			STR_CASE(SCE_NP_COMMUNITY_SERVER_ERROR_USER_PROFILE_NOT_FOUND);
			STR_CASE(SCE_NP_COMMUNITY_SERVER_ERROR_UPLOADER_DATA_NOT_FOUND);
			STR_CASE(SCE_NP_COMMUNITY_SERVER_ERROR_QUOTA_MASTER_NOT_FOUND);
			STR_CASE(SCE_NP_COMMUNITY_SERVER_ERROR_RANKING_TITLE_NOT_FOUND);
			STR_CASE(SCE_NP_COMMUNITY_SERVER_ERROR_BLACKLISTED_USER_ID);
			STR_CASE(SCE_NP_COMMUNITY_SERVER_ERROR_GAME_RANKING_NOT_FOUND);
			STR_CASE(SCE_NP_COMMUNITY_SERVER_ERROR_RANKING_STORE_NOT_FOUND);
			STR_CASE(SCE_NP_COMMUNITY_SERVER_ERROR_NOT_BEST_SCORE);
			STR_CASE(SCE_NP_COMMUNITY_SERVER_ERROR_LATEST_UPDATE_NOT_FOUND);
			STR_CASE(SCE_NP_COMMUNITY_SERVER_ERROR_RANKING_BOARD_MASTER_NOT_FOUND);
			STR_CASE(SCE_NP_COMMUNITY_SERVER_ERROR_RANKING_GAME_DATA_MASTER_NOT_FOUND);
			STR_CASE(SCE_NP_COMMUNITY_SERVER_ERROR_INVALID_ANTICHEAT_DATA);
			STR_CASE(SCE_NP_COMMUNITY_SERVER_ERROR_TOO_LARGE_DATA);
			STR_CASE(SCE_NP_COMMUNITY_SERVER_ERROR_NO_SUCH_USER_NPID);
			STR_CASE(SCE_NP_COMMUNITY_SERVER_ERROR_INVALID_ENVIRONMENT);
			STR_CASE(SCE_NP_COMMUNITY_SERVER_ERROR_INVALID_ONLINE_NAME_CHARACTER);
			STR_CASE(SCE_NP_COMMUNITY_SERVER_ERROR_INVALID_ONLINE_NAME_LENGTH);
			STR_CASE(SCE_NP_COMMUNITY_SERVER_ERROR_INVALID_ABOUT_ME_CHARACTER);
			STR_CASE(SCE_NP_COMMUNITY_SERVER_ERROR_INVALID_ABOUT_ME_LENGTH);
			STR_CASE(SCE_NP_COMMUNITY_SERVER_ERROR_INVALID_SCORE);
			STR_CASE(SCE_NP_COMMUNITY_SERVER_ERROR_OVER_THE_RANKING_LIMIT);
			STR_CASE(SCE_NP_COMMUNITY_SERVER_ERROR_FAIL_TO_CREATE_SIGNATURE);
			STR_CASE(SCE_NP_COMMUNITY_SERVER_ERROR_RANKING_MASTER_INFO_NOT_FOUND);
			STR_CASE(SCE_NP_COMMUNITY_SERVER_ERROR_OVER_THE_GAME_DATA_LIMIT);
			STR_CASE(SCE_NP_COMMUNITY_SERVER_ERROR_SELF_DATA_NOT_FOUND);
			STR_CASE(SCE_NP_COMMUNITY_SERVER_ERROR_USER_NOT_ASSIGNED);
			STR_CASE(SCE_NP_COMMUNITY_SERVER_ERROR_GAME_DATA_ALREADY_EXISTS);
			STR_CASE(SCE_NP_COMMUNITY_SERVER_ERROR_TOO_MANY_RESULTS);
			STR_CASE(SCE_NP_COMMUNITY_SERVER_ERROR_NOT_RECORDABLE_VERSION);
			STR_CASE(SCE_NP_COMMUNITY_SERVER_ERROR_USER_STORAGE_TITLE_MASTER_NOT_FOUND);
			STR_CASE(SCE_NP_COMMUNITY_SERVER_ERROR_INVALID_VIRTUAL_USER);
			STR_CASE(SCE_NP_COMMUNITY_SERVER_ERROR_USER_STORAGE_DATA_NOT_FOUND);
			STR_CASE(SCE_NP_COMMUNITY_SERVER_ERROR_CONDITIONS_NOT_SATISFIED);
			STR_CASE(SCE_NP_COMMUNITY_SERVER_ERROR_MATCHING_BEFORE_SERVICE);
			STR_CASE(SCE_NP_COMMUNITY_SERVER_ERROR_MATCHING_END_OF_SERVICE);
			STR_CASE(SCE_NP_COMMUNITY_SERVER_ERROR_MATCHING_MAINTENANCE);
			STR_CASE(SCE_NP_COMMUNITY_SERVER_ERROR_RANKING_BEFORE_SERVICE);
			STR_CASE(SCE_NP_COMMUNITY_SERVER_ERROR_RANKING_END_OF_SERVICE);
			STR_CASE(SCE_NP_COMMUNITY_SERVER_ERROR_RANKING_MAINTENANCE);
			STR_CASE(SCE_NP_COMMUNITY_SERVER_ERROR_NO_SUCH_TITLE);
			STR_CASE(SCE_NP_COMMUNITY_SERVER_ERROR_TITLE_USER_STORAGE_BEFORE_SERVICE);
			STR_CASE(SCE_NP_COMMUNITY_SERVER_ERROR_TITLE_USER_STORAGE_END_OF_SERVICE);
			STR_CASE(SCE_NP_COMMUNITY_SERVER_ERROR_TITLE_USER_STORAGE_MAINTENANCE);
			STR_CASE(SCE_NP_COMMUNITY_SERVER_ERROR_UNSPECIFIED);
			STR_CASE(SCE_NP_DRM_ERROR_LICENSE_NOT_FOUND);
			STR_CASE(SCE_NP_DRM_ERROR_OUT_OF_MEMORY);
			STR_CASE(SCE_NP_DRM_ERROR_INVALID_PARAM);
			STR_CASE(SCE_NP_DRM_ERROR_SERVER_RESPONSE);
			STR_CASE(SCE_NP_DRM_ERROR_NO_ENTITLEMENT);
			STR_CASE(SCE_NP_DRM_ERROR_BAD_ACT);
			STR_CASE(SCE_NP_DRM_ERROR_BAD_FORMAT);
			STR_CASE(SCE_NP_DRM_ERROR_NO_LOGIN);
			STR_CASE(SCE_NP_DRM_ERROR_INTERNAL);
			STR_CASE(SCE_NP_DRM_ERROR_BAD_PERM);
			STR_CASE(SCE_NP_DRM_ERROR_UNKNOWN_VERSION);
			STR_CASE(SCE_NP_DRM_ERROR_TIME_LIMIT);
			STR_CASE(SCE_NP_DRM_ERROR_DIFFERENT_ACCOUNT_ID);
			STR_CASE(SCE_NP_DRM_ERROR_DIFFERENT_DRM_TYPE);
			STR_CASE(SCE_NP_DRM_ERROR_SERVICE_NOT_STARTED);
			STR_CASE(SCE_NP_DRM_ERROR_BUSY);
			STR_CASE(SCE_NP_DRM_ERROR_IO);
			STR_CASE(SCE_NP_DRM_ERROR_FORMAT);
			STR_CASE(SCE_NP_DRM_ERROR_FILENAME);
			STR_CASE(SCE_NP_DRM_ERROR_K_LICENSEE);
			STR_CASE(SCE_NP_AUTH_EINVAL);
			STR_CASE(SCE_NP_AUTH_ENOMEM);
			STR_CASE(SCE_NP_AUTH_ESRCH);
			STR_CASE(SCE_NP_AUTH_EBUSY);
			STR_CASE(SCE_NP_AUTH_EABORT);
			STR_CASE(SCE_NP_AUTH_EEXIST);
			STR_CASE(SCE_NP_AUTH_EINVALID_ARGUMENT);
			STR_CASE(SCE_NP_AUTH_ERROR_SERVICE_END);
			STR_CASE(SCE_NP_AUTH_ERROR_SERVICE_DOWN);
			STR_CASE(SCE_NP_AUTH_ERROR_SERVICE_BUSY);
			STR_CASE(SCE_NP_AUTH_ERROR_SERVER_MAINTENANCE);
			STR_CASE(SCE_NP_AUTH_ERROR_INVALID_DATA_LENGTH);
			STR_CASE(SCE_NP_AUTH_ERROR_INVALID_USER_AGENT);
			STR_CASE(SCE_NP_AUTH_ERROR_INVALID_VERSION);
			STR_CASE(SCE_NP_AUTH_ERROR_INVALID_SERVICE_ID);
			STR_CASE(SCE_NP_AUTH_ERROR_INVALID_CREDENTIAL);
			STR_CASE(SCE_NP_AUTH_ERROR_INVALID_ENTITLEMENT_ID);
			STR_CASE(SCE_NP_AUTH_ERROR_INVALID_CONSUMED_COUNT);
			STR_CASE(SCE_NP_AUTH_ERROR_INVALID_CONSOLE_ID);
			STR_CASE(SCE_NP_AUTH_ERROR_CONSOLE_ID_SUSPENDED);
			STR_CASE(SCE_NP_AUTH_ERROR_ACCOUNT_CLOSED);
			STR_CASE(SCE_NP_AUTH_ERROR_ACCOUNT_SUSPENDED);
			STR_CASE(SCE_NP_AUTH_ERROR_ACCOUNT_RENEW_EULA);
			STR_CASE(SCE_NP_AUTH_ERROR_ACCOUNT_RENEW_ACCOUNT1);
			STR_CASE(SCE_NP_AUTH_ERROR_ACCOUNT_RENEW_ACCOUNT2);
			STR_CASE(SCE_NP_AUTH_ERROR_ACCOUNT_RENEW_ACCOUNT3);
			STR_CASE(SCE_NP_AUTH_ERROR_ACCOUNT_RENEW_ACCOUNT4);
			STR_CASE(SCE_NP_AUTH_ERROR_ACCOUNT_RENEW_ACCOUNT5);
			STR_CASE(SCE_NP_AUTH_ERROR_ACCOUNT_RENEW_ACCOUNT6);
			STR_CASE(SCE_NP_AUTH_ERROR_ACCOUNT_RENEW_ACCOUNT7);
			STR_CASE(SCE_NP_AUTH_ERROR_ACCOUNT_RENEW_ACCOUNT8);
			STR_CASE(SCE_NP_AUTH_ERROR_ACCOUNT_RENEW_ACCOUNT9);
			STR_CASE(SCE_NP_AUTH_ERROR_ACCOUNT_RENEW_ACCOUNT10);
			STR_CASE(SCE_NP_AUTH_ERROR_ACCOUNT_RENEW_ACCOUNT11);
			STR_CASE(SCE_NP_AUTH_ERROR_ACCOUNT_RENEW_ACCOUNT12);
			STR_CASE(SCE_NP_AUTH_ERROR_ACCOUNT_RENEW_ACCOUNT13);
			STR_CASE(SCE_NP_AUTH_ERROR_ACCOUNT_RENEW_ACCOUNT14);
			STR_CASE(SCE_NP_AUTH_ERROR_ACCOUNT_RENEW_ACCOUNT15);
			STR_CASE(SCE_NP_AUTH_ERROR_ACCOUNT_RENEW_ACCOUNT16);
			STR_CASE(SCE_NP_AUTH_ERROR_UNKNOWN);
			STR_CASE(SCE_NP_CORE_UTIL_ERROR_INVALID_ARGUMENT);
			STR_CASE(SCE_NP_CORE_UTIL_ERROR_OUT_OF_MEMORY);
			STR_CASE(SCE_NP_CORE_UTIL_ERROR_INSUFFICIENT);
			STR_CASE(SCE_NP_CORE_UTIL_ERROR_PARSER_FAILED);
			STR_CASE(SCE_NP_CORE_UTIL_ERROR_INVALID_PROTOCOL_ID);
			STR_CASE(SCE_NP_CORE_UTIL_ERROR_INVALID_EXTENSION);
			STR_CASE(SCE_NP_CORE_UTIL_ERROR_INVALID_TEXT);
			STR_CASE(SCE_NP_CORE_UTIL_ERROR_UNKNOWN_TYPE);
			STR_CASE(SCE_NP_CORE_UTIL_ERROR_UNKNOWN);
			STR_CASE(SCE_NP_CORE_PARSER_ERROR_NOT_INITIALIZED);
			STR_CASE(SCE_NP_CORE_PARSER_ERROR_ALREADY_INITIALIZED);
			STR_CASE(SCE_NP_CORE_PARSER_ERROR_OUT_OF_MEMORY);
			STR_CASE(SCE_NP_CORE_PARSER_ERROR_INSUFFICIENT);
			STR_CASE(SCE_NP_CORE_PARSER_ERROR_INVALID_FORMAT);
			STR_CASE(SCE_NP_CORE_PARSER_ERROR_INVALID_ARGUMENT);
			STR_CASE(SCE_NP_CORE_PARSER_ERROR_INVALID_HANDLE);
			STR_CASE(SCE_NP_CORE_PARSER_ERROR_INVALID_ICON);
			STR_CASE(SCE_NP_CORE_PARSER_ERROR_UNKNOWN);
			STR_CASE(SCE_NP_CORE_ERROR_ALREADY_INITIALIZED);
			STR_CASE(SCE_NP_CORE_ERROR_NOT_INITIALIZED);
			STR_CASE(SCE_NP_CORE_ERROR_INVALID_ARGUMENT);
			STR_CASE(SCE_NP_CORE_ERROR_OUT_OF_MEMORY);
			STR_CASE(SCE_NP_CORE_ERROR_ID_NOT_AVAILABLE);
			STR_CASE(SCE_NP_CORE_ERROR_USER_OFFLINE);
			STR_CASE(SCE_NP_CORE_ERROR_SESSION_RUNNING);
			STR_CASE(SCE_NP_CORE_ERROR_SESSION_NOT_ESTABLISHED);
			STR_CASE(SCE_NP_CORE_ERROR_SESSION_INVALID_STATE);
			STR_CASE(SCE_NP_CORE_ERROR_SESSION_ID_TOO_LONG);
			STR_CASE(SCE_NP_CORE_ERROR_SESSION_INVALID_NAMESPACE);
			STR_CASE(SCE_NP_CORE_ERROR_CONNECTION_TIMEOUT);
			STR_CASE(SCE_NP_CORE_ERROR_GETSOCKOPT);
			STR_CASE(SCE_NP_CORE_ERROR_SSL_NOT_INITIALIZED);
			STR_CASE(SCE_NP_CORE_ERROR_SSL_ALREADY_INITIALIZED);
			STR_CASE(SCE_NP_CORE_ERROR_SSL_NO_CERT);
			STR_CASE(SCE_NP_CORE_ERROR_SSL_NO_TRUSTWORTHY_CA);
			STR_CASE(SCE_NP_CORE_ERROR_SSL_INVALID_CERT);
			STR_CASE(SCE_NP_CORE_ERROR_SSL_CERT_VERIFY);
			STR_CASE(SCE_NP_CORE_ERROR_SSL_CN_CHECK);
			STR_CASE(SCE_NP_CORE_ERROR_SSL_HANDSHAKE_FAILED);
			STR_CASE(SCE_NP_CORE_ERROR_SSL_SEND);
			STR_CASE(SCE_NP_CORE_ERROR_SSL_RECV);
			STR_CASE(SCE_NP_CORE_ERROR_SSL_CREATE_CTX);
			STR_CASE(SCE_NP_CORE_ERROR_PARSE_PEM);
			STR_CASE(SCE_NP_CORE_ERROR_INVALID_INITIATE_STREAM);
			STR_CASE(SCE_NP_CORE_ERROR_SASL_NOT_SUPPORTED);
			STR_CASE(SCE_NP_CORE_ERROR_NAMESPACE_ALREADY_EXISTS);
			STR_CASE(SCE_NP_CORE_ERROR_FROM_ALREADY_EXISTS);
			STR_CASE(SCE_NP_CORE_ERROR_MODULE_NOT_REGISTERED);
			STR_CASE(SCE_NP_CORE_ERROR_MODULE_FROM_NOT_FOUND);
			STR_CASE(SCE_NP_CORE_ERROR_UNKNOWN_NAMESPACE);
			STR_CASE(SCE_NP_CORE_ERROR_INVALID_VERSION);
			STR_CASE(SCE_NP_CORE_ERROR_LOGIN_TIMEOUT);
			STR_CASE(SCE_NP_CORE_ERROR_TOO_MANY_SESSIONS);
			STR_CASE(SCE_NP_CORE_ERROR_SENDLIST_NOT_FOUND);
			STR_CASE(SCE_NP_CORE_ERROR_NO_ID);
			STR_CASE(SCE_NP_CORE_ERROR_LOAD_CERTS);
			STR_CASE(SCE_NP_CORE_ERROR_NET_SELECT);
			STR_CASE(SCE_NP_CORE_ERROR_DISCONNECTED);
			STR_CASE(SCE_NP_CORE_ERROR_TICKET_TOO_SMALL);
			STR_CASE(SCE_NP_CORE_ERROR_INVALID_TICKET);
			STR_CASE(SCE_NP_CORE_ERROR_INVALID_ONLINEID);
			STR_CASE(SCE_NP_CORE_ERROR_GETHOSTBYNAME);
			STR_CASE(SCE_NP_CORE_ERROR_UNDEFINED_STREAM_ERROR);
			STR_CASE(SCE_NP_CORE_ERROR_INTERNAL);
			STR_CASE(SCE_NP_CORE_ERROR_DNS_HOST_NOT_FOUND);
			STR_CASE(SCE_NP_CORE_ERROR_DNS_TRY_AGAIN);
			STR_CASE(SCE_NP_CORE_ERROR_DNS_NO_RECOVERY);
			STR_CASE(SCE_NP_CORE_ERROR_DNS_NO_DATA);
			STR_CASE(SCE_NP_CORE_ERROR_DNS_NO_ADDRESS);
			STR_CASE(SCE_NP_CORE_SERVER_ERROR_CONFLICT);
			STR_CASE(SCE_NP_CORE_SERVER_ERROR_NOT_AUTHORIZED);
			STR_CASE(SCE_NP_CORE_SERVER_ERROR_REMOTE_CONNECTION_FAILED);
			STR_CASE(SCE_NP_CORE_SERVER_ERROR_RESOURCE_CONSTRAINT);
			STR_CASE(SCE_NP_CORE_SERVER_ERROR_SYSTEM_SHUTDOWN);
			STR_CASE(SCE_NP_CORE_SERVER_ERROR_UNSUPPORTED_CLIENT_VERSION);
			STR_CASE(SCE_NP_SIGNALING_ERROR_NOT_INITIALIZED);
			STR_CASE(SCE_NP_SIGNALING_ERROR_ALREADY_INITIALIZED);
			STR_CASE(SCE_NP_SIGNALING_ERROR_OUT_OF_MEMORY);
			STR_CASE(SCE_NP_SIGNALING_ERROR_CTXID_NOT_AVAILABLE);
			STR_CASE(SCE_NP_SIGNALING_ERROR_CTX_NOT_FOUND);
			STR_CASE(SCE_NP_SIGNALING_ERROR_REQID_NOT_AVAILABLE);
			STR_CASE(SCE_NP_SIGNALING_ERROR_REQ_NOT_FOUND);
			STR_CASE(SCE_NP_SIGNALING_ERROR_PARSER_CREATE_FAILED);
			STR_CASE(SCE_NP_SIGNALING_ERROR_PARSER_FAILED);
			STR_CASE(SCE_NP_SIGNALING_ERROR_INVALID_NAMESPACE);
			STR_CASE(SCE_NP_SIGNALING_ERROR_NETINFO_NOT_AVAILABLE);
			STR_CASE(SCE_NP_SIGNALING_ERROR_PEER_NOT_RESPONDING);
			STR_CASE(SCE_NP_SIGNALING_ERROR_CONNID_NOT_AVAILABLE);
			STR_CASE(SCE_NP_SIGNALING_ERROR_CONN_NOT_FOUND);
			STR_CASE(SCE_NP_SIGNALING_ERROR_PEER_UNREACHABLE);
			STR_CASE(SCE_NP_SIGNALING_ERROR_TERMINATED_BY_PEER);
			STR_CASE(SCE_NP_SIGNALING_ERROR_TIMEOUT);
			STR_CASE(SCE_NP_SIGNALING_ERROR_CTX_MAX);
			STR_CASE(SCE_NP_SIGNALING_ERROR_RESULT_NOT_FOUND);
			STR_CASE(SCE_NP_SIGNALING_ERROR_CONN_IN_PROGRESS);
			STR_CASE(SCE_NP_SIGNALING_ERROR_INVALID_ARGUMENT);
			STR_CASE(SCE_NP_SIGNALING_ERROR_OWN_NP_ID);
			STR_CASE(SCE_NP_SIGNALING_ERROR_TOO_MANY_CONN);
			STR_CASE(SCE_NP_SIGNALING_ERROR_TERMINATED_BY_MYSELF);
			STR_CASE(SCE_NP_CUSTOM_MENU_ERROR_ALREADY_INITIALIZED);
			STR_CASE(SCE_NP_CUSTOM_MENU_ERROR_NOT_INITIALIZED);
			STR_CASE(SCE_NP_CUSTOM_MENU_ERROR_OUT_OF_MEMORY);
			STR_CASE(SCE_NP_CUSTOM_MENU_ERROR_NOT_SUPPORTED);
			STR_CASE(SCE_NP_CUSTOM_MENU_ERROR_INSUFFICIENT);
			STR_CASE(SCE_NP_CUSTOM_MENU_ERROR_CANCEL);
			STR_CASE(SCE_NP_CUSTOM_MENU_ERROR_STATUS);
			STR_CASE(SCE_NP_CUSTOM_MENU_ERROR_BUSY);
			STR_CASE(SCE_NP_CUSTOM_MENU_ERROR_INVALID_ARGUMENT);
			STR_CASE(SCE_NP_CUSTOM_MENU_ERROR_ABORT);
			STR_CASE(SCE_NP_CUSTOM_MENU_ERROR_NOT_REGISTERED);
			STR_CASE(SCE_NP_CUSTOM_MENU_ERROR_EXCEEDS_MAX);
			STR_CASE(SCE_NP_CUSTOM_MENU_ERROR_INVALID_CHARACTER);
		}

		return unknown;
	});
}

s32 g_psn_connection_status = SCE_NP_MANAGER_STATUS_OFFLINE;

int sceNpInit(uint32_t poolsize, void* poolptr)
{
	LOG_SCE_TRACE("(poolsize=0x%x, poolptr=*0x%x)", poolsize, poolptr);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		const auto manager = g_fxo->get<sce_np_manager>();
	
		if (manager->is_initialized)
		{
			ret = SCE_NP_ERROR_ALREADY_INITIALIZED;
			break;
		}
	
		if (poolsize == 0)
		{
			ret = SCE_NP_ERROR_INVALID_ARGUMENT;
			break;
		}
		else if (poolsize < SCE_NP_MIN_POOLSIZE)
		{
			ret = SCE_NP_ERROR_INSUFFICIENT_BUFFER;
			break;
		}
	
		if (!poolptr)
		{
			ret = SCE_NP_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		manager->is_initialized = true;
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpTerm()
{
	LOG_SCE_TRACE("()");

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		const auto manager = g_fxo->get<sce_np_manager>();
	
		if (!manager->is_initialized)
		{
			ret = SCE_NP_ERROR_NOT_INITIALIZED;
			break;
		}
	
		manager->is_initialized = false;
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

error_code npDrmIsAvailable(vm::cptr<u8> k_licensee_addr, vm::cptr<char> drm_path)
{
	std::array<u8, 0x10> k_licensee{};

	if (k_licensee_addr)
	{
		std::copy_n(k_licensee_addr.get_ptr(), k_licensee.size(), k_licensee.begin());
		sceNp.notice("npDrmIsAvailable(): KLicense key %s", *reinterpret_cast<be_t<v128, 1>*>(k_licensee.data()));
	}

	const std::string enc_drm_path = drm_path.get_ptr();

	if (!fs::is_file(vfs::get(enc_drm_path)))
	{
		sceNp.warning("npDrmIsAvailable(): '%s' not found", enc_drm_path);
		return CELL_ENOENT;
	}

	auto npdrmkeys = g_fxo->get<loaded_npdrm_keys>();

	npdrmkeys->devKlic.fill(0);
	npdrmkeys->rifKey.fill(0);

	std::string rap_dir_path = "/dev_hdd0/home/" + Emu.GetUsr() + "/exdata/";

	const std::string& enc_drm_path_local = vfs::get(enc_drm_path);
	const fs::file enc_file(enc_drm_path_local);

	u32 magic;

	enc_file.read<u32>(magic);
	enc_file.seek(0);

	if (magic == "SCE\0"_u32)
	{
		if (!k_licensee_addr)
			k_licensee = get_default_self_klic();

		if (verify_npdrm_self_headers(enc_file, k_licensee.data()))
		{
			npdrmkeys->devKlic = std::move(k_licensee);
		}
		else
		{
			sceNp.error("npDrmIsAvailable(): Failed to verify sce file %s", enc_drm_path);
			return SCE_NP_DRM_ERROR_NO_ENTITLEMENT;
		}

	}
	else if (magic == "NPD\0"_u32)
	{
		// edata / sdata files

		std::string contentID;

		if (VerifyEDATHeaderWithKLicense(enc_file, enc_drm_path_local, k_licensee, &contentID))
		{
			const std::string rap_file = rap_dir_path + contentID + ".rap";
			npdrmkeys->devKlic = std::move(k_licensee);

			if (fs::is_file(vfs::get(rap_file)))
				npdrmkeys->rifKey = GetEdatRifKeyFromRapFile(fs::file{ vfs::get(rap_file) });
			else
				sceNp.warning("npDrmIsAvailable(): Rap file not found: %s", rap_file.c_str());
		}
		else
		{
			sceNp.error("npDrmIsAvailable(): Failed to verify npd file %s", enc_drm_path);
			return SCE_NP_DRM_ERROR_NO_ENTITLEMENT;
		}
	}
	else
	{
		// for now assume its just unencrypted
		sceNp.notice("npDrmIsAvailable(): Assuming npdrm file is unencrypted at %s", enc_drm_path);
	}
	return CELL_OK;
}

int sceNpDrmIsAvailable(const uint8_t* k_licensee_addr, const char* drm_path)
{
	LOG_SCE_TRACE("(k_licensee=*0x%x, drm_path=%s)", k_licensee_addr, drm_path);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		ret = npDrmIsAvailable(k_licensee_addr, drm_path);
			break;
}

int sceNpDrmIsAvailable2(const uint8_t* k_licensee_addr, const char* drm_path)
{
	LOG_SCE_TRACE("(k_licensee=*0x%x, drm_path=%s)", k_licensee_addr, drm_path);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		ret = npDrmIsAvailable(k_licensee_addr, drm_path);
			break;
}

int sceNpDrmVerifyUpgradeLicense(const char* content_id)
{
	LOG_SCE_TRACE("(content_id=%s)", content_id);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!content_id)
		{
			ret = SCE_NP_DRM_ERROR_INVALID_PARAM;
			break;
		}
	
		if (!fs::is_file(vfs::get("/dev_hdd0/home/" + Emu.GetUsr() + "/exdata/" + content_id.get_ptr() + ".rap")))
		{
			// Game hasn't been purchased therefore no RAP file present
			ret = SCE_NP_DRM_ERROR_LICENSE_NOT_FOUND;
			break;
		}
	
		// Game has been purchased and there's a RAP file present
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpDrmVerifyUpgradeLicense2(const char* content_id)
{
	LOG_SCE_TRACE("(content_id=%s)", content_id);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!content_id)
		{
			ret = SCE_NP_DRM_ERROR_INVALID_PARAM;
			break;
		}
	
		if (!fs::is_file(vfs::get("/dev_hdd0/home/" + Emu.GetUsr() + "/exdata/" + content_id.get_ptr() + ".rap")))
		{
			// Game hasn't been purchased therefore no RAP file present
			ret = SCE_NP_DRM_ERROR_LICENSE_NOT_FOUND;
			break;
		}
	
		// Game has been purchased and there's a RAP file present
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpDrmExecuteGamePurchase()
{
	LOG_SCE_TRACE("()");

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpDrmGetTimelimit(const char* path, uint64_t* time_remain)
{
	LOG_SCE_TRACE("(path=%s, time_remain=*0x%x)", path, time_remain);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!path || !time_remain)
		{
			ret = SCE_NP_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		*time_remain = 0x7FFFFFFFFFFFFFFFULL;
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpDrmProcessExitSpawn(ppu_thread& ppu, const uint8_t* klicensee, const char* path, vm::cpptr<char> argv, vm::cpptr<char> envp, uint32_t data, uint32_t data_size, int32_t prio, uint64_t flags)
{
	LOG_SCE_TRACE("(klicensee=*0x%x, path=%s, argv=**0x%x, envp=**0x%x, data=*0x%x, data_size=0x%x, prio=%d, flags=0x%x)", klicensee, path, argv, envp, data, data_size, prio, flags);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (s32 error = npDrmIsAvailable(klicensee, path))
		{
			ret = error;
			break;
		}
	
		sys_game_process_exitspawn(ppu, path, argv, envp, data, data_size, prio, flags);
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpDrmProcessExitSpawn2(ppu_thread& ppu, const uint8_t* klicensee, const char* path, vm::cpptr<char> argv, vm::cpptr<char> envp, uint32_t data, uint32_t data_size, int32_t prio, uint64_t flags)
{
	LOG_SCE_TRACE("(klicensee=*0x%x, path=%s, argv=**0x%x, envp=**0x%x, data=*0x%x, data_size=0x%x, prio=%d, flags=0x%x)", klicensee, path, argv, envp, data, data_size, prio, flags);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (s32 error = npDrmIsAvailable(klicensee, path))
		{
			ret = error;
			break;
		}
	
		sys_game_process_exitspawn2(ppu, path, argv, envp, data, data_size, prio, flags);
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpBasicRegisterHandler(const SceNpCommunicationId* context, SceNpBasicEventHandler* handler, void* arg)
{
	LOG_SCE_TRACE("(context=*0x%x, handler=*0x%x, arg=*0x%x)", context, handler, arg);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_BASIC_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!context || !handler)
		{
			ret = SCE_NP_BASIC_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpBasicRegisterContextSensitiveHandler(const SceNpCommunicationId* context, SceNpBasicEventHandler* handler, void* arg)
{
	LOG_SCE_TRACE("(context=*0x%x, handler=*0x%x, arg=*0x%x)", context, handler, arg);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_BASIC_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!context || !handler)
		{
			ret = SCE_NP_BASIC_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpBasicUnregisterHandler()
{
	LOG_SCE_TRACE("()");

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_BASIC_ERROR_NOT_INITIALIZED;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpBasicSetPresence(const void* data, uint64_t size)
{
	LOG_SCE_TRACE("(data=*0x%x, size=%d)", data, size);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_BASIC_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (size > SCE_NP_BASIC_MAX_PRESENCE_SIZE)
		{
			ret = SCE_NP_BASIC_ERROR_EXCEEDS_MAX;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpBasicSetPresenceDetails(const SceNpBasicPresenceDetails* pres, uint32_t options)
{
	LOG_SCE_TRACE("(pres=*0x%x, options=0x%x)", pres, options);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_BASIC_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!pres || options > SCE_NP_BASIC_PRESENCE_OPTIONS_ALL_OPTIONS)
		{
			ret = SCE_NP_BASIC_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		if (pres->size > SCE_NP_BASIC_MAX_PRESENCE_SIZE)
		{
			ret = SCE_NP_BASIC_ERROR_EXCEEDS_MAX;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpBasicSetPresenceDetails2(const SceNpBasicPresenceDetails2* pres, uint32_t options)
{
	LOG_SCE_TRACE("(pres=*0x%x, options=0x%x)", pres, options);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_BASIC_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!pres || options > SCE_NP_BASIC_PRESENCE_OPTIONS_ALL_OPTIONS)
		{
			ret = SCE_NP_BASIC_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		if (pres->size > SCE_NP_BASIC_MAX_PRESENCE_SIZE)
		{
			ret = SCE_NP_BASIC_ERROR_EXCEEDS_MAX;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpBasicSendMessage(const SceNpId* to, const void* data, uint64_t size)
{
	LOG_SCE_TRACE("(to=*0x%x, data=*0x%x, size=%d)", to, data, size);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_BASIC_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!to || to->handle.data[0] == '\0' || !data || !size)
		{
			ret = SCE_NP_BASIC_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		if (size > SCE_NP_BASIC_MAX_MESSAGE_SIZE)
		{
			ret = SCE_NP_BASIC_ERROR_EXCEEDS_MAX;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpBasicSendMessageGui(const SceNpBasicMessageDetails* msg, sys_memory_container_t containerId)
{
	LOG_SCE_TRACE("(msg=*0x%x, containerId=%d)", msg, containerId);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_BASIC_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!msg || msg->count > SCE_NP_BASIC_SEND_MESSAGE_MAX_RECIPIENTS || msg->npids.handle.data[0] == '\0' || !(msg->msgFeatures & SCE_NP_BASIC_MESSAGE_FEATURES_ALL_FEATURES))
		{
			ret = SCE_NP_BASIC_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		if (msg->size > SCE_NP_BASIC_MAX_MESSAGE_SIZE)
		{
			ret = SCE_NP_BASIC_ERROR_EXCEEDS_MAX;
			break;
		}
	
		if (g_psn_connection_status != SCE_NP_MANAGER_STATUS_ONLINE)
		{
			ret = not_an_error(SCE_NP_BASIC_ERROR_NOT_CONNECTED);
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpBasicSendMessageAttachment(const SceNpId* to, const char* subject, const char* body, const char* data, uint64_t size, sys_memory_container_t containerId)
{
	LOG_SCE_TRACE("(to=*0x%x, subject=%s, body=%s, data=%s, size=%d, containerId=%d)", to, subject, body, data, size, containerId);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_BASIC_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!to || to->handle.data[0] == '\0' || !data || !size)
		{
			ret = SCE_NP_BASIC_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		if (strlen(subject.get_ptr()) > SCE_NP_BASIC_BODY_CHARACTER_MAX || strlen(body.get_ptr()) > SCE_NP_BASIC_BODY_CHARACTER_MAX)
		{
			ret = SCE_NP_BASIC_ERROR_EXCEEDS_MAX;
			break;
		}
	
		if (g_psn_connection_status != SCE_NP_MANAGER_STATUS_ONLINE)
		{
			ret = not_an_error(SCE_NP_BASIC_ERROR_NOT_CONNECTED);
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpBasicRecvMessageAttachment(sys_memory_container_t containerId)
{
	LOG_SCE_TRACE("(containerId=%d)", containerId);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_BASIC_ERROR_NOT_INITIALIZED;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpBasicRecvMessageAttachmentLoad(uint32_t id, void* buffer, uint64_t* size)
{
	LOG_SCE_TRACE("(id=%d, buffer=*0x%x, size=*0x%x)", id, buffer, size);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_BASIC_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!buffer || !size)
		{
			ret = SCE_NP_BASIC_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		if (id > SCE_NP_BASIC_SELECTED_MESSAGE_DATA)
		{
			ret = SCE_NP_BASIC_ERROR_INVALID_DATA_ID;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpBasicRecvMessageCustom(uint16_t mainType, uint32_t recvOptions, sys_memory_container_t containerId)
{
	LOG_SCE_TRACE("(mainType=%d, recvOptions=%d, containerId=%d)", mainType, recvOptions, containerId);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_BASIC_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!(recvOptions & SCE_NP_BASIC_RECV_MESSAGE_OPTIONS_ALL_OPTIONS))
		{
			ret = SCE_NP_BASIC_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpBasicMarkMessageAsUsed(SceNpBasicMessageId msgId)
{
	LOG_SCE_TRACE("(msgId=%d)", msgId);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_BASIC_ERROR_NOT_INITIALIZED;
			break;
		}
	
		//if (!msgId > ?)
		//{
		//	ret = SCE_NP_BASIC_ERROR_INVALID_ARGUMENT;
			break;
		//}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpBasicAbortGui()
{
	LOG_SCE_TRACE("()");

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_BASIC_ERROR_NOT_INITIALIZED;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpBasicAddFriend(const SceNpId* contact, const char* body, sys_memory_container_t containerId)
{
	LOG_SCE_TRACE("(contact=*0x%x, body=%s, containerId=%d)", contact, body, containerId);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_BASIC_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!contact || contact->handle.data[0] == '\0')
		{
			ret = SCE_NP_BASIC_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		if (strlen(body.get_ptr()) > SCE_NP_BASIC_BODY_CHARACTER_MAX)
		{
			ret = SCE_NP_BASIC_ERROR_EXCEEDS_MAX;
			break;
		}
	
		if (g_psn_connection_status != SCE_NP_MANAGER_STATUS_ONLINE)
		{
			ret = not_an_error(SCE_NP_BASIC_ERROR_NOT_CONNECTED);
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpBasicGetFriendListEntryCount(uint32_t* count)
{
	LOG_SCE_TRACE("(count=*0x%x)", count);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_BASIC_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!count)
		{
			ret = SCE_NP_BASIC_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		// TODO: Find the correct test which ret =s SCE_NP_ERROR_ID_NOT_FOUND
			break;
		if (g_psn_connection_status != SCE_NP_MANAGER_STATUS_ONLINE)
		{
			ret = SCE_NP_ERROR_ID_NOT_FOUND;
			break;
		}
	
		// TODO: Check if there are any friends
		*count = 0;
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpBasicGetFriendListEntry(uint32_t index, SceNpId* npid)
{
	LOG_SCE_TRACE("(index=%d, npid=*0x%x)", index, npid);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_BASIC_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!npid)
		{
			// TODO: check index
			ret = SCE_NP_BASIC_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		// TODO: Find the correct test which ret =s SCE_NP_ERROR_ID_NOT_FOUND
			break;
		if (g_psn_connection_status != SCE_NP_MANAGER_STATUS_ONLINE)
		{
			ret = SCE_NP_ERROR_ID_NOT_FOUND;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpBasicGetFriendPresenceByIndex(uint32_t index, SceNpUserInfo* user, SceNpBasicPresenceDetails* pres, uint32_t options)
{
	LOG_SCE_TRACE("(index=%d, user=*0x%x, pres=*0x%x, options=%d)", index, user, pres, options);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_BASIC_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!user || !pres)
		{
			// TODO: check index and (options & SCE_NP_BASIC_PRESENCE_OPTIONS_ALL_OPTIONS) depending on fw
			ret = SCE_NP_BASIC_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpBasicGetFriendPresenceByIndex2(uint32_t index, SceNpUserInfo* user, SceNpBasicPresenceDetails2* pres, uint32_t options)
{
	LOG_SCE_TRACE("(index=%d, user=*0x%x, pres=*0x%x, options=%d)", index, user, pres, options);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_BASIC_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!user || !pres)
		{
			// TODO: check index and (options & SCE_NP_BASIC_PRESENCE_OPTIONS_ALL_OPTIONS) depending on fw
			ret = SCE_NP_BASIC_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpBasicGetFriendPresenceByNpId(const SceNpId* npid, SceNpBasicPresenceDetails* pres, uint32_t options)
{
	LOG_SCE_TRACE("(npid=*0x%x, pres=*0x%x, options=%d)", npid, pres, options);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_BASIC_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!npid || !pres)
		{
			// TODO: check (options & SCE_NP_BASIC_PRESENCE_OPTIONS_ALL_OPTIONS) depending on fw
			ret = SCE_NP_BASIC_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpBasicGetFriendPresenceByNpId2(const SceNpId* npid, SceNpBasicPresenceDetails2* pres, uint32_t options)
{
	LOG_SCE_TRACE("(npid=*0x%x, pres=*0x%x, options=%d)", npid, pres, options);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_BASIC_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!npid || !pres)
		{
			// TODO: check (options & SCE_NP_BASIC_PRESENCE_OPTIONS_ALL_OPTIONS) depending on fw
			ret = SCE_NP_BASIC_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpBasicAddPlayersHistory(const SceNpId* npid, char* description)
{
	LOG_SCE_TRACE("(npid=*0x%x, description=*0x%x)", npid, description);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_BASIC_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!npid || npid->handle.data[0] == '\0')
		{
			ret = SCE_NP_BASIC_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		if (description && strlen(description.get_ptr()) > SCE_NP_BASIC_DESCRIPTION_CHARACTER_MAX)
		{
			ret = SCE_NP_BASIC_ERROR_EXCEEDS_MAX;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpBasicAddPlayersHistoryAsync(const SceNpId* npids, uint32_t count, char* description, uint32_t* reqId)
{
	LOG_SCE_TRACE("(npids=*0x%x, count=%d, description=*0x%x, reqId=*0x%x)", npids, count, description, reqId);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_BASIC_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!count)
		{
			ret = SCE_NP_BASIC_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		if (count > SCE_NP_BASIC_PLAYER_HISTORY_MAX_PLAYERS)
		{
			ret = SCE_NP_BASIC_ERROR_EXCEEDS_MAX;
			break;
		}
	
		if (!npids)
		{
			ret = SCE_NP_BASIC_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		for (u32 i = 0; i < count; i++)
		{
			if (npids[i].handle.data[0] == '\0')
			{
				ret = SCE_NP_BASIC_ERROR_INVALID_ARGUMENT;
			break;
			}
		}
	
		if (description && strlen(description.get_ptr()) > SCE_NP_BASIC_DESCRIPTION_CHARACTER_MAX)
		{
			ret = SCE_NP_BASIC_ERROR_EXCEEDS_MAX;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpBasicGetPlayersHistoryEntryCount(uint32_t options, uint32_t* count)
{
	LOG_SCE_TRACE("(options=%d, count=*0x%x)", options, count);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_BASIC_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!count)
		{
			ret = SCE_NP_BASIC_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		// TODO: Find the correct test which ret =s SCE_NP_ERROR_ID_NOT_FOUND
			break;
		if (g_psn_connection_status != SCE_NP_MANAGER_STATUS_ONLINE)
		{
			ret = SCE_NP_ERROR_ID_NOT_FOUND;
			break;
		}
	
		// TODO: Check if there are players histories
		*count = 0;
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpBasicGetPlayersHistoryEntry(uint32_t options, uint32_t index, SceNpId* npid)
{
	LOG_SCE_TRACE("(options=%d, index=%d, npid=*0x%x)", options, index, npid);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_BASIC_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!npid)
		{
			// TODO: Check index
			ret = SCE_NP_BASIC_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		// TODO: Find the correct test which ret =s SCE_NP_ERROR_ID_NOT_FOUND
			break;
		if (g_psn_connection_status != SCE_NP_MANAGER_STATUS_ONLINE)
		{
			ret = SCE_NP_ERROR_ID_NOT_FOUND;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpBasicAddBlockListEntry(const SceNpId* npid)
{
	LOG_SCE_TRACE("(npid=*0x%x)", npid);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_BASIC_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!npid || npid->handle.data[0] == '\0')
		{
			ret = SCE_NP_BASIC_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		if (g_psn_connection_status != SCE_NP_MANAGER_STATUS_ONLINE)
		{
			ret = not_an_error(SCE_NP_BASIC_ERROR_NOT_CONNECTED);
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpBasicGetBlockListEntryCount(uint32_t* count)
{
	LOG_SCE_TRACE("(count=*0x%x)", count);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_BASIC_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!count)
		{
			ret = SCE_NP_BASIC_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		// TODO: Find the correct test which ret =s SCE_NP_ERROR_ID_NOT_FOUND
			break;
		if (g_psn_connection_status != SCE_NP_MANAGER_STATUS_ONLINE)
		{
			ret = SCE_NP_ERROR_ID_NOT_FOUND;
			break;
		}
	
		// TODO: Check if there are block lists
		*count = 0;
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpBasicGetBlockListEntry(uint32_t index, SceNpId* npid)
{
	LOG_SCE_TRACE("(index=%d, npid=*0x%x)", index, npid);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_BASIC_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!npid)
		{
			// TODO: check index
			ret = SCE_NP_BASIC_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		// TODO: Find the correct test which ret =s SCE_NP_ERROR_ID_NOT_FOUND
			break;
		if (g_psn_connection_status != SCE_NP_MANAGER_STATUS_ONLINE)
		{
			ret = SCE_NP_ERROR_ID_NOT_FOUND;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpBasicGetMessageAttachmentEntryCount(uint32_t* count)
{
	LOG_SCE_TRACE("(count=*0x%x)", count);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_BASIC_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!count)
		{
			ret = SCE_NP_BASIC_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		// TODO: Find the correct test which ret =s SCE_NP_ERROR_ID_NOT_FOUND
			break;
		if (g_psn_connection_status != SCE_NP_MANAGER_STATUS_ONLINE)
		{
			ret = SCE_NP_ERROR_ID_NOT_FOUND;
			break;
		}
	
		// TODO: Check if there are message attachments
		*count = 0;
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpBasicGetMessageAttachmentEntry(uint32_t index, SceNpUserInfo* from)
{
	LOG_SCE_TRACE("(index=%d, from=*0x%x)", index, from);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_BASIC_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!from)
		{
			// TODO: check index
			ret = SCE_NP_BASIC_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		// TODO: Find the correct test which ret =s SCE_NP_ERROR_ID_NOT_FOUND
			break;
		if (g_psn_connection_status != SCE_NP_MANAGER_STATUS_ONLINE)
		{
			ret = SCE_NP_ERROR_ID_NOT_FOUND;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpBasicGetCustomInvitationEntryCount(uint32_t* count)
{
	LOG_SCE_TRACE("(count=*0x%x)", count);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!count)
		{
			ret = SCE_NP_AUTH_EINVAL;
			break;
		}
	
		// TODO: Find the correct test which ret =s SCE_NP_AUTH_ESRCH
			break;
		if (g_psn_connection_status != SCE_NP_MANAGER_STATUS_ONLINE)
		{
			ret = SCE_NP_AUTH_ESRCH;
			break;
		}
	
		// TODO: Check if there are custom invitations
		*count = 0;
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpBasicGetCustomInvitationEntry(uint32_t index, SceNpUserInfo* from)
{
	LOG_SCE_TRACE("(index=%d, from=*0x%x)", index, from);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!from)
		{
			// TODO: check index
			ret = SCE_NP_AUTH_EINVAL;
			break;
		}
	
		// TODO: Find the correct test which ret =s SCE_NP_ERROR_ID_NOT_FOUND
			break;
		if (g_psn_connection_status != SCE_NP_MANAGER_STATUS_ONLINE)
		{
			ret = SCE_NP_AUTH_ESRCH;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpBasicGetMatchingInvitationEntryCount(uint32_t* count)
{
	LOG_SCE_TRACE("(count=*0x%x)", count);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_BASIC_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!count)
		{
			ret = SCE_NP_BASIC_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		// TODO: Find the correct test which ret =s SCE_NP_ERROR_ID_NOT_FOUND
			break;
		if (g_psn_connection_status != SCE_NP_MANAGER_STATUS_ONLINE)
		{
			ret = SCE_NP_ERROR_ID_NOT_FOUND;
			break;
		}
	
		// TODO: Check if there are matching invitations
		*count = 0;
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpBasicGetMatchingInvitationEntry(uint32_t index, SceNpUserInfo* from)
{
	LOG_SCE_TRACE("(index=%d, from=*0x%x)", index, from);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_BASIC_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!from)
		{
			// TODO: check index
			ret = SCE_NP_BASIC_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		// TODO: Find the correct test which ret =s SCE_NP_ERROR_ID_NOT_FOUND
			break;
		if (g_psn_connection_status != SCE_NP_MANAGER_STATUS_ONLINE)
		{
			ret = SCE_NP_ERROR_ID_NOT_FOUND;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpBasicGetClanMessageEntryCount(uint32_t* count)
{
	LOG_SCE_TRACE("(count=*0x%x)", count);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_BASIC_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!count)
		{
			ret = SCE_NP_BASIC_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		// TODO: Find the correct test which ret =s SCE_NP_ERROR_ID_NOT_FOUND
			break;
		if (g_psn_connection_status != SCE_NP_MANAGER_STATUS_ONLINE)
		{
			ret = SCE_NP_ERROR_ID_NOT_FOUND;
			break;
		}
	
		// TODO: Check if there are clan messages
		*count = 0;
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpBasicGetClanMessageEntry(uint32_t index, SceNpUserInfo* from)
{
	LOG_SCE_TRACE("(index=%d, from=*0x%x)", index, from);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_BASIC_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!from)
		{
			// TODO: check index
			ret = SCE_NP_BASIC_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		// TODO: Find the correct test which ret =s SCE_NP_ERROR_ID_NOT_FOUND
			break;
		if (g_psn_connection_status != SCE_NP_MANAGER_STATUS_ONLINE)
		{
			ret = SCE_NP_ERROR_ID_NOT_FOUND;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpBasicGetMessageEntryCount(uint32_t type, uint32_t* count)
{
	LOG_SCE_TRACE("(type=%d, count=*0x%x)", type, count);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_BASIC_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!count)
		{
			ret = SCE_NP_BASIC_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		// TODO: Find the correct test which ret =s SCE_NP_ERROR_ID_NOT_FOUND
			break;
		if (g_psn_connection_status != SCE_NP_MANAGER_STATUS_ONLINE)
		{
			ret = SCE_NP_ERROR_ID_NOT_FOUND;
			break;
		}
	
		// TODO: Check if there are messages
		*count = 0;
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpBasicGetMessageEntry(uint32_t type, uint32_t index, SceNpUserInfo* from)
{
	LOG_SCE_TRACE("(type=%d, index=%d, from=*0x%x)", type, index, from);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_BASIC_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!from)
		{
			// TODO: check index
			ret = SCE_NP_BASIC_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		// TODO: Find the correct test which ret =s SCE_NP_ERROR_ID_NOT_FOUND
			break;
		if (g_psn_connection_status != SCE_NP_MANAGER_STATUS_ONLINE)
		{
			ret = SCE_NP_ERROR_ID_NOT_FOUND;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpBasicGetEvent(int32_t* event, SceNpUserInfo* from, int32_t* data, uint32_t* size)
{
	LOG_SCE_TRACE("(event=*0x%x, from=*0x%x, data=*0x%x, size=*0x%x)", event, from, data, size);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_BASIC_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!event || !from || !data || !size)
		{
			ret = SCE_NP_BASIC_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		// TODO: Check for other error and pass other events
		//*event = SCE_NP_BASIC_EVENT_OFFLINE; // This event only indicates a contact is offline, not the current status of the connection
	
		ret = not_an_error(SCE_NP_BASIC_ERROR_NO_EVENT);
			break;
}

int sceNpCommerceCreateCtx(uint32_t version, SceNpId* npId, SceNpCommerceHandler* handler, void* arg, uint32_t* ctx_id)
{
	LOG_SCE_TRACE("(version=%d, event=*0x%x, from=*0x%x, arg=*0x%x, ctx_id=*0x%x)", version, npId, handler, arg, ctx_id);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpCommerceDestroyCtx(uint32_t ctx_id)
{
	LOG_SCE_TRACE("(ctx_id=%d)", ctx_id);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpCommerceInitProductCategory(SceNpCommerceProductCategory* pc, const void* data, uint64_t data_size)
{
	LOG_SCE_TRACE("(pc=*0x%x, data=*0x%x, data_size=%d)", pc, data, data_size);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
		ret = SCE_OK;
	} while(false);
	return ret;
}

void sceNpCommerceDestroyProductCategory(vm::ptr<SceNpCommerceProductCategory> pc)
{
	sceNp.todo("sceNpCommerceDestroyProductCategory(pc=*0x%x)", pc);
}

int sceNpCommerceGetProductCategoryStart(uint32_t ctx_id, const char* category_id, int32_t lang_code, uint32_t* req_id)
{
	LOG_SCE_TRACE("(ctx_id=%d, category_id=%s, lang_code=%d, req_id=*0x%x)", ctx_id, category_id, lang_code, req_id);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpCommerceGetProductCategoryFinish(uint32_t req_id)
{
	LOG_SCE_TRACE("(req_id=%d)", req_id);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpCommerceGetProductCategoryResult(uint32_t req_id, void* buf, uint64_t buf_size, uint64_t* fill_size)
{
	LOG_SCE_TRACE("(req_id=%d, buf=*0x%x, buf_size=%d, fill_size=*0x%x)", req_id, buf, buf_size, fill_size);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpCommerceGetProductCategoryAbort(uint32_t req_id)
{
	LOG_SCE_TRACE("(req_id=%d)", req_id);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
		ret = SCE_OK;
	} while(false);
	return ret;
}

vm::cptr<char> sceNpCommerceGetProductId(vm::ptr<SceNpCommerceProductSkuInfo> info)
{
	sceNp.todo("sceNpCommerceGetProductId(info=*0x%x)", info);
	return vm::null;
}

vm::cptr<char> sceNpCommerceGetProductName(vm::ptr<SceNpCommerceProductSkuInfo> info)
{
	sceNp.todo("sceNpCommerceGetProductName(info=*0x%x)", info);
	return vm::null;
}

vm::cptr<char> sceNpCommerceGetCategoryDescription(vm::ptr<SceNpCommerceCategoryInfo> info)
{
	sceNp.todo("sceNpCommerceGetCategoryDescription(info=*0x%x)", info);
	return vm::null;
}

vm::cptr<char> sceNpCommerceGetCategoryId(vm::ptr<SceNpCommerceCategoryInfo> info)
{
	sceNp.todo("sceNpCommerceGetCategoryId(info=*0x%x)", info);
	return vm::null;
}

vm::cptr<char> sceNpCommerceGetCategoryImageURL(vm::ptr<SceNpCommerceCategoryInfo> info)
{
	sceNp.todo("sceNpCommerceGetCategoryImageURL(info=*0x%x)", info);
	return vm::null;
}

int sceNpCommerceGetCategoryInfo(SceNpCommerceProductCategory* pc, SceNpCommerceCategoryInfo* info)
{
	LOG_SCE_TRACE("(pc=*0x%x, info=*0x%x)", pc, info);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
		ret = SCE_OK;
	} while(false);
	return ret;
}

vm::cptr<char> sceNpCommerceGetCategoryName(vm::ptr<SceNpCommerceCategoryInfo> info)
{
	sceNp.todo("sceNpCommerceGetCategoryName(info=*0x%x)", info);
	return vm::null;
}

vm::cptr<char> sceNpCommerceGetCurrencyCode(vm::ptr<SceNpCommerceCurrencyInfo> info)
{
	sceNp.todo("sceNpCommerceGetCurrencyCode(info=*0x%x)", info);
	return vm::null;
}

u32 sceNpCommerceGetCurrencyDecimals(vm::ptr<SceNpCommerceCurrencyInfo> info)
{
	sceNp.todo("sceNpCommerceGetCurrencyDecimals(info=*0x%x)", info);
	return 0;
}

int sceNpCommerceGetCurrencyInfo(SceNpCommerceProductCategory* pc, SceNpCommerceCurrencyInfo* info)
{
	LOG_SCE_TRACE("(pc=*0x%x, info=*0x%x)", pc, info);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpCommerceGetNumOfChildCategory(SceNpCommerceProductCategory* pc, uint32_t* num)
{
	LOG_SCE_TRACE("(pc=*0x%x, num=*0x%x)", pc, num);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpCommerceGetNumOfChildProductSku(SceNpCommerceProductCategory* pc, uint32_t* num)
{
	LOG_SCE_TRACE("(pc=*0x%x, num=*0x%x)", pc, num);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
		ret = SCE_OK;
	} while(false);
	return ret;
}

vm::cptr<char> sceNpCommerceGetSkuDescription(vm::ptr<SceNpCommerceProductSkuInfo> info)
{
	sceNp.todo("sceNpCommerceGetSkuDescription(info=*0x%x)", info);
	return vm::null;
}

vm::cptr<char> sceNpCommerceGetSkuId(vm::ptr<SceNpCommerceProductSkuInfo> info)
{
	sceNp.todo("sceNpCommerceGetSkuId(info=*0x%x)", info);
	return vm::null;
}

vm::cptr<char> sceNpCommerceGetSkuImageURL(vm::ptr<SceNpCommerceProductSkuInfo> info)
{
	sceNp.todo("sceNpCommerceGetSkuImageURL(info=*0x%x)", info);
	return vm::null;
}

vm::cptr<char> sceNpCommerceGetSkuName(vm::ptr<SceNpCommerceProductSkuInfo> info)
{
	sceNp.todo("sceNpCommerceGetSkuName(info=*0x%x)", info);
	return vm::null;
}

void sceNpCommerceGetSkuPrice(vm::ptr<SceNpCommerceProductSkuInfo> info, vm::ptr<SceNpCommercePrice> price)
{
	sceNp.todo("sceNpCommerceGetSkuPrice(info=*0x%x, price=*0x%x)", info, price);
}

vm::cptr<char> sceNpCommerceGetSkuUserData(vm::ptr<SceNpCommerceProductSkuInfo> info)
{
	sceNp.todo("sceNpCommerceGetSkuUserData(info=*0x%x)", info);
	return vm::null;
}

int sceNpCommerceSetDataFlagStart()
{
		UNIMPLEMENTED_FUNC(sceNp);
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpCommerceGetDataFlagStart()
{
		UNIMPLEMENTED_FUNC(sceNp);
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpCommerceSetDataFlagFinish()
{
		UNIMPLEMENTED_FUNC(sceNp);
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpCommerceGetDataFlagFinish()
{
		UNIMPLEMENTED_FUNC(sceNp);
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpCommerceGetDataFlagState()
{
		UNIMPLEMENTED_FUNC(sceNp);
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpCommerceGetDataFlagAbort()
{
		UNIMPLEMENTED_FUNC(sceNp);
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpCommerceGetChildCategoryInfo(SceNpCommerceProductCategory* pc, uint32_t child_index, SceNpCommerceCategoryInfo* info)
{
	LOG_SCE_TRACE("(pc=*0x%x, child_index=%d, info=*0x%x)", pc, child_index, info);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpCommerceGetChildProductSkuInfo(SceNpCommerceProductCategory* pc, uint32_t child_index, SceNpCommerceProductSkuInfo* info)
{
	LOG_SCE_TRACE("(pc=*0x%x, child_index=%d, info=*0x%x)", pc, child_index, info);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpCommerceDoCheckoutStartAsync(uint32_t ctx_id, vm::cpptr<char> sku_ids, uint32_t sku_num, sys_memory_container_t container, uint32_t* req_id)
{
	LOG_SCE_TRACE("(ctx_id=%d, sku_ids=*0x%x, sku_num=%d, container=%d, req_id=*0x%x)", ctx_id, sku_ids, sku_num, container, req_id);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpCommerceDoCheckoutFinishAsync(uint32_t req_id)
{
	LOG_SCE_TRACE("(req_id=%d)", req_id);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpCustomMenuRegisterActions(const SceNpCustomMenu* menu, SceNpCustomMenuEventHandler* handler, void* userArg, uint64_t options)
{
	LOG_SCE_TRACE("(menu=*0x%x, handler=*0x%x, userArg=*0x%x, options=0x%x)", menu, handler, userArg, options);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_CUSTOM_MENU_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!menu || !handler)
		{
			ret = SCE_NP_CUSTOM_MENU_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpCustomMenuActionSetActivation(const SceNpCustomMenuIndexArray* array, uint64_t options)
{
	LOG_SCE_TRACE("(array=*0x%x, options=0x%x)", array, options);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_CUSTOM_MENU_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!array)
		{
			ret = SCE_NP_CUSTOM_MENU_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpCustomMenuRegisterExceptionList(const SceNpCustomMenuActionExceptions* items, uint32_t numItems, uint64_t options)
{
	LOG_SCE_TRACE("(items=*0x%x, numItems=%d, options=0x%x)", items, numItems, options);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_CUSTOM_MENU_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!items)
		{
			ret = SCE_NP_CUSTOM_MENU_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		if (numItems > SCE_NP_CUSTOM_MENU_ACTION_ITEMS_MAX)
		{
			// TODO: what about SCE_NP_CUSTOM_MENU_ACTION_ITEMS_TOTAL_MAX
			ret = SCE_NP_CUSTOM_MENU_ERROR_EXCEEDS_MAX;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpFriendlist(SceNpFriendlistResultHandler* resultHandler, void* userArg, sys_memory_container_t containerId)
{
	LOG_SCE_TRACE("(resultHandler=*0x%x, userArg=*0x%x, containerId=%d)", resultHandler, userArg, containerId);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_CUSTOM_MENU_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!resultHandler)
		{
			ret = SCE_NP_FRIENDLIST_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpFriendlistCustom(SceNpFriendlistCustomOptions options, SceNpFriendlistResultHandler* resultHandler, void* userArg, sys_memory_container_t containerId)
{
	LOG_SCE_TRACE("(options=0x%x, resultHandler=*0x%x, userArg=*0x%x, containerId=%d)", options, resultHandler, userArg, containerId);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_CUSTOM_MENU_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!resultHandler)
		{
			ret = SCE_NP_FRIENDLIST_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpFriendlistAbortGui()
{
	LOG_SCE_TRACE("()");

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_CUSTOM_MENU_ERROR_NOT_INITIALIZED;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpLookupInit()
{
	LOG_SCE_TRACE("()");

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		const auto lookup_manager = g_fxo->get<sce_np_lookup_manager>();
	
		if (lookup_manager->is_initialized)
		{
			ret = SCE_NP_COMMUNITY_ERROR_ALREADY_INITIALIZED;
			break;
		}
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_ERROR_NOT_INITIALIZED;
			break;
		}
	
		lookup_manager->is_initialized = true;
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpLookupTerm()
{
	LOG_SCE_TRACE("()");

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		const auto lookup_manager = g_fxo->get<sce_np_lookup_manager>();
	
		if (!lookup_manager->is_initialized)
		{
			ret = SCE_NP_COMMUNITY_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_ERROR_NOT_INITIALIZED;
			break;
		}
	
		lookup_manager->is_initialized = false;
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpLookupCreateTitleCtx(const SceNpCommunicationId* communicationId, const SceNpId* selfNpId)
{
	LOG_SCE_TRACE("(communicationId=*0x%x, selfNpId=0x%x)", communicationId, selfNpId);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_lookup_manager>()->is_initialized)
		{
			ret = SCE_NP_COMMUNITY_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!communicationId || !selfNpId)
		{
			ret = SCE_NP_COMMUNITY_ERROR_INSUFFICIENT_ARGUMENT;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpLookupDestroyTitleCtx(const SceNpCommunicationId* communicationId, const SceNpId* selfNpId)
{
	LOG_SCE_TRACE("(communicationId=*0x%x, selfNpId=0x%x)", communicationId, selfNpId);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_lookup_manager>()->is_initialized)
		{
			ret = SCE_NP_COMMUNITY_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!communicationId || !selfNpId)
		{
			ret = SCE_NP_COMMUNITY_ERROR_INSUFFICIENT_ARGUMENT;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpLookupCreateTransactionCtx(int32_t titleCtxId)
{
	LOG_SCE_TRACE("(titleCtxId=%d)", titleCtxId);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_lookup_manager>()->is_initialized)
		{
			ret = SCE_NP_COMMUNITY_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (g_psn_connection_status != SCE_NP_MANAGER_STATUS_ONLINE)
		{
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ONLINE_ID;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpLookupDestroyTransactionCtx(int32_t transId)
{
	LOG_SCE_TRACE("(transId=%d)", transId);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_lookup_manager>()->is_initialized)
		{
			ret = SCE_NP_COMMUNITY_ERROR_NOT_INITIALIZED;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpLookupSetTimeout(int32_t ctxId, usecond_t timeout)
{
	LOG_SCE_TRACE("(ctxId=%d, timeout=%d)", ctxId, timeout);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_lookup_manager>()->is_initialized)
		{
			ret = SCE_NP_COMMUNITY_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (timeout > 10000000) // 10 seconds
		{
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpLookupAbortTransaction(int32_t transId)
{
	LOG_SCE_TRACE("(transId=%d)", transId);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_lookup_manager>()->is_initialized)
		{
			ret = SCE_NP_COMMUNITY_ERROR_NOT_INITIALIZED;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpLookupWaitAsync(int32_t transId, int32_t* result)
{
	LOG_SCE_TRACE("(transId=%d, result=%d)", transId, result);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_lookup_manager>()->is_initialized)
		{
			ret = SCE_NP_COMMUNITY_ERROR_NOT_INITIALIZED;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpLookupPollAsync(int32_t transId, int32_t* result)
{
	LOG_SCE_TRACE("(transId=%d, result=%d)", transId, result);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_lookup_manager>()->is_initialized)
		{
			ret = SCE_NP_COMMUNITY_ERROR_NOT_INITIALIZED;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpLookupNpId(int32_t transId, const SceNpOnlineId* onlineId, SceNpId* npId, void* option)
{
	LOG_SCE_TRACE("(transId=%d, onlineId=*0x%x, npId=*0x%x, option=*0x%x)", transId, onlineId, npId, option);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_lookup_manager>()->is_initialized)
		{
			ret = SCE_NP_COMMUNITY_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!onlineId || !npId)
		{
			ret = SCE_NP_COMMUNITY_ERROR_INSUFFICIENT_ARGUMENT;
			break;
		}
	
		if (option) // option check at least until fw 4.71
		{
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		if (g_psn_connection_status != SCE_NP_MANAGER_STATUS_ONLINE)
		{
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ONLINE_ID;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpLookupNpIdAsync(int32_t transId, SceNpOnlineId* onlineId, SceNpId* npId, int32_t prio, void* option)
{
	LOG_SCE_TRACE("(transId=%d, onlineId=*0x%x, npId=*0x%x, prio=%d, option=*0x%x)", transId, onlineId, npId, prio, option);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_lookup_manager>()->is_initialized)
		{
			ret = SCE_NP_COMMUNITY_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!onlineId || !npId)
		{
			ret = SCE_NP_COMMUNITY_ERROR_INSUFFICIENT_ARGUMENT;
			break;
		}
	
		if (option) // option check at least until fw 4.71
		{
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		if (g_psn_connection_status != SCE_NP_MANAGER_STATUS_ONLINE)
		{
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ONLINE_ID;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpLookupUserProfile(int32_t transId, const SceNpId* npId, SceNpUserInfo* userInfo, SceNpAboutMe* aboutMe, SceNpMyLanguages* languages, SceNpCountryCode* countryCode, SceNpAvatarImage* avatarImage, void* option)
{
	LOG_SCE_TRACE("(transId=%d, npId=*0x%x, userInfo=*0x%x, aboutMe=*0x%x, languages=*0x%x, countryCode=*0x%x, avatarImage=*0x%x, option=*0x%x)", transId, npId, userInfo, aboutMe, languages, countryCode, avatarImage, option);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_lookup_manager>()->is_initialized)
		{
			ret = SCE_NP_COMMUNITY_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!npId)
		{
			ret = SCE_NP_COMMUNITY_ERROR_INSUFFICIENT_ARGUMENT;
			break;
		}
	
		if (option) // option check at least until fw 4.71
		{
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		if (g_psn_connection_status != SCE_NP_MANAGER_STATUS_ONLINE)
		{
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ONLINE_ID;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpLookupUserProfileAsync(int32_t transId, const SceNpId* npId, SceNpUserInfo* userInfo, SceNpAboutMe* aboutMe,
	SceNpMyLanguages* languages, SceNpCountryCode* countryCode, SceNpAvatarImage* avatarImage, int32_t prio, void* option)
{
	LOG_SCE_TRACE("(transId=%d, npId=*0x%x, userInfo=*0x%x, aboutMe=*0x%x, languages=*0x%x, countryCode=*0x%x, avatarImage=*0x%x, prio=%d, option=*0x%x)",
		transId, npId, userInfo, aboutMe, languages, countryCode, avatarImage, prio, option);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_lookup_manager>()->is_initialized)
		{
			ret = SCE_NP_COMMUNITY_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!npId)
		{
			ret = SCE_NP_COMMUNITY_ERROR_INSUFFICIENT_ARGUMENT;
			break;
		}
	
		if (option) // option check at least until fw 4.71
		{
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		if (g_psn_connection_status != SCE_NP_MANAGER_STATUS_ONLINE)
		{
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ONLINE_ID;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpLookupUserProfileWithAvatarSize(int32_t transId, int32_t avatarSizeType, const SceNpId* npId, SceNpUserInfo* userInfo, SceNpAboutMe* aboutMe,
	SceNpMyLanguages* languages, SceNpCountryCode* countryCode, void* avatarImageData, uint64_t avatarImageDataMaxSize, uint64_t* avatarImageDataSize, void* option)
{
	LOG_SCE_TRACE("(transId=%d, avatarSizeType=%d, npId=*0x%x, userInfo=*0x%x, aboutMe=*0x%x, languages=*0x%x, countryCode=*0x%x, avatarImageData=*0x%x, avatarImageDataMaxSize=%d, avatarImageDataSize=*0x%x, option=*0x%x)",
		transId, avatarSizeType, npId, userInfo, aboutMe, languages, countryCode, avatarImageData, avatarImageDataMaxSize, avatarImageDataSize, option);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_lookup_manager>()->is_initialized)
		{
			ret = SCE_NP_COMMUNITY_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!npId)
		{
			ret = SCE_NP_COMMUNITY_ERROR_INSUFFICIENT_ARGUMENT;
			break;
		}
	
		if (option) // option check at least until fw 4.71
		{
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		if (g_psn_connection_status != SCE_NP_MANAGER_STATUS_ONLINE)
		{
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ONLINE_ID;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpLookupUserProfileWithAvatarSizeAsync(int32_t transId, int32_t avatarSizeType, const SceNpId* npId, SceNpUserInfo* userInfo, SceNpAboutMe* aboutMe,
	SceNpMyLanguages* languages, SceNpCountryCode* countryCode, void* avatarImageData, uint64_t avatarImageDataMaxSize, uint64_t* avatarImageDataSize, int32_t prio, void* option)
{
	LOG_SCE_TRACE("(transId=%d, avatarSizeType=%d, npId=*0x%x, userInfo=*0x%x, aboutMe=*0x%x, languages=*0x%x, countryCode=*0x%x, avatarImageData=*0x%x, avatarImageDataMaxSize=%d, avatarImageDataSize=*0x%x, prio=%d, option=*0x%x)",
		transId, avatarSizeType, npId, userInfo, aboutMe, languages, countryCode, avatarImageData, avatarImageDataMaxSize, avatarImageDataSize, prio, option);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_lookup_manager>()->is_initialized)
		{
			ret = SCE_NP_COMMUNITY_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!npId)
		{
			ret = SCE_NP_COMMUNITY_ERROR_INSUFFICIENT_ARGUMENT;
			break;
		}
	
		if (option) // option check at least until fw 4.71
		{
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		if (g_psn_connection_status != SCE_NP_MANAGER_STATUS_ONLINE)
		{
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ONLINE_ID;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpLookupAvatarImage(int32_t transId, SceNpAvatarUrl* avatarUrl, SceNpAvatarImage* avatarImage, void* option)
{
	LOG_SCE_TRACE("(transId=%d, avatarUrl=*0x%x, avatarImage=*0x%x, option=*0x%x)", transId, avatarUrl, avatarImage, option);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_lookup_manager>()->is_initialized)
		{
			ret = SCE_NP_COMMUNITY_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!avatarUrl || !avatarImage)
		{
			ret = SCE_NP_COMMUNITY_ERROR_INSUFFICIENT_ARGUMENT;
			break;
		}
	
		if (option) // option check at least until fw 4.71
		{
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		if (g_psn_connection_status != SCE_NP_MANAGER_STATUS_ONLINE)
		{
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ONLINE_ID;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpLookupAvatarImageAsync(int32_t transId, SceNpAvatarUrl* avatarUrl, SceNpAvatarImage* avatarImage, int32_t prio, void* option)
{
	LOG_SCE_TRACE("(transId=%d, avatarUrl=*0x%x, avatarImage=*0x%x, prio=%d, option=*0x%x)", transId, avatarUrl, avatarImage, prio, option);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_lookup_manager>()->is_initialized)
		{
			ret = SCE_NP_COMMUNITY_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!avatarUrl || !avatarImage)
		{
			ret = SCE_NP_COMMUNITY_ERROR_INSUFFICIENT_ARGUMENT;
			break;
		}
	
		if (option) // option check at least until fw 4.71
		{
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		if (g_psn_connection_status != SCE_NP_MANAGER_STATUS_ONLINE)
		{
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ONLINE_ID;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpLookupTitleStorage()
{
		UNIMPLEMENTED_FUNC(sceNp);
	
		if (!g_fxo->get<sce_np_lookup_manager>()->is_initialized)
		{
			ret = SCE_NP_COMMUNITY_ERROR_NOT_INITIALIZED;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpLookupTitleStorageAsync()
{
		UNIMPLEMENTED_FUNC(sceNp);
	
		if (!g_fxo->get<sce_np_lookup_manager>()->is_initialized)
		{
			ret = SCE_NP_COMMUNITY_ERROR_NOT_INITIALIZED;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpLookupTitleSmallStorage(int32_t transId, void* data, uint64_t maxSize, uint64_t* contentLength, void* option)
{
	LOG_SCE_TRACE("(transId=%d, data=*0x%x, maxSize=%d, contentLength=*0x%x, option=*0x%x)", transId, data, maxSize, contentLength, option);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_lookup_manager>()->is_initialized)
		{
			ret = SCE_NP_COMMUNITY_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!data)
		{
			ret = SCE_NP_COMMUNITY_ERROR_INSUFFICIENT_ARGUMENT;
			break;
		}
	
		if (option) // option check at least until fw 4.71
		{
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		//if (something > maxSize)
		//{
		//	ret = SCE_NP_COMMUNITY_ERROR_BODY_TOO_LARGE;
			break;
		//}
	
		if (g_psn_connection_status != SCE_NP_MANAGER_STATUS_ONLINE)
		{
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ONLINE_ID;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpLookupTitleSmallStorageAsync(int32_t transId, void* data, uint64_t maxSize, uint64_t* contentLength, int32_t prio, void* option)
{
	LOG_SCE_TRACE("(transId=%d, data=*0x%x, maxSize=%d, contentLength=*0x%x, prio=%d, option=*0x%x)", transId, data, maxSize, contentLength, prio, option);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_lookup_manager>()->is_initialized)
		{
			ret = SCE_NP_COMMUNITY_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!data)
		{
			ret = SCE_NP_COMMUNITY_ERROR_INSUFFICIENT_ARGUMENT;
			break;
		}
	
		if (option) // option check at least until fw 4.71
		{
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		//if (something > maxSize)
		//{
		//	ret = SCE_NP_COMMUNITY_ERROR_BODY_TOO_LARGE;
			break;
		//}
	
		if (g_psn_connection_status != SCE_NP_MANAGER_STATUS_ONLINE)
		{
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ONLINE_ID;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpManagerRegisterCallback(SceNpManagerCallback* callback, void* arg)
{
	LOG_SCE_TRACE("(callback=*0x%x, arg=*0x%x)", callback, arg);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!callback)
		{
			ret = SCE_NP_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpManagerUnregisterCallback()
{
	LOG_SCE_TRACE("()");

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_ERROR_NOT_INITIALIZED;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpManagerGetStatus(int32_t* status)
{
	LOG_SCE_TRACE("(status=*0x%x)", status);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!status)
		{
			ret = SCE_NP_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		*status = g_psn_connection_status;
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpManagerGetNetworkTime(CellRtcTick* pTick)
{
	LOG_SCE_TRACE("(pTick=*0x%x)", pTick);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!pTick)
		{
			ret = SCE_NP_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		if (g_psn_connection_status == SCE_NP_MANAGER_STATUS_OFFLINE)
		{
			ret = not_an_error(SCE_NP_ERROR_OFFLINE);
			break;
		}
	
		if (g_psn_connection_status != SCE_NP_MANAGER_STATUS_ONLINE)
		{
			ret = SCE_NP_ERROR_INVALID_STATE;
			break;
		}
	
		// FIXME: Get the network time
		auto now = std::chrono::system_clock::now();
		pTick->tick = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count();
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpManagerGetOnlineId(SceNpOnlineId* onlineId)
{
	LOG_SCE_TRACE("(onlineId=*0x%x)", onlineId);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!onlineId)
		{
			ret = SCE_NP_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		if (g_psn_connection_status == SCE_NP_MANAGER_STATUS_OFFLINE)
		{
			ret = not_an_error(SCE_NP_ERROR_OFFLINE);
			break;
		}
	
		if (g_psn_connection_status != SCE_NP_MANAGER_STATUS_LOGGING_IN && g_psn_connection_status != SCE_NP_MANAGER_STATUS_ONLINE)
		{
			ret = SCE_NP_ERROR_INVALID_STATE;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpManagerGetNpId(ppu_thread& ppu, SceNpId* npId)
{
	LOG_SCE_TRACE("(npId=*0x%x)", npId);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!npId)
		{
			ret = SCE_NP_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		if (g_psn_connection_status == SCE_NP_MANAGER_STATUS_OFFLINE)
		{
			ret = not_an_error(SCE_NP_ERROR_OFFLINE);
			break;
		}
	
		if (g_psn_connection_status != SCE_NP_MANAGER_STATUS_LOGGING_IN && g_psn_connection_status != SCE_NP_MANAGER_STATUS_ONLINE)
		{
			ret = SCE_NP_ERROR_INVALID_STATE;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpManagerGetOnlineName(SceNpOnlineName* onlineName)
{
	LOG_SCE_TRACE("(onlineName=*0x%x)", onlineName);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!onlineName)
		{
			ret = SCE_NP_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		if (g_psn_connection_status == SCE_NP_MANAGER_STATUS_OFFLINE)
		{
			ret = not_an_error(SCE_NP_ERROR_OFFLINE);
			break;
		}
	
		if (g_psn_connection_status != SCE_NP_MANAGER_STATUS_ONLINE)
		{
			ret = SCE_NP_ERROR_INVALID_STATE;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpManagerGetAvatarUrl(SceNpAvatarUrl* avatarUrl)
{
	LOG_SCE_TRACE("(avatarUrl=*0x%x)", avatarUrl);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!avatarUrl)
		{
			ret = SCE_NP_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		if (g_psn_connection_status == SCE_NP_MANAGER_STATUS_OFFLINE)
		{
			ret = not_an_error(SCE_NP_ERROR_OFFLINE);
			break;
		}
	
		if (g_psn_connection_status != SCE_NP_MANAGER_STATUS_ONLINE)
		{
			ret = SCE_NP_ERROR_INVALID_STATE;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpManagerGetMyLanguages(SceNpMyLanguages* myLanguages)
{
	LOG_SCE_TRACE("(myLanguages=*0x%x)", myLanguages);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!myLanguages)
		{
			ret = SCE_NP_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		if (g_psn_connection_status == SCE_NP_MANAGER_STATUS_OFFLINE)
		{
			ret = not_an_error(SCE_NP_ERROR_OFFLINE);
			break;
		}
	
		if (g_psn_connection_status != SCE_NP_MANAGER_STATUS_ONLINE)
		{
			ret = SCE_NP_ERROR_INVALID_STATE;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpManagerGetAccountRegion(SceNpCountryCode* countryCode, int32_t* language)
{
	LOG_SCE_TRACE("(countryCode=*0x%x, language=*0x%x)", countryCode, language);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!countryCode || !language)
		{
			ret = SCE_NP_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		if (g_psn_connection_status == SCE_NP_MANAGER_STATUS_OFFLINE)
		{
			ret = not_an_error(SCE_NP_ERROR_OFFLINE);
			break;
		}
	
		if (g_psn_connection_status != SCE_NP_MANAGER_STATUS_LOGGING_IN && g_psn_connection_status != SCE_NP_MANAGER_STATUS_ONLINE)
		{
			ret = SCE_NP_ERROR_INVALID_STATE;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpManagerGetAccountAge(int32_t* age)
{
	LOG_SCE_TRACE("(age=*0x%x)", age);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!age)
		{
			ret = SCE_NP_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		if (g_psn_connection_status == SCE_NP_MANAGER_STATUS_OFFLINE)
		{
			ret = not_an_error(SCE_NP_ERROR_OFFLINE);
			break;
		}
	
		if (g_psn_connection_status != SCE_NP_MANAGER_STATUS_LOGGING_IN && g_psn_connection_status != SCE_NP_MANAGER_STATUS_ONLINE)
		{
			ret = SCE_NP_ERROR_INVALID_STATE;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpManagerGetContentRatingFlag(int32_t* isRestricted, int32_t* age)
{
	LOG_SCE_TRACE("(isRestricted=*0x%x, age=*0x%x)", isRestricted, age);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!isRestricted || !age)
		{
			ret = SCE_NP_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		if (g_psn_connection_status == SCE_NP_MANAGER_STATUS_OFFLINE)
		{
			ret = not_an_error(SCE_NP_ERROR_OFFLINE);
			break;
		}
	
		if (g_psn_connection_status != SCE_NP_MANAGER_STATUS_LOGGING_IN && g_psn_connection_status != SCE_NP_MANAGER_STATUS_ONLINE)
		{
			ret = SCE_NP_ERROR_INVALID_STATE;
			break;
		}
	
		// TODO: read user's parental control information
		*isRestricted = 0;
		*age = 18;
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpManagerGetChatRestrictionFlag(int32_t* isRestricted)
{
	LOG_SCE_TRACE("(isRestricted=*0x%x)", isRestricted);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!isRestricted)
		{
			ret = SCE_NP_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		if (g_psn_connection_status == SCE_NP_MANAGER_STATUS_OFFLINE)
		{
			ret = not_an_error(SCE_NP_ERROR_OFFLINE);
			break;
		}
	
		if (g_psn_connection_status != SCE_NP_MANAGER_STATUS_LOGGING_IN && g_psn_connection_status != SCE_NP_MANAGER_STATUS_ONLINE)
		{
			ret = SCE_NP_ERROR_INVALID_STATE;
			break;
		}
	
		// TODO: read user's parental control information
		*isRestricted = 0;
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpManagerGetCachedInfo(CellSysutilUserId userId, SceNpManagerCacheParam* param)
{
	LOG_SCE_TRACE("(userId=%d, param=*0x%x)", userId, param);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!param)
		{
			ret = SCE_NP_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpManagerGetPsHandle()
{
		UNIMPLEMENTED_FUNC(sceNp);
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpManagerRequestTicket(const SceNpId* npId, const char* serviceId, const void* cookie, uint32_t cookieSize, const char* entitlementId, uint32_t consumedCount)
{
	LOG_SCE_TRACE("(npId=*0x%x, serviceId=%s, cookie=*0x%x, cookieSize=%d, entitlementId=%s, consumedCount=%d)",
		npId, serviceId, cookie, cookieSize, entitlementId, consumedCount);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!serviceId || !cookie || cookieSize > SCE_NP_COOKIE_MAX_SIZE || !entitlementId)
		{
			ret = SCE_NP_AUTH_EINVALID_ARGUMENT;
			break;
		}
	
		if (g_psn_connection_status == SCE_NP_MANAGER_STATUS_OFFLINE)
		{
			ret = not_an_error(SCE_NP_ERROR_OFFLINE);
			break;
		}
	
		if (g_psn_connection_status != SCE_NP_MANAGER_STATUS_LOGGING_IN && g_psn_connection_status != SCE_NP_MANAGER_STATUS_ONLINE)
		{
			ret = SCE_NP_ERROR_INVALID_STATE;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpManagerRequestTicket2(const SceNpId* npId, const SceNpTicketVersion* version, const char* serviceId,
	const void* cookie, uint32_t cookieSize, const char* entitlementId, uint32_t consumedCount)
{
	LOG_SCE_TRACE("(npId=*0x%x, version=*0x%x, serviceId=%s, cookie=*0x%x, cookieSize=%d, entitlementId=%s, consumedCount=%d)",
		npId, version, serviceId, cookie, cookieSize, entitlementId, consumedCount);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!serviceId || !cookie || cookieSize > SCE_NP_COOKIE_MAX_SIZE || !entitlementId)
		{
			ret = SCE_NP_AUTH_EINVALID_ARGUMENT;
			break;
		}
	
		if (g_psn_connection_status == SCE_NP_MANAGER_STATUS_OFFLINE)
		{
			ret = not_an_error(SCE_NP_ERROR_OFFLINE);
			break;
		}
	
		if (g_psn_connection_status != SCE_NP_MANAGER_STATUS_LOGGING_IN && g_psn_connection_status != SCE_NP_MANAGER_STATUS_ONLINE)
		{
			ret = SCE_NP_ERROR_INVALID_STATE;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpManagerGetTicket(void* buffer, uint32_t* bufferSize)
{
	LOG_SCE_TRACE("(buffer=*0x%x, bufferSize=*0x%x)", buffer, bufferSize);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!bufferSize)
		{
			ret = SCE_NP_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpManagerGetTicketParam(int32_t paramId, SceNpTicketParam* param)
{
	LOG_SCE_TRACE("(paramId=%d, param=*0x%x)", paramId, param);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!param)
		{
			// TODO: check paramId
			ret = SCE_NP_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpManagerGetEntitlementIdList(SceNpEntitlementId* entIdList, uint32_t entIdListNum)
{
	LOG_SCE_TRACE("(entIdList=*0x%x, entIdListNum=%d)", entIdList, entIdListNum);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_ERROR_NOT_INITIALIZED;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpManagerGetEntitlementById(const char* entId, SceNpEntitlement* ent)
{
	LOG_SCE_TRACE("(entId=%s, ent=*0x%x)", entId, ent);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!entId)
		{
			ret = SCE_NP_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpManagerGetSigninId()
{
		UNIMPLEMENTED_FUNC(sceNp);
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpManagerSubSignin(CellSysutilUserId userId, SceNpManagerSubSigninCallback* cb_func, void* cb_arg, int32_t flag)
{
	LOG_SCE_TRACE("(userId=%d, cb_func=*0x%x, cb_arg=*0x%x, flag=%d)", userId, cb_func, cb_arg, flag);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_ERROR_NOT_INITIALIZED;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpManagerSubSigninAbortGui()
{
		UNIMPLEMENTED_FUNC(sceNp);
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpManagerSubSignout(SceNpId* npId)
{
	LOG_SCE_TRACE("(npId=*0x%x)", npId);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_ERROR_NOT_INITIALIZED;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpMatchingCreateCtx(SceNpId* npId, SceNpMatchingHandler* handler, void* arg, uint32_t* ctx_id)
{
	LOG_SCE_TRACE("(npId=*0x%x, handler=*0x%x, arg=*0x%x, ctx_id=*0x%x)", npId, handler, arg, ctx_id);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpMatchingDestroyCtx(uint32_t ctx_id)
{
	LOG_SCE_TRACE("(ctx_id=%d)", ctx_id);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpMatchingGetResult(uint32_t ctx_id, uint32_t req_id, void* buf, uint64_t* size, int32_t* event)
{
	LOG_SCE_TRACE("(ctx_id=%d, req_id=%d, buf=*0x%x, size=*0x%x, event=*0x%x)", ctx_id, req_id, buf, size, event);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpMatchingGetResultGUI(void* buf, uint64_t* size, int32_t* event)
{
	LOG_SCE_TRACE("(buf=*0x%x, size=*0x%x, event=*0x%x)", buf, size, event);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpMatchingSetRoomInfo(uint32_t ctx_id, SceNpLobbyId* lobby_id, SceNpRoomId* room_id, SceNpMatchingAttr* attr, uint32_t* req_id)
{
	LOG_SCE_TRACE("(ctx_id=%d, lobby_id=*0x%x, room_id=*0x%x, attr=*0x%x, req_id=*0x%x)", ctx_id, lobby_id, room_id, attr, req_id);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpMatchingSetRoomInfoNoLimit(uint32_t ctx_id, SceNpLobbyId* lobby_id, SceNpRoomId* room_id, SceNpMatchingAttr* attr, uint32_t* req_id)
{
	LOG_SCE_TRACE("(ctx_id=%d, lobby_id=*0x%x, room_id=*0x%x, attr=*0x%x, req_id=*0x%x)", ctx_id, lobby_id, room_id, attr, req_id);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpMatchingGetRoomInfo(uint32_t ctx_id, SceNpLobbyId* lobby_id, SceNpRoomId* room_id, SceNpMatchingAttr* attr, uint32_t* req_id)
{
	LOG_SCE_TRACE("(ctx_id=%d, lobby_id=*0x%x, room_id=*0x%x, attr=*0x%x, req_id=*0x%x)", ctx_id, lobby_id, room_id, attr, req_id);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpMatchingGetRoomInfoNoLimit(uint32_t ctx_id, SceNpLobbyId* lobby_id, SceNpRoomId* room_id, SceNpMatchingAttr* attr, uint32_t* req_id)
{
	LOG_SCE_TRACE("(ctx_id=%d, lobby_id=*0x%x, room_id=*0x%x, attr=*0x%x, req_id=*0x%x)", ctx_id, lobby_id, room_id, attr, req_id);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpMatchingSetRoomSearchFlag(uint32_t ctx_id, SceNpLobbyId* lobby_id, SceNpRoomId* room_id, int32_t flag, uint32_t* req_id)
{
	LOG_SCE_TRACE("(ctx_id=%d, lobby_id=*0x%x, room_id=*0x%x, flag=%d, req_id=*0x%x)", ctx_id, lobby_id, room_id, flag, req_id);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpMatchingGetRoomSearchFlag(uint32_t ctx_id, SceNpLobbyId* lobby_id, SceNpRoomId* room_id, uint32_t* req_id)
{
	LOG_SCE_TRACE("(ctx_id=%d, lobby_id=*0x%x, room_id=*0x%x, req_id=*0x%x)", ctx_id, lobby_id, room_id, req_id);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpMatchingGetRoomMemberListLocal(uint32_t ctx_id, SceNpRoomId* room_id, uint64_t* buflen, void* buf)
{
	LOG_SCE_TRACE("(ctx_id=%d, room_id=*0x%x, buflen=*0x%x, buf=*0x%x)", ctx_id, room_id, buflen, buf);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpMatchingGetRoomListLimitGUI(uint32_t ctx_id, SceNpCommunicationId* communicationId, SceNpMatchingReqRange* range,
	SceNpMatchingSearchCondition* cond, SceNpMatchingAttr* attr, SceNpMatchingGUIHandler* handler, void* arg)
{
	LOG_SCE_TRACE("(ctx_id=%d, communicationId=*0x%x, range=*0x%x, cond=*0x%x, attr=*0x%x, handler=*0x%x, arg=*0x%x)",
		ctx_id, communicationId, range, cond, attr, handler, arg);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpMatchingKickRoomMember(uint32_t ctx_id, const SceNpRoomId* room_id, const SceNpId* user_id, uint32_t* req_id)
{
	LOG_SCE_TRACE("(ctx_id=%d, room_id=*0x%x, user_id=*0x%x, req_id=*0x%x)", ctx_id, room_id, user_id, req_id);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpMatchingKickRoomMemberWithOpt(uint32_t ctx_id, const SceNpRoomId* room_id, const SceNpId* user_id, const void* opt, int32_t opt_len, uint32_t* req_id)
{
	LOG_SCE_TRACE("(ctx_id=%d, room_id=*0x%x, user_id=*0x%x, opt=*0x%x, opt_len=%d, req_id=*0x%x)", ctx_id, room_id, user_id, opt, opt_len, req_id);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpMatchingQuickMatchGUI(uint32_t ctx_id, const SceNpCommunicationId* communicationId, const SceNpMatchingSearchCondition* cond,
	int32_t available_num, int32_t timeout, SceNpMatchingGUIHandler* handler, void* arg)
{
	LOG_SCE_TRACE("(ctx_id=%d, communicationId=*0x%x, cond=*0x%x, available_num=%d, timeout=%d, handler=*0x%x, arg=*0x%x)",
		ctx_id, communicationId, cond, available_num, timeout, handler, arg);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpMatchingSendInvitationGUI(uint32_t ctx_id, const SceNpRoomId* room_id, const SceNpCommunicationId* communicationId, const SceNpId* dsts, int32_t num,
	int32_t slot_type, const char* subject, const char* body, sys_memory_container_t container, SceNpMatchingGUIHandler* handler, void* arg)
{
	LOG_SCE_TRACE("(ctx_id=%d, room_id=*0x%x, communicationId=*0x%x, dsts=*0x%x, num=%d, slot_type=%d, subject=%s, body=%s, container=%d, handler=*0x%x, arg=*0x%x)",
		ctx_id, room_id, communicationId, dsts, num, slot_type, subject, body, container, handler, arg);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpMatchingAcceptInvitationGUI(uint32_t ctx_id, const SceNpCommunicationId* communicationId, sys_memory_container_t container, SceNpMatchingGUIHandler* handler, void* arg)
{
	LOG_SCE_TRACE("(ctx_id=%d, communicationId=*0x%x, container=%d, handler=*0x%x, arg=*0x%x)", ctx_id, communicationId, container, handler, arg);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpMatchingCreateRoomGUI(uint32_t ctx_id, const SceNpCommunicationId* communicationId, const SceNpMatchingAttr* attr, SceNpMatchingGUIHandler* handler, void* arg)
{
	LOG_SCE_TRACE("(ctx_id=%d, communicationId=*0x%x, attr=*0x%x, handler=*0x%x, arg=*0x%x)", ctx_id, communicationId, attr, handler, arg);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpMatchingJoinRoomGUI(uint32_t ctx_id, SceNpRoomId* room_id, SceNpMatchingGUIHandler* handler, void* arg)
{
	LOG_SCE_TRACE("(ctx_id=%d, room_id=*0x%x, handler=*0x%x, arg=*0x%x)", ctx_id, room_id, handler, arg);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpMatchingLeaveRoom(uint32_t ctx_id, const SceNpRoomId* room_id, uint32_t* req_id)
{
	LOG_SCE_TRACE("(ctx_id=%d, room_id=*0x%x, req_id=*0x%x)", ctx_id, room_id, req_id);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpMatchingSearchJoinRoomGUI(uint32_t ctx_id, const SceNpCommunicationId* communicationId, const SceNpMatchingSearchCondition* cond,
	const SceNpMatchingAttr* attr, SceNpMatchingGUIHandler* handler, void* arg)
{
	LOG_SCE_TRACE("(ctx_id=%d, communicationId=*0x%x, cond=*0x%x, attr=*0x%x, handler=*0x%x, arg=*0x%x)",
		ctx_id, communicationId, cond, attr, handler, arg);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpMatchingGrantOwnership(uint32_t ctx_id, const SceNpRoomId* room_id, const SceNpId* user_id, uint32_t* req_id)
{
	LOG_SCE_TRACE("(ctx_id=%d, room_id=*0x%x, user_id=*0x%x, req_id=*0x%x)", ctx_id, room_id, user_id, req_id);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpProfileCallGui(const SceNpId* npid, SceNpProfileResultHandler* handler, void* userArg, uint64_t options)
{
	LOG_SCE_TRACE("(npid=*0x%x, handler=*0x%x, userArg=*0x%x, options=0x%x)", npid, handler, userArg, options);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!handler)
		{
			ret = SCE_NP_PROFILE_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpProfileAbortGui()
{
	LOG_SCE_TRACE("()");

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_ERROR_NOT_INITIALIZED;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpScoreInit()
{
	LOG_SCE_TRACE("()");

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		const auto score_manager = g_fxo->get<sce_np_score_manager>();
	
		if (score_manager->is_initialized)
		{
			ret = SCE_NP_COMMUNITY_ERROR_ALREADY_INITIALIZED;
			break;
		}
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_ERROR_NOT_INITIALIZED;
			break;
		}
	
		score_manager->is_initialized = true;
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpScoreTerm()
{
	LOG_SCE_TRACE("()");

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		const auto score_manager = g_fxo->get<sce_np_score_manager>();
	
		if (!score_manager->is_initialized)
		{
			ret = SCE_NP_COMMUNITY_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_ERROR_NOT_INITIALIZED;
			break;
		}
	
		score_manager->is_initialized = false;
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpScoreCreateTitleCtx(const SceNpCommunicationId* communicationId, const SceNpCommunicationPassphrase* passphrase, const SceNpId* selfNpId)
{
	LOG_SCE_TRACE("(communicationId=*0x%x, passphrase=*0x%x, selfNpId=*0x%x)", communicationId, passphrase, selfNpId);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_score_manager>()->is_initialized)
		{
			ret = SCE_NP_COMMUNITY_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!communicationId || !passphrase || !selfNpId)
		{
			ret = SCE_NP_COMMUNITY_ERROR_INSUFFICIENT_ARGUMENT;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpScoreDestroyTitleCtx(int32_t titleCtxId)
{
	LOG_SCE_TRACE("(titleCtxId=%d)", titleCtxId);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_score_manager>()->is_initialized)
		{
			ret = SCE_NP_COMMUNITY_ERROR_NOT_INITIALIZED;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpScoreCreateTransactionCtx(int32_t titleCtxId)
{
	LOG_SCE_TRACE("(titleCtxId=%d)", titleCtxId);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_score_manager>()->is_initialized)
		{
			ret = SCE_NP_COMMUNITY_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (g_psn_connection_status == SCE_NP_MANAGER_STATUS_OFFLINE)
		{
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ONLINE_ID;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpScoreDestroyTransactionCtx(int32_t transId)
{
	LOG_SCE_TRACE("(transId=%d)", transId);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_score_manager>()->is_initialized)
		{
			ret = SCE_NP_COMMUNITY_ERROR_NOT_INITIALIZED;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpScoreSetTimeout(int32_t ctxId, usecond_t timeout)
{
	LOG_SCE_TRACE("(ctxId=%d, timeout=%d)", ctxId, timeout);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_score_manager>()->is_initialized)
		{
			ret = SCE_NP_COMMUNITY_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (timeout > 10000000) // 10 seconds
		{
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpScoreSetPlayerCharacterId(int32_t ctxId, SceNpScorePcId pcId)
{
	LOG_SCE_TRACE("(ctxId=%d, pcId=%d)", ctxId, pcId);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_score_manager>()->is_initialized)
		{
			ret = SCE_NP_COMMUNITY_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (pcId < 0)
		{
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpScoreWaitAsync(int32_t transId, int32_t* result)
{
	LOG_SCE_TRACE("(transId=%d, result=*0x%x)", transId, result);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_score_manager>()->is_initialized)
		{
			ret = SCE_NP_COMMUNITY_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (transId <= 0)
		{
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ID;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpScorePollAsync(int32_t transId, int32_t* result)
{
	LOG_SCE_TRACE("(transId=%d, result=*0x%x)", transId, result);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_score_manager>()->is_initialized)
		{
			ret = SCE_NP_COMMUNITY_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (transId <= 0)
		{
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ID;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpScoreGetBoardInfo(int32_t transId, SceNpScoreBoardId boardId, SceNpScoreBoardInfo* boardInfo, void* option)
{
	LOG_SCE_TRACE("(transId=%d, boardId=%d, boardInfo=*0x%x, option=*0x%x)", transId, boardId, boardInfo, option);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_score_manager>()->is_initialized)
		{
			ret = SCE_NP_COMMUNITY_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!boardInfo)
		{
			ret = SCE_NP_COMMUNITY_ERROR_INSUFFICIENT_ARGUMENT;
			break;
		}
	
		if (option) // option check at least until fw 4.71
		{
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		if (g_psn_connection_status != SCE_NP_MANAGER_STATUS_ONLINE)
		{
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ONLINE_ID;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpScoreGetBoardInfoAsync(int32_t transId, SceNpScoreBoardId boardId, SceNpScoreBoardInfo* boardInfo, int32_t prio, void* option)
{
	LOG_SCE_TRACE("(transId=%d, boardId=%d, boardInfo=*0x%x, prio=%d, option=*0x%x)", transId, boardId, boardInfo, prio, option);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_score_manager>()->is_initialized)
		{
			ret = SCE_NP_COMMUNITY_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (option) // option check at least until fw 4.71
		{
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpScoreRecordScore(int32_t transId, SceNpScoreBoardId boardId, SceNpScoreValue score, const SceNpScoreComment* scoreComment,
	const SceNpScoreGameInfo* gameInfo, SceNpScoreRankNumber* tmpRank, SceNpScoreRecordOptParam* option)
{
	LOG_SCE_TRACE("(transId=%d, boardId=%d, score=%d, scoreComment=*0x%x, gameInfo=*0x%x, tmpRank=*0x%x, option=*0x%x)",
		transId, boardId, score, scoreComment, gameInfo, tmpRank, option);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_score_manager>()->is_initialized)
		{
			ret = SCE_NP_COMMUNITY_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (option) // option check at least until fw 4.71
		{
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		if (g_psn_connection_status != SCE_NP_MANAGER_STATUS_ONLINE)
		{
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ONLINE_ID;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpScoreRecordScoreAsync(int32_t transId, SceNpScoreBoardId boardId, SceNpScoreValue score, const SceNpScoreComment* scoreComment,
	const SceNpScoreGameInfo* gameInfo, SceNpScoreRankNumber* tmpRank, int32_t prio, SceNpScoreRecordOptParam* option)
{
	LOG_SCE_TRACE("(transId=%d, boardId=%d, score=%d, scoreComment=*0x%x, gameInfo=*0x%x, tmpRank=*0x%x, prio=%d, option=*0x%x)",
		transId, boardId, score, scoreComment, gameInfo, tmpRank, prio, option);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_score_manager>()->is_initialized)
		{
			ret = SCE_NP_COMMUNITY_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (option) // option check at least until fw 4.71
		{
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpScoreRecordGameData(int32_t transId, SceNpScoreBoardId boardId, SceNpScoreValue score, uint64_t totalSize, uint64_t sendSize, const void* data, void* option)
{
	LOG_SCE_TRACE("(transId=%d, boardId=%d, score=%d, totalSize=%d, sendSize=%d, data=*0x%x, option=*0x%x)",
		transId, boardId, score, totalSize, sendSize, data, option);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_score_manager>()->is_initialized)
		{
			ret = SCE_NP_COMMUNITY_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!data)
		{
			ret = SCE_NP_COMMUNITY_ERROR_INSUFFICIENT_ARGUMENT;
			break;
		}
	
		if (option) // option check at least until fw 4.71
		{
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		if (g_psn_connection_status != SCE_NP_MANAGER_STATUS_ONLINE)
		{
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ONLINE_ID;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpScoreRecordGameDataAsync(int32_t transId, SceNpScoreBoardId boardId, SceNpScoreValue score, uint64_t totalSize, uint64_t sendSize, const void* data, int32_t prio, void* option)
{
	LOG_SCE_TRACE("(transId=%d, boardId=%d, score=%d, totalSize=%d, sendSize=%d, data=*0x%x, prio=%d, option=*0x%x)",
		transId, boardId, score, totalSize, sendSize, data, prio, option);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_score_manager>()->is_initialized)
		{
			ret = SCE_NP_COMMUNITY_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (option) // option check at least until fw 4.71
		{
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpScoreGetGameData(int32_t transId, SceNpScoreBoardId boardId, const SceNpId* npId, uint64_t* totalSize, uint64_t recvSize, void* data, void* option)
{
	LOG_SCE_TRACE("(transId=%d, boardId=%d, npId=*0x%x, totalSize=*0x%x, recvSize=%d, data=*0x%x, option=*0x%x)",
		transId, boardId, npId, totalSize, recvSize, data, option);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_score_manager>()->is_initialized)
		{
			ret = SCE_NP_COMMUNITY_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!npId || !totalSize || !data)
		{
			ret = SCE_NP_COMMUNITY_ERROR_INSUFFICIENT_ARGUMENT;
			break;
		}
	
		if (option) // option check at least until fw 4.71
		{
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		if (g_psn_connection_status != SCE_NP_MANAGER_STATUS_ONLINE)
		{
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ONLINE_ID;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpScoreGetGameDataAsync(int32_t transId, SceNpScoreBoardId boardId, const SceNpId* npId, uint64_t* totalSize, uint64_t recvSize, void* data, int32_t prio, void* option)
{
	LOG_SCE_TRACE("(transId=%d, boardId=%d, npId=*0x%x, totalSize=*0x%x, recvSize=%d, data=*0x%x, prio=%d, option=*0x%x)",
		transId, boardId, npId, totalSize, recvSize, data, prio, option);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_score_manager>()->is_initialized)
		{
			ret = SCE_NP_COMMUNITY_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (option) // option check at least until fw 4.71
		{
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpScoreGetRankingByNpId(int32_t transId, SceNpScoreBoardId boardId, const SceNpId* npIdArray, uint64_t npIdArraySize, SceNpScorePlayerRankData* rankArray,
	uint64_t rankArraySize, SceNpScoreComment* commentArray, uint64_t commentArraySize, void* infoArray, uint64_t infoArraySize,
	uint64_t arrayNum, CellRtcTick* lastSortDate, SceNpScoreRankNumber* totalRecord, void* option)
{
	LOG_SCE_TRACE("(transId=%d, boardId=%d, npIdArray=*0x%x, npIdArraySize=%d, rankArray=*0x%x, rankArraySize=%d, commentArray=*0x%x, commentArraySize=%d, infoArray=*0x%x, infoArraySize=%d, arrayNum=%d, lastSortDate=*0x%x, totalRecord=*0x%x, option=*0x%x)",
		transId, boardId, npIdArray, npIdArraySize, rankArray, rankArraySize, commentArray, commentArraySize, infoArray, infoArraySize, arrayNum, lastSortDate, totalRecord, option);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_score_manager>()->is_initialized)
		{
			ret = SCE_NP_COMMUNITY_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!npIdArray || !rankArray || !totalRecord || !lastSortDate || !arrayNum)
		{
			ret = SCE_NP_COMMUNITY_ERROR_INSUFFICIENT_ARGUMENT;
			break;
		}
	
		if (option) // option check at least until fw 4.71
		{
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		if (arrayNum > SCE_NP_SCORE_MAX_NPID_NUM_PER_TRANS)
		{
			ret = SCE_NP_COMMUNITY_ERROR_TOO_MANY_NPID;
			break;
		}
	
		if (g_psn_connection_status != SCE_NP_MANAGER_STATUS_ONLINE)
		{
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ONLINE_ID;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpScoreGetRankingByNpIdAsync(int32_t transId, SceNpScoreBoardId boardId, const SceNpId* npIdArray, uint64_t npIdArraySize, SceNpScorePlayerRankData* rankArray,
	uint64_t rankArraySize, SceNpScoreComment* commentArray, uint64_t commentArraySize, void* infoArray, uint64_t infoArraySize,
	uint64_t arrayNum, CellRtcTick* lastSortDate, SceNpScoreRankNumber* totalRecord, int32_t prio, void* option)
{
	LOG_SCE_TRACE("(transId=%d, boardId=%d, npIdArray=*0x%x, npIdArraySize=%d, rankArray=*0x%x, rankArraySize=%d, commentArray=*0x%x, commentArraySize=%d, infoArray=*0x%x, infoArraySize=%d, arrayNum=%d, lastSortDate=*0x%x, totalRecord=*0x%x, prio=%d, option=*0x%x)",
		transId, boardId, npIdArray, npIdArraySize, rankArray, rankArraySize, commentArray, commentArraySize, infoArray, infoArraySize, arrayNum, lastSortDate, totalRecord, prio, option);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_score_manager>()->is_initialized)
		{
			ret = SCE_NP_COMMUNITY_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!npIdArray || !arrayNum)
		{
			ret = SCE_NP_COMMUNITY_ERROR_INSUFFICIENT_ARGUMENT;
			break;
		}
	
		if (option) // option check at least until fw 4.71
		{
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpScoreGetRankingByRange(int32_t transId, SceNpScoreBoardId boardId, SceNpScoreRankNumber startSerialRank, SceNpScoreRankData* rankArray,
	uint64_t rankArraySize, SceNpScoreComment* commentArray, uint64_t commentArraySize, void* infoArray, uint64_t infoArraySize,
	uint64_t arrayNum, CellRtcTick* lastSortDate, SceNpScoreRankNumber* totalRecord, void* option)
{
	LOG_SCE_TRACE("(transId=%d, boardId=%d, startSerialRank=%d, rankArray=*0x%x, rankArraySize=%d, commentArray=*0x%x, commentArraySize=%d, infoArray=*0x%x, infoArraySize=%d, arrayNum=%d, lastSortDate=*0x%x, totalRecord=*0x%x, option=*0x%x)",
		transId, boardId, startSerialRank, rankArray, rankArraySize, commentArray, commentArraySize, infoArray, infoArraySize, arrayNum, lastSortDate, totalRecord, option);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_score_manager>()->is_initialized)
		{
			ret = SCE_NP_COMMUNITY_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!rankArray || !totalRecord || !lastSortDate || !arrayNum)
		{
			ret = SCE_NP_COMMUNITY_ERROR_INSUFFICIENT_ARGUMENT;
			break;
		}
	
		if (!startSerialRank || option) // option check at least until fw 4.71
		{
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		if (arrayNum > SCE_NP_SCORE_MAX_RANGE_NUM_PER_TRANS)
		{
			ret = SCE_NP_COMMUNITY_ERROR_TOO_LARGE_RANGE;
			break;
		}
	
		if (g_psn_connection_status != SCE_NP_MANAGER_STATUS_ONLINE)
		{
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ONLINE_ID;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpScoreGetRankingByRangeAsync(int32_t transId, SceNpScoreBoardId boardId, SceNpScoreRankNumber startSerialRank, SceNpScoreRankData* rankArray,
	uint64_t rankArraySize, SceNpScoreComment* commentArray, uint64_t commentArraySize, void* infoArray, uint64_t infoArraySize,
	uint64_t arrayNum, CellRtcTick* lastSortDate, SceNpScoreRankNumber* totalRecord, int32_t prio, void* option)
{
	LOG_SCE_TRACE("(transId=%d, boardId=%d, startSerialRank=%d, rankArray=*0x%x, rankArraySize=%d, commentArray=*0x%x, commentArraySize=%d, infoArray=*0x%x, infoArraySize=%d, arrayNum=%d, lastSortDate=*0x%x, totalRecord=*0x%x, prio=%d, option=*0x%x)",
		transId, boardId, startSerialRank, rankArray, rankArraySize, commentArray, commentArraySize, infoArray, infoArraySize, arrayNum, lastSortDate, totalRecord, prio, option);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_score_manager>()->is_initialized)
		{
			ret = SCE_NP_COMMUNITY_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!arrayNum)
		{
			ret = SCE_NP_COMMUNITY_ERROR_INSUFFICIENT_ARGUMENT;
			break;
		}
	
		if (!startSerialRank || option) // option check at least until fw 4.71
		{
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpScoreGetFriendsRanking(int32_t transId, SceNpScoreBoardId boardId, int32_t includeSelf, SceNpScoreRankData* rankArray, uint64_t rankArraySize,
	SceNpScoreComment* commentArray, uint64_t commentArraySize, SceNpScoreGameInfo* infoArray, uint64_t infoArraySize, uint64_t arrayNum,
	CellRtcTick* lastSortDate, SceNpScoreRankNumber* totalRecord, void* option)
{
	LOG_SCE_TRACE("(transId=%d, boardId=%d, includeSelf=%d, rankArray=*0x%x, rankArraySize=%d, commentArray=*0x%x, commentArraySize=%d, infoArray=*0x%x, infoArraySize=%d, arrayNum=%d, lastSortDate=*0x%x, totalRecord=*0x%x, option=*0x%x)",
		transId, boardId, includeSelf, rankArray, rankArraySize, commentArray, commentArraySize, infoArray, infoArraySize, arrayNum, lastSortDate, totalRecord, option);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_score_manager>()->is_initialized)
		{
			ret = SCE_NP_COMMUNITY_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!rankArray || !arrayNum)
		{
			ret = SCE_NP_COMMUNITY_ERROR_INSUFFICIENT_ARGUMENT;
			break;
		}
	
		if (arrayNum > SCE_NP_SCORE_MAX_SELECTED_FRIENDS_NUM || option) // option check at least until fw 4.71
		{
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpScoreGetFriendsRankingAsync(int32_t transId, SceNpScoreBoardId boardId, int32_t includeSelf, SceNpScoreRankData* rankArray, uint64_t rankArraySize,
	SceNpScoreComment* commentArray, uint64_t commentArraySize, SceNpScoreGameInfo* infoArray, uint64_t infoArraySize, uint64_t arrayNum,
	CellRtcTick* lastSortDate, SceNpScoreRankNumber* totalRecord, int32_t prio, void* option)
{
	LOG_SCE_TRACE("(transId=%d, boardId=%d, includeSelf=%d, rankArray=*0x%x, rankArraySize=%d, commentArray=*0x%x, commentArraySize=%d, infoArray=*0x%x, infoArraySize=%d, arrayNum=%d, lastSortDate=*0x%x, totalRecord=*0x%x, prio=%d, option=*0x%x)",
		transId, boardId, includeSelf, rankArray, rankArraySize, commentArray, commentArraySize, infoArray, infoArraySize, arrayNum, lastSortDate, totalRecord, prio, option);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_score_manager>()->is_initialized)
		{
			ret = SCE_NP_COMMUNITY_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!rankArray || !arrayNum)
		{
			ret = SCE_NP_COMMUNITY_ERROR_INSUFFICIENT_ARGUMENT;
			break;
		}
	
		if (arrayNum > SCE_NP_SCORE_MAX_SELECTED_FRIENDS_NUM || option) // option check at least until fw 4.71
		{
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpScoreCensorComment(int32_t transId, const char* comment, void* option)
{
	LOG_SCE_TRACE("(transId=%d, comment=%s, option=*0x%x)", transId, comment, option);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_score_manager>()->is_initialized)
		{
			ret = SCE_NP_COMMUNITY_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!comment)
		{
			ret = SCE_NP_COMMUNITY_ERROR_INSUFFICIENT_ARGUMENT;
			break;
		}
	
		if (strlen(comment.get_ptr()) > SCE_NP_SCORE_CENSOR_COMMENT_MAXLEN || option) // option check at least until fw 4.71
		{
			// TODO: is SCE_NP_SCORE_CENSOR_COMMENT_MAXLEN + 1 allowed ?
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		if (g_psn_connection_status != SCE_NP_MANAGER_STATUS_ONLINE)
		{
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ONLINE_ID;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpScoreCensorCommentAsync(int32_t transId, const char* comment, int32_t prio, void* option)
{
	LOG_SCE_TRACE("(transId=%d, comment=%s, prio=%d, option=*0x%x)", transId, comment, prio, option);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_score_manager>()->is_initialized)
		{
			ret = SCE_NP_COMMUNITY_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!comment)
		{
			ret = SCE_NP_COMMUNITY_ERROR_INSUFFICIENT_ARGUMENT;
			break;
		}
	
		if (strlen(comment.get_ptr()) > SCE_NP_SCORE_CENSOR_COMMENT_MAXLEN || option) // option check at least until fw 4.71
		{
			// TODO: is SCE_NP_SCORE_CENSOR_COMMENT_MAXLEN + 1 allowed ?
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpScoreSanitizeComment(int32_t transId, const char* comment, char* sanitizedComment, void* option)
{
	LOG_SCE_TRACE("(transId=%d, comment=%s, sanitizedComment=*0x%x, option=*0x%x)", transId, comment, sanitizedComment, option);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_score_manager>()->is_initialized)
		{
			ret = SCE_NP_COMMUNITY_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!comment)
		{
			ret = SCE_NP_COMMUNITY_ERROR_INSUFFICIENT_ARGUMENT;
			break;
		}
	
		if (strlen(comment.get_ptr()) > SCE_NP_SCORE_CENSOR_COMMENT_MAXLEN || option) // option check at least until fw 4.71
		{
			// TODO: is SCE_NP_SCORE_CENSOR_COMMENT_MAXLEN + 1 allowed ?
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		if (g_psn_connection_status != SCE_NP_MANAGER_STATUS_ONLINE)
		{
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ONLINE_ID;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpScoreSanitizeCommentAsync(int32_t transId, const char* comment, char* sanitizedComment, int32_t prio, void* option)
{
	LOG_SCE_TRACE("(transId=%d, comment=%s, sanitizedComment=*0x%x, prio=%d, option=*0x%x)", transId, comment, sanitizedComment, prio, option);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_score_manager>()->is_initialized)
		{
			ret = SCE_NP_COMMUNITY_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!comment)
		{
			ret = SCE_NP_COMMUNITY_ERROR_INSUFFICIENT_ARGUMENT;
			break;
		}
	
		if (strlen(comment.get_ptr()) > SCE_NP_SCORE_CENSOR_COMMENT_MAXLEN || option) // option check at least until fw 4.71
		{
			// TODO: is SCE_NP_SCORE_CENSOR_COMMENT_MAXLEN + 1 allowed ?
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpScoreGetRankingByNpIdPcId(int32_t transId, SceNpScoreBoardId boardId, const SceNpScoreNpIdPcId* idArray, uint64_t idArraySize, SceNpScorePlayerRankData* rankArray,
	uint64_t rankArraySize, SceNpScoreComment* commentArray, uint64_t commentArraySize, void* infoArray, uint64_t infoArraySize,
	uint64_t arrayNum, CellRtcTick* lastSortDate, SceNpScoreRankNumber* totalRecord, void* option)
{
	LOG_SCE_TRACE("(transId=%d, boardId=%d, idArray=*0x%x, idArraySize=%d, rankArray=*0x%x, rankArraySize=%d, commentArray=*0x%x, commentArraySize=%d, infoArray=*0x%x, infoArraySize=%d, arrayNum=%d, lastSortDate=*0x%x, totalRecord=*0x%x, option=*0x%x)",
		transId, boardId, idArray, idArraySize, rankArray, rankArraySize, commentArray, commentArraySize, infoArray, infoArraySize, arrayNum, lastSortDate, totalRecord, option);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_score_manager>()->is_initialized)
		{
			ret = SCE_NP_COMMUNITY_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!idArray || !rankArray || !totalRecord || !lastSortDate || !arrayNum)
		{
			ret = SCE_NP_COMMUNITY_ERROR_INSUFFICIENT_ARGUMENT;
			break;
		}
	
		if (option) // option check at least until fw 4.71
		{
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		if (arrayNum > SCE_NP_SCORE_MAX_NPID_NUM_PER_TRANS)
		{
			ret = SCE_NP_COMMUNITY_ERROR_TOO_MANY_NPID;
			break;
		}
	
		if (g_psn_connection_status != SCE_NP_MANAGER_STATUS_ONLINE)
		{
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ONLINE_ID;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpScoreGetRankingByNpIdPcIdAsync(int32_t transId, SceNpScoreBoardId boardId, const SceNpScoreNpIdPcId* idArray, uint64_t idArraySize, SceNpScorePlayerRankData* rankArray,
	uint64_t rankArraySize, SceNpScoreComment* commentArray, uint64_t commentArraySize, void* infoArray, uint64_t infoArraySize,
	uint64_t arrayNum, CellRtcTick* lastSortDate, SceNpScoreRankNumber* totalRecord, int32_t prio, void* option)
{
	LOG_SCE_TRACE("(transId=%d, boardId=%d, idArray=*0x%x, idArraySize=%d, rankArray=*0x%x, rankArraySize=%d, commentArray=*0x%x, commentArraySize=%d, infoArray=*0x%x, infoArraySize=%d, arrayNum=%d, lastSortDate=*0x%x, totalRecord=*0x%x, prio=%d, option=*0x%x)",
		transId, boardId, idArray, idArraySize, rankArray, rankArraySize, commentArray, commentArraySize, infoArray, infoArraySize, arrayNum, lastSortDate, totalRecord, prio, option);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_score_manager>()->is_initialized)
		{
			ret = SCE_NP_COMMUNITY_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!idArray || !arrayNum)
		{
			ret = SCE_NP_COMMUNITY_ERROR_INSUFFICIENT_ARGUMENT;
			break;
		}
	
		if (option) // option check at least until fw 4.71
		{
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpScoreAbortTransaction(int32_t transId)
{
	LOG_SCE_TRACE("(transId=%d)", transId);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_score_manager>()->is_initialized)
		{
			ret = SCE_NP_COMMUNITY_ERROR_NOT_INITIALIZED;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpScoreGetClansMembersRankingByNpId(int32_t transId, SceNpClanId clanId, SceNpScoreBoardId boardId, const SceNpId* idArray, uint64_t idArraySize, SceNpScorePlayerRankData* rankArray,
	uint64_t rankArraySize, SceNpScoreComment* commentArray, uint64_t commentArraySize, SceNpScoreGameInfo* infoArray, uint64_t infoArraySize,
	SceNpScoreClansMemberDescription* descriptArray, uint64_t descriptArraySize, uint64_t arrayNum, SceNpScoreClanBasicInfo* clanInfo, CellRtcTick* lastSortDate,
	SceNpScoreRankNumber* totalRecord, void* option)
{
	LOG_SCE_TRACE("(transId=%d, clanId=%d, boardId=%d, idArray=*0x%x, idArraySize=%d, rankArray=*0x%x, rankArraySize=%d, commentArray=*0x%x, commentArraySize=%d, infoArray=*0x%x, infoArraySize=%d, descriptArray=*0x%x, descriptArraySize=%d, arrayNum=%d, clanInfo=*0x%x, lastSortDate=*0x%x, totalRecord=*0x%x, option=*0x%x)",
		transId, clanId, boardId, idArray, idArraySize, rankArray, rankArraySize, commentArray, commentArraySize, infoArray, infoArraySize, descriptArray, descriptArraySize, arrayNum, clanInfo, lastSortDate, totalRecord, option);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_score_manager>()->is_initialized)
		{
			ret = SCE_NP_COMMUNITY_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!idArray || !rankArray || !totalRecord || !lastSortDate || !arrayNum)
		{
			ret = SCE_NP_COMMUNITY_ERROR_INSUFFICIENT_ARGUMENT;
			break;
		}
	
		if (option) // option check at least until fw 4.71
		{
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		if (arrayNum > SCE_NP_SCORE_MAX_NPID_NUM_PER_TRANS)
		{
			ret = SCE_NP_COMMUNITY_ERROR_TOO_MANY_NPID;
			break;
		}
	
		if (g_psn_connection_status != SCE_NP_MANAGER_STATUS_ONLINE)
		{
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ONLINE_ID;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpScoreGetClansMembersRankingByNpIdAsync(int32_t transId, SceNpClanId clanId, SceNpScoreBoardId boardId, const SceNpId* idArray, uint64_t idArraySize, SceNpScorePlayerRankData* rankArray,
	uint64_t rankArraySize, SceNpScoreComment* commentArray, uint64_t commentArraySize, SceNpScoreGameInfo* infoArray, uint64_t infoArraySize,
	SceNpScoreClansMemberDescription* descriptArray, uint64_t descriptArraySize, uint64_t arrayNum, SceNpScoreClanBasicInfo* clanInfo, CellRtcTick* lastSortDate,
	SceNpScoreRankNumber* totalRecord, int32_t prio, void* option)
{
	LOG_SCE_TRACE("(transId=%d, clanId=%d, boardId=%d, idArray=*0x%x, idArraySize=%d, rankArray=*0x%x, rankArraySize=%d, commentArray=*0x%x, commentArraySize=%d, infoArray=*0x%x, infoArraySize=%d, descriptArray=*0x%x, descriptArraySize=%d, arrayNum=%d, clanInfo=*0x%x, lastSortDate=*0x%x, totalRecord=*0x%x, prio=%d, option=*0x%x)",
		transId, clanId, boardId, idArray, idArraySize, rankArray, rankArraySize, commentArray, commentArraySize, infoArray, infoArraySize, descriptArray, descriptArraySize, arrayNum, clanInfo, lastSortDate, totalRecord, prio, option);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_score_manager>()->is_initialized)
		{
			ret = SCE_NP_COMMUNITY_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!idArray || !arrayNum)
		{
			ret = SCE_NP_COMMUNITY_ERROR_INSUFFICIENT_ARGUMENT;
			break;
		}
	
		if (option) // option check at least until fw 4.71
		{
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpScoreGetClansMembersRankingByNpIdPcId(int32_t transId, SceNpClanId clanId, SceNpScoreBoardId boardId, const SceNpId* idArray, uint64_t idArraySize, SceNpScorePlayerRankData* rankArray,
	uint64_t rankArraySize, SceNpScoreComment* commentArray, uint64_t commentArraySize, SceNpScoreGameInfo* infoArray, uint64_t infoArraySize,
	SceNpScoreClansMemberDescription* descriptArray, uint64_t descriptArraySize, uint64_t arrayNum, SceNpScoreClanBasicInfo* clanInfo, CellRtcTick* lastSortDate,
	SceNpScoreRankNumber* totalRecord, void* option)
{
	LOG_SCE_TRACE("(transId=%d, clanId=%d, boardId=%d, idArray=*0x%x, idArraySize=%d, rankArray=*0x%x, rankArraySize=%d, commentArray=*0x%x, commentArraySize=%d, infoArray=*0x%x, infoArraySize=%d, descriptArray=*0x%x, descriptArraySize=%d, arrayNum=%d, clanInfo=*0x%x, lastSortDate=*0x%x, totalRecord=*0x%x, option=*0x%x)",
		transId, clanId, boardId, idArray, idArraySize, rankArray, rankArraySize, commentArray, commentArraySize, infoArray, infoArraySize, descriptArray, descriptArraySize, arrayNum, clanInfo, lastSortDate, totalRecord, option);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_score_manager>()->is_initialized)
		{
			ret = SCE_NP_COMMUNITY_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!idArray || !rankArray || !totalRecord || !lastSortDate || !arrayNum)
		{
			ret = SCE_NP_COMMUNITY_ERROR_INSUFFICIENT_ARGUMENT;
			break;
		}
	
		if (option) // option check at least until fw 4.71
		{
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		if (arrayNum > SCE_NP_SCORE_MAX_NPID_NUM_PER_TRANS)
		{
			ret = SCE_NP_COMMUNITY_ERROR_TOO_MANY_NPID;
			break;
		}
	
		if (g_psn_connection_status != SCE_NP_MANAGER_STATUS_ONLINE)
		{
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ONLINE_ID;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpScoreGetClansMembersRankingByNpIdPcIdAsync(int32_t transId, SceNpClanId clanId, SceNpScoreBoardId boardId, const SceNpId* idArray, uint64_t idArraySize, SceNpScorePlayerRankData* rankArray,
	uint64_t rankArraySize, SceNpScoreComment* commentArray, uint64_t commentArraySize, SceNpScoreGameInfo* infoArray, uint64_t infoArraySize,
	SceNpScoreClansMemberDescription* descriptArray, uint64_t descriptArraySize, uint64_t arrayNum, SceNpScoreClanBasicInfo* clanInfo, CellRtcTick* lastSortDate,
	SceNpScoreRankNumber* totalRecord, int32_t prio, void* option)
{
	LOG_SCE_TRACE("(transId=%d, clanId=%d, boardId=%d, idArray=*0x%x, idArraySize=%d, rankArray=*0x%x, rankArraySize=%d, commentArray=*0x%x, commentArraySize=%d, infoArray=*0x%x, infoArraySize=%d, descriptArray=*0x%x, descriptArraySize=%d, arrayNum=%d, clanInfo=*0x%x, lastSortDate=*0x%x, totalRecord=*0x%x, prio=%d, option=*0x%x)",
		transId, clanId, boardId, idArray, idArraySize, rankArray, rankArraySize, commentArray, commentArraySize, infoArray, infoArraySize, descriptArray, descriptArraySize, arrayNum, clanInfo, lastSortDate, totalRecord, prio, option);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_score_manager>()->is_initialized)
		{
			ret = SCE_NP_COMMUNITY_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!idArray || !arrayNum)
		{
			ret = SCE_NP_COMMUNITY_ERROR_INSUFFICIENT_ARGUMENT;
			break;
		}
	
		if (option) // option check at least until fw 4.71
		{
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpScoreGetClansRankingByRange(int32_t transId, SceNpScoreClansBoardId clanBoardId, SceNpScoreRankNumber startSerialRank, SceNpScoreClanIdRankData* rankArray,
	uint64_t rankArraySize, void* reserved1, uint64_t reservedSize1, void* reserved2, uint64_t reservedSize2, uint64_t arrayNum,
	CellRtcTick* lastSortDate, SceNpScoreRankNumber* totalRecord, void* option)
{
	LOG_SCE_TRACE("(transId=%d, clanBoardId=%d, startSerialRank=%d, rankArray=*0x%x, rankArraySize=%d, reserved1=*0x%x, reservedSize1=%d, reserved2=*0x%x, reservedSize2=%d, arrayNum=%d, lastSortDate=*0x%x, totalRecord=*0x%x, option=*0x%x)",
		transId, clanBoardId, startSerialRank, rankArray, rankArraySize, reserved1, reservedSize1, reserved2, reservedSize2, arrayNum, lastSortDate, totalRecord, option);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_score_manager>()->is_initialized)
		{
			ret = SCE_NP_COMMUNITY_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!rankArray || !totalRecord || !lastSortDate || !arrayNum)
		{
			ret = SCE_NP_COMMUNITY_ERROR_INSUFFICIENT_ARGUMENT;
			break;
		}
	
		if (!startSerialRank || reserved1 || reservedSize1 || reserved2 || reservedSize2 || option) // reserved and option checks at least until fw 4.71
		{
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		if (arrayNum > SCE_NP_SCORE_MAX_CLAN_NUM_PER_TRANS)
		{
			ret = SCE_NP_COMMUNITY_ERROR_TOO_LARGE_RANGE;
			break;
		}
	
		if (g_psn_connection_status != SCE_NP_MANAGER_STATUS_ONLINE)
		{
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ONLINE_ID;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpScoreGetClansRankingByRangeAsync(int32_t transId, SceNpScoreClansBoardId clanBoardId, SceNpScoreRankNumber startSerialRank, SceNpScoreClanIdRankData* rankArray,
	uint64_t rankArraySize, void* reserved1, uint64_t reservedSize1, void* reserved2, uint64_t reservedSize2, uint64_t arrayNum,
	CellRtcTick* lastSortDate, SceNpScoreRankNumber* totalRecord, int32_t prio, void* option)
{
	LOG_SCE_TRACE("(transId=%d, clanBoardId=%d, startSerialRank=%d, rankArray=*0x%x, rankArraySize=%d, reserved1=*0x%x, reservedSize1=%d, reserved2=*0x%x, reservedSize2=%d, arrayNum=%d, lastSortDate=*0x%x, totalRecord=*0x%x, prio=%d, option=*0x%x)",
		transId, clanBoardId, startSerialRank, rankArray, rankArraySize, reserved1, reservedSize1, reserved2, reservedSize2, arrayNum, lastSortDate, totalRecord, prio, option);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_score_manager>()->is_initialized)
		{
			ret = SCE_NP_COMMUNITY_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!arrayNum)
		{
			ret = SCE_NP_COMMUNITY_ERROR_INSUFFICIENT_ARGUMENT;
			break;
		}
	
		if (!startSerialRank || reserved1 || reservedSize1 || reserved2 || reservedSize2 || option) // reserved and option checks at least until fw 4.71
		{
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpScoreGetClanMemberGameData(int32_t transId, SceNpScoreBoardId boardId, SceNpClanId clanId, const SceNpId* npId,
	uint64_t* totalSize, uint64_t recvSize, void* data, void* option)
{
	LOG_SCE_TRACE("(transId=%d, boardId=%d, clanId=%d, npId=*0x%x, totalSize=*0x%x, recvSize=%d, data=*0x%x, option=*0x%x)",
		transId, boardId, clanId, npId, totalSize, recvSize, data, option);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_score_manager>()->is_initialized)
		{
			ret = SCE_NP_COMMUNITY_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!npId || !totalSize || !data)
		{
			ret = SCE_NP_COMMUNITY_ERROR_INSUFFICIENT_ARGUMENT;
			break;
		}
	
		if (option) // option check at least until fw 4.71
		{
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		if (g_psn_connection_status != SCE_NP_MANAGER_STATUS_ONLINE)
		{
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ONLINE_ID;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpScoreGetClanMemberGameDataAsync(int32_t transId, SceNpScoreBoardId boardId, SceNpClanId clanId, const SceNpId* npId,
	uint64_t* totalSize, uint64_t recvSize, void* data, int32_t prio, void* option)
{
	LOG_SCE_TRACE("(transId=%d, boardId=%d, clanId=%d, npId=*0x%x, totalSize=*0x%x, recvSize=%d, data=*0x%x, prio=%d, option=*0x%x)",
		transId, boardId, clanId, npId, totalSize, recvSize, data, prio, option);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_score_manager>()->is_initialized)
		{
			ret = SCE_NP_COMMUNITY_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (option) // option check at least until fw 4.71
		{
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpScoreGetClansRankingByClanId(int32_t transId, SceNpScoreClansBoardId clanBoardId, const SceNpClanId* clanIdArray, uint64_t clanIdArraySize,
	SceNpScoreClanIdRankData* rankArray, uint64_t rankArraySize, void* reserved1, uint64_t reservedSize1, void* reserved2, uint64_t reservedSize2, uint64_t arrayNum,
	CellRtcTick* lastSortDate, SceNpScoreRankNumber* totalRecord, void* option)
{
	LOG_SCE_TRACE("(transId=%d, clanBoardId=%d, clanIdArray=*0x%x, clanIdArraySize=%d, rankArray=*0x%x, rankArraySize=%d, reserved1=*0x%x, reservedSize1=%d, reserved2=*0x%x, reservedSize2=%d, arrayNum=%d, lastSortDate=*0x%x, totalRecord=*0x%x, option=*0x%x)",
		transId, clanBoardId, clanIdArray, clanIdArraySize, rankArray, rankArraySize, reserved1, reservedSize1, reserved2, reservedSize2, arrayNum, lastSortDate, totalRecord, option);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_score_manager>()->is_initialized)
		{
			ret = SCE_NP_COMMUNITY_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!clanIdArray || !rankArray || !totalRecord || !lastSortDate || !arrayNum)
		{
			ret = SCE_NP_COMMUNITY_ERROR_INSUFFICIENT_ARGUMENT;
			break;
		}
	
		if (reserved1 || reservedSize1 || reserved2 || reservedSize2 || option) // reserved and option checks at least until fw 4.71
		{
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		if (arrayNum > SCE_NP_SCORE_MAX_NPID_NUM_PER_TRANS)
		{
			ret = SCE_NP_COMMUNITY_ERROR_TOO_MANY_NPID;
			break;
		}
	
		if (g_psn_connection_status != SCE_NP_MANAGER_STATUS_ONLINE)
		{
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ONLINE_ID;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpScoreGetClansRankingByClanIdAsync(int32_t transId, SceNpScoreClansBoardId clanBoardId, const SceNpClanId* clanIdArray, uint64_t clanIdArraySize,
	SceNpScoreClanIdRankData* rankArray, uint64_t rankArraySize, void* reserved1, uint64_t reservedSize1, void* reserved2, uint64_t reservedSize2, uint64_t arrayNum,
	CellRtcTick* lastSortDate, SceNpScoreRankNumber* totalRecord, int32_t prio, void* option)
{
	LOG_SCE_TRACE("(transId=%d, clanBoardId=%d, clanIdArray=*0x%x, clanIdArraySize=%d, rankArray=*0x%x, rankArraySize=%d, reserved1=*0x%x, reservedSize1=%d, reserved2=*0x%x, reservedSize2=%d, arrayNum=%d, lastSortDate=*0x%x, totalRecord=*0x%x, prio=%d, option=*0x%x)",
		transId, clanBoardId, clanIdArray, clanIdArraySize, rankArray, rankArraySize, reserved1, reservedSize1, reserved2, reservedSize2, arrayNum, lastSortDate, totalRecord, prio, option);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_score_manager>()->is_initialized)
		{
			ret = SCE_NP_COMMUNITY_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!arrayNum)
		{
			ret = SCE_NP_COMMUNITY_ERROR_INSUFFICIENT_ARGUMENT;
			break;
		}
	
		if (reserved1 || reservedSize1 || reserved2 || reservedSize2 || option) // reserved and option checks at least until fw 4.71
		{
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpScoreGetClansMembersRankingByRange(int32_t transId, SceNpClanId clanId, SceNpScoreBoardId boardId, SceNpScoreRankNumber startSerialRank,
	SceNpScoreClanIdRankData* rankArray, uint64_t rankArraySize, SceNpScoreComment* commentArray, uint64_t commentArraySize, SceNpScoreGameInfo* infoArray,
	uint64_t infoArraySize, SceNpScoreClansMemberDescription* descriptArray, uint64_t descriptArraySize, uint64_t arrayNum, SceNpScoreClanBasicInfo* clanInfo,
	CellRtcTick* lastSortDate, SceNpScoreRankNumber* totalRecord, void* option)
{
	LOG_SCE_TRACE("(transId=%d, clanId=%d, boardId=%d, startSerialRank=%d, rankArray=*0x%x, rankArraySize=%d, commentArray=*0x%x, commentArraySize=%d, infoArray=*0x%x, infoArraySize=%d, descriptArray=*0x%x, descriptArraySize=%d, arrayNum=%d, clanInfo=*0x%x, lastSortDate=*0x%x, totalRecord=*0x%x, option=*0x%x)",
		transId, clanId, boardId, startSerialRank, rankArray, rankArraySize, commentArray, commentArraySize, infoArray, infoArraySize, descriptArray, descriptArraySize, arrayNum, clanInfo, lastSortDate, totalRecord, option);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_score_manager>()->is_initialized)
		{
			ret = SCE_NP_COMMUNITY_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!rankArray || !totalRecord || !lastSortDate || !arrayNum)
		{
			ret = SCE_NP_COMMUNITY_ERROR_INSUFFICIENT_ARGUMENT;
			break;
		}
	
		if (!startSerialRank || option) // option check at least until fw 4.71
		{
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		if (arrayNum > SCE_NP_SCORE_MAX_RANGE_NUM_PER_TRANS)
		{
			ret = SCE_NP_COMMUNITY_ERROR_TOO_LARGE_RANGE;
			break;
		}
	
		if (g_psn_connection_status != SCE_NP_MANAGER_STATUS_ONLINE)
		{
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ONLINE_ID;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpScoreGetClansMembersRankingByRangeAsync(int32_t transId, SceNpClanId clanId, SceNpScoreBoardId boardId, SceNpScoreRankNumber startSerialRank,
	SceNpScoreClanIdRankData* rankArray, uint64_t rankArraySize, SceNpScoreComment* commentArray, uint64_t commentArraySize, SceNpScoreGameInfo* infoArray,
	uint64_t infoArraySize, SceNpScoreClansMemberDescription* descriptArray, uint64_t descriptArraySize, uint64_t arrayNum, SceNpScoreClanBasicInfo* clanInfo,
	CellRtcTick* lastSortDate, SceNpScoreRankNumber* totalRecord, int32_t prio, void* option)
{
	LOG_SCE_TRACE("(transId=%d, clanId=%d, boardId=%d, startSerialRank=%d, rankArray=*0x%x, rankArraySize=%d, commentArray=*0x%x, commentArraySize=%d, infoArray=*0x%x, infoArraySize=%d, descriptArray=*0x%x, descriptArraySize=%d, arrayNum=%d, clanInfo=*0x%x, lastSortDate=*0x%x, totalRecord=*0x%x, prio=%d, option=*0x%x)",
		transId, clanId, boardId, startSerialRank, rankArray, rankArraySize, commentArray, commentArraySize, infoArray, infoArraySize, descriptArray, descriptArraySize, arrayNum, clanInfo, lastSortDate, totalRecord, prio, option);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_score_manager>()->is_initialized)
		{
			ret = SCE_NP_COMMUNITY_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!arrayNum)
		{
			ret = SCE_NP_COMMUNITY_ERROR_INSUFFICIENT_ARGUMENT;
			break;
		}
	
		if (!startSerialRank || option) // option check at least until fw 4.71
		{
			ret = SCE_NP_COMMUNITY_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpSignalingCreateCtx(SceNpId* npId, SceNpSignalingHandler* handler, void* arg, uint32_t* ctx_id)
{
	LOG_SCE_TRACE("(npId=*0x%x, handler=*0x%x, arg=*0x%x, ctx_id=*0x%x)", npId, handler, arg, ctx_id);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_SIGNALING_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!npId || !ctx_id)
		{
			ret = SCE_NP_SIGNALING_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		//if (current_contexts > SCE_NP_SIGNALING_CTX_MAX)
		//{
		//	ret = SCE_NP_SIGNALING_ERROR_CTX_MAX;
			break;
		//}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpSignalingDestroyCtx(uint32_t ctx_id)
{
	LOG_SCE_TRACE("(ctx_id=%d)", ctx_id);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_SIGNALING_ERROR_NOT_INITIALIZED;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpSignalingAddExtendedHandler(uint32_t ctx_id, SceNpSignalingHandler* handler, void* arg)
{
	LOG_SCE_TRACE("(ctx_id=%d, handler=*0x%x, arg=*0x%x)", ctx_id, handler, arg);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_SIGNALING_ERROR_NOT_INITIALIZED;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpSignalingSetCtxOpt(uint32_t ctx_id, int32_t optname, int32_t optval)
{
	LOG_SCE_TRACE("(ctx_id=%d, optname=%d, optval=%d)", ctx_id, optname, optval);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_SIGNALING_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!optname || !optval)
		{
			ret = SCE_NP_SIGNALING_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpSignalingGetCtxOpt(uint32_t ctx_id, int32_t optname, int32_t* optval)
{
	LOG_SCE_TRACE("(ctx_id=%d, optname=%d, optval=*0x%x)", ctx_id, optname, optval);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_SIGNALING_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!optname || !optval)
		{
			ret = SCE_NP_SIGNALING_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpSignalingActivateConnection(uint32_t ctx_id, SceNpId* npId, uint32_t conn_id)
{
	LOG_SCE_TRACE("(ctx_id=%d, npId=*0x%x, conn_id=%d)", ctx_id, npId, conn_id);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_SIGNALING_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!npId || !conn_id)
		{
			ret = SCE_NP_SIGNALING_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpSignalingDeactivateConnection(uint32_t ctx_id, uint32_t conn_id)
{
	LOG_SCE_TRACE("(ctx_id=%d, conn_id=%d)", ctx_id, conn_id);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_SIGNALING_ERROR_NOT_INITIALIZED;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpSignalingTerminateConnection(uint32_t ctx_id, uint32_t conn_id)
{
	LOG_SCE_TRACE("(ctx_id=%d, conn_id=%d)", ctx_id, conn_id);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_SIGNALING_ERROR_NOT_INITIALIZED;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpSignalingGetConnectionStatus(uint32_t ctx_id, uint32_t conn_id, int32_t* conn_status, in_addr* peer_addr, in_port_t* peer_port)
{
	LOG_SCE_TRACE("(ctx_id=%d, conn_id=%d, conn_status=*0x%x, peer_addr=*0x%x, peer_port=*0x%x)", ctx_id, conn_id, conn_status, peer_addr, peer_port);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_SIGNALING_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!conn_status)
		{
			ret = SCE_NP_SIGNALING_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpSignalingGetConnectionInfo(uint32_t ctx_id, uint32_t conn_id, int32_t code, SceNpSignalingConnectionInfo* info)
{
	LOG_SCE_TRACE("(ctx_id=%d, conn_id=%d, code=%d, info=*0x%x)", ctx_id, conn_id, code, info);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_SIGNALING_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!info)
		{
			ret = SCE_NP_SIGNALING_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpSignalingGetConnectionFromNpId(uint32_t ctx_id, SceNpId* npId, uint32_t* conn_id)
{
	LOG_SCE_TRACE("(ctx_id=%d, npId=*0x%x, conn_id=*0x%x)", ctx_id, npId, conn_id);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_SIGNALING_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!npId || !conn_id)
		{
			ret = SCE_NP_SIGNALING_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpSignalingGetConnectionFromPeerAddress(uint32_t ctx_id, in_addr* peer_addr, in_port_t peer_port, uint32_t* conn_id)
{
	LOG_SCE_TRACE("(ctx_id=%d, peer_addr=*0x%x, peer_port=%d, conn_id=*0x%x)", ctx_id, peer_addr, peer_port, conn_id);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_SIGNALING_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!conn_id)
		{
			ret = SCE_NP_SIGNALING_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpSignalingGetLocalNetInfo(uint32_t ctx_id, SceNpSignalingNetInfo* info)
{
	LOG_SCE_TRACE("(ctx_id=%d, info=*0x%x)", ctx_id, info);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_SIGNALING_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!info)
		{
			// TODO: check info->size
			ret = SCE_NP_SIGNALING_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpSignalingGetPeerNetInfo(uint32_t ctx_id, SceNpId* npId, uint32_t* req_id)
{
	LOG_SCE_TRACE("(ctx_id=%d, npId=*0x%x, req_id=*0x%x)", ctx_id, npId, req_id);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_SIGNALING_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!npId || !req_id)
		{
			ret = SCE_NP_SIGNALING_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpSignalingCancelPeerNetInfo(uint32_t ctx_id, uint32_t req_id)
{
	LOG_SCE_TRACE("(ctx_id=%d, req_id=%d)", ctx_id, req_id);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_SIGNALING_ERROR_NOT_INITIALIZED;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpSignalingGetPeerNetInfoResult(uint32_t ctx_id, uint32_t req_id, SceNpSignalingNetInfo* info)
{
	LOG_SCE_TRACE("(ctx_id=%d, req_id=%d, info=*0x%x)", ctx_id, req_id, info);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!g_fxo->get<sce_np_manager>()->is_initialized)
		{
			ret = SCE_NP_SIGNALING_ERROR_NOT_INITIALIZED;
			break;
		}
	
		if (!info)
		{
			// TODO: check info->size
			ret = SCE_NP_SIGNALING_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpUtilCanonicalizeNpIdForPs3()
{
		UNIMPLEMENTED_FUNC(sceNp);
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpUtilCanonicalizeNpIdForPsp()
{
		UNIMPLEMENTED_FUNC(sceNp);
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpUtilCmpNpId(SceNpId* id1, SceNpId* id2)
{
	LOG_SCE_TRACE("(id1=*0x%x, id2=*0x%x)", id1, id2);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!id1 || !id2)
		{
			ret = SCE_NP_UTIL_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		// Unknown what this constant means
		if (id1->reserved[0] != 1 || id2->reserved[0] != 1)
		{
			ret = SCE_NP_UTIL_ERROR_INVALID_NP_ID;
			break;
		}
	
		if (strncmp(id1->handle.data, id2->handle.data, 16) || id1->unk1[0] != id2->unk1[0])
		{
			ret = SCE_NP_UTIL_ERROR_NOT_MATCH;
			break;
		}
	
		if (id1->unk1[1] != id2->unk1[1])
		{
			// If either is zero they match
			if (id1->opt[4] && id2->opt[4])
			{
				ret = SCE_NP_UTIL_ERROR_NOT_MATCH;
			break;
			}
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpUtilCmpNpIdInOrder(const SceNpId* id1, const SceNpId* id2, int32_t* order)
{
	LOG_SCE_TRACE("(id1=*0x%x, id2=*0x%x, order=*0x%x)", id1, id2, order);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!id1 || !id2)
		{
			ret = SCE_NP_UTIL_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		if (id1->reserved[0] != 1 || id2->reserved[0] != 1)
		{
			ret = SCE_NP_UTIL_ERROR_INVALID_NP_ID;
			break;
		}
	
		if (s32 res = strncmp(id1->handle.data, id2->handle.data, 16))
		{
			*order = std::clamp<s32>(res, -1, 1);
		ret = SCE_OK;
	} while(false);
	return ret;
		}
	
		if (s32 res = memcmp(id1->unk1, id2->unk1, 4))
		{
			*order = std::clamp<s32>(res, -1, 1);
		ret = SCE_OK;
	} while(false);
	return ret;
		}
	
		const u8 opt14 = id1->opt[4];
		const u8 opt24 = id2->opt[4];
	
		if (opt14 == 0 && opt24 == 0)
		{
			*order = 0;
		ret = SCE_OK;
	} while(false);
	return ret;
		}
	
		if (opt14 != 0 && opt24 != 0)
		{
			s32 res = memcmp(id1->unk1 + 1, id2->unk1 + 1, 4);
			*order = std::clamp<s32>(res, -1, 1);
		ret = SCE_OK;
	} while(false);
	return ret;
		}
	
		s32 res = memcmp((opt14 != 0 ? id1 : id2)->unk1 + 1, "ps3", 4);
		*order = std::clamp<s32>(res, -1, 1);
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpUtilCmpOnlineId(const SceNpId* id1, const SceNpId* id2)
{
	LOG_SCE_TRACE("(id1=*0x%x, id2=*0x%x)", id1, id2);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!id1 || !id2)
		{
			ret = SCE_NP_UTIL_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		if (id1->reserved[0] != 1 || id2->reserved[0] != 1)
		{
			ret = SCE_NP_UTIL_ERROR_INVALID_NP_ID;
			break;
		}
	
		if (strncmp(id1->handle.data, id2->handle.data, 16) != 0)
		{
			ret = SCE_NP_UTIL_ERROR_NOT_MATCH;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

int sceNpUtilGetPlatformType(const SceNpId* npId)
{
	LOG_SCE_TRACE("(npId=*0x%x)", npId);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!npId)
		{
			ret = SCE_NP_UTIL_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		switch (npId->unk1[1])
		{
		case "ps4\0"_u32:
			ret = not_an_error(SCE_NP_PLATFORM_TYPE_PS4);
			break;
		case "psp2"_u32:
			ret = not_an_error(SCE_NP_PLATFORM_TYPE_VITA);
			break;
		case "ps3\0"_u32:
			ret = not_an_error(SCE_NP_PLATFORM_TYPE_PS3); 
			break;
		case 0u:
			ret = not_an_error(SCE_NP_PLATFORM_TYPE_NONE);
			break;
		default:
			break;
		}
	
		ret = SCE_NP_UTIL_ERROR_UNKNOWN_PLATFORM_TYPE;
			break;
}

int sceNpUtilSetPlatformType(SceNpId* npId, SceNpPlatformType platformType)
{
	LOG_SCE_TRACE("(npId=*0x%x, platformType=%d)", npId, platformType);

	int ret = SCE_ERROR_UNKNOWN;
	do
	{
	
		if (!npId)
		{
			ret = SCE_NP_UTIL_ERROR_INVALID_ARGUMENT;
			break;
		}
	
		switch (platformType)
		{
		case SCE_NP_PLATFORM_TYPE_PS4:
			npId->unk1[1] = "ps4\0"_u32; break;
		case SCE_NP_PLATFORM_TYPE_VITA:
			npId->unk1[1] = "psp2"_u32; break;
		case SCE_NP_PLATFORM_TYPE_PS3:
			npId->unk1[1] = "ps3\0"_u32; break;
		case SCE_NP_PLATFORM_TYPE_NONE:
			npId->unk1[1] = 0; break;
		default:
			ret = SCE_NP_UTIL_ERROR_UNKNOWN_PLATFORM_TYPE;
			break;
		}
	
		ret = SCE_OK;
	} while(false);
	return ret;
}

error_code _sceNpSysutilClientMalloc()
{
	UNIMPLEMENTED_FUNC(sceNp);
	return CELL_OK;
}

error_code _sceNpSysutilClientFree()
{
	UNIMPLEMENTED_FUNC(sceNp);
	return CELL_OK;
}

s32 _Z33_sce_np_sysutil_send_empty_packetiPN16sysutil_cxmlutil11FixedMemoryEPKcS3_()
{
	sceNp.todo("_Z33_sce_np_sysutil_send_empty_packetiPN16sysutil_cxmlutil11FixedMemoryEPKcS3_()");
	return CELL_OK;
}

s32 _Z27_sce_np_sysutil_send_packetiRN4cxml8DocumentE()
{
	sceNp.todo("_Z27_sce_np_sysutil_send_packetiRN4cxml8DocumentE()");
	return CELL_OK;
}

s32 _Z36_sce_np_sysutil_recv_packet_fixedmemiPN16sysutil_cxmlutil11FixedMemoryERN4cxml8DocumentERNS2_7ElementE()
{
	sceNp.todo("_Z36_sce_np_sysutil_recv_packet_fixedmemiPN16sysutil_cxmlutil11FixedMemoryERN4cxml8DocumentERNS2_7ElementE()");
	return CELL_OK;
}

s32 _Z40_sce_np_sysutil_recv_packet_fixedmem_subiPN16sysutil_cxmlutil11FixedMemoryERN4cxml8DocumentERNS2_7ElementE()
{
	sceNp.todo("_Z40_sce_np_sysutil_recv_packet_fixedmem_subiPN16sysutil_cxmlutil11FixedMemoryERN4cxml8DocumentERNS2_7ElementE()");
	return CELL_OK;
}

s32 _Z27_sce_np_sysutil_recv_packetiRN4cxml8DocumentERNS_7ElementE()
{
	sceNp.todo("_Z27_sce_np_sysutil_recv_packetiRN4cxml8DocumentERNS_7ElementE()");
	return CELL_OK;
}

s32 _Z29_sce_np_sysutil_cxml_set_npidRN4cxml8DocumentERNS_7ElementEPKcPK7SceNpId()
{
	sceNp.todo("_Z29_sce_np_sysutil_cxml_set_npidRN4cxml8DocumentERNS_7ElementEPKcPK7SceNpId()");
	return CELL_OK;
}

s32 _Z31_sce_np_sysutil_send_packet_subiRN4cxml8DocumentE()
{
	sceNp.todo("_Z31_sce_np_sysutil_send_packet_subiRN4cxml8DocumentE()");
	return CELL_OK;
}

s32 _Z37sce_np_matching_set_matching2_runningb()
{
	sceNp.todo("_Z37sce_np_matching_set_matching2_runningb()");
	return CELL_OK;
}

s32 _Z32_sce_np_sysutil_cxml_prepare_docPN16sysutil_cxmlutil11FixedMemoryERN4cxml8DocumentEPKcRNS2_7ElementES6_i()
{
	sceNp.todo("_Z32_sce_np_sysutil_cxml_prepare_docPN16sysutil_cxmlutil11FixedMemoryERN4cxml8DocumentEPKcRNS2_7ElementES6_i()");
	return CELL_OK;
}


DECLARE(ppu_module_manager::sceNp)("sceNp", []()
{
	REG_FUNC(sceNp, sceNpInit);
	REG_FUNC(sceNp, sceNpTerm);
	REG_FUNC(sceNp, sceNpDrmIsAvailable);
	REG_FUNC(sceNp, sceNpDrmIsAvailable2);
	REG_FUNC(sceNp, sceNpDrmVerifyUpgradeLicense);
	REG_FUNC(sceNp, sceNpDrmVerifyUpgradeLicense2);
	REG_FUNC(sceNp, sceNpDrmExecuteGamePurchase);
	REG_FUNC(sceNp, sceNpDrmGetTimelimit);
	REG_FUNC(sceNp, sceNpDrmProcessExitSpawn);
	REG_FUNC(sceNp, sceNpDrmProcessExitSpawn2);
	REG_FUNC(sceNp, sceNpBasicRegisterHandler);
	REG_FUNC(sceNp, sceNpBasicRegisterContextSensitiveHandler);
	REG_FUNC(sceNp, sceNpBasicUnregisterHandler);
	REG_FUNC(sceNp, sceNpBasicSetPresence);
	REG_FUNC(sceNp, sceNpBasicSetPresenceDetails);
	REG_FUNC(sceNp, sceNpBasicSetPresenceDetails2);
	REG_FUNC(sceNp, sceNpBasicSendMessage);
	REG_FUNC(sceNp, sceNpBasicSendMessageGui);
	REG_FUNC(sceNp, sceNpBasicSendMessageAttachment);
	REG_FUNC(sceNp, sceNpBasicRecvMessageAttachment);
	REG_FUNC(sceNp, sceNpBasicRecvMessageAttachmentLoad);
	REG_FUNC(sceNp, sceNpBasicRecvMessageCustom);
	REG_FUNC(sceNp, sceNpBasicMarkMessageAsUsed);
	REG_FUNC(sceNp, sceNpBasicAbortGui);
	REG_FUNC(sceNp, sceNpBasicAddFriend);
	REG_FUNC(sceNp, sceNpBasicGetFriendListEntryCount);
	REG_FUNC(sceNp, sceNpBasicGetFriendListEntry);
	REG_FUNC(sceNp, sceNpBasicGetFriendPresenceByIndex);
	REG_FUNC(sceNp, sceNpBasicGetFriendPresenceByIndex2);
	REG_FUNC(sceNp, sceNpBasicGetFriendPresenceByNpId);
	REG_FUNC(sceNp, sceNpBasicGetFriendPresenceByNpId2);
	REG_FUNC(sceNp, sceNpBasicAddPlayersHistory);
	REG_FUNC(sceNp, sceNpBasicAddPlayersHistoryAsync);
	REG_FUNC(sceNp, sceNpBasicGetPlayersHistoryEntryCount);
	REG_FUNC(sceNp, sceNpBasicGetPlayersHistoryEntry);
	REG_FUNC(sceNp, sceNpBasicAddBlockListEntry);
	REG_FUNC(sceNp, sceNpBasicGetBlockListEntryCount);
	REG_FUNC(sceNp, sceNpBasicGetBlockListEntry);
	REG_FUNC(sceNp, sceNpBasicGetMessageAttachmentEntryCount);
	REG_FUNC(sceNp, sceNpBasicGetMessageAttachmentEntry);
	REG_FUNC(sceNp, sceNpBasicGetCustomInvitationEntryCount);
	REG_FUNC(sceNp, sceNpBasicGetCustomInvitationEntry);
	REG_FUNC(sceNp, sceNpBasicGetMatchingInvitationEntryCount);
	REG_FUNC(sceNp, sceNpBasicGetMatchingInvitationEntry);
	REG_FUNC(sceNp, sceNpBasicGetClanMessageEntryCount);
	REG_FUNC(sceNp, sceNpBasicGetClanMessageEntry);
	REG_FUNC(sceNp, sceNpBasicGetMessageEntryCount);
	REG_FUNC(sceNp, sceNpBasicGetMessageEntry);
	REG_FUNC(sceNp, sceNpBasicGetEvent);
	REG_FUNC(sceNp, sceNpCommerceCreateCtx);
	REG_FUNC(sceNp, sceNpCommerceDestroyCtx);
	REG_FUNC(sceNp, sceNpCommerceInitProductCategory);
	REG_FUNC(sceNp, sceNpCommerceDestroyProductCategory);
	REG_FUNC(sceNp, sceNpCommerceGetProductCategoryStart);
	REG_FUNC(sceNp, sceNpCommerceGetProductCategoryFinish);
	REG_FUNC(sceNp, sceNpCommerceGetProductCategoryResult);
	REG_FUNC(sceNp, sceNpCommerceGetProductCategoryAbort);
	REG_FUNC(sceNp, sceNpCommerceGetProductId);
	REG_FUNC(sceNp, sceNpCommerceGetProductName);
	REG_FUNC(sceNp, sceNpCommerceGetCategoryDescription);
	REG_FUNC(sceNp, sceNpCommerceGetCategoryId);
	REG_FUNC(sceNp, sceNpCommerceGetCategoryImageURL);
	REG_FUNC(sceNp, sceNpCommerceGetCategoryInfo);
	REG_FUNC(sceNp, sceNpCommerceGetCategoryName);
	REG_FUNC(sceNp, sceNpCommerceGetCurrencyCode);
	REG_FUNC(sceNp, sceNpCommerceGetCurrencyDecimals);
	REG_FUNC(sceNp, sceNpCommerceGetCurrencyInfo);
	REG_FUNC(sceNp, sceNpCommerceGetNumOfChildCategory);
	REG_FUNC(sceNp, sceNpCommerceGetNumOfChildProductSku);
	REG_FUNC(sceNp, sceNpCommerceGetSkuDescription);
	REG_FUNC(sceNp, sceNpCommerceGetSkuId);
	REG_FUNC(sceNp, sceNpCommerceGetSkuImageURL);
	REG_FUNC(sceNp, sceNpCommerceGetSkuName);
	REG_FUNC(sceNp, sceNpCommerceGetSkuPrice);
	REG_FUNC(sceNp, sceNpCommerceGetSkuUserData);
	REG_FUNC(sceNp, sceNpCommerceSetDataFlagStart);
	REG_FUNC(sceNp, sceNpCommerceGetDataFlagStart);
	REG_FUNC(sceNp, sceNpCommerceSetDataFlagFinish);
	REG_FUNC(sceNp, sceNpCommerceGetDataFlagFinish);
	REG_FUNC(sceNp, sceNpCommerceGetDataFlagState);
	REG_FUNC(sceNp, sceNpCommerceGetDataFlagAbort);
	REG_FUNC(sceNp, sceNpCommerceGetChildCategoryInfo);
	REG_FUNC(sceNp, sceNpCommerceGetChildProductSkuInfo);
	REG_FUNC(sceNp, sceNpCommerceDoCheckoutStartAsync);
	REG_FUNC(sceNp, sceNpCommerceDoCheckoutFinishAsync);
	REG_FUNC(sceNp, sceNpCustomMenuRegisterActions);
	REG_FUNC(sceNp, sceNpCustomMenuActionSetActivation);
	REG_FUNC(sceNp, sceNpCustomMenuRegisterExceptionList);
	REG_FUNC(sceNp, sceNpFriendlist);
	REG_FUNC(sceNp, sceNpFriendlistCustom);
	REG_FUNC(sceNp, sceNpFriendlistAbortGui);
	REG_FUNC(sceNp, sceNpLookupInit);
	REG_FUNC(sceNp, sceNpLookupTerm);
	REG_FUNC(sceNp, sceNpLookupCreateTitleCtx);
	REG_FUNC(sceNp, sceNpLookupDestroyTitleCtx);
	REG_FUNC(sceNp, sceNpLookupCreateTransactionCtx);
	REG_FUNC(sceNp, sceNpLookupDestroyTransactionCtx);
	REG_FUNC(sceNp, sceNpLookupSetTimeout);
	REG_FUNC(sceNp, sceNpLookupAbortTransaction);
	REG_FUNC(sceNp, sceNpLookupWaitAsync);
	REG_FUNC(sceNp, sceNpLookupPollAsync);
	REG_FUNC(sceNp, sceNpLookupNpId);
	REG_FUNC(sceNp, sceNpLookupNpIdAsync);
	REG_FUNC(sceNp, sceNpLookupUserProfile);
	REG_FUNC(sceNp, sceNpLookupUserProfileAsync);
	REG_FUNC(sceNp, sceNpLookupUserProfileWithAvatarSize);
	REG_FUNC(sceNp, sceNpLookupUserProfileWithAvatarSizeAsync);
	REG_FUNC(sceNp, sceNpLookupAvatarImage);
	REG_FUNC(sceNp, sceNpLookupAvatarImageAsync);
	REG_FUNC(sceNp, sceNpLookupTitleStorage);
	REG_FUNC(sceNp, sceNpLookupTitleStorageAsync);
	REG_FUNC(sceNp, sceNpLookupTitleSmallStorage);
	REG_FUNC(sceNp, sceNpLookupTitleSmallStorageAsync);
	REG_FUNC(sceNp, sceNpManagerRegisterCallback);
	REG_FUNC(sceNp, sceNpManagerUnregisterCallback);
	REG_FUNC(sceNp, sceNpManagerGetStatus);
	REG_FUNC(sceNp, sceNpManagerGetNetworkTime);
	REG_FUNC(sceNp, sceNpManagerGetOnlineId);
	REG_FUNC(sceNp, sceNpManagerGetNpId);
	REG_FUNC(sceNp, sceNpManagerGetOnlineName);
	REG_FUNC(sceNp, sceNpManagerGetAvatarUrl);
	REG_FUNC(sceNp, sceNpManagerGetMyLanguages);
	REG_FUNC(sceNp, sceNpManagerGetAccountRegion);
	REG_FUNC(sceNp, sceNpManagerGetAccountAge);
	REG_FUNC(sceNp, sceNpManagerGetContentRatingFlag);
	REG_FUNC(sceNp, sceNpManagerGetChatRestrictionFlag);
	REG_FUNC(sceNp, sceNpManagerGetCachedInfo);
	REG_FUNC(sceNp, sceNpManagerGetPsHandle);
	REG_FUNC(sceNp, sceNpManagerRequestTicket);
	REG_FUNC(sceNp, sceNpManagerRequestTicket2);
	REG_FUNC(sceNp, sceNpManagerGetTicket);
	REG_FUNC(sceNp, sceNpManagerGetTicketParam);
	REG_FUNC(sceNp, sceNpManagerGetEntitlementIdList);
	REG_FUNC(sceNp, sceNpManagerGetEntitlementById);
	REG_FUNC(sceNp, sceNpManagerGetSigninId);
	REG_FUNC(sceNp, sceNpManagerSubSignin);
	REG_FUNC(sceNp, sceNpManagerSubSigninAbortGui);
	REG_FUNC(sceNp, sceNpManagerSubSignout);
	REG_FUNC(sceNp, sceNpMatchingCreateCtx);
	REG_FUNC(sceNp, sceNpMatchingDestroyCtx);
	REG_FUNC(sceNp, sceNpMatchingGetResult);
	REG_FUNC(sceNp, sceNpMatchingGetResultGUI);
	REG_FUNC(sceNp, sceNpMatchingSetRoomInfo);
	REG_FUNC(sceNp, sceNpMatchingSetRoomInfoNoLimit);
	REG_FUNC(sceNp, sceNpMatchingGetRoomInfo);
	REG_FUNC(sceNp, sceNpMatchingGetRoomInfoNoLimit);
	REG_FUNC(sceNp, sceNpMatchingSetRoomSearchFlag);
	REG_FUNC(sceNp, sceNpMatchingGetRoomSearchFlag);
	REG_FUNC(sceNp, sceNpMatchingGetRoomMemberListLocal);
	REG_FUNC(sceNp, sceNpMatchingGetRoomListLimitGUI);
	REG_FUNC(sceNp, sceNpMatchingKickRoomMember);
	REG_FUNC(sceNp, sceNpMatchingKickRoomMemberWithOpt);
	REG_FUNC(sceNp, sceNpMatchingQuickMatchGUI);
	REG_FUNC(sceNp, sceNpMatchingSendInvitationGUI);
	REG_FUNC(sceNp, sceNpMatchingAcceptInvitationGUI);
	REG_FUNC(sceNp, sceNpMatchingCreateRoomGUI);
	REG_FUNC(sceNp, sceNpMatchingJoinRoomGUI);
	REG_FUNC(sceNp, sceNpMatchingLeaveRoom);
	REG_FUNC(sceNp, sceNpMatchingSearchJoinRoomGUI);
	REG_FUNC(sceNp, sceNpMatchingGrantOwnership);
	REG_FUNC(sceNp, sceNpProfileCallGui);
	REG_FUNC(sceNp, sceNpProfileAbortGui);
	REG_FUNC(sceNp, sceNpScoreInit);
	REG_FUNC(sceNp, sceNpScoreTerm);
	REG_FUNC(sceNp, sceNpScoreCreateTitleCtx);
	REG_FUNC(sceNp, sceNpScoreDestroyTitleCtx);
	REG_FUNC(sceNp, sceNpScoreCreateTransactionCtx);
	REG_FUNC(sceNp, sceNpScoreDestroyTransactionCtx);
	REG_FUNC(sceNp, sceNpScoreSetTimeout);
	REG_FUNC(sceNp, sceNpScoreSetPlayerCharacterId);
	REG_FUNC(sceNp, sceNpScoreWaitAsync);
	REG_FUNC(sceNp, sceNpScorePollAsync);
	REG_FUNC(sceNp, sceNpScoreGetBoardInfo);
	REG_FUNC(sceNp, sceNpScoreGetBoardInfoAsync);
	REG_FUNC(sceNp, sceNpScoreRecordScore);
	REG_FUNC(sceNp, sceNpScoreRecordScoreAsync);
	REG_FUNC(sceNp, sceNpScoreRecordGameData);
	REG_FUNC(sceNp, sceNpScoreRecordGameDataAsync);
	REG_FUNC(sceNp, sceNpScoreGetGameData);
	REG_FUNC(sceNp, sceNpScoreGetGameDataAsync);
	REG_FUNC(sceNp, sceNpScoreGetRankingByNpId);
	REG_FUNC(sceNp, sceNpScoreGetRankingByNpIdAsync);
	REG_FUNC(sceNp, sceNpScoreGetRankingByRange);
	REG_FUNC(sceNp, sceNpScoreGetRankingByRangeAsync);
	REG_FUNC(sceNp, sceNpScoreGetFriendsRanking);
	REG_FUNC(sceNp, sceNpScoreGetFriendsRankingAsync);
	REG_FUNC(sceNp, sceNpScoreCensorComment);
	REG_FUNC(sceNp, sceNpScoreCensorCommentAsync);
	REG_FUNC(sceNp, sceNpScoreSanitizeComment);
	REG_FUNC(sceNp, sceNpScoreSanitizeCommentAsync);
	REG_FUNC(sceNp, sceNpScoreGetRankingByNpIdPcId);
	REG_FUNC(sceNp, sceNpScoreGetRankingByNpIdPcIdAsync);
	REG_FUNC(sceNp, sceNpScoreAbortTransaction);
	REG_FUNC(sceNp, sceNpScoreGetClansMembersRankingByNpId);
	REG_FUNC(sceNp, sceNpScoreGetClansMembersRankingByNpIdAsync);
	REG_FUNC(sceNp, sceNpScoreGetClansMembersRankingByNpIdPcId);
	REG_FUNC(sceNp, sceNpScoreGetClansMembersRankingByNpIdPcIdAsync);
	REG_FUNC(sceNp, sceNpScoreGetClansMembersRankingByRange);
	REG_FUNC(sceNp, sceNpScoreGetClansMembersRankingByRangeAsync);
	REG_FUNC(sceNp, sceNpScoreGetClanMemberGameData);
	REG_FUNC(sceNp, sceNpScoreGetClanMemberGameDataAsync);
	REG_FUNC(sceNp, sceNpScoreGetClansRankingByClanId);
	REG_FUNC(sceNp, sceNpScoreGetClansRankingByClanIdAsync);
	REG_FUNC(sceNp, sceNpScoreGetClansRankingByRange);
	REG_FUNC(sceNp, sceNpScoreGetClansRankingByRangeAsync);
	REG_FUNC(sceNp, sceNpSignalingCreateCtx);
	REG_FUNC(sceNp, sceNpSignalingDestroyCtx);
	REG_FUNC(sceNp, sceNpSignalingAddExtendedHandler);
	REG_FUNC(sceNp, sceNpSignalingSetCtxOpt);
	REG_FUNC(sceNp, sceNpSignalingGetCtxOpt);
	REG_FUNC(sceNp, sceNpSignalingActivateConnection);
	REG_FUNC(sceNp, sceNpSignalingDeactivateConnection);
	REG_FUNC(sceNp, sceNpSignalingTerminateConnection);
	REG_FUNC(sceNp, sceNpSignalingGetConnectionStatus);
	REG_FUNC(sceNp, sceNpSignalingGetConnectionInfo);
	REG_FUNC(sceNp, sceNpSignalingGetConnectionFromNpId);
	REG_FUNC(sceNp, sceNpSignalingGetConnectionFromPeerAddress);
	REG_FUNC(sceNp, sceNpSignalingGetLocalNetInfo);
	REG_FUNC(sceNp, sceNpSignalingGetPeerNetInfo);
	REG_FUNC(sceNp, sceNpSignalingCancelPeerNetInfo);
	REG_FUNC(sceNp, sceNpSignalingGetPeerNetInfoResult);
	REG_FUNC(sceNp, sceNpUtilCanonicalizeNpIdForPs3);
	REG_FUNC(sceNp, sceNpUtilCanonicalizeNpIdForPsp);
	REG_FUNC(sceNp, sceNpUtilCmpNpId);
	REG_FUNC(sceNp, sceNpUtilCmpNpIdInOrder);
	REG_FUNC(sceNp, sceNpUtilCmpOnlineId);
	REG_FUNC(sceNp, sceNpUtilGetPlatformType);
	REG_FUNC(sceNp, sceNpUtilSetPlatformType);
	REG_FUNC(sceNp, _sceNpSysutilClientMalloc);
	REG_FUNC(sceNp, _sceNpSysutilClientFree);
	REG_FUNC(sceNp, _Z33_sce_np_sysutil_send_empty_packetiPN16sysutil_cxmlutil11FixedMemoryEPKcS3_);
	REG_FUNC(sceNp, _Z27_sce_np_sysutil_send_packetiRN4cxml8DocumentE);
	REG_FUNC(sceNp, _Z36_sce_np_sysutil_recv_packet_fixedmemiPN16sysutil_cxmlutil11FixedMemoryERN4cxml8DocumentERNS2_7ElementE);
	REG_FUNC(sceNp, _Z40_sce_np_sysutil_recv_packet_fixedmem_subiPN16sysutil_cxmlutil11FixedMemoryERN4cxml8DocumentERNS2_7ElementE);
	REG_FUNC(sceNp, _Z27_sce_np_sysutil_recv_packetiRN4cxml8DocumentERNS_7ElementE);
	REG_FUNC(sceNp, _Z29_sce_np_sysutil_cxml_set_npidRN4cxml8DocumentERNS_7ElementEPKcPK7SceNpId);
	REG_FUNC(sceNp, _Z31_sce_np_sysutil_send_packet_subiRN4cxml8DocumentE);
	REG_FUNC(sceNp, _Z37sce_np_matching_set_matching2_runningb);
	REG_FUNC(sceNp, _Z32_sce_np_sysutil_cxml_prepare_docPN16sysutil_cxmlutil11FixedMemoryERN4cxml8DocumentEPKcRNS2_7ElementES6_i);
});
