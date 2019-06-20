#include "sce_rudp.h"


// Note:
// The codebase is genarated using GenerateCode.py
// You may need to modify the code manually to fit development needs


static const SCE_EXPORT_FUNCTION g_pSceRudp_libSceRudp_FunctionTable[] =
{
	{ 0x97848B0692940CAE, "sceRudpBind", (void*)sceRudpBind },
	{ 0x0806DB5FA06E419D, "sceRudpCreateContext", (void*)sceRudpCreateContext },
	{ 0xDE106FC2A130AA3F, "sceRudpEnd", (void*)sceRudpEnd },
	{ 0x3F1D26883D8BB96D, "sceRudpGetNumberOfPacketsToRead", (void*)sceRudpGetNumberOfPacketsToRead },
	{ 0x9824088529823FAA, "sceRudpGetOption", (void*)sceRudpGetOption },
	{ 0x7D17356A1429A51E, "sceRudpGetSizeWritable", (void*)sceRudpGetSizeWritable },
	{ 0x6A6B817C8FC041CE, "sceRudpInit", (void*)sceRudpInit },
	{ 0xB33115BBE79D5D5E, "sceRudpInitiate", (void*)sceRudpInitiate },
	{ 0xBCFCC995D0D2C577, "sceRudpNetReceived", (void*)sceRudpNetReceived },
	{ 0x85A32973B4C5C740, "sceRudpPollControl", (void*)sceRudpPollControl },
	{ 0x2E3C1B1E911E5B40, "sceRudpPollDestroy", (void*)sceRudpPollDestroy },
	{ 0x33A820BE2C17A4BB, "sceRudpPollWait", (void*)sceRudpPollWait },
	{ 0xF54F66D581F449C4, "sceRudpProcessEvents", (void*)sceRudpProcessEvents },
	{ 0xAD9A9657779780E0, "sceRudpRead", (void*)sceRudpRead },
	{ 0x4941157ACF20BE6C, "sceRudpSetEventHandler", (void*)sceRudpSetEventHandler },
	{ 0xD32CD87597F42301, "sceRudpSetOption", (void*)sceRudpSetOption },
	{ 0x38C6114D4D2E738C, "sceRudpTerminate", (void*)sceRudpTerminate },
	{ 0x29A3CBDDF6D32C20, "sceRudpWrite", (void*)sceRudpWrite },
	SCE_FUNCTION_ENTRY_END
};

static const SCE_EXPORT_LIBRARY g_pSceRudp_LibTable[] =
{
	{ "libSceRudp", g_pSceRudp_libSceRudp_FunctionTable },
	SCE_LIBRARY_ENTRY_END
};

const SCE_EXPORT_MODULE g_ExpModuleSceRudp =
{
	"libSceRudp",
	g_pSceRudp_LibTable
};


