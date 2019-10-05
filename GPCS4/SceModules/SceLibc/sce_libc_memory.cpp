#include "sce_libc.h"


void* PS4API scec_malloc(size_t size)
{
	LOG_SCE_TRACE("size %d", size)
	return malloc(size);
}


int PS4API scec_realloc(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


void PS4API scec_free(void *ptr)
{
	LOG_SCE_TRACE("ptr %p", ptr)
	return free(ptr);
}

