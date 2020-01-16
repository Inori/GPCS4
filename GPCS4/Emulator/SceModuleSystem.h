#pragma once

#include "GPCS4Common.h"
#include "UtilSingleton.h"
#include "Module.h"
#include <string>
#include <unordered_map>

struct SCE_EXPORT_FUNCTION;
struct SCE_EXPORT_LIBRARY;
struct SCE_EXPORT_MODULE;

// Note:
// Here "override" always mean native over builtin

class CSceModuleSystem final : public Singleton<CSceModuleSystem>
{
	friend class  Singleton<CSceModuleSystem>;

public:
	struct LibraryRecord
	{
		enum class OverridingPolicy
		{
			DisallowList,
			AllowList,
		};

		OverridingPolicy mode;
		bool overrideable;
		std::unordered_map<uint64_t, bool> functions;
	};

	struct ModuleRecord
	{
		bool overridable;
		std::unordered_map<std::string, LibraryRecord> libraries;
	};

private:

	//since we won't insert or remove item after initialization
	//we use unordered_map instead of map
	typedef std::unordered_map<uint64_t, void*> NidFuncMap;
	typedef std::unordered_map<std::string, void*> NameFuncMap;

	typedef std::unordered_map<std::string, NidFuncMap> SceLibMapNid;
	typedef std::unordered_map<std::string, NameFuncMap> SceLibMapName;

	typedef std::unordered_map<std::string, SceLibMapNid> SceModuleMapNid;
	typedef std::unordered_map<std::string, SceLibMapName> SceModuleMapName;

	typedef std::unordered_map<std::string, bool> SceAllowedFileMap;
	typedef std::unordered_map<std::string, ModuleRecord> SceOverridableMapNid;
	typedef std::unordered_map<std::string, MemoryMappedModule> SceMappedModuleMap;

	typedef std::vector<MemoryMappedModule> SceMappedModuleList;
	typedef std::unordered_map<std::string, size_t> SceModuleNameIndexMap;

public:
	void clearModules();

	bool registerBuiltinModule(const SCE_EXPORT_MODULE& stModule);

	bool registerNativeFunction(std::string const &modName, std::string const &libName, uint64_t nid, void *p);

	bool registerSymbol(std::string const &modName,
						std::string const &libName,
						std::string const &symbolName,
						void *p);

	bool registerMemoryMappedModule(std::string const &modName, MemoryMappedModule &&mod);

	bool isMemoryMappedModuleLoaded(std::string const &modName);

	SceMappedModuleList &getMemoryMappedModules();

	bool getMemoryMappedModule(std::string const &modName, MemoryMappedModule **ppMod);
	
	void* findFunction(const std::string& strModName, const std::string& strLibName, uint64_t nNid);
	void *findSymbol(std::string const &modName, std::string const &libName, std::string const &symbolName);

	bool isModuleLoaded(const std::string& modName);

	bool isNativeModuleLoadable(std::string const &modName) const;

	bool isLibraryLoaded(std::string const &modName, std::string const &libName);

	bool setModuleOverridability(const std::string &modName, bool isOverridable);

	bool setLibraryOverridability(const std::string &modName,
								  const std::string &libName,
								  bool isOverridable,
								  LibraryRecord::OverridingPolicy mode = LibraryRecord::OverridingPolicy::DisallowList);

	bool setFunctionOverridability(const std::string &modName,
								 const std::string &libName,
								 uint64_t nid, bool isOverridable);

	bool isFileAllowedToLoad(std::string const &fileName);
	bool isModuleOverridable(std::string const &modName) const;
	bool isLibraryOverridable(std::string const &modName, std::string const &libName) const;
	bool isFunctionOverridable(std::string const &modName, std::string const &libName, uint64_t nid) const;

	const MODULE_INFO* getEbootModuleInfo() const;

private:
	// Check if a builtin module is defined
	bool isBuitinModuleDefined(std::string const& name) const;

	// Check if a module overridability defination is empty(does not contain any
	// library definations). This function also returns true when the module
	// overridability does not exist.
	bool isModuleOVRDDefinationEmpty(std::string const& modName) const;
	bool isLibraryOverridabilityDefined(std::string const &modName, std::string const &libName) const; 

	bool IsEndFunctionEntry(const SCE_EXPORT_FUNCTION* pFunc);
	bool IsEndLibraryEntry(const SCE_EXPORT_LIBRARY* pLib);

	CSceModuleSystem();
	~CSceModuleSystem();
	CSceModuleSystem(const CSceModuleSystem&);
	CSceModuleSystem& operator = (const CSceModuleSystem&) = delete;

private:
	SceModuleMapNid m_moduleNidMap;
	SceModuleMapName m_moduleSymbNameMap;
	SceOverridableMapNid m_overridableModules;

	std::vector<std::string> m_builtinModules;
	SceMappedModuleList m_mappedModules;
	SceModuleNameIndexMap m_mappedModuleNameIndexMap;
};

