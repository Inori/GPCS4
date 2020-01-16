#include "Util/UtilContainer.h"
#include "ModuleManger.h"

void ModuleManager::registerBuiltinModule(std::string const& modName)
{
	m_builtinModules.emplace_back(modName);
}

void ModuleManager::registerNativeModule(std::string const& modName,
										 NativeModule&& mod)
{
	auto index = m_nativeModules.size();

	m_nativeModuleNameIndexMap.emplace(std::make_pair(modName, index));
	m_nativeModules.emplace_back(std::move(mod));
}

bool ModuleManager::isBuiltinModuleDefined(std::string const& modName) const
{
	return util::contains(m_builtinModules, modName);
}

bool ModuleManager::isNativeModuleLoaded(std::string const& modName)
{
	return util::contains(m_nativeModuleNameIndexMap, modName);
}

bool ModuleManager::getNativeModule(std::string const& modName,
									NativeModule** modOut)
{
	bool ret  = false;
	do
	{
		auto iter = m_nativeModuleNameIndexMap.find(modName); 
		if (iter == m_nativeModuleNameIndexMap.end())
		{
			ret = false;
			break;
		}

		auto &mod = m_nativeModules.at(iter->second);
		*modOut   = &mod;
		ret = true;

	} while (false);

	return ret;
}

std::vector<NativeModule>& ModuleManager::getNativeModules()
{
	return m_nativeModules;
}
const std::vector<NativeModule>& ModuleManager::getNativeModules() const
{
	return m_nativeModules;
}


