#pragma once

#include "GPCS4Common.h"
#include "Module.h"
#include "VirtualCPU.h"
#include "VirtualGPU.h"
#include "UtilSingleton.h"


typedef void (PS4NORETURN PS4API *PFUNC_EntryPoint)(void* pEnv, void* pfnExitHandler);

class Emulator final : public Singleton<Emulator>
{
	friend class Singleton<Emulator>;
public:

	bool Init();

	void Unit();

	bool Run(NativeModule const &mod);

	VirtualCPU& CPU();

	sce::VirtualGPU& GPU();

private:
	Emulator();
	~Emulator();

private:
	bool registerModules();

	void loadIntoCPU(NativeModule const& mod);

	bool executeEntry(NativeModule const& mod);

	static void PS4API LastExitHandler(void);
private:
	VirtualCPU      m_cpu;
	sce::VirtualGPU m_gpu;
};



// for convenience access 

inline Emulator& TheEmulator()
{
	return *Emulator::GetInstance();
}

inline VirtualCPU& CPU()
{
	return TheEmulator().CPU();
}

inline sce::VirtualGPU& GPU()
{
	return TheEmulator().GPU();
}