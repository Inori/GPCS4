#include "sce_savedata.h"


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs


static const SCE_EXPORT_FUNCTION g_pSceSaveData_libSceSaveData_FunctionTable[] =
{
	{ 0x4B51A478F235EF34, "sceSaveDataDelete", (void*)sceSaveDataDelete },
	{ 0x7722219D7ABFD123, "sceSaveDataDirNameSearch", (void*)sceSaveDataDirNameSearch },
	{ 0x5E0BD2B88767325C, "sceSaveDataGetParam", (void*)sceSaveDataGetParam },
	{ 0x4F2C2B14A0A82C66, "sceSaveDataInitialize3", (void*)sceSaveDataInitialize3 },
	{ 0xD33E393C81FE48D2, "sceSaveDataMount2", (void*)sceSaveDataMount2 },
	{ 0x73CF18CB9E0CC74C, "sceSaveDataSaveIcon", (void*)sceSaveDataSaveIcon },
	{ 0xF39CEE97FFDE197B, "sceSaveDataSetParam", (void*)sceSaveDataSetParam },
	{ 0xC8A0F2F12E722C0D, "sceSaveDataTerminate", (void*)sceSaveDataTerminate },
	{ 0x04C47817F51E9371, "sceSaveDataUmount", (void*)sceSaveDataUmount },
	SCE_FUNCTION_ENTRY_END
};

static const SCE_EXPORT_LIBRARY g_pSceSaveData_LibTable[] =
{
	{ "libSceSaveData", g_pSceSaveData_libSceSaveData_FunctionTable },
	SCE_LIBRARY_ENTRY_END
};

const SCE_EXPORT_MODULE g_ExpModuleSceSaveData =
{
	"libSceSaveData",
	g_pSceSaveData_LibTable
};


