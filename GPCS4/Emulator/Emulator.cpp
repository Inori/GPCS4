#include "Emulator.h"
#include "GameThread.h"

CEmulator::CEmulator() {}

CEmulator::~CEmulator() {}

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
	CTLSHandlerWin::Uninstall();
	m_oEboot.Unload();
}

bool CEmulator::LoadEboot(const std::string &strEbtPath)
{
	bool bRet = false;
	do
	{
		m_oEboot.SetLinker(m_pLinker);

		if (!m_oEboot.Load(strEbtPath))
		{
			break;
		}

		void *pTls     = NULL;
		uint nInitSize = 0, nTotalSize = 0;
		if (!m_oEboot.GetTlsInfo(&pTls, nInitSize, nTotalSize))
		{
			break;
		}

#ifdef GPCS4_WINDOWS
		if (!CTLSHandlerWin::Install(pTls, nInitSize, nTotalSize))
		{
			break;
		}
#else

#endif // GPCS4_WINDOWS
		bRet = true;
	} while (false);
	return bRet;
}

bool CEmulator::Run()
{

	bool bRet = false;
	do
	{

		void *pEntryPoint = m_oEboot.EntryPoint();
		if (!pEntryPoint)
		{
			break;
		}

		const int nEnvNum    = 0x10;
		uint64 pEnv[nEnvNum] = {0xDEADBEE1, 0xDEADBEE2, 0xDEADBEE3, 0xDEADBEE4,
								0xDEADBEE5, 0xDEADBEE6, 0xDEADBEE7, 0xDEADBEE8,
								0xDEADBEE9, 0xDEADBEEA};

		LOG_DEBUG("run into eboot.");
		CGameThread oMainThread(pEntryPoint, pEnv,
								(void *)CEmulator::LastExitHandler);
		if (!oMainThread.Start())
		{
			break;
		}

		// block the emulator's thread
		if (!oMainThread.Join(NULL))
		{
			break;
		}

		bRet = true;
	} while (false);
	return bRet;
}

bool CEmulator::Run(MemoryMappedModule const &mod)
{
	bool retVal = false;

	do
	{
		if (mod.isModule())
		{
			LOG_ERR("%s is not an executable module", mod.fileName.c_str());
			break;
		}

		void *entryPoint = mod.getEntryPoint();
		if (entryPoint == nullptr)
		{
			LOG_ERR("fail to get entry point");
			break;
		}

		const int nEnvNum    = 0x10;
		uint64 pEnv[nEnvNum] = {0xDEADBEE1, 0xDEADBEE2, 0xDEADBEE3, 0xDEADBEE4,
								0xDEADBEE5, 0xDEADBEE6, 0xDEADBEE7, 0xDEADBEE8,
								0xDEADBEE9, 0xDEADBEEA};

		LOG_DEBUG("run into eboot.");
		CGameThread oMainThread(entryPoint, pEnv,
								(void *)CEmulator::LastExitHandler);
		if (!oMainThread.Start())
		{
			break;
		}

		// block the emulator's thread
		if (!oMainThread.Join(NULL))
		{
			break;
		}

	retVal = true;
	} while (false);

	return retVal;
}

void PS4API CEmulator::LastExitHandler(void) { LOG_DEBUG("program exit."); }

