#pragma once

#include "GPCS4Common.h"
#include "UtilSingleton.h"
#include "PolicyManager.h"
#include "SymbolManager.h"
#include "ModuleManger.h"

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



private:
	SceModuleMapNid m_moduleNidMap;
	SceModuleMapName m_moduleSymbNameMap;
	SceOverridableMapNid m_overridableModules;

	std::vector<std::string> m_builtinModules;
	SceMappedModuleList m_mappedModules;
	SceModuleNameIndexMap m_mappedModuleNameIndexMap;
//////////////////////////////////////////////////////////////////
/// NEW APIS
public:
	/**
	 * @brief Checks if a native module is loaded from firmware.
	 * 
	 * @param modName module name
	 * @return true when the native module is loaded.
	 * @return false when the native module isn't loaded.
	 */
	bool isNativeModuleLoaded(std::string const &modName);

	/**
	 * @brief Registers a builtin module to the module system
	 * 
	 * @param stModule module defination
	 * @return true when succeeded
	 * @return false when failed
	 */
	bool registerBuiltinModule(const SCE_EXPORT_MODULE& stModule);

	/**
	 * @brief Registers a native module to the module system.
	 * The ownship of the module is transfered after calling 
	 * this method.
	 * 
	 * @param modName module name
	 * @param mod module
	 * @return true when succeeded
	 * @return false when failed
	 */
	bool registerNativeModule(std::string const &modName,
									MemoryMappedModule &&mod);
	
	/**
	 * @brief Registers a symbol from native module
	 * 
	 * @param modName module name
	 * @param libName library name
	 * @param nid symbol NID
	 * @param p address
	 * @return true when succeeded
	 * @return false when failed
	 */
	bool registerNativeSymbol(std::string const &modName,
								std::string const &libName,
								uint64_t nid,
								void *p);
	/**
	 * @brief Registers a symbol from native modue
	 * 
	 * @param modName module name
	 * @param libName library name
	 * @param symbolName symbol name
	 * @param p address
	 * @return true when succeeded
	 * @return false when failed
	 */
	bool registerNativeSymbol(std::string const &modName,
							  std::string const &libName,
							  std::string const &symbolName,
							  void *p);

	/**
	 * @brief Retrieves policy of a module
	 * 
	 * @param modName module name
	 * @return Policy 
	 */
	Policy getModulePolicy(std::string const& modName) const;

	/**
	 * @brief Retrieves policy of a symbol 
	 * 
	 * @param modName module name
	 * @param libName library name
	 * @param nid symbol NID
	 * @return Policy 
	 */
	Policy getSymbolPolicy(std::string const &modName,
						   std::string const &libName,
						   uint64_t nid) const;

	/**
	 * @brief Retrieves policy of a symbol 
	 * 
	 * @param modName module name
	 * @param libName library name
	 * @param name symbol name
	 * @return Policy 
	 */
	Policy getSymbolPolicy(std::string const& modName,
						   std::string const& libName,
						   std::string const& name) const;
	
	/**
	 * @brief Gets the Symbol Manager object
	 * 
	 * @return const SymbolManager& 
	 */
	const SymbolManager& getSymbolManager() const;

	/**
	 * @brief Gets the Policy Manager object
	 * 
	 * @return PolicyManager& 
	 */
	PolicyManager& getPolicyManager();
private:
	CSceModuleSystem();
	~CSceModuleSystem();
	CSceModuleSystem(const CSceModuleSystem&) = delete;
	CSceModuleSystem& operator = (const CSceModuleSystem&) = delete;
	bool IsEndFunctionEntry(const SCE_EXPORT_FUNCTION* pFunc);
	bool IsEndLibraryEntry(const SCE_EXPORT_LIBRARY* pLib);

	ModuleManager m_moduleManager;
	SymbolManager m_symbolManager;
	PolicyManager m_policyManager;
};

