#pragma once

#include "ELFMapper.h"
#include "Emulator/Linker.h"
#include "Emulator/SceModuleSystem.h"

#include <queue>

class ModuleLoader
{
public:
	ModuleLoader(CSceModuleSystem *modSystem);
	bool loadModule(std::string const &fileName);
private:
	bool loadModuleFromFile(std::string const &fileName, MemoryMappedModule *mod);
	bool loadDepedencies();
	bool addDepedenciesToLoad(MemoryMappedModule const &mod);
	bool mapModuleNameToFilePath(std::string const &modName, std::string *path);
	bool mapFilePathToModuleName(std::string const &filePath, std::string *modName);
	bool registerSymbol(std::string const &encName, void *pointer);

	std::queue<std::string> m_modulesToLoad;
	CSceModuleSystem *m_modSystem;
	ELFMapper m_mapper;
	CLinker m_linker;
};
