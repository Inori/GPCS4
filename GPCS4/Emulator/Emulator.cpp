#include "Emulator.h"
#include "GameThread.h"
#include "SceModuleSystem.h"

LOG_CHANNEL(Emulator);

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
	auto modManager = CSceModuleSystem::GetInstance();
	modManager->clearModules();
}

bool CEmulator::Run(NativeModule const &mod)
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

		struct PS4StartupParams 
		{
			uint64_t argc = 1;
			const char *argv[1] = { "eboot.bin" };
		};

		PS4StartupParams startupParams;

		LOG_DEBUG("run into eboot.");
		CGameThread oMainThread(entryPoint, &startupParams, CEmulator::LastExitHandler);
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

