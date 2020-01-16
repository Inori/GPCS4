#include "SymbolManager.h"

const void *SymbolManager::findNativeSymbol(std::string const &modName,
	                                   std::string const &libName,
	                                   uint64_t nid) const
{
	return findSymbolGeneric(m_nativeModuleSymbolNidDir, modName, libName, nid);
}

const void* SymbolManager::findNativeSymbol(std::string const& modName,
									  std::string const& libName,
									  std::string const& symbName) const
{
	return findSymbolGeneric(m_nativeModuleSymbolNameDir, modName, libName, symbName);
}

const void* SymbolManager::findBuiltinSymbol(std::string const& modName,
											 std::string const& libName,
											 uint64_t nid) const
{
	return findSymbolGeneric(m_builtinModuleSymbolNidDir, modName, libName, nid);
}

const void* SymbolManager::findBuiltinSymbol(std::string const& modName, 
											 std::string const& libName,
											 std::string const& name) const
{
	return findSymbolGeneric(m_builtinModuleSymbolNameDir, modName, libName, name);
}

bool SymbolManager::registerNativeSymbol(std::string const& modName,
										 std::string const& libName,
										 uint64_t nid,
										 const void* address)
{
	return registerSymbolNidGeneric(m_nativeModuleSymbolNidDir,
									modName,
									libName,
									nid,
									address);
}

bool SymbolManager::registerNativeSymbol(std::string const& modName,
										 std::string const& libName,
										 std::string const& name,
										 const void* address)
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
										  const void* address)
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
										  const void* address)
{
	return registerSymbolNidGeneric(m_builtinModuleSymbolNameDir,
									modName,
									libName,
									name,
									address);
}


