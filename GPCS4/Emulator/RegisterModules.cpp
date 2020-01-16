#include "Emulator.h"
#include "SceModuleSystem.h"
#include "sce_modules.h"


#define REGISTER_MODULE(name)                                                                  \
	if (!pModuleSystem->registerBuiltinModule(name))                                           \
	{                                                                                          \
		LOG_ERR("Register module failed: %s", name.szModuleName ? name.szModuleName : "null"); \
		break;                                                                                 \
	}

bool CEmulator::registerLibC(CSceModuleSystem* pModuleSystem)
{
	bool ret = false;
	do
	{
		if (!pModuleSystem)
		{
			break;
		}
		auto& policyManager = pModuleSystem->getPolicyManager();

		/** 
		 * Defines policy for libc.
		 * 
		 * The default policy for this module is Policy::UseBuitlin,
		 * which means when a sub-library of libc is not defined, 
		 * it applies the Policy::UseBuiltin by default. 
		 * 
		 * The libc sub-library uses implementations from native modules
		 * except the symbols listed below.
		 */
		policyManager.declareModule("libc").withDefault(Policy::UseBuiltin)
			.declareSubLibrary("libc").with(Policy::UseNative).except
			({ 
				0xC5E60EE2EEEEC89DULL, // fopen
				0xAD0155057A7F0B18ULL, // fssek
				0x41ACF2F0B9974EFCULL, // ftell
				0x95B07E52566A546DULL, // fread
				0xBA874B632522A76DULL, // fclose
				0x8105FEE060D08E93ULL, // malloc
				0x63B689D6EC9D3CCAULL, // realloc
				0xD97E5A8058CAC4C7ULL, // calloc
				0xB4886CAA3D2AB051ULL, // free
				0x5CA45E82C1691299ULL, // catchReturnFromMain
				0xB8C7A2D56F6EC8DAULL, // exit
				0xC0B9459301BD51C4ULL, // time
				0x80D435576BDF5C31ULL, // setjmp
				0x94A10DD8879B809DULL  // longjmp
			 });

		/*
		 * Defines policy for libSceLibcInternal
		 *
		 * We load all of its symbols
		 */
		policyManager
			.declareModule("libSceLibcInternal").withDefault(Policy::UseNative);

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

		auto& policyManager = pModuleSystem->getPolicyManager();

		/** 
		 * Defines policy for libkernel.
		 * 
		 * The default policy for this module is Policy::UseBuitlin,
		 * which means when a sub-library of libkernel is not defined, 
		 * it applies the Policy::UseBuiltin policy by default. 
		 * 
		 * The libc sub-library uses implementations from builtin modules
		 * except the symbols listed below.
		 */
		policyManager.declareModule("libkernel").withDefault(Policy::UseBuiltin)
			.declareSubLibrary("libkernel").with(Policy::UseBuiltin).except
			({ 
				0xF41703CA43E6A352, // __error
				0x581EBA7AFBBC6EC5  // sceKernelGetCompiledSdkVersion
			 });


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

		if (!registerLibKernel(pModuleSystem))
		{
			break;
		}

		if (!registerLibC(pModuleSystem))
		{
			break;
		}

		// Yes, the policy is testable now, although we should not put the 
		// test code here
		auto testPolicy = [&]() {
			auto pm = pModuleSystem->getPolicyManager();

			auto p1 = pm.getSymbolPolicy("libc", "libc", 0xC5E60EE2EEEEC89DULL);
			LOG_ASSERT(p1 == Policy::UseBuiltin, "policy error");

			auto p2 = pm.getSymbolPolicy("libkernel", "libkernel", 0xF41703CA43E6A352);
			LOG_ASSERT(p2 == Policy::UseNative, "policy error2");

			auto p3 = pm.getSymbolPolicy("libSceNgs2", "libSceNgs2", 0xDE908D6D5335D540);
			LOG_ASSERT(p3 == Policy::UseNative, "policy error3");

			auto p4 = pm.getSymbolPolicy("libSceLibcInternal", "libSceLibcInternal", 0x80D435576BDF5C31);
			LOG_ASSERT(p4 == Policy::UseNative, "policy error4");
		};

		testPolicy();

		bRet = true;
	} while (false);
	return bRet;

}
