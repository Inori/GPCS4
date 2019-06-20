#include "sce_npmatching2.h"


// Note:
// The codebase is genarated using GenerateCode.py
// You may need to modify the code manually to fit development needs


static const SCE_EXPORT_FUNCTION g_pSceNpMatching2_libSceNpMatching2_FunctionTable[] =
{
	{ 0xEEF8CD43A675A29D, "sceNpMatching2ContextStart", (void*)sceNpMatching2ContextStart },
	{ 0xFDFE8CE1C68D7BC9, "sceNpMatching2ContextStop", (void*)sceNpMatching2ContextStop },
	{ 0x61F9A95BBD7DACCA, "sceNpMatching2CreateContext", (void*)sceNpMatching2CreateContext },
	{ 0xCC25999975CDEB4D, "sceNpMatching2CreateJoinRoom", (void*)sceNpMatching2CreateJoinRoom },
	{ 0x373FD913BBABDF62, "sceNpMatching2DestroyContext", (void*)sceNpMatching2DestroyContext },
	{ 0xDBABD6ACF01625F3, "sceNpMatching2GetRoomDataExternalList", (void*)sceNpMatching2GetRoomDataExternalList },
	{ 0x26B6B189F9A87ADE, "sceNpMatching2GetRoomDataInternal", (void*)sceNpMatching2GetRoomDataInternal },
	{ 0xE6586F3AA85E1410, "sceNpMatching2GetRoomMemberDataInternal", (void*)sceNpMatching2GetRoomMemberDataInternal },
	{ 0x282F869C7CEB2B6A, "sceNpMatching2GetRoomMemberIdListLocal", (void*)sceNpMatching2GetRoomMemberIdListLocal },
	{ 0x2E108F72D2080B14, "sceNpMatching2GetServerId", (void*)sceNpMatching2GetServerId },
	{ 0xAC934F26A0C2A622, "sceNpMatching2GetWorldInfoList", (void*)sceNpMatching2GetWorldInfoList },
	{ 0x3423F76CB18FB7EA, "sceNpMatching2GrantRoomOwner", (void*)sceNpMatching2GrantRoomOwner },
	{ 0xD74B777B9F893E75, "sceNpMatching2Initialize", (void*)sceNpMatching2Initialize },
	{ 0x09220C0EC563B3F8, "sceNpMatching2JoinRoom", (void*)sceNpMatching2JoinRoom },
	{ 0x01455F53A6F28377, "sceNpMatching2KickoutRoomMember", (void*)sceNpMatching2KickoutRoomMember },
	{ 0x043EA47F1E38D83A, "sceNpMatching2LeaveRoom", (void*)sceNpMatching2LeaveRoom },
	{ 0x7D041F3FCEC8EE1B, "sceNpMatching2RegisterContextCallback", (void*)sceNpMatching2RegisterContextCallback },
	{ 0xA7ED849F199A00C3, "sceNpMatching2RegisterRoomEventCallback", (void*)sceNpMatching2RegisterRoomEventCallback },
	{ 0xB81112CF3E02430B, "sceNpMatching2RegisterRoomMessageCallback", (void*)sceNpMatching2RegisterRoomMessageCallback },
	{ 0xD1431E5911A764A0, "sceNpMatching2RegisterSignalingCallback", (void*)sceNpMatching2RegisterSignalingCallback },
	{ 0x56A657ECF3A0D8C9, "sceNpMatching2SearchRoom", (void*)sceNpMatching2SearchRoom },
	{ 0x230DA1D09AEB6F95, "sceNpMatching2SendRoomMessage", (void*)sceNpMatching2SendRoomMessage },
	{ 0xABB18AF7CFE76121, "sceNpMatching2SetRoomDataExternal", (void*)sceNpMatching2SetRoomDataExternal },
	{ 0x4BD0FC252608AE31, "sceNpMatching2SetRoomDataInternal", (void*)sceNpMatching2SetRoomDataInternal },
	{ 0x1E8A93AE44BD7394, "sceNpMatching2SetRoomMemberDataInternal", (void*)sceNpMatching2SetRoomMemberDataInternal },
	{ 0x32AA77949FAC8F2E, "sceNpMatching2Terminate", (void*)sceNpMatching2Terminate },
	SCE_FUNCTION_ENTRY_END
};

static const SCE_EXPORT_LIBRARY g_pSceNpMatching2_LibTable[] =
{
	{ "libSceNpMatching2", g_pSceNpMatching2_libSceNpMatching2_FunctionTable },
	SCE_LIBRARY_ENTRY_END
};

const SCE_EXPORT_MODULE g_ExpModuleSceNpMatching2 =
{
	"libSceNpMatching2",
	g_pSceNpMatching2_LibTable
};


