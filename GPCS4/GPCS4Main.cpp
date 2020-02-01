#include "Emulator/Emulator.h"
#include "Emulator/SceModuleSystem.h"
#include "Emulator/TLSHandler.h"
#include "Loader/ModuleLoader.h"

#include <cxxopts/cxxopts.hpp>
#include <memory>
#include <cmath>

LOG_CHANNEL(main);

cxxopts::ParseResult processCommandLine(int argc, char* argv[])
{
	cxxopts::Options opts("GPCS4", "PlayStation 4 Emulator");

	opts.add_options()
		("E,eboot", "Set main executable", cxxopts::value<std::string>())
		("D,debug-channel", "Enable debug channel", cxxopts::value<std::vector<std::string>>())
		("L,list-channels", "List debug channel");

	return opts.parse(argc, argv);
}

int main(int argc, char *argv[])
{
	std::unique_ptr<CEmulator> pEmulator = std::make_unique<CEmulator>();
	int nRet = -1;

	do
	{
		auto optResult = processCommandLine(argc, argv);

		if (!optResult.count("E"))
		{
			break;
		}
		auto eboot = optResult["E"].as<std::string>();

		// Initialize log system.
		logsys::init(optResult);

		// Initialize the whole emulator.

		LOG_DEBUG("GPCS4 start.");

		if (!pEmulator->Init())
		{
			break;
		}

		if (!installTLSManager())
		{
			break;
		}

		CLinker linker      = {*CSceModuleSystem::GetInstance()};
		ModuleLoader loader = { *CSceModuleSystem::GetInstance(), linker };

		MemoryMappedModule *ebootModule = nullptr;
		if (!loader.loadModule(eboot, &ebootModule))
		{
			break;
		}

		if(!pEmulator->Run(*ebootModule))
		{
			break;
		}

		uninstallTLSManager();
		pEmulator->Unit();
		
		nRet = 0;
	} while (false);

	return nRet;
}
