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
	// TODO: Doc
	const void *getSymbolAddress(std::string const& modName,
								 std::string const& libName,
								 uint64_t nid) const;
	// TODO: Doc
	void clearModules();
	// TODO: Doc
	const MODULE_INFO* getEbootModuleInfo() const;
	// TODO: Doc
	bool isFileAllowedToLoad(std::string const &fileName);
	// TODO: Doc
	bool isNativeModuleLoadable(std::string const &modName) const;
	// TODO: Doc
	std::vector<MemoryMappedModule> &getMemoryMappedModules(); 
	// TODO: Doc
	bool getMemoryMappedModule(std::string const &modName,
							   MemoryMappedModule **ppMod);
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

