#include "SceModuleSystem.h"
#include "Platform/PlatformUtils.h"
#include "Util/UtilContainer.h"
#include "sce_modules.h"

LOG_CHANNEL(Emulator);

CSceModuleSystem::CSceModuleSystem():m_policyManager{m_moduleManager} {}

CSceModuleSystem::~CSceModuleSystem() {}


bool CSceModuleSystem::isNativeModuleLoadable(std::string const &modName) const
{
	return m_policyManager.isModuleLoadable(modName);
}

std::vector<NativeModule>& CSceModuleSystem::getAllNativeModules()
{
	return m_moduleManager.getNativeModules();
}

bool CSceModuleSystem::registerBuiltinModule(const SCE_EXPORT_MODULE &stModule)
{
	bool bRet = false;
	do
	{
		if (!stModule.szModuleName)
		{
			break;
		}

		const char *szModName          = stModule.szModuleName;
		const SCE_EXPORT_LIBRARY *pLib = stModule.pLibraries;

		m_moduleManager.registerBuiltinModule(szModName);

		while (!pLib->isEndEntry())
		{
			const char *szLibName            = pLib->szLibraryName;
			const SCE_EXPORT_FUNCTION *pFunc = pLib->pFunctionEntries;


			while (!pFunc->isEndEntry())
			{
				m_symbolManager.registerBuiltinSymbol(szModName,
													  szLibName,
													  pFunc->nNid,
													  pFunc->pFunction);

				m_symbolManager.registerBuiltinSymbol(szModName,
													  szLibName,
													  pFunc->szFunctionName,
													  pFunc->pFunction);

				pFunc = pFunc->iterNext();
			}
			pLib = pLib->iterNext();
		}
		bRet = true;
	} while (false);
	return bRet;
}

bool CSceModuleSystem::registerNativeSymbol(std::string const &modName,
										std::string const &libName,
										uint64_t nid,
										void *p)
{
	m_symbolManager.registerNativeSymbol(modName, libName, nid, p);
	return true;
}

bool CSceModuleSystem::registerNativeSymbol(std::string const &modName,
											std::string const &libName,
											std::string const &symbolName, 
											void *p)
{
	m_symbolManager.registerNativeSymbol(modName, libName, symbolName, p);
	return true;
}

bool CSceModuleSystem::registerNativeModule(std::string const &modName,
												  NativeModule &&mod)
{
	m_moduleManager.registerNativeModule(modName, std::move(mod));
	return true;
}

Policy CSceModuleSystem::getModulePolicy(std::string const& modName) const
{
	return m_policyManager.getModulePolicy(modName);
}

Policy CSceModuleSystem::getSymbolPolicy(std::string const& modName,
										 std::string const& libName,
										 uint64_t nid) const
{
	return m_policyManager.getSymbolPolicy(modName, libName, nid);
}

Policy CSceModuleSystem::getSymbolPolicy(std::string const& modName,
										 std::string const& libName,
										 std::string const& name) const
{
	return m_policyManager.getSymbolPolicy(modName, libName, name);
}

const SymbolManager& CSceModuleSystem::getSymbolManager() const
{
	return m_symbolManager;
}

PolicyManager& CSceModuleSystem::getPolicyManager()
{
	return m_policyManager;
}

bool CSceModuleSystem::isNativeModuleLoaded(std::string const &modName)
{
	return m_moduleManager.isNativeModuleLoaded(modName);
}


bool CSceModuleSystem::getNativeModule(std::string const &modName,
											 NativeModule **ppMod)
{
	return m_moduleManager.getNativeModule(modName, ppMod);
}


bool CSceModuleSystem::isFileAllowedToLoad(std::string const &fileName)
{
	bool retVal = false;

	std::string name      = {};
	std::string extension = {};

	auto ret = UtilPath::splitFileName(fileName, &name, &extension);

	do
	{
		if (ret == false)
		{
			retVal = false;
			break;
		}

		if (extension == "prx" || extension == "sprx")
		{
			if (isNativeModuleLoadable(name))
			{
				retVal = true;
			}
			else
			{
				retVal = false;
			}
		}
		else
		{
			retVal = false;
		}
	} while (false);

	return retVal;
}

const MODULE_INFO* CSceModuleSystem::getEbootModuleInfo() const
{
	const MODULE_INFO* retVal = nullptr;
	do
	{
		auto &modules = m_moduleManager.getNativeModules();
		if (modules.size() == 0)
		{
			break;
		}

		const auto &ebootModule = modules.at(0);
		retVal                  = &ebootModule.getModuleInfo();
	} while (false);
	return retVal;
}

const void* CSceModuleSystem::getSymbolAddress(std::string const& modName,
											   std::string const& libName,
										       uint64_t nid) const
{
	auto policy = m_policyManager.getSymbolPolicy(modName, libName, nid);
	void* address = nullptr;

	if (policy == Policy::UseBuiltin)
	{
		address = const_cast<void*>(m_symbolManager.findBuiltinSymbol(modName, libName, nid));
	}
	else
	{
		address = const_cast<void*>(m_symbolManager.findNativeSymbol(modName, libName, nid));
	}

	return address;
}

// TODO: To be done
void CSceModuleSystem::clearModules()
{
}
