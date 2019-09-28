#pragma once

#include "GPCS4Common.h"
#include "Singleton.h"
#include "Module.h"
#include <string>
#include <unordered_map>

struct SCE_EXPORT_FUNCTION;
struct SCE_EXPORT_LIBRARY;
struct SCE_EXPORT_MODULE;

class CSceModuleSystem final : public Singleton<CSceModuleSystem>
{
	friend class  Singleton<CSceModuleSystem>;
private:
	struct LibraryRecord
	{
		bool overrideable;
		std::unordered_map<uint64_t, bool> functions;

	};

	struct ModuleRecord
	{
		bool overridable;
		std::unordered_map<std::string, LibraryRecord> libraries;
	};

	//since we won't insert or remove item after initialization
	//we use unordered_map instead of map
	typedef std::unordered_map<uint64, void*> NidFuncMap;
	typedef std::unordered_map<std::string, void*> NameFuncMap;

	typedef std::unordered_map<std::string, NidFuncMap> SceLibMapNid;
	typedef std::unordered_map<std::string, NameFuncMap> SceLibMapName;

	typedef std::unordered_map<std::string, SceLibMapNid> SceModuleMapNid;
	typedef std::unordered_map<std::string, SceLibMapName> SceModuleMapName;
	typedef std::unordered_map<std::string, ModuleRecord> SceOverridableMapNid;
	typedef std::unordered_map<std::string, MemoryMappedModule> SceMappedModuleMap;

public:

	bool RegisterModule(const SCE_EXPORT_MODULE& stModule);

	bool RegisterFunction(std::string const &modName, std::string const &libName, uint64_t nid, void *p);

	// I use rvalue reference here to express "sink" semantics, which means the container would take ownership
	// of the object after registeration and the original one would no longer be valid.
	bool RegisterMemoryMappedModule(std::string const &modName, MemoryMappedModule &&mod);

	bool GetMemoryMappedModule(std::string const &modName, MemoryMappedModule **ppMod);
	
	void* FindFunction(const std::string& strModName, const std::string& strLibName, uint64 nNid);

	bool IsModuleLoaded(const std::string& modName);

	bool isModuleLoadable(std::string const &modName);

	bool isLibraryLoaded(std::string const &modName, std::string const &libName);

	bool setModuleOverridability(const std::string &modName, bool isOverridable);

	bool setLibraryOverridability(const std::string &modName, const std::string &libName, bool isOverridable);

	bool setFunctionOverridability(const std::string &modName,
								 const std::string &libName,
								 uint64_t nid, bool isOverridable);

private:
	bool IsEndFunctionEntry(const SCE_EXPORT_FUNCTION* pFunc);
	bool IsEndLibraryEntry(const SCE_EXPORT_LIBRARY* pLib);

	bool isLibraryLoadable(std::string const &modName, std::string const &libName);
	bool isFunctionLoadable(std::string const &modName,std::string const &libName, uint64_t nid);

	bool isModuleOverridable(std::string const &modName);
	bool isLibraryOverridable(std::string const &modName, std::string const &libName);
	bool isFunctionOverridable(std::string const &modName, std::string const &libName, uint64_t nid);

private:

	SceModuleMapNid m_umpModuleMapNid;
	SceModuleMapName m_umpModuleMapName;

	SceOverridableMapNid m_overridableModules;
	SceMappedModuleMap m_mappedModules;

private:
	CSceModuleSystem();
	~CSceModuleSystem();
	CSceModuleSystem(const CSceModuleSystem&);
	CSceModuleSystem& operator = (const CSceModuleSystem&);
};

