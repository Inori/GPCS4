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

		retVal = loadDepedencies();
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

bool ModuleLoader::loadDepedencies()
{
	bool retVal = true;

	while (!m_modulesToLoad.empty())
	{
		auto modName = m_modulesToLoad.front();
		m_modulesToLoad.pop();
		
		if (!m_modSystem->isModuleLoadable(modName))
		{
			LOG_DEBUG("module %s is not loadable", modName.c_str());
			continue;
		}

		std::string path = {};
		retVal = mapModuleNameToFilePath(modName, &path);
		if (!retVal)
		{
			LOG_ERR("unable to locate module %s", modName.c_str());
			break;
		}

		MemoryMappedModule mod = {};
		retVal = loadModuleFromFile(path, &mod);
		if (!retVal)
		{
			LOG_ERR("fail to get module name from file name");
			break;
		}
	}

	return retVal;
}

bool ModuleLoader::addDepedenciesToLoad(MemoryMappedModule const &mod)
{
	for (auto const &importMod : mod.importModules)
	{
		m_modulesToLoad.push(importMod.strName);
	}

	return true;
}

bool ModuleLoader::mapModuleNameToFilePath(std::string const & modName, std::string * path)
{
	LOG_FIXME("not implemented");
	return false;
}

bool ModuleLoader::mapFilePathToModuleName(std::string const & filePath, std::string * modName)
{
	LOG_FIXME("not implemented");
	return false;
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
