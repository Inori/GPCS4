#include "Module.h"
#include "Platform/PlatformUtils.h"

#include <algorithm>


const MODULE_INFO &MemoryMappedModule::getModuleInfo() const { return moduleInfo; }
MODULE_INFO &MemoryMappedModule::getModuleInfo() { return moduleInfo; }

const ByteArray &MemoryMappedModule::getFileMemory() const { return fileMemory; }
ByteArray &MemoryMappedModule::getFileMemory() { return fileMemory; }

bool MemoryMappedModule::isModule()
{
	return elfHeader->e_type == ET_SCE_DYNAMIC ? true : false;
}

const FileList &MemoryMappedModule::getNeededFiles() const
{
	return neededFiles; 
}

const std::vector<size_t> &MemoryMappedModule::getExportSymbols() const
{
	return exportSymbols;
}

std::vector<size_t> &MemoryMappedModule::getExportSymbols()
{
	return exportSymbols; 
}

const UtilMemory::memory_uptr &MemoryMappedModule::getMappedMemory() const
{
	return mappedMemory;
}

UtilMemory::memory_uptr &MemoryMappedModule::getMappedMemory()
{
	return mappedMemory;
}

bool MemoryMappedModule::getImportSymbolInfo(std::string const &encSymbol,
											 std::string *modName,
											 std::string *libName,
											 uint64_t *nid) const
{
	return getSymbolInfo(encSymbol, importModules, importLibraries, modName,
						 libName, nid);
}

bool MemoryMappedModule::getExportSymbolInfo(std::string const &encSymbol,
											 std::string *modName,
											 std::string *libName,
											 uint64_t *nid) const
{
	return getSymbolInfo(encSymbol, exportModules, exportLibraries, modName,
						 libName, nid);
}

bool MemoryMappedModule::getSymbol(std::string const &encName,
								   SymbolInfo const **symbolInfo) const
{
	bool retVal = false;
	if (nameSymbolMap.count(encName) == 0)
	{
		retVal = false;
	}
	else
	{
		auto idx    = nameSymbolMap.at(encName);
		*symbolInfo = &symbols[idx];
		retVal      = true;
	}

	return retVal;
}

bool MemoryMappedModule::getSymbolInfo(std::string const &encSymbol,
									   ModuleList const &mods,
									   LibraryList const &libs,
									   std::string *modName,
									   std::string *libName,
									   uint64_t *nid) const
{
	bool retVal = false;

	do
	{
		uint modId = 0, libId = 0;

		if (modName == nullptr || libName == nullptr || nid == nullptr)
		{
			break;
		}

		if (!isEncodedSymbol(encSymbol))
		{
			*modName = "";
			*libName = "";
			*nid     = 0;
			retVal   = true;
			break;
		}

		retVal = decodeSymbol(encSymbol, &modId, &libId, nid);
		if (!retVal)
		{
			LOG_ERR("fail to decode encoded symbol");
			break;
		}

		retVal = getModNameFromId(modId, mods, modName);
		if (!retVal)
		{
			LOG_ERR("fail to get module name");
			break;
		}

		retVal = getLibNameFromId(libId, libs, libName);
		if (!retVal)
		{
			LOG_ERR("fail to get library name");
			break;
		}

		retVal = true;
	} while (false);

	return retVal;
}

bool MemoryMappedModule::getModNameFromId(uint64_t id,
										  ModuleList const &mods,
										  std::string *modName) const
{
	bool retVal   = false;
	auto &modules = mods;
	do
	{
		if (modName == nullptr)
		{
			break;
		}

		auto iter = std::find_if(
			modules.begin(), modules.end(),
			[=](IMPORT_MODULE const &mod) { return id == mod.id; });

		if (iter == modules.end())
		{
			break;
		}

		*modName = iter->strName;
		retVal   = true;
	} while (false);

	return retVal;
}

bool MemoryMappedModule::getLibNameFromId(uint64_t id,
										  LibraryList const &libs,
										  std::string *libName) const
{
	bool retVal = false;

	do
	{
		if (libName == nullptr)
		{
			break;
		}

		auto iter = std::find_if(
			libs.begin(), libs.end(),
			[=](IMPORT_LIBRARY const &lib) { return id == lib.id; });

		if (iter == libs.end())
		{
			break;
		}

		*libName = iter->strName;
		retVal   = true;

	} while (false);

	return retVal;
}

bool MemoryMappedModule::decodeValue(std::string const &encodedStr,
									 uint64_t &value) const
{
	bool bRet = false;

	// the max length for an encode id is 11
	// from orbis-ld.exe
	const uint nEncLenMax = 11;
	const char pCodes[] =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+-";

	do
	{

		if (encodedStr.size() > nEncLenMax)
		{
			LOG_ERR("encode id too long: %s", encodedStr.c_str());
			break;
		}

		bool bError = false;
		value       = 0;

		for (int i = 0; i < encodedStr.size(); ++i)
		{
			auto pChPos = strchr(pCodes, encodedStr[i]);
			uint nIndex = 0;

			if (pChPos != nullptr)
			{
				nIndex = static_cast<uint>(pChPos - pCodes);
			}
			else
			{
				bError = true;
				break;
			}

			// NID is 64 bits long, thus we do 6 x 10 + 4 times
			if (i < nEncLenMax - 1)
			{
				value <<= 6;
				value |= nIndex;
			}
			else
			{
				value <<= 4;
				value |= (nIndex >> 2);
			}
		}

		if (bError)
		{
			break;
		}

		bRet = true;
	} while (false);
	return bRet;
}

bool MemoryMappedModule::decodeSymbol(std::string const &strEncName,
									  uint *modId,
									  uint *libId,
									  uint64_t *funcNid) const
{
	bool bRet = false;

	do
	{
		if (modId == nullptr || libId == nullptr || funcNid == nullptr)
		{
			break;
		}

		auto &nModuleId  = *modId;
		auto &nLibraryId = *libId;
		auto &nNid       = *funcNid;

		std::vector<std::string> vtNameParts;
		if (!UtilString::Split(strEncName, '#', vtNameParts))
		{
			break;
		}

		if (!decodeValue(vtNameParts[0], nNid))
		{
			break;
		}

		uint64 nLibId = 0;
		if (!decodeValue(vtNameParts[1], nLibId))
		{
			break;
		}
		nLibraryId = static_cast<uint>(nLibId);

		uint64 nModId = 0;
		if (!decodeValue(vtNameParts[2], nModId))
		{
			break;
		}
		nModuleId = static_cast<uint>(nModId);

		bRet = true;
	} while (false);

	return bRet;
}

bool MemoryMappedModule::isEncodedSymbol(std::string const &symbolName) const
{
	bool retVal = false;
	if (symbolName.length() != 15)
	{
		retVal = false;
	}
	else if (symbolName[11] != '#' || symbolName[13] != '#')
	{
		retVal = false;
	}
	else
	{
		retVal = true;
	}

	return retVal;
}

int MemoryMappedModule::initialize() 
{ 
	int retVal = 0;
	if (isModule() && moduleInfo.pEntryPoint != nullptr)
	{
		auto ep = reinterpret_cast<intialize_func>(moduleInfo.pEntryPoint);
		retVal = ep(0, nullptr, nullptr);
	}

	return retVal;
}

bool MemoryMappedModule::getSymbol(size_t index,
								   SymbolInfo const **symbolInfo) const
{
	bool retVal = false;
	if (index > symbols.size() - 1)
	{
		retVal = false;
	}
	else
	{
		*symbolInfo = &symbols[index];
		retVal      = true;
	}

	return retVal;
}