#include "ModuleLoader.h"

ModuleLoader::ModuleLoader(CSceModuleSystem * modSystem):
	m_modSystem(modSystem)
{
}

bool ModuleLoader::loadModule(std::string const & fileName)
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

		retVal = true;

	} while (false);

	return retVal;
}

bool ModuleLoader::loadModuleFromFile(std::string const & fileName, MemoryMappedModule *mod)
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
		retVal = mapFilePathToModuleName(fileName, &moduleName);
		if (!retVal)
		{
			break;
		}

		for (auto &symbol : mod->exportSymbols)
		{
			registerSymbol(symbol.first, symbol.second);
		}

		m_modSystem->RegisterMemoryMappedModule(moduleName, std::move(*mod));
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
		
		if (!m_modSystem->isFileAllowedToLoad(fileName))
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

		MemoryMappedModule mod = {};
		retVal = loadModuleFromFile(path, &mod);
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

bool ModuleLoader::mapModuleNameToFilePath(std::string const & modName, std::string * path)
{
	LOG_FIXME("Test only implementation");
	*path = modName;
	return true;
}

bool ModuleLoader::mapFilePathToModuleName(std::string const & filePath, std::string * modName)
{
	// TODO: an unified implementation is needed.
	bool retVal = false;
	do
	{
		auto idx = filePath.find_last_of("/\\");
		if (idx == std::string::npos)
		{
			retVal = true;
			*modName = filePath;
			break;
		}

		if (idx == filePath.size() - 1)
		{
			LOG_ERR("file name %s is invalid", filePath.c_str());
			break;
		}

		*modName = filePath.substr(idx + 1);
		retVal = true;
	} while (false);

	return retVal;
}

bool ModuleLoader::registerSymbol(std::string const & encName, void * pointer)
{
	bool retVal = false;

	do
	{
		uint modId = 0, libId = 0;
		uint64_t funcNid = 0;

		retVal = m_mapper.decodeEncodedName(encName, &modId, &libId, &funcNid);
		if (!retVal)
		{
			break;
		}

		std::string modName = {};
		std::string libName = {};

		retVal = m_mapper.getModuleNameFromId(modId, &modName);
		if (!retVal)
		{
			break;
		}

		retVal = m_mapper.getLibraryNameFromId(libId, &libName);
		if (!retVal)
		{
			break;
		}

		m_modSystem->RegisterFunction(modName, libName, funcNid, pointer);

	} while (false);

	return retVal;
}
