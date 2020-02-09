#pragma once

#include "GPCS4Common.h"
#include "SceModuleSystem.h"
#include "Module.h"
#include <string>

class CLinker
{
public:
	CLinker();
	CLinker(CSceModuleSystem &modSystem) : m_modSystem{modSystem} {};

	bool resolveSymbol(NativeModule const &mod,
					   std::string const &name,
					   uint64_t *addr) const;

	bool relocateModules();

private:
	void* getSymbolAddress(std::string const &modName, std::string const& libName, uint64_t nid) const;
	void* getSymbolAddress(std::string const& modName, std::string const& libName, std::string const& symbName) const;
	bool relocateModule(NativeModule &mod);
	bool relocateRela(NativeModule &mod);
	bool relocatePltRela(NativeModule &mod);
	void* generateStubFunction(const SymbolInfo* sybInfo, void* oldFunc) const;

private:
	CSceModuleSystem &m_modSystem;
};
