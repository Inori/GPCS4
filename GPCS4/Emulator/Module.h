#pragma once

#include "GPCS4Common.h"
#include "Loader/elf.h"
#include "Platform/UtilMemory.h"

#include <vector>
#include <memory>


struct MODULE_INFO
{
	void* pEntryPoint;

	//virtual memory
	byte* pMappedAddr;
	uint nMappedSize;

	//loadable segments
	byte* pCodeAddr;
	uint nCodeSize;

	byte* pDataAddr;
	uint nDataSize;

	byte* pTlsAddr;
	uint nTlsInitSize;  // initialized tls data size
	uint nTlsSize;	// whole tls data size, including nTlsInitSize

	//the following segments do not have vaddr or memsz in phdr (at least in GOW4),
	//so we don't have to load them into virtual memory,
	//just record it's pointer in file memory
	byte* pDynamic;
	uint nDynamicSize;

	byte* pSceDynLib;
	uint nSceDynLibSize;

	byte* pRela;
	uint nRelaCount;
	byte* pPltRela;
	uint nPltRelaCount;
	uint nPltRelType;

	byte* pSymTab;
	uint nSymTabSize;

	byte* pStrTab;
	uint nStrTabSize;

	byte* pSceComment;
	uint nSceCommentSize;

	byte* pSceLibVersion;
	uint nSceLibVersionSize;
};

struct IMPORT_MODULE
{
	std::string strName;
	union
	{
		uint64_t value;
		struct
		{
			uint32_t name_offset;
			uint8_t version_minor;
			uint8_t version_major;
			uint16_t id;
		};
	};
};

struct IMPORT_LIBRARY
{
	std::string strName;
	union
	{
		uint64_t value;
		struct
		{
			uint32_t name_offset;
			uint16_t version;
			uint16_t id;
		};
	};
};

struct MemoryMappedModule
{
	Elf64_Ehdr *elfHeader;
	std::vector<Elf64_Phdr> segmentHeaders;
	std::vector<IMPORT_LIBRARY> importLibraries;
	std::vector<IMPORT_MODULE> importModules;
	UtilMemory::memory_uptr mappedMemory;
	size_t mappedSize;
	std::vector<byte> fileMemory;

	MODULE_INFO moduleInfo;
};
