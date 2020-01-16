#include "ModuleManger.h"

#include <map>
#include <cstdint>

enum class Policy
{
    UseBuiltin = 0,
    UseNative  = 1,
};

class LibraryPolicy
{
public:
    using NidList = std::initializer_list<uint64_t>;

	LibraryPolicy() = default;
    void excludeSymbol(uint64_t nid);
    void excludeSymbol(std::string const &name);
    void setPolicy(Policy policy);

    Policy getSymbolPolicy(uint64_t nid) const;
    Policy getSymoblPolicy(std::string const&  name) const;

private:
    Policy m_policy;
    std::map<uint64_t, bool> m_excludeNids;
    std::map<std::string, bool> m_excludeNames;
};


class ModulePolicy
{
public:
    using LibPolicyEntry = std::pair<std::string, LibraryPolicy>;
    using LibraryPolicyList = std::initializer_list<LibPolicyEntry>;

	ModulePolicy() = default;
	LibraryPolicy& addLibraryPolicy(std::string const &name, LibraryPolicy &&lp);
    bool getLibraryPolicy(std::string const &name, const LibraryPolicy **p) const;
	void setPolicy(Policy policy);
	Policy getPolicy() const;

private:
    Policy m_policy;
    std::map<std::string, LibraryPolicy> m_libPolicyTable;
};


class ModulePolicyAdder;
class LibrayNameAdder;

class PolicyManager
{
public:
	using ModulePolicyEntry = std::pair<std::string, ModulePolicy>;
	PolicyManager(ModuleManager& modMgr);

	/**
	 * @brief Retireves the policy of a module
	 * 
	 * @param modName module name
	 * @return Policy 
	 */
	Policy getModulePolicy(std::string const &modName) const;

	/**
	 * @brief Retrieves the policy of a symbol
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
	 * @brief Retrieves the policy of a symbol
	 * 
	 * @param modName module name
	 * @param libName library name
	 * @param name symbol name
	 * @return Policy 
	 */
	Policy getSymbolPolicy(std::string const &modName,
	                      std::string const &libName,
                          std::string const& name) const;

	/**
	 * @brief Checks if a native module is loadable.
	 * 
	 * @param modName module name
	 * @return true if the module if loadable
	 * @return false if the module is not loadable
	 */
	bool isModuleLoadable(std::string const &modName) const;
	
	/**
	 * @brief Declares policy for a module
	 * 
	 * @param name module name
	 * @return ModulePolicyAdder 
	 */
	ModulePolicyAdder declareModule(std::string const &name);

private:
	const Policy m_defaultPolicy = Policy::UseBuiltin;
	const bool m_useNativeImplsWhenBuiltinModuleNotDefined = true;	
    std::map<std::string, ModulePolicy> m_modPolicyTable;
	ModuleManager& m_modManager;
};



///////////////////////////////////////////////////////////////////////////////

class LibrarySymbolExcluder
{
public:
	LibrarySymbolExcluder(ModulePolicy &mp, LibraryPolicy &lp) :
		m_policy{ lp }, m_modPolicy{ mp }{};

	LibrayNameAdder except(std::initializer_list<uint64_t> nids);
private:
	LibraryPolicy &m_policy;
	ModulePolicy &m_modPolicy;
};

class LibraryPolicyAdder
{
public:
	LibraryPolicyAdder(ModulePolicy &mp, LibraryPolicy &lp) 
		: m_policy{ lp }, m_modPolicy{ mp } {};

	LibrarySymbolExcluder with(Policy policy);

private:
	LibraryPolicy &m_policy;
	ModulePolicy &m_modPolicy;
};

class LibrayNameAdder
{
public:
	LibrayNameAdder(ModulePolicy &mp):
		m_modPolicy{mp} {}

	LibraryPolicyAdder declareSubLibrary(std::string const &name);

private:
	ModulePolicy &m_modPolicy;
};

class ModulePolicyAdder
{
public:
	ModulePolicyAdder(ModulePolicy &mp):
		m_modPolicy{ mp } {}

	LibrayNameAdder withDefault(Policy policy);

private:
	ModulePolicy &m_modPolicy;
};


