#pragma once

#include "GPCS4Common.h"
#include "Module.h"
#include "UtilSingleton.h"

#include <memory>

class VirtualCPU;

namespace sce
{
class VirtualGPU;
}  // namespace sce



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
	std::unique_ptr<VirtualCPU>      m_cpu;
	std::unique_ptr<sce::VirtualGPU> m_gpu;
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