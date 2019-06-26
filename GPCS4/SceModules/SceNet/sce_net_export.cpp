#include "sce_net.h"


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs


static const SCE_EXPORT_FUNCTION g_pSceNet_libSceNet_FunctionTable[] =
{
	{ 0x3C85AA867F684B17, "sceNetAccept", (void*)sceNetAccept },
	{ 0x6C4AF1E3D3E0C726, "sceNetBind", (void*)sceNetBind },
	{ 0x3975D7E26524DEE9, "sceNetConnect", (void*)sceNetConnect },
	{ 0xC36D5880618DB419, "sceNetEpollAbort", (void*)sceNetEpollAbort },
	{ 0x655C38E9BB1AB009, "sceNetEpollControl", (void*)sceNetEpollControl },
	{ 0x485E3B901D8C353A, "sceNetEpollCreate", (void*)sceNetEpollCreate },
	{ 0x227A7595F2FE25DC, "sceNetEpollDestroy", (void*)sceNetEpollDestroy },
	{ 0x76B8C86C36C0ED44, "sceNetEpollWait", (void*)sceNetEpollWait },
	{ 0x1D03B09DF3068A94, "sceNetErrnoLoc", (void*)sceNetErrnoLoc },
	{ 0xC6986B66EB25EFC1, "sceNetGetsockopt", (void*)sceNetGetsockopt },
	{ 0xF53DA90C5D91CAA8, "sceNetHtonl", (void*)sceNetHtonl },
	{ 0x896416AF0892B7C0, "sceNetHtons", (void*)sceNetHtons },
	{ 0x3657AFECB83C9370, "sceNetInit", (void*)sceNetInit },
	{ 0x90E8F51E2006139E, "sceNetListen", (void*)sceNetListen },
	{ 0x45BBEDFB9636884C, "sceNetNtohs", (void*)sceNetNtohs },
	{ 0x76024169E2671A9A, "sceNetPoolCreate", (void*)sceNetPoolCreate },
	{ 0x2BB465AD3908FE6C, "sceNetPoolDestroy", (void*)sceNetPoolDestroy },
	{ 0xF703BD5EB32C3617, "sceNetRecv", (void*)sceNetRecv },
	{ 0xDF4E28A0D6715836, "sceNetRecvfrom", (void*)sceNetRecvfrom },
	{ 0x6DE4635C19FFCFEA, "sceNetSend", (void*)sceNetSend },
	{ 0x82F0F582B782BB40, "sceNetSendto", (void*)sceNetSendto },
	{ 0xDA6297D92A6CA3B2, "sceNetSetsockopt", (void*)sceNetSetsockopt },
	{ 0x4D233AC21B5E9289, "sceNetShutdown", (void*)sceNetShutdown },
	{ 0x438A81B8DFDCD193, "sceNetSocket", (void*)sceNetSocket },
	{ 0xCC919FF318C59D01, "sceNetSocketAbort", (void*)sceNetSocketAbort },
	{ 0xE398201336A43C94, "sceNetSocketClose", (void*)sceNetSocketClose },
	{ 0x7131A473AFD30652, "sceNetTerm", (void*)sceNetTerm },
	{ 0xE8E7346CBB0861ED, "sceNetGetMacAddress", (void*)sceNetGetMacAddress },
	{ 0x0B85200C71CFBDDC, "sceNetResolverCreate", (void*)sceNetResolverCreate },
	{ 0x9099581F9B8C0162, "sceNetResolverDestroy", (void*)sceNetResolverDestroy },
	{ 0x35DF7559A5A61B6C, "sceNetResolverStartNtoa", (void*)sceNetResolverStartNtoa },
	SCE_FUNCTION_ENTRY_END
};

static const SCE_EXPORT_LIBRARY g_pSceNet_LibTable[] =
{
	{ "libSceNet", g_pSceNet_libSceNet_FunctionTable },
	SCE_LIBRARY_ENTRY_END
};

const SCE_EXPORT_MODULE g_ExpModuleSceNet =
{
	"libSceNet",
	g_pSceNet_LibTable
};


