#include "SceModuleSystem.h"
#include "Platform/PlatformUtils.h"
#include "sce_modules.h"

CSceModuleSystem::CSceModuleSystem() {}

CSceModuleSystem::~CSceModuleSystem() {}

bool CSceModuleSystem::IsEndLibraryEntry(const SCE_EXPORT_LIBRARY *pLib)
{
	return (pLib->szLibraryName == NULL && pLib->pFunctionEntries == NULL);
}

bool CSceModuleSystem::isModuleLoadable(std::string const &modName)
{
	bool retVal = false;

	if (m_umpModuleMapName.count(modName) == 0)
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

bool CSceModuleSystem::isLibraryLoadable(std::string const &modName,
										 std::string const &libName)
{
	bool retVal = false;

	do
	{
		if (IsModuleLoaded(modName) == false)
		{
			retVal = true;
			break;
		}

		if (isModuleLoadable(modName) == false)
		{
			retVal = false;
			break;
		}

		if (m_umpModuleMapNid.at(modName).count(libName) == 0)
		{
			retVal = true;
			break;
		}

		if (isLibraryOverridable(modName, libName))
		{
			retVal = true;
		}
		else
		{
			retVal = false;
		}

	} while (false);

	return retVal;
}

bool CSceModuleSystem::isFunctionLoadable(std::string const &modName,
										  std::string const &libName,
										  uint64_t nid)
{
	bool retVal = false;

	do
	{
		// TODO:
		//if (FindFunction(modName, libName, nid) == nullptr)
		//{
		//	retVal = true;
		//	break;
		//}

		if (isFunctionOverridable(modName, libName, nid))
		{
			retVal = true;
		}
		else
		{
			retVal = false;
		}

	} while (false);

	return retVal;
}

bool CSceModuleSystem::isModuleOverridable(std::string const &modName) const
{
	return m_overridableModules.count(modName) > 0
			   ? m_overridableModules.at(modName).overridable
			   : false;
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

		auto &mr = m_overridableModules.at(modName);
		if (mr.libraries.empty())
		{
			// if the override library table is empty, all bibraries in the module is
			// overridable
			retVal = true;
			break;
		}

		retVal = mr.libraries.count(libName) > 0
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

		if (m_overridableModules.at(modName).libraries.count(libName) == 0)
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

		if (lr.mode == LibraryRecord::Mode::Allow)
		{
			retVal = lr.functions.count(nid) > 0 ? true : false;
		}
		else
		{
			retVal = lr.functions.count(nid) > 0 ? false : true;
		}

	} while (false);

	return retVal;
}

bool CSceModuleSystem::decodeValue(std::string const &encodedStr, uint64_t &value)
{
	bool bRet = false;

	// the max length for an encode id is 11
	// from orbis-ld.exe
	const uint nEncLenMax = 11;
	const char pCodes[] =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+-";

	do
	{

		if (encodedStr.size() > nEncLenMax)
		{
			LOG_ERR("encode id too long: %s", encodedStr.c_str());
			break;
		}

		bool bError = false;
		value       = 0;

		for (int i = 0; i < encodedStr.size(); ++i)
		{
			auto pChPos = strchr(pCodes, encodedStr[i]);
			uint nIndex = 0;

			if (pChPos != nullptr)
			{
				nIndex = static_cast<uint>(pChPos - pCodes);
			}
			else
			{
				bError = true;
				break;
			}

			// NID is 64 bits long, thus we do 6 x 10 + 4 times
			if (i < nEncLenMax - 1)
			{
				value <<= 6;
				value |= nIndex;
			}
			else
			{
				value <<= 4;
				value |= (nIndex >> 2);
			}
		}

		if (bError)
		{
			break;
		}

		bRet = true;
	} while (false);
	return bRet;
	return false;
}

