#pragma once
#include "sce_libkernel.h"
#include "winpthreads/include/pthread.h"
#include "MapSlot.h"

struct SCE_THREAD_PARAM
{
	void* entry;
	void* arg;
};

void *newThreadWrapper(void *arg);
typedef void* (PS4API *PFUNC_PS4_THREAD_ENTRY)(void*);


