#include "Emulator/Emulator.h"
#include "Loader/ModuleLoader.h"
#include "Emulator/SceModuleSystem.h"
#include "Emulator/TLSHandler.h"

#include <memory>
#include <cmath>

int main(int argc, char *argv[])
{
	LOG_DEBUG("enter main function.");

	std::unique_ptr<CEmulator> pEmulator = std::make_unique<CEmulator>();
	int nRet                             = -1;
	do
	{
		if (argc != 2)
		{
			printf("usage %s eboot.bin\n", argv[0]);
			break;
		}

		char *szEboot = argv[1];

		if (!pEmulator->Init())
		{
			break;
		}

		// TODO: for testing only. remember to remove this

		CTLSHandlerWin tlsHandler = {};

		CLinker linker      = {*CSceModuleSystem::GetInstance()};
		ModuleLoader loader = {*CSceModuleSystem::GetInstance(), linker, tlsHandler};

		MemoryMappedModule *ebootModule = nullptr;
		if (!loader.loadModule(szEboot, &ebootModule))
		{
			break;
		}

		auto modSystem = CSceModuleSystem::GetInstance();
		//auto ptr = modSystem->FindFunction("libkernel", "libkernel", 0xF41703CA43E6A352);
		


		if(!pEmulator->Run(*ebootModule))
		{
			break;
		}

		// if (!pEmulator->LoadEboot(szEboot))
		//{
		//	break;
		//}

		// pEmulator->Run();

		// pEmulator->Unit();
		nRet = 0;
	} while (false);

	return nRet;
}