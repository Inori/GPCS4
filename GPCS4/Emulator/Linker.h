#pragma once

#include "GPCS4Common.h"
#include "SceModuleSystem.h"
#include "Module.h"
#include <string>

class CLinker
{
public:
	CLinker();
	CLinker(CSceModuleSystem &modSystem): m_modSystem{modSystem} {};

	bool ResolveSymbol(const std::string& strModName, const std::string& strLibName, uint64 nNid, void** ppAddress);
	bool resolveSymbol(MemoryMappedModule const &mod, std::string const &encName, uint64_t *addr);
	bool relocateModules();

private:
	bool relocateModule(MemoryMappedModule const &mod);
	bool relocateRela(MemoryMappedModule const &mod);
	bool relocatePltRela(MemoryMappedModule const &mod);
	CSceModuleSystem &m_modSystem;

};


