#pragma once

#include "GPCS4Common.h"

#define SCE_SAVE_DATA_TITLE_MAXSIZE (128)

#define SCE_SAVE_DATA_SUBTITLE_MAXSIZE (128)

#define SCE_SAVE_DATA_DETAIL_MAXSIZE (1024)


struct SceSaveDataMemoryParam
{
	char title[SCE_SAVE_DATA_TITLE_MAXSIZE];
	char subTitle[SCE_SAVE_DATA_SUBTITLE_MAXSIZE];
	char detail[SCE_SAVE_DATA_DETAIL_MAXSIZE];
	uint32_t userParam;
	uint8_t reserved[32];
};


typedef uint32_t SceSaveDataSaveDataMemoryOption;

struct SceSaveDataParam
{
	char title[SCE_SAVE_DATA_TITLE_MAXSIZE];
	char subTitle[SCE_SAVE_DATA_SUBTITLE_MAXSIZE];
	char detail[SCE_SAVE_DATA_DETAIL_MAXSIZE];
	uint32_t userParam;
	int : 32;
	sce_time_t mtime;
	uint8_t reserved[32];
};

struct SceSaveDataIcon
{
	void* buf;
	size_t bufSize;
	size_t dataSize;
	uint8_t reserved[32];
};

struct SceSaveDataMemoryData
{
	void* buf;
	size_t bufSize;
	sce_off_t offset;
	uint8_t reserved[40];
};

struct SceSaveDataMemorySetup2
{
	SceSaveDataSaveDataMemoryOption option;
	SceUserServiceUserId userId;
	size_t memorySize;
	size_t iconMemorySize;
	const SceSaveDataParam* initParam;
	const SceSaveDataIcon* initIcon;
	uint8_t reserved[24];
};

struct SceSaveDataMemorySetupResult
{
	size_t existedMemorySize;
	uint8_t reserved[16];
};

struct SceSaveDataMemoryGet2
{
	SceUserServiceUserId userId;
	uint8_t padding[4];
	SceSaveDataMemoryData* data;
	SceSaveDataParam* param;
	SceSaveDataIcon* icon;
	uint8_t reserved[32];
};

struct SceSaveDataMemorySet2
{
	SceUserServiceUserId userId;
	uint8_t padding[4];
	const SceSaveDataMemoryData* data;
	const SceSaveDataParam* param;
	const SceSaveDataIcon* icon;
	uint8_t reserved[32];
};