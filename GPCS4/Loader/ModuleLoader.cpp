#include "ModuleLoader.h"
#include "Platform/PlatformUtils.h"


ModuleLoader::ModuleLoader(CSceModuleSystem &modSystem,
						   CLinker &linker,
						   CTLSHandler &tlsHandler)
	: m_modSystem{modSystem}, m_linker{linker}, m_tlsHandler{tlsHandler}
{
}

bool ModuleLoader::loadModule(std::string const &fileName,
							  MemoryMappedModule **modOut)
{
	bool retVal = false;

	do
	{
		MemoryMappedModule mod = {};

		retVal = loadModuleFromFile(fileName, &mod);
		if (!retVal)
		{
			break;
		}

		retVal = loadDependencies();
		if (!retVal)
		{
			break;
		}

		retVal = m_linker.relocateModules();
		if (!retVal)
		{
			break;
		}

		retVal = initializeModules();
		if (!retVal)
		{
			break;
		}

		*modOut = &(m_modSystem.getMemoryMappedModules()[0]);
		retVal  = true;
	} while (false);

	return retVal;
}

bool ModuleLoader::loadModuleFromFile(std::string const &fileName,
									  MemoryMappedModule *mod)
{
	bool retVal = false;
	do
	{

		retVal = mapFilePathToModuleName(fileName, &mod->fileName);
		if (!retVal)
		{
			break;
		}

		bool isLoaded = m_modSystem.isMemoryMappedModuleLoaded(mod->fileName);
		if (isLoaded)
		{
			LOG_DEBUG("module %s has already been loaded", mod->fileName.c_str());
			break;
		}

		retVal = m_mapper.loadFile(fileName, mod);
		if (!retVal)
		{
			break;
		}

		retVal = m_mapper.validateHeader();
		if (!retVal)
		{
			break;
		}

		retVal = m_mapper.parseSegmentHeaders();
		if (!retVal)
		{
			break;
		}

		retVal = m_mapper.parseDynamicSection();
		if (!retVal)
		{
			break;
		}

		retVal = m_mapper.mapImageIntoMemroy();
		if (!retVal)
		{
			break;
		}

		retVal = m_mapper.parseSymbols();
		if (!retVal)
		{
			break;
		}

		addDepedenciesToLoad(*mod);

		std::string moduleName = {};
		retVal                 = mapFilePathToModuleName(fileName, &moduleName);
		if (!retVal)
		{
			break;
		}

		for (auto &id : mod->getExportSymbols())
		{
			const SymbolInfo *info = nullptr;
			mod->getSymbol(id, &info);

			registerSymbol(*mod, info->symbolName,
						   reinterpret_cast<void *>(info->address));
		}

		m_modSystem.registerMemoryMappedModule(mod->fileName, std::move(*mod));
		retVal = true;
	} while (false);

	return retVal;
}

bool ModuleLoader::loadDependencies()
{
	bool retVal = true;

	while (!m_filesToLoad.empty())
	{
		auto fileName = m_filesToLoad.front();
		m_filesToLoad.pop();

		if (!m_modSystem.isFileAllowedToLoad(fileName))
		{
			LOG_DEBUG("File %s is not loadable", fileName.c_str());
			continue;
		}

		std::string path = {};

		retVal = mapModuleNameToFilePath(fileName, &path);
		if (!retVal)
		{
			LOG_ERR("Unable to locate file %s", fileName.c_str());
			break;
		}

		auto mod = MemoryMappedModule{};
		retVal   = loadModuleFromFile(path, &mod);
		if (!retVal)
		{
			LOG_ERR("Failed to load module %s", fileName.c_str());

			if (IGNORE_NOT_FOUND_MODULES)
			{
				retVal = true;
			}
			else
			{
				break;
			}
		}
	}

	return retVal;
}

bool ModuleLoader::addDepedenciesToLoad(MemoryMappedModule const &mod)
{
	for (auto const &file : mod.getNeededFiles())
	{
		m_filesToLoad.push(file);
	}

	return true;
}

