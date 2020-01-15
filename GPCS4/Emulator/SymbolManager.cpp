#include "SymbolManager.h"

void *SymbolManager::findNativeSymbol(std::string const &modName,
	                                   std::string const &libName,
	                                   uint64_t nid)
{
	return findSymbolGeneric(m_nativeModuleSymbolNidDir, modName, libName, nid);
}

void* SymbolManager::findNativeSymbol(std::string const& modName,
									  std::string const& libName,
									  std::string const& symbName)
{
	return findSymbolGeneric(m_nativeModuleSymbolNameDir, modName, libName, symbName);
}

void* SymbolManager::findBuiltinSymbol(std::string const& modName,
									   std::string const& libName,
									   uint64_t nid)
{
	return findSymbolGeneric(m_builtinModuleSymbolNidDir, modName, libName, nid);
}

void* SymbolManager::findBuiltinSymbol(std::string const& modName,
									   std::string const& libName,
									   std::string const& name)
{
	return findSymbolGeneric(m_builtinModuleSymbolNameDir, modName, libName, name);
}

bool SymbolManager::registerNativeSymbol(std::string const& modName,
										 std::string const& libName,
										 uint64_t nid,
										 void* address)
{
	return registerSymbolNidGeneric(m_nativeModuleSymbolNidDir,
									modName,
									libName,
									nid,
									address);
}

bool SymbolManager::registerNativeSymbol(std::string const& modName, std::string const& libName, std::string const& name, void* address)
{
	return registerSymbolNidGeneric(m_nativeModuleSymbolNameDir,
									modName,
									libName,
									name,
									address);
}

bool SymbolManager::registerBuiltinSymbol(std::string const& modName,
										  std::string const& libName,
										  uint64_t nid,
										  void* address)
{
	return registerSymbolNidGeneric(m_builtinModuleSymbolNidDir,
									modName,
									libName,
									nid,
									address);
}

bool SymbolManager::registerBuiltinSymbol(std::string const& modName,
										  std::string const& libName,
										  std::string const& name,
										  void* address)
{
	return registerSymbolNidGeneric(m_builtinModuleSymbolNameDir,
									modName,
									libName,
									name,
									address);
}


