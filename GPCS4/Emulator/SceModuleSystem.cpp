#include "SceModuleSystem.h"
#include "Platform/PlatformUtils.h"
#include "Util/UtilContainer.h"
#include "sce_modules.h"

LOG_CHANNEL(Emulator);

CSceModuleSystem::CSceModuleSystem() {}

CSceModuleSystem::~CSceModuleSystem() {}

bool CSceModuleSystem::IsEndLibraryEntry(const SCE_EXPORT_LIBRARY *pLib)
{
	return (pLib->szLibraryName == NULL && pLib->pFunctionEntries == NULL);
}

bool CSceModuleSystem::isNativeModuleLoadable(std::string const &modName) const
{
	bool retVal = false;

	if (m_moduleSymbNameMap.count(modName) == 0)
	{
		retVal = true;
	}
	else if (isModuleOverridable(modName))
	{
		retVal = true;
	}
	else
	{
		retVal = false;
	}

	return retVal;
}

bool CSceModuleSystem::isModuleOverridable(std::string const &modName) const
{
	bool ret = false;

	do
	{
		if (util::contains(m_overridableModules, modName) == true)
		{
			ret = m_overridableModules.at(modName).overridable;
			break;
		}
		else
		{
			if (isBuitinModuleDefined(modName) == false)
			{
				ret = true;
			}
			else
			{
				ret = false;
			}
			break;
		}

	} while (false);

	return ret;
}

bool CSceModuleSystem::isLibraryOverridable(std::string const &modName,
											std::string const &libName) const
{
	bool retVal = false;

	do
	{
		if (isModuleOverridable(modName) == false)
		{
			retVal = false;
			break;
		}

		if (isModuleOVRDDefinationEmpty(modName))
		{
			// If no library overridability defined for a overridable module,
			// All libraries in the module are overridable.
			retVal = true;
			break;
		}

		if (isLibraryOverridabilityDefined(modName, libName) == false)
		{
			retVal = false;
			break;
		}

		auto &mr = m_overridableModules.at(modName);

		retVal = util::contains(mr.libraries, libName) 
					 ? mr.libraries.at(libName).overrideable
					 : false;
	} while (false);

	return retVal;
}

bool CSceModuleSystem::isFunctionOverridable(std::string const &modName,
											 std::string const &libName,
											 uint64_t nid) const
{
	bool retVal = false;

	do
	{
		if (!isLibraryOverridable(modName, libName))
		{
			retVal = false;
			break;
		}

		if (isModuleOVRDDefinationEmpty(modName))
		{
			retVal = true;
			break;
		}

		auto &lr = m_overridableModules.at(modName).libraries.at(libName);
		if (lr.functions.empty())
		{
			retVal = true;
			break;
		}

		if (lr.mode == LibraryRecord::OverridingPolicy::AllowList)
		{
			retVal = util::contains(lr.functions, nid) ? true : false;
		}
		else
		{
			retVal = util::contains(lr.functions, nid) ? false : true;
		}

	} while (false);

	return retVal;
}

bool CSceModuleSystem::isBuitinModuleDefined(std::string const& name) const
{
	return util::contains(m_builtinModules, name);
}

bool CSceModuleSystem::isModuleOVRDDefinationEmpty(std::string const& modName) const
{
	bool ret = false;
	do
	{
		if (!util::contains(m_overridableModules, modName))
		{
			ret = true;
			break;
		}
		
		if (m_overridableModules.at(modName).libraries.empty())
		{
			ret = true;
			break;
		}

	} while (false);

	return ret;
}

bool CSceModuleSystem::isLibraryOverridabilityDefined(std::string const& modName,
													  std::string const& libName) const
{
	bool ret = false;
	do 
	{
		auto modIt = m_overridableModules.find(modName);
		if (modIt == m_overridableModules.end())
		{
			ret = false;
			break;
		}

		auto libIt = modIt->second.libraries.find(libName);
		if (libIt == modIt->second.libraries.end())
		{
			ret = false;
			break;
		}
		
		ret = true;

	} while (false);

	return ret;
}

bool CSceModuleSystem::IsEndFunctionEntry(const SCE_EXPORT_FUNCTION* pFunc)
{
	return (pFunc->nNid == 0 && pFunc->szFunctionName == NULL &&
			pFunc->pFunction == NULL);
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
		SceLibMapNid libMapNid;
		SceLibMapName libMapName;

		m_builtinModules.emplace_back(szModName);

		while (!IsEndLibraryEntry(pLib))
		{
			const char *szLibName            = pLib->szLibraryName;
			const SCE_EXPORT_FUNCTION *pFunc = pLib->pFunctionEntries;

			NidFuncMap nidMap;
			NameFuncMap nameMap;
			while (!IsEndFunctionEntry(pFunc))
			{
				nidMap.insert(
					std::make_pair((uint64_t)pFunc->nNid, (void *)pFunc->pFunction));
				nameMap.insert(std::make_pair(std::string(pFunc->szFunctionName),
											  (void *)pFunc->pFunction));
				++pFunc;
			}
			libMapNid.insert(std::make_pair((char *)szLibName, nidMap));
			libMapName.insert(std::make_pair((char *)szLibName, nameMap));
			++pLib;
		}

		m_moduleNidMap.insert(std::make_pair((char *)szModName, libMapNid));
		m_moduleSymbNameMap.insert(std::make_pair((char *)szModName, libMapName));

		bRet = true;
	} while (false);
	return bRet;
}

