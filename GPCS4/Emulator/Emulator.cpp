#include "Emulator.h"
#include "GameThread.h"
#include "SceModuleSystem.h"
#include "VirtualCPU.h"
#include "VirtualGPU.h"
#include "Sce/SceGnmDriver.h"
#include "Sce/SceVideoOut.h"

LOG_CHANNEL(Emulator);

Emulator::Emulator() 
{
	m_cpu = std::make_unique<VirtualCPU>();
	m_gpu = std::make_unique<sce::VirtualGPU>();
}

Emulator::~Emulator() {}

void Emulator::loadIntoCPU(NativeModule const& mod)
{
	auto modInfo = mod.getModuleInfo();
	// assume there is only one code segment for a sce elf file.
	// this is true if the target file is dumped by hack tools like
	// ps4-dumper-vtx
	CodeBlock block = { modInfo.pCodeAddr, modInfo.nCodeSize };
	m_cpu->load(block);
}

bool Emulator::executeEntry(NativeModule const& mod)
{
	bool ret = false;
	do
	{
		void* entryPoint = mod.getEntryPoint();
		if (entryPoint == nullptr)
		{
			LOG_ERR("fail to get entry point");
			break;
		}

		struct PS4StartupParams
		{
			uint64_t    argc    = 1;
			const char* argv[1] = { "eboot.bin" };
		};

		PS4StartupParams startupParams;

		LOG_DEBUG("run into eboot.");
		CGameThread oMainThread(entryPoint, &startupParams, Emulator::LastExitHandler);
		if (!oMainThread.Start())
		{
			break;
		}

		// block the emulator's thread
		if (!oMainThread.Join(NULL))
		{
			break;
		}

		ret  = true;
	}while(false);
	return ret;
}

bool Emulator::Init()
{
	bool bRet = false;
	do
	{
		if (!registerModules())
		{
			break;
		}

		bRet = true;
	} while (false);
	return bRet;
}

void Emulator::Unit()
{
	auto modManager = CSceModuleSystem::GetInstance();
	modManager->clearModules();
}

bool Emulator::Run(NativeModule const &mod)
{
	bool retVal = false;

	do
	{
		if (mod.isModule())
		{
			LOG_ERR("%s is not an executable", mod.fileName.c_str());
			break;
		}

		loadIntoCPU(mod);

		if (!executeEntry(mod))
		{
			break;
		}

	retVal = true;
	} while (false);

	return retVal;
}

VirtualCPU& Emulator::CPU()
{
	return *m_cpu;
}

sce::VirtualGPU& Emulator::GPU()
{
	return *m_gpu;
}

void PS4API Emulator::LastExitHandler(void) { LOG_DEBUG("program exit."); }

