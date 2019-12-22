#include "ModuleLoader.h"
#include "Platform/PlatformUtils.h"


ModuleLoader::ModuleLoader(CSceModuleSystem &modSystem,
						   CLinker &linker,
						   CTLSHandler &tlsHandler)
	: 
	m_modSystem{ modSystem }, 
	m_tlsHandler{ tlsHandler },
	m_linker{ linker }
{
}

bool ModuleLoader::loadModule(std::string const &fileName,
							  MemoryMappedModule **modOut)
{
	bool retVal = false;

	do
	{
		MemoryMappedModule mod = {};

		bool exist = false;
		retVal = loadModuleFromFile(fileName, &mod, &exist);
		if (!retVal)
		{
			break;
		}

// Output NIDs of functions that are not implemented in HLE.
#ifdef OUTPUT_NOT_IMPLEMENTED_HLE
		mod.outputUnresolvedSymbols("unresolved_HLE.txt");
#endif // OUTPUT_NOT_IMPLEMENTED_HLE

		retVal = m_modSystem.registerMemoryMappedModule(mod.fileName, std::move(mod));
		if (!retVal)
		{
			LOG_ERR("Failed to register module: %s", mod.fileName.c_str());
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
									  MemoryMappedModule *mod,
									  bool *exist)
{
	LOG_ASSERT(mod != nullptr, "mod is nullpointer");
	LOG_ASSERT(exist != nullptr, "mod is nullpointer");

	*exist = false;
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
			retVal = true;
			*exist = true;
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

		retVal = addDepedenciesToLoad(*mod);
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

		bool exist = false;
		auto mod = MemoryMappedModule{};
		retVal   = loadModuleFromFile(path, &mod, &exist);
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

		if (!exist)
		{
			retVal = m_modSystem.registerMemoryMappedModule(mod.fileName, std::move(mod));
			if (!retVal)
			{
				LOG_ERR("Failed to register module: %s", mod.fileName.c_str());
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

// TODO: This is a temporary implenmentation.
// Should be configurable.
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
