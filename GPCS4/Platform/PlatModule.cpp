#include "PlatModule.h"

#ifdef GPCS4_WINDOWS

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#undef WIN32_LEAN_AND_MEAN

#else
#error "not implemented"
#endif

namespace plat
{
#ifdef GPCS4_WINDOWS

	void* moduleLoad(const char* name)
	{
		return (void*)LoadLibraryA(name);
	}

	void moduleFree(void* handle)
	{
		FreeLibrary((HMODULE)handle);
	}

	void* moduleGet(const char* name)
	{
		return (void*)GetModuleHandleA(name);
	}

	void* moduleGetSymbol(void* handle, const char* symbol)
	{
		return (void*)GetProcAddress((HMODULE)handle, symbol);
	}

#else

	void* moduleLoad(const char* name)
	{
	}

	void moduleFree(void* handle)
	{
	}

	void* moduleGet(const char* name)
	{
	}

	void* moduleGetSymbol(void* handle, char* symbol)
	{
	}


#endif



}  // namespace plat

