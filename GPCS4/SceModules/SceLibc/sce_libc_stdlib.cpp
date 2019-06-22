#include "sce_libc.h"

// This is the very first function a game calls
int PS4API scec_init_env(void)
{
	LOG_SCE_TRACE("do nothing.");
	return SCE_OK;
}


int PS4API scec_atexit(void)
{
	LOG_FIXME("Not implemented");
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

