#include "Emulator/Emulator.h"
#include "Loader/ModuleLoader.h"
#include "Emulator/SceModuleSystem.h"
#include <memory>

int main(int argc, char* argv[])
{
	LOG_DEBUG("enter main function.");

	std::unique_ptr<CEmulator> pEmulator = std::make_unique<CEmulator>();
	int nRet = -1;
	do 
	{
		if (argc != 2)
		{
			printf("usage %s eboot.bin\n", argv[0]);
			break;
		}

		char* szEboot = argv[1];

		if (!pEmulator->Init())
		{
			break;
		}

		// TODO: for testing only. remember to remove this
		ModuleLoader loader = { CSceModuleSystem::GetInstance() };
		if (!loader.loadModule(szEboot))
		{
			break;
		}
		//if (!pEmulator->LoadEboot(szEboot))
		//{
		//	break;
		//}

		//pEmulator->Run();

		//pEmulator->Unit();
		nRet = 0;
	} while (false);

	return nRet;
}