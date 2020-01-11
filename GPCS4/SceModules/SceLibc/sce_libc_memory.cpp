#include "sce_libc.h"


void* PS4API scec_malloc(size_t size)
{
	LOG_SCE_TRACE("size %d", size)
	return malloc(size);
}


void* PS4API scec_realloc(void *ptr, size_t size)
{
	LOG_SCE_TRACE("ptr %p size %d", ptr, size);
	return realloc(ptr, size);
}

void* PS4API scec_calloc(size_t nmemb, size_t size)
{
	LOG_SCE_TRACE("count %d size %d", nmemb, size);
	return calloc(nmemb, size);
}

void PS4API scec_free(void *ptr)
{
	LOG_SCE_TRACE("ptr %p", ptr)
	return free(ptr);
}

