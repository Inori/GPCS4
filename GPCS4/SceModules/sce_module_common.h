#pragma once

#include "GPCS4Common.h"

#include "sce_types.h"
#include "sce_errors.h"


struct SCE_EXPORT_FUNCTION
{
	const uint64_t nNid;
	const char* szFunctionName;
	const void* pFunction;

	bool isEndEntry() const
	{
		auto pFunc = this;
		return (pFunc->nNid == 0 && pFunc->szFunctionName == nullptr &&
			pFunc->pFunction == nullptr);
	}

	const SCE_EXPORT_FUNCTION *iterNext() const
	{
		return this + 1;
	}
};
#define SCE_FUNCTION_ENTRY_END {0, NULL, NULL}


struct SCE_EXPORT_LIBRARY
{
	const char* szLibraryName;
	const SCE_EXPORT_FUNCTION *pFunctionEntries;

	bool isEndEntry() const
	{
		auto pLib = this;
		return (pLib->szLibraryName == nullptr && pLib->pFunctionEntries == nullptr);
	}

	const SCE_EXPORT_LIBRARY *iterNext() const
	{
		return this + 1;
	}
};
#define SCE_LIBRARY_ENTRY_END {NULL, NULL}


struct SCE_EXPORT_MODULE
{
	const char* szModuleName;
	const SCE_EXPORT_LIBRARY* pLibraries;
};

