#include "ModuleLoader.h"

ModuleLoader::ModuleLoader(CSceModuleSystem &modSystem)
	: m_modSystem(modSystem), m_linker(modSystem)
{
}

bool ModuleLoader::loadModule(std::string const &fileName)
{
	bool retVal = false;

	do
	{
		MemoryMappedModule mod = {};
		retVal                 = loadModuleFromFile(fileName, &mod);
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

		retVal = true;
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

		for (auto &symbol : mod->exportSymbols)
		{
			registerSymbol(*mod, symbol.first, symbol.second);
		}

		m_modSystem.registerMemoryMappedModule(moduleName, std::move(*mod));
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
		retVal           = mapModuleNameToFilePath(fileName, &path);
		if (!retVal)
		{
			LOG_ERR("Unable to locate file %s", fileName.c_str());
			break;
		}

		auto mod = MemoryMappedModule{};
		retVal   = loadModuleFromFile(path, &mod);
		if (!retVal)
		{
			LOG_ERR("Fail to load module");
			break;
		}
	}

	return retVal;
}

bool ModuleLoader::addDepedenciesToLoad(MemoryMappedModule const &mod)
{
	for (auto const &file : mod.neededFiles)
	{
		m_filesToLoad.push(file);
	}

	return true;
}

bool ModuleLoader::mapModuleNameToFilePath(std::string const &modName,
										   std::string *path)
{
	LOG_FIXME("Test only implementation");
	*path = modName;
	return true;
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

bool ModuleLoader::relocateRela(MemoryMappedModule const &mod) const
{
	bool retVal = false;
	do
	{
		if (mod.fileMemory.empty())
		{
			break;
		}

		auto &info = mod.moduleInfo;

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

				*(uint64 *)&pImageBase[pRela->r_offset] =
					nSymVal + pRela->r_addend;
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
		auto &fileData = mod.fileMemory;
		auto &info     = mod.moduleInfo;

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
					symValue = reinterpret_cast<uint64_t>(pImageBase +
														  symbol.st_value);
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

bool ModuleLoader::relocateModule(MemoryMappedModule const &mod) const
{
	bool retVal = false;

	if (relocateRela(mod) && relocatePltRela(mod))
	{
		retVal = true;
	}

	return retVal;
}
