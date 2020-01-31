#include "sce_libc.h"

LOG_CHANNEL(SceModules.SceLibc.memory);

void* PS4API scec_malloc(size_t size)
{
	auto p = malloc(size);
	LOG_SCE_TRACE("size %d, addr:%p", size, p);
	return p;
}


void* PS4API scec_realloc(void *ptr, size_t size)
{
	void *p = realloc(ptr, size);
	LOG_SCE_TRACE("ptr %p size %d, newptr:%p", ptr, size, p);
	return p;

}

void* PS4API scec_calloc(size_t nmemb, size_t size)
{
	LOG_SCE_TRACE("count %d size %d", nmemb, size);
	return calloc(nmemb, size);
}

void PS4API scec_free(void *ptr)
{
	LOG_SCE_DUMMY_IMPL();
	return;
	//LOG_SCE_TRACE("ptr %p", ptr)
	//return free(ptr);
}

