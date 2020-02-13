#pragma once

#include "GPCS4Common.h"
#include "Loader/elf.h"
#include "Platform/UtilMemory.h"

#include <vector>
#include <memory>
#include <string>
#include <map>
#include <unordered_map>

struct MODULE_INFO
{
	void *pEntryPoint;
	void *pInitProc; // pointer to .init_proc()
	void *pProcParam;

	// virtual memory
	uint8_t *pMappedAddr;
	uint32_t nMappedSize;

	// loadable segments
	uint8_t *pCodeAddr;
	uint32_t nCodeSize;

	uint8_t *pDataAddr;
	uint32_t nDataSize;

	uint8_t *pTlsAddr;
	uint32_t nTlsInitSize; // initialized tls data size
	uint32_t nTlsSize;     // whole tls data size, including nTlsInitSize
	uint32_t nTlsAlign;

	// the following segments do not have vaddr or memsz in phdr (at least in
	// GOW4), so we don't have to load them into virtual memory, just record
	// it's pointer in file memory
	uint8_t *pDynamic;
	uint32_t nDynamicSize;

	uint8_t *pSceDynLib;
	uint32_t nSceDynLibSize;

	uint8_t *pRela;
	uint32_t nRelaCount;
	uint8_t *pPltRela;
	uint32_t nPltRelaCount;
	uint32_t nPltRelType;

	uint8_t *pSymTab;
	uint32_t nSymTabSize;

	uint8_t *pStrTab;
	uint32_t nStrTabSize;

	uint8_t *pSceComment;
	uint32_t nSceCommentSize;

	uint8_t *pSceLibVersion;
	uint32_t nSceLibVersionSize;
};

struct IMPORT_MODULE
{
	std::string strName;
	union {
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
	union {
		uint64_t value;
		struct
		{
			uint32_t name_offset;
			uint16_t version;
			uint16_t id;
		};
	};
};

struct SymbolInfo
{
	enum class Binding
	{
		GLOBAL,
		LOCAL,
		WEAK
	};

	enum class Type
	{
		NOTYPE,
		OBJECT,
		FUNC,
		SECTION,
		FILE,
		COMMON,
		LOOS = 10,
		HIOS = 12,
		LOPROC,
		SPARC_REGISTER,
		HIPROC
	};

	Type type;
	Binding binding;
	std::string symbolName;
	std::string moduleName;
	std::string libraryName;
	uint64_t address;
	uint64_t nid;
	bool isEncoded;
};

using SegmentHeaderList  = std::vector<Elf64_Phdr>;
using LibraryList        = std::vector<IMPORT_LIBRARY>;
using ModuleList         = std::vector<IMPORT_MODULE>;
using SymbolList         = std::vector<SymbolInfo>;
using NameSymbolIndexMap = std::unordered_map<std::string, size_t>;
using FileList           = std::vector<std::string>;
using SymbolAddrMap      = std::map<std::string, void *>;
using ByteArray          = std::vector<uint8_t>;

class ELFMapper;
struct NativeModule
{
	friend ELFMapper;

public:
	typedef int PS4API (*init_proc)(size_t argc, void* argv[], int (*post_init)(size_t argc, void* argv[]));
	std::string fileName;

	const FileList &getNeededFiles() const;
	const std::vector<size_t> &getExportSymbols() const;
	std::vector<size_t> &getExportSymbols();
	const UtilMemory::memory_uptr &getMappedMemory() const;
	UtilMemory::memory_uptr &getMappedMemory();
	const MODULE_INFO &getModuleInfo() const;
	MODULE_INFO &getModuleInfo();
	const ByteArray &getFileMemory() const;
	ByteArray &getFileMemory();
	bool isModule() const;

	int initialize();
	bool getImportSymbolInfo(std::string const &encSymbol,
							 std::string *modName,
							 std::string *libName,
							 uint64_t *nid) const;

	bool getExportSymbolInfo(std::string const &encSymbol,
							 std::string *modName,
							 std::string *libName,
							 uint64_t *nid) const;

	bool getSymbol(std::string const &encName,
				   SymbolInfo const **symbolInfo) const;
	bool getSymbol(size_t index, SymbolInfo const **symbolInfo) const;

	bool getTLSInfo(void** pTls, uint32_t* initSIze, uint32_t* totalSize, uint32_t* align) const;
	void *getEntryPoint() const;

	bool getImportSymbols(SymbolList *symbols) const;
	bool getExportSymbols(SymbolList *symbols) const;
	bool getUnresolvedSymbols(SymbolList *symbols) const;
	bool outputUnresolvedSymbols(std::string const &fileName) const;
private:
	bool isEncodedSymbol(std::string const &symbolName) const;
	bool getSymbolInfo(std::string const &encSymbol,
					   ModuleList const &mods,
					   LibraryList const &libs,
					   std::string *modName,
					   std::string *libName,
					   uint64_t *nid) const;

	bool getModNameFromId(uint64_t id,
						  ModuleList const &mods,
						  std::string *modName) const;

	bool getLibNameFromId(uint64_t id,
						  LibraryList const &libs,
						  std::string *libName) const;

	bool decodeValue(std::string const &encodedStr, uint64_t &value) const;

	bool decodeSymbol(std::string const &strEncName,
					  uint32_t *modId,
					  uint32_t *libId,
					  uint64_t *funcNid) const;

private:
	SegmentHeaderList m_segmentHeaders;
	LibraryList m_importLibraries;
	LibraryList m_exportLibraries;

	ModuleList m_importModules;
	ModuleList m_exportModules;
	FileList m_neededFiles;

	SymbolList m_symbols;
	NameSymbolIndexMap m_nameSymbolMap;
	std::vector<size_t> m_exportSymbols;
	std::vector<size_t> m_importSymbols;

	UtilMemory::memory_uptr m_mappedMemory;
	size_t m_mappedSize;
	ByteArray m_fileMemory;

	Elf64_Ehdr *m_elfHeader;
	MODULE_INFO m_moduleInfo;
};