bool CSceModuleSystem::IsEndFunctionEntry(const SCE_EXPORT_FUNCTION *pFunc)
{
	return (pFunc->nNid == 0 && pFunc->szFunctionName == NULL &&
			pFunc->pFunction == NULL);
}

bool CSceModuleSystem::RegisterModule(const SCE_EXPORT_MODULE &stModule)
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
			LOG_DEBUG("Module %s has already been loaded and is not overrideable",
					  stModule.szModuleName);
			bRet = true;
			break;
		}

		const char *szModName          = stModule.szModuleName;
		const SCE_EXPORT_LIBRARY *pLib = stModule.pLibraries;
		SceLibMapNid libMapNid;
		SceLibMapName libMapName;
		while (!IsEndLibraryEntry(pLib))
		{
			const char *szLibName            = pLib->szLibraryName;
			const SCE_EXPORT_FUNCTION *pFunc = pLib->pFunctionEntries;
			if (!isLibraryLoadable(szModName, szLibName))
			{
				LOG_DEBUG("Library %s of Module %s has already been loaded and is "
						  "not overrideable",
						  szLibName, szModName);
				continue;
			}

			NidFuncMap nidMap;
			NameFuncMap nameMap;
			while (!IsEndFunctionEntry(pFunc))
			{
				//if (!isFunctionLoadable(szModName, szLibName, pFunc->nNid))
				//{
				//	LOG_DEBUG("function %s is not loadable", pFunc->szFunctionName);
				//	continue;
				//}

				nidMap.insert(
					std::make_pair((uint64)pFunc->nNid, (void *)pFunc->pFunction));
				nameMap.insert(std::make_pair(std::string(pFunc->szFunctionName),
											  (void *)pFunc->pFunction));
				++pFunc;
			}
			libMapNid.insert(std::make_pair((char *)szLibName, nidMap));
			libMapName.insert(std::make_pair((char *)szLibName, nameMap));
			++pLib;
		}

		m_umpModuleMapNid.insert(std::make_pair((char *)szModName, libMapNid));
		m_umpModuleMapName.insert(std::make_pair((char *)szModName, libMapName));

		bRet = true;
	} while (false);
	return bRet;
}

bool CSceModuleSystem::registerFunction(std::string const &modName,
										std::string const &libName,
										uint64_t nid,
										void *p)
{
	bool retVal = false;

	do
	{
		if (isFunctionLoadable(modName, libName, nid) == false)
		{
			retVal = false;
			break;
		}

		if (!IsModuleLoaded(modName))
		{
			SceLibMapNid libMapNid   = {};
			SceLibMapName libMapName = {};

			m_umpModuleMapName.insert(std::make_pair(modName, libMapName));
			m_umpModuleMapNid.insert(std::make_pair(modName, libMapNid));
		}

		if (!isLibraryLoaded(modName, libName))
		{
			NidFuncMap nidMap = {};

			nidMap.insert(std::make_pair(nid, p));
			m_umpModuleMapNid.at(modName).insert(std::make_pair(libName, nidMap));
			break;
		}

		if (m_umpModuleMapNid.at(modName).at(libName).count(nid) != 0)
		{
			m_umpModuleMapNid.at(modName).at(libName).at(nid) = p;
		}
		else
		{
			m_umpModuleMapNid.at(modName).at(libName).insert(std::make_pair(nid, p));
		}

	} while (false);

	return retVal;
}

