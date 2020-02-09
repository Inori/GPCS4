#include "Module.h"
#include "Platform/PlatformUtils.h"
#include "Emulator/SceModuleSystem.h"

#include <spdlog/fmt/fmt.h>
#include <fstream>
#include <algorithm>

LOG_CHANNEL(Module);

const MODULE_INFO &NativeModule::getModuleInfo() const { return m_moduleInfo; }
MODULE_INFO &NativeModule::getModuleInfo() { return m_moduleInfo; }

const ByteArray &NativeModule::getFileMemory() const { return m_fileMemory; }
ByteArray &NativeModule::getFileMemory() { return m_fileMemory; }

bool NativeModule::isModule() const
{
	return m_elfHeader->e_type == ET_SCE_DYNAMIC ? true : false;
}

const FileList &NativeModule::getNeededFiles() const { return m_neededFiles; }

const std::vector<size_t> &NativeModule::getExportSymbols() const
{
	return m_exportSymbols;
}

std::vector<size_t> &NativeModule::getExportSymbols()
{
	return m_exportSymbols;
}

const UtilMemory::memory_uptr &NativeModule::getMappedMemory() const
{
	return m_mappedMemory;
}

UtilMemory::memory_uptr &NativeModule::getMappedMemory()
{
	return m_mappedMemory;
}

bool NativeModule::getImportSymbolInfo(std::string const &encSymbol,
											 std::string *modName,
											 std::string *libName,
											 uint64_t *nid) const
{
	return getSymbolInfo(encSymbol, m_importModules, m_importLibraries, modName,
						 libName, nid);
}

bool NativeModule::getExportSymbolInfo(std::string const &encSymbol,
											 std::string *modName,
											 std::string *libName,
											 uint64_t *nid) const
{
	return getSymbolInfo(encSymbol, m_exportModules, m_exportLibraries, modName,
						 libName, nid);
}

bool NativeModule::getSymbol(std::string const &encName,
								   SymbolInfo const **symbolInfo) const
{
	bool retVal = false;
	if (m_nameSymbolMap.count(encName) == 0)
	{
		retVal = false;
	}
	else
	{
		auto idx    = m_nameSymbolMap.at(encName);
		*symbolInfo = &m_symbols[idx];
		retVal      = true;
	}

	return retVal;
}

bool NativeModule::getSymbolInfo(std::string const &encSymbol,
									   ModuleList const &mods,
									   LibraryList const &libs,
									   std::string *modName,
									   std::string *libName,
									   uint64_t *nid) const
{
	bool retVal = false;

	do
	{
		uint32_t modId = 0, libId = 0;

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
			LOG_ERR("fail to get module name for symbol: %s in %s",
					encSymbol.c_str(), fileName.c_str());
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

bool NativeModule::getModNameFromId(uint64_t id,
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

		auto iter =
			std::find_if(modules.begin(), modules.end(),
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

bool NativeModule::getLibNameFromId(uint64_t id,
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

		auto iter =
			std::find_if(libs.begin(), libs.end(),
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

bool NativeModule::decodeValue(std::string const &encodedStr,
									 uint64_t &value) const
{
	bool bRet = false;

	// the max length for an encode id is 11
	// from orbis-ld.exe
	const uint32_t nEncLenMax = 11;
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
			uint32_t nIndex = 0;

			if (pChPos != nullptr)
			{
				nIndex = static_cast<uint32_t>(pChPos - pCodes);
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

bool NativeModule::decodeSymbol(std::string const &strEncName,
									  uint32_t *modId,
									  uint32_t *libId,
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

		std::vector<std::string> vtNameParts = UtilString::Split(strEncName, '#');
		if (vtNameParts.empty())
		{
			break;
		}

		if (!decodeValue(vtNameParts[0], nNid))
		{
			break;
		}

		uint64_t nLibId = 0;
		if (!decodeValue(vtNameParts[1], nLibId))
		{
			break;
		}
		nLibraryId = static_cast<uint32_t>(nLibId);

		uint64_t nModId = 0;
		if (!decodeValue(vtNameParts[2], nModId))
		{
			break;
		}
		nModuleId = static_cast<uint32_t>(nModId);

		bRet = true;
	} while (false);

	return bRet;
}

bool NativeModule::isEncodedSymbol(std::string const &symbolName) const
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

int NativeModule::initialize()
{
	int retVal = 0;

	if (isModule())
	{
		LOG_DEBUG("(%s) .init_proc() start.", fileName.c_str());
		auto init = reinterpret_cast<init_proc>(m_moduleInfo.pInitProc);
		retVal    = init(0, 0, nullptr);

		LOG_DEBUG("(%s) .init_proc() end. result = 0x%x", fileName.c_str(), retVal);
	}

	return retVal;
}

bool NativeModule::getTLSInfo(void **pTls,
									uint32_t *initSize,
									uint32_t *totalSize,
									uint32_t *align) const
{
	bool retVal = false;
	do
	{
		if (pTls == nullptr)
		{
			LOG_ERR("nullptr error");
			break;
		}

		*pTls      = m_moduleInfo.pTlsAddr;
		*initSize  = m_moduleInfo.nTlsInitSize;
		*totalSize = m_moduleInfo.nTlsSize;
		*align     = m_moduleInfo.nTlsAlign;

		retVal = true;
	} while (false);

	return retVal;
}

void *NativeModule::getEntryPoint() const { return m_moduleInfo.pEntryPoint; }

bool NativeModule::getSymbol(size_t index, SymbolInfo const **symbolInfo) const
{
	bool retVal = false;
	if (index > m_symbols.size() - 1)
	{
		retVal = false;
	}
	else
	{
		*symbolInfo = &m_symbols[index];
		retVal      = true;
	}

	return retVal;
}

bool NativeModule::getImportSymbols(SymbolList *importSymbols) const
{
	LOG_ASSERT(importSymbols != nullptr, "%s", "null pointer error");

	for (auto index : m_importSymbols)
	{
		importSymbols->emplace_back(m_symbols[index]);
	}

	return true;
}

bool NativeModule::getExportSymbols(SymbolList *exportSymbols) const
{
	LOG_ASSERT(exportSymbols != nullptr, "%s", "null pointer error");

	for (auto index : m_exportSymbols)
	{
		exportSymbols->emplace_back(m_symbols[index]);
	}

	return true;
}

bool NativeModule::getUnresolvedSymbols(SymbolList *symbolList) const
{
	LOG_ASSERT(symbolList != nullptr, "%s", "null pointer error");
	auto modSystem = CSceModuleSystem::GetInstance();

	for (auto index : m_importSymbols)
	{
		auto &symbol = m_symbols[index];
		auto addr = modSystem->getSymbolAddress(symbol.moduleName,
												symbol.libraryName,
												symbol.nid);
		if (addr == nullptr)
		{
			symbolList->emplace_back(symbol);
		}
	}

	return true;
}

bool NativeModule::outputUnresolvedSymbols(std::string const &fileName) const
{
	bool retVal = false;
	do
	{
		SymbolList symbols = {};
		retVal = getUnresolvedSymbols(&symbols);
		if (!retVal)
		{
			LOG_ERR("Failed to get unresolved symbols");
			break;
		}

		std::ofstream ofs;
		ofs.open(fileName);

		if (ofs.bad())
		{
			LOG_ERR("Failed to open file: %s", fileName.c_str());
			break;
		}

		for (auto &symb : symbols)
		{
			auto line = fmt::format("{} {} {:x}\n",
									symb.moduleName,
									symb.libraryName,
									symb.nid);
			ofs << line;
		}
		
		retVal = true;
	} while (false);

	return retVal;
}
