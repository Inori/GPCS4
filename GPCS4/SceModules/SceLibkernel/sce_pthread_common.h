#pragma once
#include "sce_libkernel.h"
#include "MapSlot.h"

#ifdef GPCS4_WINDOWS
	#include "winpthreads/include/pthread.h"
#else
	#include <pthread.h>
#endif

struct SCE_THREAD_PARAM
{
	void* entry;
	void* arg;
};

void *newThreadWrapper(void *arg);
typedef void* (PS4API *PFUNC_PS4_THREAD_ENTRY)(void*);