// TODO: This is a temporary implenmentation
bool ModuleLoader::mapModuleNameToFilePath(std::string const &modName,
										   std::string *path)
{
	std::string fileName  = {};
	std::string extension = {};

	bool retVal = false;

	do
	{
		retVal = UtilPath::splitFileName(modName, &fileName, &extension);
		if (retVal == false)
		{
			break;
		}

		auto outName = UtilString::Format("lib\\%s.sprx", fileName.c_str());

		*path  = std::move(outName);
		retVal = true;
	} while (false);

	return retVal;
}

bool ModuleLoader::mapFilePathToModuleName(std::string const &filePath,
										   std::string *modName)
{
	// TODO: an unified implementation is needed.
	bool retVal = false;
	do
	{
		auto idx = filePath.find_last_of("/\\");
		if (idx == std::string::npos)
		{
			retVal   = true;
			*modName = filePath;
			break;
		}

		if (idx == filePath.size() - 1)
		{
			LOG_ERR("file name %s is invalid", filePath.c_str());
			break;
		}

		*modName = filePath.substr(idx + 1);
		retVal   = true;
	} while (false);

	return retVal;
}

bool ModuleLoader::registerSymbol(MemoryMappedModule const &mod,
								  std::string const &encName,
								  void *pointer)
{
	bool retVal = false;
	do
	{
		std::string modName = {};
		std::string libName = {};
		uint64_t nid        = 0;

		retVal = mod.getExportSymbolInfo(encName, &modName, &libName, &nid);
		if (!retVal)
		{
			break;
		}

		retVal = m_modSystem.registerFunction(modName, libName, nid, pointer);
		if (!retVal)
		{
			break;
		}

		retVal = true;
	} while (false);

	return retVal;
}

bool ModuleLoader::registerSymbol(MemoryMappedModule const &mod, size_t idx)
{
	const SymbolInfo *info = nullptr;
	mod.getSymbol(idx, &info);

	if (info->isEncoded)
	{
		m_modSystem.registerFunction(info->moduleName, info->libraryName, info->nid,
									 reinterpret_cast<void *>(info->address));
	}
	else
	{
		m_modSystem.registerSymbol(info->moduleName, info->libraryName,
								   info->symbolName,
								   reinterpret_cast<void *>(info->address));
	}

	return true;
}

bool ModuleLoader::relocateRela(MemoryMappedModule const &mod) const
{
	bool retVal = false;
	do
	{
		if (mod.getFileMemory().empty())
		{
			break;
		}

		auto &info = mod.getModuleInfo();

		byte *pImageBase         = info.pCodeAddr;
		byte *pStrTab            = info.pStrTab;
		Elf64_Sym *pSymTab       = (Elf64_Sym *)info.pSymTab;
		Elf64_Rela *pRelaEntries = (Elf64_Rela *)info.pRela;
		for (uint i = 0; i != info.nRelaCount; ++i)
		{
			Elf64_Rela *pRela = &pRelaEntries[i];
			auto nType        = ELF64_R_TYPE(pRela->r_info);
			auto nSymIdx      = ELF64_R_SYM(pRela->r_info);

			switch (nType)
			{
			case R_X86_64_NONE:
			case R_X86_64_PC32:
			case R_X86_64_COPY:
			case R_X86_64_GLOB_DAT:
			case R_X86_64_TPOFF64:
			case R_X86_64_TPOFF32:
			case R_X86_64_DTPMOD64:
			case R_X86_64_DTPOFF64:
			case R_X86_64_DTPOFF32:
				break;
			case R_X86_64_64:
			{
				Elf64_Sym &symbol = pSymTab[nSymIdx];
				auto nBinding     = ELF64_ST_BIND(symbol.st_info);
				uint64 nSymVal    = 0;

				if (nBinding == STB_LOCAL)
				{
					nSymVal = (uint64)(pImageBase + symbol.st_value);
				}
				else if (nBinding == STB_GLOBAL || nBinding == STB_WEAK)
				{
					char *pName = (char *)&pStrTab[symbol.st_name];

					// if (!ResolveSymbol(pName, nSymVal))
					if (!m_linker.resolveSymbol(mod, pName, &nSymVal))
					{
						LOG_ERR("can not get symbol address.");
						break;
					}
				}
				else
				{
					LOG_ERR("invalid sym bingding %d", nBinding);
				}

				*(uint64 *)&pImageBase[pRela->r_offset] = nSymVal + pRela->r_addend;
			}
			break;
			case R_X86_64_RELATIVE:
			{
				*(uint64 *)&pImageBase[pRela->r_offset] =
					(uint64)(pImageBase + pRela->r_addend);
			}
			break;
			default:
				LOG_FIXME("rela type not handled %d", nType);
				break;
			}
		}
		retVal = true;

	} while (false);

	return retVal;
}

