/*
 *    GPCS4
 *    
 *    This file implements:
 *    module: libSceSaveData
 *        library: libSceSaveData
 *    
 */

#pragma once

#include "sce_module_common.h"
#include "sce_savedata_types.h"

extern const SCE_EXPORT_MODULE g_ExpModuleSceSaveData;


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs



//////////////////////////////////////////////////////////////////////////
// library: libSceSaveData
//////////////////////////////////////////////////////////////////////////

int PS4API sceSaveDataInitialize(void);


int PS4API sceSaveDataInitialize3(void);


int PS4API sceSaveDataTerminate(void);


int PS4API sceSaveDataSetupSaveDataMemory(const SceUserServiceUserId userId, const uint64_t memorySize, const SceSaveDataMemoryParam* param);


int PS4API sceSaveDataSetupSaveDataMemory2(const SceSaveDataMemorySetup2 *setupParam, SceSaveDataMemorySetupResult *result);


int PS4API sceSaveDataSetSaveDataMemory(const SceUserServiceUserId userId, const void *buf, const size_t bufSize, const uint64_t offset);


int PS4API sceSaveDataSetSaveDataMemory2(const SceSaveDataMemorySet2 *setParam);


int PS4API sceSaveDataGetSaveDataMemory(const SceUserServiceUserId userId, void *buf, const size_t bufSize, const uint64_t offset);


int PS4API sceSaveDataGetSaveDataMemory2(SceSaveDataMemoryGet2 *getParam);


int PS4API sceSaveDataSyncSaveDataMemory(void);


int PS4API sceSaveDataDelete(void);


int PS4API sceSaveDataDirNameSearch(void);


int PS4API sceSaveDataGetParam(void);


int PS4API sceSaveDataMount(void);


int PS4API sceSaveDataMount2(void);


int PS4API sceSaveDataUmount(void);


int PS4API sceSaveDataSaveIcon(void);


int PS4API sceSaveDataSetParam(void);


int PS4API sceSaveDataGetEventResult(void);


int PS4API sceSaveDataMount(void);


