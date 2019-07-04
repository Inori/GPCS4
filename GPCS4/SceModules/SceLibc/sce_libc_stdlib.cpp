#include "sce_libc.h"
#include <ctime>


//////////////////////////////////////////////////////////////////////////
// This is the very first function a game calls
int PS4API scec_init_env(void* env)
{
	LOG_SCE_TRACE("env %p", env);
	return SCE_OK;
}


int PS4API scec_atexit(pfunc_exit_handler handler)
{
	LOG_SCE_TRACE("handler %p", handler);
	return scec___cxa_atexit((pfunc_cxa_exit_handler)handler, NULL, NULL);
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



void PS4API scec_qsort(void *base, size_t nmemb, size_t size, 
	int(*compar)(const void *, const void *))
{
	LOG_SCE_TRACE("base %p nmemb %d size %d compar %p", base, nmemb, size, compar);
	qsort(base, nmemb, size, compar);
}


int PS4API scec_rand(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


scetime_t PS4API scec_time(scetime_t* pt)
{
	LOG_SCE_TRACE("pt %p", pt);
	return time(pt);
}

