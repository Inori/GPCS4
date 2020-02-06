#include "sce_libc.h"
#include <ctime>

LOG_CHANNEL(SceModules.SceLibc.stdlib);

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


void PS4API scec_exit(int status)
{
	LOG_SCE_TRACE("status %d", status);
#ifdef GPCS4_WINDOWS
	__debugbreak();
#else
	raise(SIGTRAP);
#endif
	exit(status);
}


int PS4API scec_abort(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}



double PS4API scec_atof(const char* nptr)
{
	LOG_SCE_TRACE("ptr %p", nptr);
	return atof(nptr);
}


unsigned long PS4API scec__Stoul(const char* str, char ** endptr, int base)
{
	LOG_SCE_TRACE("str %p endptr %p base %d", str, endptr, base);
	return strtoul(str, endptr, base);
}


int PS4API scec__Stoull(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_strtok(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


long int PS4API scec_strtol(const char *nptr, char **endptr, int base)
{
	LOG_SCE_TRACE("nptr %p endptr %p base %d", nptr, endptr, base);
	return strtol(nptr, endptr, base);
}



int PS4API scec_bsearch(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}

typedef int (PS4API *PFUNC_QsortCmp)(const void*, const void*);
thread_local static PFUNC_QsortCmp t_qsort_compair_ps4;

int qsort_compair(const void* arg1, const void* arg2)
{
	return t_qsort_compair_ps4(arg1, arg2);
}

void PS4API scec_qsort(void *base, size_t nmemb, size_t size, 
	int(PS4API *compar)(const void *, const void *))
{
	LOG_SCE_TRACE("base %p nmemb %d size %d compar %p", base, nmemb, size, compar);
	t_qsort_compair_ps4 = compar;
	qsort(base, nmemb, size, qsort_compair);
}


int PS4API scec_rand(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


sce_time_t PS4API scec_time(sce_time_t* pt)
{
	LOG_SCE_TRACE("pt %p", pt);
	return time(pt);
}

