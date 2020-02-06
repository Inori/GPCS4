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
	{ 0x8FCC4AB62163D126, "sceSaveDataGetEventResult", (void*)sceSaveDataGetEventResult },
	{ 0x43038EEEF7A09D5F, "sceSaveDataGetSaveDataMemory2", (void*)sceSaveDataGetSaveDataMemory2 },
	{ 0x71DBB2F6FE18993E, "sceSaveDataSetSaveDataMemory2", (void*)sceSaveDataSetSaveDataMemory2 },
	{ 0xA10C921147E05D10, "sceSaveDataSetupSaveDataMemory2", (void*)sceSaveDataSetupSaveDataMemory2 },
	{ 0xC224FD8DE0BBC4FC, "sceSaveDataSyncSaveDataMemory", (void*)sceSaveDataSyncSaveDataMemory },
	{ 0xEC1B79A410BF01CA, "sceSaveDataGetSaveDataMemory", (void*)sceSaveDataGetSaveDataMemory },
	{ 0x664661B2408F5C5C, "sceSaveDataInitialize", (void*)sceSaveDataInitialize },
	{ 0x8776144735C64954, "sceSaveDataSetSaveDataMemory", (void*)sceSaveDataSetSaveDataMemory },
	{ 0xBFB00000CA342F3E, "sceSaveDataSetupSaveDataMemory", (void*)sceSaveDataSetupSaveDataMemory },
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


