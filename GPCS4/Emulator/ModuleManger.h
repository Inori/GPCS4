#pragma once
#include "Module.h"

struct SCE_EXPORT_FUNCTION;
struct SCE_EXPORT_LIBRARY;
struct SCE_EXPORT_MODULE;

class ModuleManager
{
public:
	void registerBuiltinModule(std::string const &modName);
	void registerNativeModule(std::string const& modName, MemoryMappedModule&& mod);

	bool isBuiltinModuleDefined(std::string const& modName) const;

	bool isNativeModuleLoaded(std::string const& modName);
	bool getNativeModule(std::string const& modName, MemoryMappedModule** modOut);
	std::vector<MemoryMappedModule>& getNativeModules();

	const std::vector<MemoryMappedModule>& getNativeModules() const;

private:
	std::vector<std::string> m_builtinModules;
	std::vector<MemoryMappedModule> m_nativeModules;
	std::map<std::string, uint64_t> m_nativeModuleNameIndexMap;
};
