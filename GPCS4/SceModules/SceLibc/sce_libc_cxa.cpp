#include "sce_libc.h"
#include <mutex>


// at least 32
// currently give a large value to prevent atexit failed
#define SCE_CXA_EXIT_HANDLER_MAX 0x1000 

// note:
// when dso_handler and arg are both NULL
// we should call exit_handler with no arg
// this is strange, but it seems it's the standard
// http://refspecs.linuxbase.org/LSB_3.1.0/LSB-Core-generic/LSB-Core-generic/baselib---cxa-atexit.html
// also, all handlers are sysv abi, of course.

struct CAX_EXIT_HANDLER_INFO
{
	pfunc_cxa_exit_handler exit_handler;
	void* dso_handler;
	void* arg;
};

struct CXA_EXIT_HANDLER_ARRAY
{
	std::mutex mutex;
	uint count;
	CAX_EXIT_HANDLER_INFO handler_array[SCE_CXA_EXIT_HANDLER_MAX];
};

CXA_EXIT_HANDLER_ARRAY g_cxa_exit_handlers;

int PS4API scec___cxa_atexit(pfunc_cxa_exit_handler func, void * arg, void * dso_handle)
{
	LOG_SCE_TRACE("func %p arg %p dso %p", func, arg, dso_handle);

	// TODO:
	// currently we just record, these should be executed on program exit

	int ret = -1;
	std::lock_guard<std::mutex> lock(g_cxa_exit_handlers.mutex);
	do 
	{
		if (g_cxa_exit_handlers.count >= SCE_CXA_EXIT_HANDLER_MAX)
		{
			break;
		}

		uint& idx = g_cxa_exit_handlers.count;
		g_cxa_exit_handlers.handler_array[idx].exit_handler = func;
		g_cxa_exit_handlers.handler_array[idx].dso_handler = dso_handle;
		g_cxa_exit_handlers.handler_array[idx].arg = arg;
		++idx;

		ret = 0;
	} while (false);

	return ret;
}


int PS4API scec___cxa_guard_acquire(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec___cxa_guard_release(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec___cxa_pure_virtual(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}
