#pragma once

#include "GPCS4Common.h"
#include <string>

class CLinker
{
public:
	CLinker();
	~CLinker();

	bool ResolveSymbol(const std::string& strModName, const std::string& strLibName, uint64 nNid, void** ppAddress);

private:

};


