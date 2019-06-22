#pragma once

#include "GPCS4Common.h"
#include "../Loader/EbootObject.h"
#include "Linker.h"
#include <memory>

typedef void (PS4NORETURN PS4API *PFUNC_EntryPoint)(void* pEnv, void* pfnExitHandler);

class CEmulator
{
public:
	CEmulator();
	~CEmulator();

	bool Init();

	void Unit();

	bool LoadEboot(const std::string& strEbtPath);

	bool Run();

private:
	bool RegisterModules();

	static void LastExitHandler(void);
private:
	CEbootObject m_oEboot;
	std::shared_ptr<CLinker> m_pLinker;
};