bool CSceModuleSystem::registerSymbol(std::string const &modName,
									  std::string const &libName,
									  std::string const &symbolName,
									  void *p)
{
	bool retVal = false;
	if (m_umpModuleMapName.count(modName) == 0)
	{
		SceLibMapName libMapName = {};
		m_umpModuleMapName.emplace(std::make_pair(modName, libMapName));
	}

	auto &mod = m_umpModuleMapName.at(modName);
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

bool CSceModuleSystem::GetMemoryMappedModule(std::string const &modName,
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

void *CSceModuleSystem::FindFunction(const std::string &strModName,
									 const std::string &strLibName,
									 uint64 nNid)
{
	void *pFunction = NULL;
	do
	{
		SceModuleMapNid::iterator iter_mod = m_umpModuleMapNid.find(strModName);
		if (iter_mod == m_umpModuleMapNid.end())
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
		auto iterMod = m_umpModuleMapName.find(modName);
		if (iterMod == m_umpModuleMapName.end())
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

bool CSceModuleSystem::IsModuleLoaded(const std::string &modName)
{
	return (m_umpModuleMapNid.count(modName) != 0);
}

bool CSceModuleSystem::isLibraryLoaded(std::string const &modName,
									   std::string const &libName)
{
	bool retVal = false;

	if (!IsModuleLoaded(modName))
	{
		retVal = false;
	}
	else if (m_umpModuleMapNid.at(modName).count(libName) > 0)
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
												LibraryRecord::Mode mode)
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

		mr.libraries.at(modName).overrideable = ovrd;

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

		auto &lr = mr.libraries.at(modName);
		if (ovrd)
		{
			if (lr.mode == LibraryRecord::Mode::Allow &&
				lr.functions.count(nid) == 0)
			{
				lr.functions.insert(std::make_pair(nid, true));
			}
			else if (lr.mode == LibraryRecord::Mode::Disallow &&
					 lr.functions.count(nid) != 0)
			{
				lr.functions.erase(nid);
			}
		}
		else
		{
			if (lr.mode == LibraryRecord::Mode::Disallow &&
				lr.functions.count(nid) == 0)
			{
				lr.functions.insert(std::make_pair(nid, true));
			}
			else if (lr.mode == LibraryRecord::Mode::Allow &&
					 lr.functions.count(nid) != 0)
			{
				lr.functions.erase(nid);
			}
		}

		retVal = true;
		// if (lr.functions.count(nid) != 0 && ovrd == false)
		// {
		// 	lr.functions.erase(nid);
		// }
		// else
		// {
		// 	lr.functions.insert(std::make_pair(nid, true));
		// }

	} while (false);

	return retVal;
}

bool CSceModuleSystem::addAllowedFile(std::string const &fileName)
{
	if (m_allowedFiles.count(fileName) == 0)
	{
		m_allowedFiles.insert(std::make_pair(fileName, true));
	}

	return true;
}

bool CSceModuleSystem::isFileAllowedToLoad(std::string const &fileName)
{
	bool retVal = false;

	std::string name      = {};
	std::string extension = {};

	auto ret = UtilPath::splitFileName(fileName, &name, &extension);

	if (m_allowedFiles.count(fileName) != 0)
	{
		retVal = true;
	}
	else if (ret && (extension == "prx" || extension == "sprx"))
	{
		if (isModuleLoadable(name))
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

	return retVal;
}

bool CSceModuleSystem::decodeEncodedName(std::string const &strEncName,
										 uint *modId,
										 uint *libId,
										 uint64_t *funcNid)
{
	bool bRet = false;
	do
	{
		if (modId == nullptr || libId == nullptr || funcNid == nullptr)
		{
			break;
		}

		auto &nModuleId  = *modId;
		auto &nLibraryId = *libId;
		auto &nNid       = *funcNid;

		std::vector<std::string> vtNameParts;
		if (!UtilString::Split(strEncName, '#', vtNameParts))
		{
			break;
		}

		if (!decodeValue(vtNameParts[0], nNid))
		{
			break;
		}

		uint64 nLibId = 0;
		if (!decodeValue(vtNameParts[1], nLibId))
		{
			break;
		}
		nLibraryId = static_cast<uint>(nLibId);

		uint64 nModId = 0;
		if (!decodeValue(vtNameParts[2], nModId))
		{
			break;
		}
		nModuleId = static_cast<uint>(nModId);

		bRet = true;
	} while (false);
	return bRet;
}
