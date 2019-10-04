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

	// virtual memory
	byte *pMappedAddr;
	uint nMappedSize;

	// loadable segments
	byte *pCodeAddr;
	uint nCodeSize;

	byte *pDataAddr;
	uint nDataSize;

	byte *pTlsAddr;
	uint nTlsInitSize; // initialized tls data size
	uint nTlsSize;     // whole tls data size, including nTlsInitSize

	// the following segments do not have vaddr or memsz in phdr (at least in
	// GOW4), so we don't have to load them into virtual memory, just record
	// it's pointer in file memory
	byte *pDynamic;
	uint nDynamicSize;

	byte *pSceDynLib;
	uint nSceDynLibSize;

	byte *pRela;
	uint nRelaCount;
	byte *pPltRela;
	uint nPltRelaCount;
	uint nPltRelType;

	byte *pSymTab;
	uint nSymTabSize;

	byte *pStrTab;
	uint nStrTabSize;

	byte *pSceComment;
	uint nSceCommentSize;

	byte *pSceLibVersion;
	uint nSceLibVersionSize;
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
struct MemoryMappedModule
{
	friend ELFMapper;

public:
	typedef int PS4API (*intialize_func)(size_t argc, void *argv, void *term);
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
	bool isModule();

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
	bool isEncodedSymbol(std::string const &symbolName) const;
	int initialize();

private:
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
					  uint *modId,
					  uint *libId,
					  uint64_t *funcNid) const;



private:
	SegmentHeaderList segmentHeaders;
	LibraryList importLibraries;
	LibraryList exportLibraries;

	ModuleList importModules;
	ModuleList exportModules;
	FileList neededFiles;

	SymbolList symbols;
	NameSymbolIndexMap nameSymbolMap;
	std::vector<size_t> exportSymbols;

	UtilMemory::memory_uptr mappedMemory;
	size_t mappedSize;
	ByteArray fileMemory;

	Elf64_Ehdr *elfHeader;
	MODULE_INFO moduleInfo;
};
