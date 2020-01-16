#include "GPCS4Common.h"

#include "Util/UtilContainer.h"
#include "PolicyManager.h"


void LibraryPolicy::excludeSymbol(uint64_t nid)
{
	auto pair = std::make_pair(nid, true);
	m_excludeNids.insert(pair);
}


void LibraryPolicy::excludeSymbol(std::string const &name)
{
	m_excludeNames.insert(std::make_pair(name, true));
}

void LibraryPolicy::setPolicy(Policy policy)
{
	m_policy = policy;
}


Policy LibraryPolicy::getSymbolPolicy(uint64_t nid) const
{
	Policy ret = Policy::UseBuiltin;
	do
	{
		if (m_policy == Policy::UseBuiltin)
		{
			if (util::contains(m_excludeNids, nid))
			{
				ret = Policy::UseNative;
			}
			else
			{
				ret = Policy::UseBuiltin;
			}
		}
		else
		{
			if (util::contains(m_excludeNids, nid))
			{
				ret = Policy::UseBuiltin;
			}
			else
			{
				ret = Policy::UseNative;
			}
		}
	} while (false);

	return ret;
}

Policy LibraryPolicy::getSymoblPolicy(std::string const& name) const
{
	Policy ret = Policy::UseBuiltin;
	do
	{
		if (m_policy == Policy::UseBuiltin)
		{
			if (util::contains(m_excludeNames, name))
			{
				ret = Policy::UseNative;
			}
			else
			{
				ret = Policy::UseBuiltin;
			}
		}
		else
		{
			if (util::contains(m_excludeNames, name))
			{
				ret = Policy::UseBuiltin;
			}
			else
			{
				ret = Policy::UseNative;
			}
		}
	} while (false);

	return ret;
}


LibraryPolicy& ModulePolicy::addLibraryPolicy(std::string const & name,
											  LibraryPolicy && lp)
{
	m_libPolicyTable.emplace(std::make_pair(name, std::move(lp)));
	return m_libPolicyTable.at(name);
}

bool ModulePolicy::getLibraryPolicy(std::string const & name,
	                                const LibraryPolicy ** p) const
{
	bool ret = false;
	do {
		auto lib = m_libPolicyTable.find(name);
		if (lib == m_libPolicyTable.end())
		{
			ret = false;
			break;
		}
		else 
		{
			*p = &lib->second;
			ret = true;
			break;
		}

	} while (false);

	return ret;
}

void ModulePolicy::setPolicy(Policy policy)
{
	m_policy = policy;
}

Policy ModulePolicy::getPolicy() const
{
	return m_policy;
}


PolicyManager::PolicyManager(ModuleManager& modMgr):
	m_modManager{modMgr}
{
}

bool PolicyManager::isModuleLoadable(std::string const &modName) const
{
	bool ret = false;
	do 
	{
		if (m_useNativeImplsWhenBuiltinModuleNotDefined &&
		 !m_modManager.isBuiltinModuleDefined(modName))
		{
			ret = true;
			break;
		}

		if (m_defaultPolicy == Policy::UseNative)
		{
			ret = true;
		}
		else
		{
			if (util::contains(m_modPolicyTable, modName))
			{
				ret = true;
			}
			else
			{
				ret = false;
			}
		}

	} while(false);

	return ret;
}

Policy PolicyManager::getModulePolicy(std::string const& modName) const
{
	Policy ret = m_defaultPolicy;
	do 
	{
		auto modIter = m_modPolicyTable.find(modName);
		if (modIter == m_modPolicyTable.end())
		{
			if (m_useNativeImplsWhenBuiltinModuleNotDefined &&
				!m_modManager.isBuiltinModuleDefined(modName))
			{
				ret = Policy::UseNative;
				break;
			}
			else
			{
				ret = m_defaultPolicy;
				break;
			}
		}
		else
		{
			ret = modIter->second.getPolicy();
			break;
		}
	} while (false);

	return ret;
}


Policy PolicyManager::getSymbolPolicy(std::string const &modName,
	                                  std::string const &libName,
                                      uint64_t nid) const
{
	Policy ret = m_defaultPolicy;
	do 
	{
		auto modIter = m_modPolicyTable.find(modName);
		if (modIter == m_modPolicyTable.end())
		{
			if (m_useNativeImplsWhenBuiltinModuleNotDefined &&
				!m_modManager.isBuiltinModuleDefined(modName))
			{
				ret = Policy::UseNative;
				break;
			}
			else
			{
				ret = m_defaultPolicy;
				break;
			}
		}
		
		const LibraryPolicy *libPolicy = nullptr;
		auto found = modIter->second.getLibraryPolicy(libName, &libPolicy);
		if (!found)
		{
			ret = getModulePolicy(modName);
			break;
		}

		ret = libPolicy->getSymbolPolicy(nid);

	} while (false);

	return ret;
}

Policy PolicyManager::getSymbolPolicy(std::string const& modName, std::string const& libName, std::string const& name) const
{
	Policy ret = m_defaultPolicy;
	do 
	{
		auto modIter = m_modPolicyTable.find(modName);
		if (modIter == m_modPolicyTable.end())
		{
			if (m_useNativeImplsWhenBuiltinModuleNotDefined &&
				!m_modManager.isBuiltinModuleDefined(modName))
			{
				ret = Policy::UseNative;
				break;
			}
			else
			{
				ret = m_defaultPolicy;
				break;
			}
		}
		
		const LibraryPolicy *libPolicy = nullptr;
		auto found = modIter->second.getLibraryPolicy(libName, &libPolicy);
		if (!found)
		{
			ret = m_defaultPolicy;
			break;
		}

		ret = libPolicy->getSymoblPolicy(name);

	} while (false);

	return ret;
}


ModulePolicyAdder PolicyManager::declareModule(std::string const & name)
{
	m_modPolicyTable.emplace(std::make_pair(name, ModulePolicy{}));
	return { m_modPolicyTable.at(name) };
}


LibrayNameAdder ModulePolicyAdder::withDefault(Policy policy)
{
	m_modPolicy.setPolicy(policy);
	return { m_modPolicy };
}


LibraryPolicyAdder LibrayNameAdder::declareSubLibrary(std::string const & name)
{
	auto &lp = m_modPolicy.addLibraryPolicy(name, LibraryPolicy{});
	return { m_modPolicy, lp };
}


LibrarySymbolExcluder LibraryPolicyAdder::with(Policy policy)
{
	m_policy.setPolicy(policy);
	return { m_modPolicy, m_policy };
}


LibrayNameAdder LibrarySymbolExcluder::except(std::initializer_list<uint64_t> nids)
{
	for (auto nid : nids)
	{
		m_policy.excludeSymbol(nid);
	}

	return { m_modPolicy };
}
