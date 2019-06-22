#include "Emulator.h"



CEmulator::CEmulator()
{
}


CEmulator::~CEmulator()
{
}

bool CEmulator::Init()
{
	bool bRet = false;
	do 
	{
		if (!RegisterModules())
		{
			break;
		}

		m_pLinker = std::make_shared<CLinker>();

		bRet = true;
	} while (false);
	return bRet;
}

void CEmulator::Unit()
{

}

bool CEmulator::LoadEboot(const std::string& strEbtPath)
{
	bool bRet = false;
	do 
	{
		m_oEboot.SetLinker(m_pLinker);

		if (!m_oEboot.Load(strEbtPath))
		{
			break;
		}

		bRet = true;
	} while (false);
	return bRet;
}

bool CEmulator::Run()
{

	bool bRet = false;
	do 
	{

		PFUNC_EntryPoint pfuncEntryPoint = (PFUNC_EntryPoint)m_oEboot.EntryPoint();

		LOG_DEBUG("run into eboot.");
		const int nEnvNum = 0x10;
		uint64 pEnv[nEnvNum] = { 0xDEADBEE1, 0xDEADBEE2, 0xDEADBEE3, 0xDEADBEE4, 0xDEADBEE5, 
			0xDEADBEE6, 0xDEADBEE7, 0xDEADBEE8, 0xDEADBEE9, 0xDEADBEEA };
		pfuncEntryPoint(pEnv, CEmulator::LastExitHandler);

		bRet = true;
	} while (false);
	return bRet;
}

void CEmulator::LastExitHandler(void)
{
	LOG_DEBUG("program exit.");
}
