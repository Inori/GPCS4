#include "sce_npwebapi.h"


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs


static const SCE_EXPORT_FUNCTION g_pSceNpWebApi_libSceNpWebApi_FunctionTable[] =
{
	{ 0x2738584CFD9F1B5F, "sceNpWebApiAbortRequest", (void*)sceNpWebApiAbortRequest },
	{ 0xD7D2A07C95E033E5, "sceNpWebApiAddMultipartPart", (void*)sceNpWebApiAddMultipartPart },
	{ 0xC7563BCA261293B7, "sceNpWebApiCreateContext", (void*)sceNpWebApiCreateContext },
	{ 0x281C6078DA684484, "sceNpWebApiCreateMultipartRequest", (void*)sceNpWebApiCreateMultipartRequest },
	{ 0xADD82CE59D4CC85C, "sceNpWebApiCreateRequest", (void*)sceNpWebApiCreateRequest },
	{ 0x5D48DDB124D36775, "sceNpWebApiDeleteContext", (void*)sceNpWebApiDeleteContext },
	{ 0x9E842095EBBE28B1, "sceNpWebApiDeleteRequest", (void*)sceNpWebApiDeleteRequest },
	{ 0xDCE9EE6D4B34D943, "sceNpWebApiGetMemoryPoolStats", (void*)sceNpWebApiGetMemoryPoolStats },
	{ 0x1B70272CD7510631, "sceNpWebApiInitialize", (void*)sceNpWebApiInitialize },
	{ 0x090B4F45217A0ECF, "sceNpWebApiReadData", (void*)sceNpWebApiReadData },
	{ 0x0EC3CE4C4BD27BB3, "sceNpWebApiSendMultipartRequest2", (void*)sceNpWebApiSendMultipartRequest2 },
	{ 0x2A335E67FDBDCAC4, "sceNpWebApiSendRequest2", (void*)sceNpWebApiSendRequest2 },
	{ 0x6ACCF74ED22A185F, "sceNpWebApiTerminate", (void*)sceNpWebApiTerminate },
	SCE_FUNCTION_ENTRY_END
};

static const SCE_EXPORT_LIBRARY g_pSceNpWebApi_LibTable[] =
{
	{ "libSceNpWebApi", g_pSceNpWebApi_libSceNpWebApi_FunctionTable },
	SCE_LIBRARY_ENTRY_END
};

const SCE_EXPORT_MODULE g_ExpModuleSceNpWebApi =
{
	"libSceNpWebApi",
	g_pSceNpWebApi_LibTable
};


