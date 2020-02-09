#include "sce_savedata.h"

#include <cstring>

// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs

LOG_CHANNEL(SceModules.SceSaveData);

//////////////////////////////////////////////////////////////////////////
// library: libSceSaveData
//////////////////////////////////////////////////////////////////////////

int PS4API sceSaveDataDelete(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceSaveDataDirNameSearch(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceSaveDataGetParam(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceSaveDataInitialize3(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceSaveDataMount2(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceSaveDataSaveIcon(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceSaveDataSetParam(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceSaveDataTerminate(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceSaveDataUmount(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceSaveDataGetEventResult(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}

uint8_t g_saveDataMemory[0x1000] = { 0 };

int PS4API sceSaveDataGetSaveDataMemory(const SceUserServiceUserId userId, void* buf, const size_t bufSize, const uint64_t offset)
{
	LOG_FIXME("Not implemented");
	std::memset(buf, 0, bufSize);
	std::memcpy(buf, g_saveDataMemory, bufSize);
	return SCE_OK;
}


int PS4API sceSaveDataGetSaveDataMemory2(SceSaveDataMemoryGet2 *getParam)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceSaveDataSetSaveDataMemory(const SceUserServiceUserId userId, const void* buf, const size_t bufSize, const uint64_t offset)
{
	LOG_FIXME("Not implemented");
	std::memcpy(g_saveDataMemory, buf, bufSize);
	return SCE_OK;
}


int PS4API sceSaveDataSetSaveDataMemory2(const SceSaveDataMemorySet2 *setParam)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceSaveDataSetupSaveDataMemory(const SceUserServiceUserId userId, const uint64_t memorySize, const SceSaveDataMemoryParam* param)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}

int PS4API sceSaveDataSetupSaveDataMemory2(const SceSaveDataMemorySetup2 *setupParam, SceSaveDataMemorySetupResult *result)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API sceSaveDataSyncSaveDataMemory(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}



int PS4API sceSaveDataInitialize(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}



