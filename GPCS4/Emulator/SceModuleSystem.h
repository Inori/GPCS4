#pragma once

#include "GPCS4Common.h"
#include "Singleton.h"
#include <string>
#include <unordered_map>

struct SCE_EXPORT_FUNCTION;
struct SCE_EXPORT_LIBRARY;
struct SCE_EXPORT_MODULE;

class CSceModuleSystem final : public Singleton<CSceModuleSystem>
{
	friend class  Singleton<CSceModuleSystem>;

	//since we won't insert or remove item after initialization
	//we use unordered_map instead of map
	typedef std::unordered_map<uint64, void*> NidFuncMap;
	typedef std::unordered_map<std::string, void*> NameFuncMap;

	typedef std::unordered_map<std::string, NidFuncMap> SceLibMapNid;
	typedef std::unordered_map<std::string, NameFuncMap> SceLibMapName;

	typedef std::unordered_map<std::string, SceLibMapNid> SceModuleMapNid;
	typedef std::unordered_map<std::string, SceLibMapName> SceModuleMapName;

public:

	bool IsEndLibraryEntry(const SCE_EXPORT_LIBRARY* pLib);
	bool IsEndFunctionEntry(const SCE_EXPORT_FUNCTION* pFunc);
	bool RegisterModule(const SCE_EXPORT_MODULE& stModule);
	
	void* FindFunction(const std::string& strModName, const std::string& strLibName, uint64 nNid);

private:
	SceModuleMapNid m_umpModuleMapNid;
	SceModuleMapName m_umpModuleMapName;
private:
	CSceModuleSystem();
	~CSceModuleSystem();
	CSceModuleSystem(const CSceModuleSystem&);
	CSceModuleSystem& operator = (const CSceModuleSystem&);
};

