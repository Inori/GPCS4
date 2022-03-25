#include "sce_libc.h"
#include "Emulator/Emulator.h"
#include "Emulator/VirtualCPU.h"

LOG_CHANNEL(SceModules.SceLibc.memory);

void* PS4API scec_malloc(size_t size)
{
	auto& allocator = CPU().allocator();
	auto  p         = allocator.sce_malloc(size);
	LOG_SCE_TRACE("size %zu, addr:%p", size, p);
	return p;
}


void* PS4API scec_realloc(void *ptr, size_t size)
{
	auto& allocator = CPU().allocator();
	auto  p         = allocator.sce_realloc(ptr, size);
	LOG_SCE_TRACE("ptr %zu size %d, newptr:%p", ptr, size, p);
	return p;

}

void* PS4API scec_calloc(size_t nmemb, size_t size)
{
	auto& allocator = CPU().allocator();
	auto  p         = allocator.sce_calloc(nmemb, size);
	LOG_SCE_TRACE("count %zu size %d", nmemb, size);
	return p;
}

void PS4API scec_free(void *ptr)
{
	LOG_SCE_DUMMY_IMPL();
	return;
	//LOG_SCE_TRACE("ptr %p", ptr)
	//return free(ptr);
}

