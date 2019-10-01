#pragma once

#include "ELFMapper.h"
#include "Emulator/Linker.h"
#include "Emulator/SceModuleSystem.h"

#include <queue>

class ModuleLoader
{
public:
	ModuleLoader(CSceModuleSystem &modSystem);
	bool loadModule(std::string const &fileName);
	bool relocateModule(MemoryMappedModule const &mod) const;

private:
	bool loadModuleFromFile(std::string const &fileName,
							MemoryMappedModule *mod);
	bool loadDependencies();
	bool addDepedenciesToLoad(MemoryMappedModule const &mod);
	bool mapModuleNameToFilePath(std::string const &modName, std::string *path);
	bool mapFilePathToModuleName(std::string const &filePath,
								 std::string *modName);

	bool registerSymbol(MemoryMappedModule const &mod,
						std::string const &encName,
						void *pointer);

	bool relocateRela(MemoryMappedModule const &mod) const;
	bool relocatePltRela(MemoryMappedModule const &mod) const;

	std::queue<std::string> m_filesToLoad;
	CSceModuleSystem &m_modSystem;
	ELFMapper m_mapper;
	CLinker m_linker;
};
