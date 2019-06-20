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
		uint64 pCtx[0x10] = { 0 };
		pfuncEntryPoint(pCtx, 0x10);

		bRet = true;
	} while (false);
	return bRet;
}
