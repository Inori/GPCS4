#include "Emulator/Emulator.h"
#include "Emulator/SceModuleSystem.h"
#include "Emulator/TLSHandler.h"
#include "Loader/ModuleLoader.h"

#include <cxxopts/cxxopts.hpp>
#include <memory>

LOG_CHANNEL(Main);

cxxopts::ParseResult processCommandLine(int argc, char* argv[])
{
	cxxopts::Options opts("GPCS4", "PlayStation 4 Emulator");
	opts.allow_unrecognised_options();
	opts.add_options()
		("E,eboot", "Set main executable. The folder where GPCS4.exe located will be mapped to /app0.", cxxopts::value<std::string>())
		("D,debug-channel", "Enable debug channel. 'ALL' for all channels.", cxxopts::value<std::vector<std::string>>())
		("L,list-channels", "List debug channels.")
		("H,help", "Print help message.")
		;

	// Backup arg count,
	// because cxxopts will change argc value internally,
	// which I think is a bad design.
	const uint32_t argCount = argc;

	auto optResult          = opts.parse(argc, argv);
	if (optResult.count("H") || argCount < 2)
	{
		auto helpString = opts.help();
		printf("%s\n", helpString.c_str());
		exit(-1);
	}

	return optResult;
}


int main(int argc, char *argv[])
{
	std::unique_ptr<CEmulator> pEmulator = std::make_unique<CEmulator>();
	int nRet = -1;

	do
	{
		auto optResult = processCommandLine(argc, argv);

		// Initialize log system.
		logsys::init(optResult);

		if (!optResult["E"].count())
		{
			break;
		}

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

		auto eboot                      = optResult["E"].as<std::string>();
		NativeModule *ebootModule = nullptr;
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
