#include "Linker.h"
#include "SceModuleSystem.h"


CLinker::CLinker()
{
}

CLinker::~CLinker()
{
}

bool CLinker::ResolveSymbol(const std::string& strModName, const std::string& strLibName, uint64 nNid, void** ppAddress)
{
	bool bRet = false;
	do 
	{
		if (!ppAddress)
		{
			break;
		}
		
		void* pFunc = CSceModuleSystem::GetInstance()->FindFunction(strModName, strLibName, nNid);
		if (!pFunc)
		{
			break;
		}

		*ppAddress = pFunc;

		bRet = true;
	} while (false);
	return bRet;
}
