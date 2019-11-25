#pragma once

#include "GPCS4Common.h"
#include "Emulator/ModuleSystemCommon.h"
#include "Emulator/Linker.h"
#include "Emulator/SceModuleSystem.h"
#include "Emulator/TLSHandler.h"
#include "ELFMapper.h"

#include <queue>

class ModuleLoader
{
public:
	ModuleLoader(CSceModuleSystem &modSystem, CLinker &linker, CTLSHandler &tlsHandler);
	bool loadModule(std::string const &fileName, MemoryMappedModule **mod);
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
	bool registerSymbol(MemoryMappedModule const &mod, size_t idx);
	bool relocateRela(MemoryMappedModule const &mod) const;
	bool relocatePltRela(MemoryMappedModule const &mod) const;
	bool initializeModules();

	std::queue<std::string> m_filesToLoad;
	CSceModuleSystem &m_modSystem;
	CTLSHandler &m_tlsHandler;
	CLinker &m_linker;
	ELFMapper m_mapper;
};
