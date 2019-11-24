#include "Emulator.h"
#include "SceModuleSystem.h"
#include "sce_modules.h"


#define REGISTER_MODULE(name) \
if (!pModuleSystem->RegisterModule(name))\
{\
	LOG_ERR("Register module failed: %s", name.szModuleName ? name.szModuleName : "null" );\
	break;\
}

#define ALLOW_MODULE_OVERRIDE(name) \
if(!pModuleSystem->setModuleOverridability(name, true)) \
{\
	LOG_ERR("Fail to set overridability for module %s", name);\
	break;\
}\

bool CEmulator::RegisterModules()
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

		ALLOW_MODULE_OVERRIDE("libc");
		ALLOW_MODULE_OVERRIDE("libSceLibcInternal");
		pModuleSystem->setLibraryOverridability("libc", "libc", true);
		pModuleSystem->setLibraryOverridability(
			"libkernel", "libkernel", true,
			CSceModuleSystem::LibraryRecord::Mode::Allow);
		
		pModuleSystem->setFunctionOverridability("libkernel", "libkernel",
												 0xF41703CA43E6A352, true);
		/* disable below functions, which are implemented in virutal libc module */

		// fopen
		pModuleSystem->setFunctionOverridability("libc", "libc",
												 14260101637949278365, false);

		// fseek
		pModuleSystem->setFunctionOverridability("libc", "libc",
												 12466338725556587288, false);

		// ftell
		pModuleSystem->setFunctionOverridability("libc", "libc",
												 4732424424179322620, false);
		// fread
		pModuleSystem->setFunctionOverridability("libc", "libc",
												 10786259999654564973, false);

		// fclose
		pModuleSystem->setFunctionOverridability("libc", "libc",
												 13440794502107408237, false);

		// malloc
		pModuleSystem->setFunctionOverridability("libc", "libc",
												 9297117245426667155, false);

		// free
		pModuleSystem->setFunctionOverridability("libc", "libc",
												 13008767002086125649, false);
		// catchReturnFromMain
		pModuleSystem->setFunctionOverridability("libc", "libc",
												 0x5CA45E82C1691299, false);

		// exit
		pModuleSystem->setFunctionOverridability("libc", "libc",
												 0xB8C7A2D56F6EC8DA, false);
		// time
		pModuleSystem->setFunctionOverridability("libc", "libc",
												 0xC0B9459301BD51C4, false);
		bRet = true;
	} while (false);
	return bRet;

}
