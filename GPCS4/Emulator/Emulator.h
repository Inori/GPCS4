#pragma once

#include "GPCS4Common.h"
#include "../Loader/EbootObject.h"
#include "Linker.h"
#include "TLSHandler.h"
#include "Module.h"

#include <memory>

typedef void (PS4NORETURN PS4API *PFUNC_EntryPoint)(void* pEnv, void* pfnExitHandler);

class CEmulator
{
public:
	CEmulator();
	~CEmulator();

	bool Init();

	void Unit();

	bool Run(NativeModule const &mod);

private:
	bool registerModules();

	static void PS4API LastExitHandler(void);
private:
	std::shared_ptr<CLinker> m_pLinker;
};

