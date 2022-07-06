#pragma once
#include "GPCS4Common.h"

namespace plat
{
	void* moduleLoad(const char* name);

	void moduleFree(void* handle);

	void* moduleGet(const char* name);

	void* moduleGetSymbol(void* handle, const char* symbol);
}  // namespace plat