#include "Emulator.h"
#include "SceModuleSystem.h"
#include "sce_modules.h"

LOG_CHANNEL(Emulator);

using Policy = CSceModuleSystem::LibraryRecord::OverridingPolicy;

#define REGISTER_MODULE(name)                                                                  \
	if (!pModuleSystem->registerBuiltinModule(name))                                           \
	{                                                                                          \
		LOG_ERR("Register module failed: %s", name.szModuleName ? name.szModuleName : "null"); \
		break;                                                                                 \
	}

#define USE_NATIVE_MODULE(name)                                    \
	if (!pModuleSystem->setModuleOverridability(name, true))       \
	{                                                              \
		LOG_ERR("Fail to set overridability for module %s", name); \
		break;                                                     \
	}\


// Define a builtin function preference list.
// The linker will use builtin functions in this list over native functions in firmware modules.
#define BUILTIN_LIST_BEGIN(mod, lib)                                                    \
	if (!pModuleSystem->setLibraryOverridability(mod, lib, true, Policy::DisallowList)) \
	{                                                                                   \
		LOG_ERR("Fail to begin builtin list for library %s", lib);                      \
		break;                                                                          \
	}

#define USE_BUILTIN_FUNCTION(mod, lib, nid)                              \
	if (!pModuleSystem->setFunctionOverridability(mod, lib, nid, false)) \
	{                                                                    \
		LOG_ERR("Fail to add function in builtin list %llx", nid);       \
		break;                                                           \
	}

#define BUILTIN_LIST_END()


// Define a native function preference list.
// The linker will use native functions (in firmware modules.) in this list over builtin functions we HLEed in SceModules.
#define NATIVE_LIST_BEGIN(mod, lib)                                                  \
	if (!pModuleSystem->setLibraryOverridability(mod, lib, true, Policy::AllowList)) \
	{                                                                                \
		LOG_ERR("Fail to set overridability for library %s", lib);                   \
		break;                                                                       \
	}

#define USE_NATIVE_FUNCTION(mod, lib, nid)                              \
	if (!pModuleSystem->setFunctionOverridability(mod, lib, nid, true)) \
	{                                                                   \
		LOG_ERR("Fail to add function in native list %llx", nid);       \
		break;                                                          \
	}

#define NATIVE_LIST_END()

			

bool CEmulator::registerLibC(CSceModuleSystem* pModuleSystem)
{
	bool ret = false;
	do
	{
		if (!pModuleSystem)
		{
			break;
		}

		BUILTIN_LIST_BEGIN("libc", "libc");
		USE_BUILTIN_FUNCTION("libc", "libc", 0xC5E60EE2EEEEC89DULL);  // fopen
		USE_BUILTIN_FUNCTION("libc", "libc", 0xAD0155057A7F0B18ULL);  // fseek
		USE_BUILTIN_FUNCTION("libc", "libc", 0x41ACF2F0B9974EFCULL);  // ftell
		USE_BUILTIN_FUNCTION("libc", "libc", 0x95B07E52566A546DULL);  // fread
		USE_BUILTIN_FUNCTION("libc", "libc", 0xBA874B632522A76DULL);  // fclose
		USE_BUILTIN_FUNCTION("libc", "libc", 0x8105FEE060D08E93ULL);  // malloc
		USE_BUILTIN_FUNCTION("libc", "libc", 0x63B689D6EC9D3CCAULL);  // realloc
		USE_BUILTIN_FUNCTION("libc", "libc", 0xD97E5A8058CAC4C7ULL);  // calloc
		USE_BUILTIN_FUNCTION("libc", "libc", 0xB4886CAA3D2AB051ULL);  // free
		USE_BUILTIN_FUNCTION("libc", "libc", 0x5CA45E82C1691299ULL);  // catchReturnFromMain
		USE_BUILTIN_FUNCTION("libc", "libc", 0xB8C7A2D56F6EC8DAULL);  // exit
		USE_BUILTIN_FUNCTION("libc", "libc", 0xC0B9459301BD51C4ULL);  // time
		USE_BUILTIN_FUNCTION("libc", "libc", 0x80D435576BDF5C31ULL);  // setjmp
		USE_BUILTIN_FUNCTION("libc", "libc", 0x94A10DD8879B809DULL);  // longjmp
		BUILTIN_LIST_END();

		ret  = true;
	}while(false);
	return ret;
}

