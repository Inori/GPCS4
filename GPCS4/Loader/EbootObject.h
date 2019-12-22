#pragma once
#if 0 // Legacy Code
#include "GPCS4Common.h"
#include "elf.h"
#include <vector>
#include <string>
#include <memory>
#include "../Emulator/Linker.h"

class CEbootObject
{
public:
	CEbootObject();
	~CEbootObject();

	bool Load(const std::string& strFileName);

	void Unload();

	void* EntryPoint();

	bool GetTlsInfo(void** pTls, uint& nInitSize, uint& nTotalSize);

	void SetLinker(std::shared_ptr<CLinker>& pLinker);

private:

	bool MapElf();
	bool CollectModuleInfo();
	bool PreParseDynamic();
	bool ParseDynamic();

	bool IsLoadableSgmt(Elf64_Phdr& phdr);
	uint GetLoadableSize();
	bool LoadInVitrualMemory();
	bool LoadSgmtCode(Elf64_Phdr& phdr);
	bool LoadSgmtSceRelro(Elf64_Phdr& phdr);
	bool LoadSgmtData(Elf64_Phdr& phdr);

	bool ProcessSegments();
	bool Relocate();
	bool RelocateRela();
	bool RelocatePltRela();

	bool ResolveSymbol(const std::string& strEncName, uint64& nAddr);
	bool DecodeEncName(const std::string& strEncName,
		uint& nModuleId, uint& nLibraryId, uint64& nNid);
	bool DecodeValue(const std::string& strEnc, uint64& nVal);

	std::string ModuleNameFromId(uint nModId);
	std::string LibraryNameFromId(uint nLibId);
	
private:
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


private:
	std::vector<byte> m_vtFileMem;

	Elf64_Ehdr m_stEhdr;
	std::vector<Elf64_Phdr> m_vtPhdrs;

	MODULE_INFO m_stModuleInfo;

	std::shared_ptr<CLinker> m_pLinker;
	std::vector<IMPORT_MODULE> m_vtModules;
	std::vector<IMPORT_LIBRARY> m_vtLibraries;
};
#endif
