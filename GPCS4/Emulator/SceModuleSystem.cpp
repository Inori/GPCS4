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

		const char* szModName = stModule.szModuleName;
		const SCE_EXPORT_LIBRARY* pLib = stModule.pLibraries;
		SceLibMapNid libMapNid;
		SceLibMapName libMapName;
		while (!IsEndLibraryEntry(pLib))
		{
			const char* szLibName = pLib->szLibraryName;
			const SCE_EXPORT_FUNCTION* pFunc = pLib->pFunctionEntries;
			NidFuncMap nidMap;
			NameFuncMap nameMap;
			while (!IsEndFunctionEntry(pFunc))
			{
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

