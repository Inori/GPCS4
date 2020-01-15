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

	Policy getModulePolicy(std::string const &modName) const;

	Policy getSymbolPolicy(std::string const &modName,
	                      std::string const &libName,
                          uint64_t nid) const;

	Policy getSymbolPolicy(std::string const &modName,
	                      std::string const &libName,
                          std::string const& name) const;
	
	ModulePolicyAdder addModule(std::string const &name);

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

	LibrayNameAdder exclude(std::initializer_list<uint64_t> nids);
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

	LibraryPolicyAdder addLibrary(std::string const &name);

private:
	ModulePolicy &m_modPolicy;
};

class ModulePolicyAdder
{
public:
	ModulePolicyAdder(ModulePolicy &mp):
		m_modPolicy{ mp } {}

	LibrayNameAdder with(Policy policy);

private:
	ModulePolicy &m_modPolicy;
};