bool ModuleLoader::relocatePltRela(MemoryMappedModule const &mod) const
{
	bool bRet = false;
	do
	{
		auto &fileData = mod.getFileMemory();
		auto &info     = mod.getModuleInfo();

		if (fileData.empty())
		{
			break;
		}

		byte *pImageBase = info.pCodeAddr;
		byte *pStrTab    = info.pStrTab;
		;
		Elf64_Sym *pSymTab       = (Elf64_Sym *)info.pSymTab;
		Elf64_Rela *pRelaEntries = (Elf64_Rela *)info.pPltRela;
		for (uint i = 0; i != info.nPltRelaCount; ++i)
		{
			Elf64_Rela *pRela = &pRelaEntries[i];
			auto type         = ELF64_R_TYPE(pRela->r_info);
			auto symbolIndex  = ELF64_R_SYM(pRela->r_info);

			switch (type)
			{
			case R_X86_64_JUMP_SLOT:
			{
				Elf64_Sym &symbol = pSymTab[symbolIndex];
				auto binding      = ELF64_ST_BIND(symbol.st_info);
				uint64 symValue   = 0;

				if (binding == STB_LOCAL)
				{
					symValue =
						reinterpret_cast<uint64_t>(pImageBase + symbol.st_value);
				}
				else if (binding == STB_GLOBAL || binding == STB_WEAK)
				{
					char *pName = (char *)&pStrTab[symbol.st_name];
					// if (!ResolveSymbol(pName, nSymVal))
					if (!m_linker.resolveSymbol(mod, pName, &symValue))
					{
						LOG_ERR("can not get symbol address.");
						break;
					}
				}
				else
				{
					LOG_ERR("invalid sym bingding %d", binding);
				}

				*(uint64 *)&pImageBase[pRela->r_offset] = symValue;
			}
			break;
			default:
				LOG_FIXME("rela type not handled %d", type);
				break;
			}
		}
		bRet = true;
	} while (false);

	return bRet;
}

bool ModuleLoader::initializeModules()
{
	auto &mods  = m_modSystem.getMemoryMappedModules();
	bool retVal = true;

	// intialize TLS
	for (auto const &mod : mods)
	{
		//auto const &mod = mods[0];

		void *pTls     = nullptr;
		uint initSize  = 0;
		uint totalSize = 0;

		retVal = mod.getTLSInfo(&pTls, &initSize, &totalSize);
		if (!retVal || pTls == nullptr)
		{
			LOG_DEBUG("no TLS info for module:%s", mod.fileName.c_str());
			continue;
		}

		retVal = m_tlsHandler.initialize(pTls, initSize, totalSize);
		if (!retVal)
		{
			LOG_ERR("fail to initialize TLS for module:%s", mod.fileName.c_str());
			continue;
		}
	}

	// skip eboot.bin
	for (size_t i = 1; i < mods.size(); i++)
	{
		int ret = mods[i].initialize();
		if (ret != 0)
		{
			LOG_ERR("unable to initialize module %s. ret=%d",
					mods[i].fileName.c_str(), ret);
			retVal = false;
			break;
		}
	}

	return retVal;
}

bool ModuleLoader::relocateModule(MemoryMappedModule const &mod) const
{
	bool retVal = false;

	if (relocateRela(mod) && relocatePltRela(mod))
	{
		retVal = true;
	}

	return retVal;
}
