#pragma once

#include "GPCS4Common.h"
#include "ELFMapper.h"

#include "Emulator/ModuleSystemCommon.h"
#include "Emulator/Linker.h"
#include "Emulator/SceModuleSystem.h"
#include "Emulator/TLSHandler.h"

#include <queue>
#include <set>
#include <string>

class ModuleLoader
{
public:
	ModuleLoader(CSceModuleSystem &modSystem, CLinker &linker);
	bool loadModule(std::string const &fileName, MemoryMappedModule **mod);
private:
	bool loadModuleFromFile(std::string const &fileName,
							MemoryMappedModule *mod,
							bool *exist);
	bool loadDependencies();
	bool addDepedenciesToLoad(MemoryMappedModule const &mod);
	bool mapModuleNameToFilePath(std::string const &modName, std::string *path);
	bool mapFilePathToModuleName(std::string const &filePath,
								 std::string *modName);

	bool registerSymbol(MemoryMappedModule const &mod,
						std::string const &encName,
						void *pointer);
	bool registerSymbol(MemoryMappedModule const &mod, size_t idx);
	bool initializeModules();

private:
	std::queue<std::string> m_filesToLoad;
	CSceModuleSystem &m_modSystem;
	CLinker &m_linker;
	ELFMapper m_mapper;

	// init_proc of modules in this black list will not be called.
	const static std::set<std::string> m_moduleInitBlackList;
};
