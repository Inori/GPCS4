#pragma once

#include "GPCS4Common.h"
#include "../Loader/EbootObject.h"
#include "Linker.h"
#include <memory>

typedef void (PS4NORETURN PS4API *PFUNC_EntryPoint)(void* pCtx, uint64 nUk);

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

private:
	CEbootObject m_oEboot;
	std::shared_ptr<CLinker> m_pLinker;
};

