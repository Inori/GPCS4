#include "SceModuleSystem.h"
#include "sce_modules.h"

CSceModuleSystem::CSceModuleSystem()
{
}

CSceModuleSystem::~CSceModuleSystem()
{
}

bool CSceModuleSystem::IsEndLibraryEntry(const SCE_EXPORT_LIBRARY* pLib)
{
	return (pLib->szLibraryName == NULL && pLib->pFunctionEntries == NULL);
}

bool CSceModuleSystem::isModuleLoadable(const char * modueleName)
{
	bool retVal = false;

	if (m_umpModuleMapName.count(modueleName) != 0)
	{
		if (m_overridableModules.count(modueleName) != 0 && 
			m_overridableModules.at(modueleName).overrideable)
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
		retVal = true;
	}

	return retVal;
}

// TODO: code style issue. use do {} while (false); instead.
bool CSceModuleSystem::isLibraryLoadable(std::string const &modName, std::string const &libName)
{
	// module has not been loaded
	if (m_umpModuleMapName.count(modName) == 0)
	{
		return true;
	}

	// library has not been loaded
	if (m_umpModuleMapName.at(modName).count(libName) == 0)
	{
		return true;
	}

	// module is not overrideable
	if (m_overridableModules.count(modName) == 0)
	{
		return false;
	}
	
	auto &mr = m_overridableModules.at(modName);
	if (mr.overrideable == false)
	{
		return false;
	}

	// library is not overrideable
	if (mr.libraries.count(libName) == 0)
	{
		return false;
	}

	if (mr.libraries.at(libName).overrideable == false)
	{
		return false;
	}

	return true;
}

// TODO: code style issue. use do {} while (false); instead. 
bool CSceModuleSystem::isFunctionLoadable(std::string const & modName, std::string const &libName, uint64_t nid)
{
	// module has not been loaded
	if (m_umpModuleMapNid.count(modName) == 0)
	{
		return true;
	}

	// library has not been loaded
	if (m_umpModuleMapNid.at(modName).count(libName) == 0)
	{
		return true;
	}

	// function has not been loaded
	if (m_umpModuleMapNid.at(modName).at(libName).count(nid) == 0)
	{
		return true;
	}

	// module is not overrideable
	if (m_overridableModules.count(modName) == 0)
	{
		return false;
	}

	auto &mr = m_overridableModules.at(modName);
	if (mr.overrideable == false)
	{
		return false;
	}

	// library is not overrideable
	if (mr.libraries.count(libName) == 0)
	{
		return false;
	}

	auto &lr = mr.libraries.at(libName);
	if (lr.overrideable == false)
	{
		return false;
	}

	if (lr.functions.count(nid) == 0)
	{
		return false;
	}

	return true;

}

bool CSceModuleSystem::IsEndFunctionEntry(const SCE_EXPORT_FUNCTION* pFunc)
{
	return (pFunc->nNid == 0 && pFunc->szFunctionName == NULL && pFunc->pFunction == NULL);
}

bool CSceModuleSystem::RegisterModule(const SCE_EXPORT_MODULE& stModule)
{
	bool bRet = false;
	do 
	{
		if (!stModule.szModuleName)
		{
			break;
		}

		if (!isModuleLoadable(stModule.szModuleName))
		{
			LOG_DEBUG("Module %s has already been loaded and is not overrideable", stModule.szModuleName);
			bRet = true;
			break;
		}


		const char* szModName = stModule.szModuleName;
		const SCE_EXPORT_LIBRARY* pLib = stModule.pLibraries;
		SceLibMapNid libMapNid;
		SceLibMapName libMapName;
		while (!IsEndLibraryEntry(pLib))
		{
			const char* szLibName = pLib->szLibraryName;
			const SCE_EXPORT_FUNCTION* pFunc = pLib->pFunctionEntries;
			if (!isLibraryLoadable(szModName, szLibName))
			{
				LOG_DEBUG("Library %s of Module %s has already been loaded and is not overrideable", szLibName, szModName);
				continue;
			}

			NidFuncMap nidMap;
			NameFuncMap nameMap;
			while (!IsEndFunctionEntry(pFunc))
			{
				if (!isFunctionLoadable(szModName, szLibName, pFunc->nNid))
				{
					LOG_DEBUG("function %s is not loadable", pFunc->szFunctionName);
					continue;
				}

				nidMap.insert(std::make_pair((uint64)pFunc->nNid, (void*)pFunc->pFunction));
				nameMap.insert(std::make_pair(std::string(pFunc->szFunctionName), (void*)pFunc->pFunction));
				++pFunc;
			}
			libMapNid.insert(std::make_pair((char*)szLibName, nidMap));
			libMapName.insert(std::make_pair((char*)szLibName, nameMap));
			++pLib;
		}

		m_umpModuleMapNid.insert(std::make_pair((char*)szModName, libMapNid));
		m_umpModuleMapName.insert(std::make_pair((char*)szModName, libMapName));

		bRet = true;
	} while (false);
	return bRet;
}

void* CSceModuleSystem::FindFunction(const std::string& strModName, const std::string& strLibName, uint64 nNid)
{
	void* pFunction = NULL;
	do 
	{
		SceModuleMapNid::iterator iter_mod = m_umpModuleMapNid.find(strModName);
		if (iter_mod == m_umpModuleMapNid.end())
		{
			break;
		}

		SceLibMapNid& libMap = iter_mod->second;
		SceLibMapNid::iterator iter_lib = libMap.find(strLibName);
		if (iter_lib == libMap.end())
		{
			break;
		}

		NidFuncMap& nidMap = iter_lib->second;
		NidFuncMap::iterator iter_func = nidMap.find(nNid);
		if (iter_func == nidMap.end())
		{
			break;
		}

		pFunction = iter_func->second;
	} while (false);
	return pFunction;
}

bool CSceModuleSystem::IsModuleLoaded(const std::string& modName)
{
	return (m_umpModuleMapNid.count(modName) != 0);
}

bool CSceModuleSystem::setModuleOverridability(const std::string & modName, bool ovrd)
{
	bool retVal = false;
	do 
	{
		if (!IsModuleLoaded(modName))
		{
			LOG_ERR("Module %s is not loaded.", modName.c_str());
			break;
		}

		if (ovrd)
		{
			if (m_overridableModules.count(modName) == 0)
			{
				ModuleRecord mr = {};
				m_overridableModules.insert(std::make_pair(modName, mr));
			}

			m_overridableModules.at(modName).overrideable = true;
		}
		else
		{
			if (m_overridableModules.count(modName) != 0)
			{
				m_overridableModules.at(modName).overrideable = false;
			}
		}

		retVal = true;

	} while (false);

	return retVal;
}

bool CSceModuleSystem::setLibraryOverridability(const std::string & modName,
												const std::string & libName,
												bool ovrd)
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
			mr.overrideable = true;
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
			lr.overrideable = true;
			mr.libraries.insert(std::make_pair(libName, lr));
		}

		mr.libraries.at(modName).overrideable = ovrd;

	} while (false);

	return retVal;
}

bool CSceModuleSystem::setFunctionOverridability(const std::string & modName,
											   const std::string & libName,
											   uint64_t nid, bool ovrd)
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
			mr.overrideable = true;
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
			lr.overrideable = true;
			mr.libraries.insert(std::make_pair(libName, lr));
		}

		auto &lr = mr.libraries.at(modName);

		if (lr.functions.count(nid) != 0 && ovrd == false)
		{
			lr.functions.erase(nid);
		}
		else
		{
			lr.functions.insert(std::make_pair(nid, true));
		}

	} while (false);


	return false;
}



