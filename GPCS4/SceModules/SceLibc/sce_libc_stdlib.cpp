#include "sce_libc.h"
#include <mutex>

#define SCE_MAX_EXIT_HANDLER 0x10
struct EXIT_HANDLER_ARRAY
{
	std::mutex mutex;
	uint nCount;
	PFUNC_exit_handler pHandlers[SCE_MAX_EXIT_HANDLER];
};

EXIT_HANDLER_ARRAY g_stExitHandlerArray;

//////////////////////////////////////////////////////////////////////////
// This is the very first function a game calls
int PS4API scec_init_env(void* env)
{
	LOG_SCE_TRACE("env %p", env);
	return SCE_OK;
}


int PS4API scec_atexit(PFUNC_exit_handler handler)
{
	LOG_SCE_TRACE("handler %p", handler);
	std::lock_guard<std::mutex> lock(g_stExitHandlerArray.mutex);

	// TODO:
	// currently we just record, these should be executed on program exit
	g_stExitHandlerArray.pHandlers[g_stExitHandlerArray.nCount] = handler;
	++g_stExitHandlerArray.nCount;

	return SCE_OK;
}


int PS4API scec_exit(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_abort(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}



int PS4API scec_atof(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_bsearch(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}



int PS4API scec_qsort(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_rand(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_time(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}