bool CSceModuleSystem::registerNativeFunction(std::string const &modName,
										std::string const &libName,
										uint64_t nid,
										void *p)
{
	bool retVal = false;

	do
	{
		if (isFunctionOverridable(modName, libName, nid) == false)
		{
			retVal = false;
			break;
		}

		if (!isModuleLoaded(modName))
		{
			SceLibMapNid libMapNid   = {};
			SceLibMapName libMapName = {};

			m_moduleSymbNameMap.insert(std::make_pair(modName, libMapName));
			m_moduleNidMap.insert(std::make_pair(modName, libMapNid));
		}

		if (!isLibraryLoaded(modName, libName))
		{
			NidFuncMap nidMap = {};

			nidMap.insert(std::make_pair(nid, p));
			m_moduleNidMap.at(modName).insert(std::make_pair(libName, nidMap));
			break;
		}

		if (m_moduleNidMap.at(modName).at(libName).count(nid) != 0)
		{
			m_moduleNidMap.at(modName).at(libName).at(nid) = p;
		}
		else
		{
			m_moduleNidMap.at(modName).at(libName).insert(std::make_pair(nid, p));
		}

		retVal = true;

	} while (false);

	return retVal;
}

bool CSceModuleSystem::registerSymbol(std::string const &modName,
									  std::string const &libName,
									  std::string const &symbolName,
									  void *p)
{
	bool retVal = false;
	if (m_moduleSymbNameMap.count(modName) == 0)
	{
		SceLibMapName libMapName = {};
		m_moduleSymbNameMap.emplace(std::make_pair(modName, libMapName));
	}

	auto &mod = m_moduleSymbNameMap.at(modName);
	if (mod.count(libName) == 0)
	{
		NameFuncMap symbolNameMap = {};
		mod.emplace(std::make_pair(libName, symbolNameMap));
	}

	auto &lib = mod.at(libName);
	if (lib.count(symbolName) != 0)
	{
		LOG_ERR("symbol %s has ready been registered", symbolName.c_str());
		retVal = false;
	}
	else
	{
		lib.emplace(std::make_pair(symbolName, p));
		retVal = true;
	}

	return retVal;
}

bool CSceModuleSystem::registerMemoryMappedModule(std::string const &modName,
												  MemoryMappedModule &&mod)
{
	auto index = m_mappedModules.size();

	m_mappedModuleNameIndexMap.insert(std::make_pair(modName, index));
	m_mappedModules.push_back(std::move(mod));

	return true;
}

bool CSceModuleSystem::isMemoryMappedModuleLoaded(std::string const &modName)
{
	bool retVal = false;

	if (m_mappedModuleNameIndexMap.count(modName) != 0)
	{
		retVal = true;
	}
	else
	{
		retVal = false;
	}

	return retVal;
}

CSceModuleSystem::SceMappedModuleList &CSceModuleSystem::getMemoryMappedModules()
{
	return m_mappedModules;
}

bool CSceModuleSystem::getMemoryMappedModule(std::string const &modName,
											 MemoryMappedModule **ppMod)
{
	bool retVal = true;

	auto iter = m_mappedModuleNameIndexMap.find(modName);
	if (iter == m_mappedModuleNameIndexMap.end())
	{
		retVal = false;
	}
	else
	{
		*ppMod = &m_mappedModules.at(iter->second);
	}

	return retVal;
}

void *CSceModuleSystem::findFunction(const std::string &strModName,
									 const std::string &strLibName,
									 uint64_t nNid)
{
	void *pFunction = NULL;
	do
	{
		SceModuleMapNid::iterator iter_mod = m_moduleNidMap.find(strModName);
		if (iter_mod == m_moduleNidMap.end())
		{
			break;
		}

		SceLibMapNid &libMap            = iter_mod->second;
		SceLibMapNid::iterator iter_lib = libMap.find(strLibName);
		if (iter_lib == libMap.end())
		{
			break;
		}

		NidFuncMap &nidMap             = iter_lib->second;
		NidFuncMap::iterator iter_func = nidMap.find(nNid);
		if (iter_func == nidMap.end())
		{
			// TODO: This is for experimental only. When a NID is not found,
			// we attempt to traversal all libs in the module to find that nid.
			//for (auto libIt : iter_mod->second)
			//{
			//	auto nidIter = libIt.second.find(nNid);
			//	if (nidIter != libIt.second.end())
			//	{
			//		pFunction = nidIter->second;
			//		break;
			//	}
			//}
			break;
		}

		pFunction = iter_func->second;
	} while (false);
	return pFunction;
}

