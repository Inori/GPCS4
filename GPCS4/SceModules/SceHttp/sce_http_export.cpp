#include "sce_http.h"


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs


static const SCE_EXPORT_FUNCTION g_pSceHttp_libSceHttp_FunctionTable[] =
{
	{ 0x03D715314B44A786, "sceHttpInit", (void*)sceHttpInit },
	{ 0x224FCAA4B4E57FB4, "sceHttpTerm", (void*)sceHttpTerm },
	{ 0x118DBC4F66E437B9, "sceHttpAddRequestHeader", (void*)sceHttpAddRequestHeader },
	{ 0xAA0C43063A2B531B, "sceHttpCreateConnectionWithURL", (void*)sceHttpCreateConnectionWithURL },
	{ 0x01EBB9C152A417DC, "sceHttpCreateRequestWithURL", (void*)sceHttpCreateRequestWithURL },
	{ 0xD206233D347FE9C6, "sceHttpCreateTemplate", (void*)sceHttpCreateTemplate },
	{ 0x3FA037CADA6C8987, "sceHttpDeleteConnection", (void*)sceHttpDeleteConnection },
	{ 0xA9EEE867EBF83D60, "sceHttpDeleteRequest", (void*)sceHttpDeleteRequest },
	{ 0xE08F2F129B84859F, "sceHttpDeleteTemplate", (void*)sceHttpDeleteTemplate },
	{ 0xCAE3B61F652F9E8B, "sceHttpGetResponseContentLength", (void*)sceHttpGetResponseContentLength },
	{ 0xD1AD9304D7C4DC15, "sceHttpGetStatusCode", (void*)sceHttpGetStatusCode },
	{ 0x3F9A5DA3290F6139, "sceHttpReadData", (void*)sceHttpReadData },
	{ 0xD5ED8137023F5F31, "sceHttpSendRequest", (void*)sceHttpSendRequest },
	{ 0xD12F6D4C7D2EA935, "sceHttpSetConnectTimeOut", (void*)sceHttpSetConnectTimeOut },
	{ 0xCA282BE15D3F1D33, "sceHttpSetRecvTimeOut", (void*)sceHttpSetRecvTimeOut },
	{ 0x3D3885214C42A497, "sceHttpSetRequestContentLength", (void*)sceHttpSetRequestContentLength },
	{ 0x2B57752EA6514074, "sceHttpSetResolveRetry", (void*)sceHttpSetResolveRetry },
	{ 0x4DCFE10180CAB507, "sceHttpSetResolveTimeOut", (void*)sceHttpSetResolveTimeOut },
	{ 0xC5E8057D9281565C, "sceHttpSetSendTimeOut", (void*)sceHttpSetSendTimeOut },
	{ 0x62E396DDD0C02987, "sceHttpUriEscape", (void*)sceHttpUriEscape },
	SCE_FUNCTION_ENTRY_END
};

static const SCE_EXPORT_LIBRARY g_pSceHttp_LibTable[] =
{
	{ "libSceHttp", g_pSceHttp_libSceHttp_FunctionTable },
	SCE_LIBRARY_ENTRY_END
};

const SCE_EXPORT_MODULE g_ExpModuleSceHttp =
{
	"libSceHttp",
	g_pSceHttp_LibTable
};


