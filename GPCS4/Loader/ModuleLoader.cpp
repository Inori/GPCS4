#include "ModuleLoader.h"

#include "Platform/PlatformUtils.h"

LOG_CHANNEL(Loader.ModuleLoader);

#define ADD_BLACK_MODULE(name) (name".sprx")

const std::set<std::string> ModuleLoader::m_moduleInitBlackList = 
{
	ADD_BLACK_MODULE("libSceNpScoreRanking"),
	ADD_BLACK_MODULE("libSceAppContent"),
	ADD_BLACK_MODULE("libSceDipsw"),
};

ModuleLoader::ModuleLoader(CSceModuleSystem &modSystem,
						   CLinker &linker)
	: 
	m_modSystem{ modSystem }, 
	m_linker{ linker }
{
}

bool ModuleLoader::loadModule(std::string const &fileName,
							  NativeModule **modOut)
{
	bool retVal = false;
	do
	{
		NativeModule mod = {};

		bool exist = false;
		retVal = loadModuleFromFile(fileName, &mod, &exist);
		if (!retVal)
		{
			break;
		}

// Output NIDs of functions that are not implemented in HLE.
#ifdef MODSYS_OUTPUT_NOT_IMPLEMENTED_HLE
		mod.outputUnresolvedSymbols("unresolved_HLE.txt");
#endif // MODSYS_OUTPUT_NOT_IMPLEMENTED_HLE

		retVal = m_modSystem.registerNativeModule(mod.fileName, std::move(mod));
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

		*modOut = &(m_modSystem.getAllNativeModules()[0]);
		retVal  = true;
	} while (false);

	return retVal;
}

bool ModuleLoader::loadModuleFromFile(std::string const &fileName,
									  NativeModule *mod,
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

		bool isLoaded = m_modSystem.isNativeModuleLoaded(mod->fileName);
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

		retVal = m_mapper.mapImageIntoMemory();
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
	bool moduleNotFoundIgnore = false;

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
		auto mod = NativeModule{};
		retVal   = loadModuleFromFile(path, &mod, &exist);
		if (!retVal)
		{
			LOG_ERR("Failed to load module %s", fileName.c_str());

#ifdef MODSYS_IGNORE_NOT_FOUND_MODULES
			moduleNotFoundIgnore = true;
			continue;
#else  // MODSYS_IGNORE_NOT_FOUND_MODULES
			break;
#endif // MODSYS_IGNORE_NOT_FOUND_MODULES
		}

		if (!exist)
		{
			retVal = m_modSystem.registerNativeModule(mod.fileName, std::move(mod));
			if (!retVal)
			{
				LOG_ERR("Failed to register module: %s", mod.fileName.c_str());
				break;
			}
		}
	}

	return retVal || moduleNotFoundIgnore;
}

bool ModuleLoader::addDepedenciesToLoad(NativeModule const &mod)
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

bool ModuleLoader::registerSymbol(NativeModule const &mod,
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

		retVal = m_modSystem.registerNativeSymbol(modName, libName, nid, pointer);
		if (!retVal)
		{
			break;
		}

		retVal = true;
	} while (false);

	return retVal;
}

bool ModuleLoader::registerSymbol(NativeModule const &mod, size_t idx)
{
	const SymbolInfo *info = nullptr;
	mod.getSymbol(idx, &info);

	if (info->isEncoded)
	{
		m_modSystem.registerNativeSymbol(info->moduleName, info->libraryName, info->nid,
									 reinterpret_cast<void *>(info->address));
	}
	else
	{
		m_modSystem.registerNativeSymbol(info->moduleName, info->libraryName,
										 info->symbolName,
										 reinterpret_cast<void *>(info->address));
	}

	return true;
}


bool ModuleLoader::initializeModules()
{
	auto &mods  = m_modSystem.getAllNativeModules();
	bool retVal = true;

	auto tlsManager = TLSManager::GetInstance();
	uint32_t tlsIndex = TLS_MODULE_ID_MAIN;
	// intialize TLS
	for (auto const &mod : mods)
	{
		void *pTls     = nullptr;
		uint32_t initSize  = 0;
		uint32_t totalSize = 0;
		uint32_t align     = 0;
		retVal         = mod.getTLSInfo(&pTls, &initSize, &totalSize, &align);
		if (!retVal || pTls == nullptr)
		{
			LOG_DEBUG("no TLS info for module:%s", mod.fileName.c_str());
			continue;
		}

		TLSBlock block;
		block.address   = pTls;
		block.initSize  = initSize;
		block.totalSize = totalSize;
		block.align     = align;
		block.index     = tlsIndex;
		block.isDynamic = false;
		block.offset    = 0;
		tlsManager->registerTLSBlock(block);
		
		++tlsIndex;
	}

	// skip eboot.bin
	for (size_t i = 1; i < mods.size(); i++)
	{
		if (m_moduleInitBlackList.find(mods[i].fileName) != m_moduleInitBlackList.end())
		{
			// skip black list modules
			continue;
		}

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


