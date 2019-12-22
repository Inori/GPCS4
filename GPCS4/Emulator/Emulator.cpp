#include "Emulator.h"
#include "GameThread.h"
#include "SceModuleSystem.h"

CEmulator::CEmulator() {}

CEmulator::~CEmulator() {}

bool CEmulator::Init()
{
	bool bRet = false;
	do
	{
		if (!registerModules())
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
	auto modManager = CSceModuleSystem::GetInstance();
	modManager->clearModules();
}

bool CEmulator::Run(MemoryMappedModule const &mod)
{
	bool retVal = false;

	do
	{
		if (mod.isModule())
		{
			LOG_ERR("%s is not an executable", mod.fileName.c_str());
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