void *CSceModuleSystem::findSymbol(std::string const &modName,
								   std::string const &libName,
								   std::string const &symbolName)
{
	void *pAddr = nullptr;
	do
	{
		auto iterMod = m_moduleSymbNameMap.find(modName);
		if (iterMod == m_moduleSymbNameMap.end())
		{
			break;
		}

		auto &libMap = iterMod->second;
		auto iterLib = libMap.find(libName);

		if (iterLib == libMap.end())
		{
			break;
		}

		auto &nameMap   = iterLib->second;
		auto iterSymbol = nameMap.find(symbolName);
		if (iterSymbol == nameMap.end())
		{
			break;
		}

		pAddr = iterSymbol->second;
	} while (false);

	return pAddr;
}

bool CSceModuleSystem::isModuleLoaded(const std::string &modName)
{
	return (m_moduleNidMap.count(modName) != 0);
}

bool CSceModuleSystem::isLibraryLoaded(std::string const &modName,
									   std::string const &libName)
{
	bool retVal = false;

	if (!isModuleLoaded(modName))
	{
		retVal = false;
	}
	else if (m_moduleNidMap.at(modName).count(libName) > 0)
	{
		retVal = true;
	}
	else
	{
		retVal = false;
	}

	return retVal;
}

bool CSceModuleSystem::setModuleOverridability(const std::string &modName, bool ovrd)
{
	bool retVal = false;
	do
	{
		if (ovrd)
		{
			if (m_overridableModules.count(modName) == 0)
			{
				ModuleRecord mr = {};
				m_overridableModules.insert(std::make_pair(modName, mr));
			}

			m_overridableModules.at(modName).overridable = true;
		}
		else
		{
			if (m_overridableModules.count(modName) != 0)
			{
				m_overridableModules.at(modName).overridable = false;
			}
		}

		retVal = true;

	} while (false);

	return retVal;
}

bool CSceModuleSystem::setLibraryOverridability(const std::string &modName,
												const std::string &libName,
												bool ovrd,
												LibraryRecord::OverridingPolicy mode)
{
	bool retVal = true;

	do
	{
		if (m_overridableModules.count(modName) == 0)
		{
			if (ovrd == false)
			{
				break;
			}

			ModuleRecord mr = {};
			mr.overridable  = true;
			m_overridableModules.insert(std::make_pair(modName, mr));
		}

		auto &mr = m_overridableModules.at(modName);

		if (mr.libraries.count(libName) == 0)
		{
			if (ovrd == false)
			{
				break;
			}

			LibraryRecord lr = {};
			lr.mode          = mode;
			lr.overrideable  = true;
			mr.libraries.insert(std::make_pair(libName, lr));
		}

		mr.libraries.at(libName).overrideable = ovrd;

	} while (false);

	return retVal;
}

bool CSceModuleSystem::setFunctionOverridability(const std::string &modName,
												 const std::string &libName,
												 uint64_t nid,
												 bool ovrd)
{
	bool retVal = false;

	do
	{
		if (m_overridableModules.count(modName) == 0)
		{
			if (ovrd == false)
			{
				break;
			}

			ModuleRecord mr = {};
			mr.overridable  = true;
			m_overridableModules.insert(std::make_pair(modName, mr));
		}

		auto &mr = m_overridableModules.at(modName);
		if (mr.libraries.count(libName) == 0)
		{
			if (ovrd == false)
			{
				break;
			}

			LibraryRecord lr = {};
			lr.overrideable  = true;
			mr.libraries.insert(std::make_pair(libName, lr));
		}

		auto &lr = mr.libraries.at(libName);
		if (ovrd)
		{
			if (lr.mode == LibraryRecord::OverridingPolicy::AllowList &&
				lr.functions.count(nid) == 0)
			{
				lr.functions.insert(std::make_pair(nid, true));
			}
			else if (lr.mode == LibraryRecord::OverridingPolicy::DisallowList &&
					 lr.functions.count(nid) != 0)
			{
				lr.functions.erase(nid);
			}
		}
		else
		{
			if (lr.mode == LibraryRecord::OverridingPolicy::DisallowList &&
				lr.functions.count(nid) == 0)
			{
				lr.functions.insert(std::make_pair(nid, true));
			}
			else if (lr.mode == LibraryRecord::OverridingPolicy::AllowList &&
					 lr.functions.count(nid) != 0)
			{
				lr.functions.erase(nid);
			}
		}

		retVal = true;
	} while (false);

	return retVal;
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
		if (m_mappedModules.size() == 0)
		{
			break;
		}
		const auto &ebootModule = m_mappedModules.at(0);
		retVal                  = &ebootModule.getModuleInfo();
	} while (false);
	return retVal;
}

void CSceModuleSystem::clearModules()
{
	m_moduleNidMap.clear();
	m_moduleSymbNameMap.clear();
	m_overridableModules.clear();
	m_mappedModules.clear();
	m_mappedModuleNameIndexMap.clear();
}