bool CEmulator::registerLibKernel(CSceModuleSystem* pModuleSystem)
{
	bool ret = false;
	do
	{
		if (!pModuleSystem)
		{
			break;
		}

		NATIVE_LIST_BEGIN("libkernel", "libkernel");
		USE_NATIVE_FUNCTION("libkernel", "libkernel", 0xF41703CA43E6A352);  // __error
		USE_NATIVE_FUNCTION("libkernel", "libkernel", 0x581EBA7AFBBC6EC5);  // sceKernelGetCompiledSdkVersion
		USE_NATIVE_FUNCTION("libkernel", "libkernel", 0x8E1FBC5E22B82DE1);  // sceKernelIsAddressSanitizerEnabled
		USE_NATIVE_FUNCTION("libkernel", "libkernel", 0x0F8CA56B7BF1E2D6);  // sceKernelError
		NATIVE_LIST_END();

		ret  = true;
	}while(false);
	return ret;
}

bool CEmulator::registerModules()
{
	bool bRet = false;
	do 
	{
		CSceModuleSystem* pModuleSystem = CSceModuleSystem::GetInstance();
		if (!pModuleSystem)
		{
			break;
		}

		REGISTER_MODULE(g_ExpModuleSceAjm);
		REGISTER_MODULE(g_ExpModuleSceAppContentUtil);
		REGISTER_MODULE(g_ExpModuleSceAudio3d);
		REGISTER_MODULE(g_ExpModuleSceAudioOut);
		REGISTER_MODULE(g_ExpModuleSceCommonDialog);
		REGISTER_MODULE(g_ExpModuleSceErrorDialog);
		REGISTER_MODULE(g_ExpModuleSceFiber);
		REGISTER_MODULE(g_ExpModuleSceFios2);
		REGISTER_MODULE(g_ExpModuleSceGameLiveStreaming);
		REGISTER_MODULE(g_ExpModuleSceGnmDriver);
		REGISTER_MODULE(g_ExpModuleSceHttp);
		REGISTER_MODULE(g_ExpModuleSceIme);
		REGISTER_MODULE(g_ExpModuleSceInvitationDialog);
		REGISTER_MODULE(g_ExpModuleSceJobManager);
		REGISTER_MODULE(g_ExpModuleSceJson);
		REGISTER_MODULE(g_ExpModuleSceLibc);
		REGISTER_MODULE(g_ExpModuleSceLibkernel);
		REGISTER_MODULE(g_ExpModuleSceMouse);
		REGISTER_MODULE(g_ExpModuleSceMsgDialog);
		REGISTER_MODULE(g_ExpModuleSceNet);
		REGISTER_MODULE(g_ExpModuleSceNetCtl);
		REGISTER_MODULE(g_ExpModuleSceNpCommerce);
		REGISTER_MODULE(g_ExpModuleSceNpCommon);
		REGISTER_MODULE(g_ExpModuleSceNpManager);
		REGISTER_MODULE(g_ExpModuleSceNpMatching2);
		REGISTER_MODULE(g_ExpModuleSceNpProfileDialog);
		REGISTER_MODULE(g_ExpModuleSceNpScore);
		REGISTER_MODULE(g_ExpModuleSceNpSignaling);
		REGISTER_MODULE(g_ExpModuleSceNpTrophy);
		REGISTER_MODULE(g_ExpModuleSceNpTus);
		REGISTER_MODULE(g_ExpModuleSceNpWebApi);
		REGISTER_MODULE(g_ExpModuleScePad);
		REGISTER_MODULE(g_ExpModuleScePlayGo);
		REGISTER_MODULE(g_ExpModuleScePlayGoDialog);
		REGISTER_MODULE(g_ExpModuleSceRtc);
		REGISTER_MODULE(g_ExpModuleSceRudp);
		REGISTER_MODULE(g_ExpModuleSceSaveData);
		REGISTER_MODULE(g_ExpModuleSceSaveDataDialog);
		REGISTER_MODULE(g_ExpModuleSceScreenShot);
		REGISTER_MODULE(g_ExpModuleSceSsl);
		REGISTER_MODULE(g_ExpModuleSceSysmodule);
		REGISTER_MODULE(g_ExpModuleSceSystemService);
		REGISTER_MODULE(g_ExpModuleSceUserService);
		REGISTER_MODULE(g_ExpModuleSceVideoOut);
		REGISTER_MODULE(g_ExpModuleSceVideoRecording);

		//////////////////////////////////////////////////////////////////////////
		USE_NATIVE_MODULE("libSceLibcInternal");
		USE_NATIVE_MODULE("libSceNpCommon");

		//////////////////////////////////////////////////////////////////////////
		if (!registerLibKernel(pModuleSystem))
		{
			break;
		}

		if (!registerLibC(pModuleSystem))
		{
			break;
		}

		bRet = true;
	} while (false);
	return bRet;

}
