#include "sce_userservice.h"


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs


static const SCE_EXPORT_FUNCTION g_pSceUserService_libSceUserService_FunctionTable[] =
{
	{ 0xC87D7B43A356B558, "sceUserServiceGetEvent", (void*)sceUserServiceGetEvent },
	{ 0x09D5A9D281D61ABD, "sceUserServiceGetInitialUser", (void*)sceUserServiceGetInitialUser },
	{ 0x8F760CBB531534DA, "sceUserServiceInitialize", (void*)sceUserServiceInitialize },
	{ 0x6F01634BE6D7F660, "sceUserServiceTerminate", (void*)sceUserServiceTerminate },
	{ 0xD71C5C3221AED9FA, "sceUserServiceGetUserName", (void*)sceUserServiceGetUserName },
	{ 0x7CF87298A36F2BF0, "sceUserServiceGetLoginUserIdList", (void*)sceUserServiceGetLoginUserIdList },
	SCE_FUNCTION_ENTRY_END
};

static const SCE_EXPORT_LIBRARY g_pSceUserService_LibTable[] =
{
	{ "libSceUserService", g_pSceUserService_libSceUserService_FunctionTable },
	SCE_LIBRARY_ENTRY_END
};

const SCE_EXPORT_MODULE g_ExpModuleSceUserService =
{
	"libSceUserService",
	g_pSceUserService_LibTable
};


