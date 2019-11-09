#pragma once
#include "sce_libkernel.h"
#include "pthreads4w/pthread.h"
#include "MapSlot.h"

struct SCE_THREAD_PARAM
{
	void* entry;
	void* arg;
};


bool isEmptyPthread(const pthread_t& pt);
bool isEqualPthread(const pthread_t& lhs, const pthread_t& rhs);

void *newThreadWrapper(void *arg);
typedef void* (PS4API *PFUNC_PS4_THREAD_ENTRY)(void*);

// for non pointer type, we need to build a map to fit the type

#define SCE_THREAD_COUNT_MAX 1024
extern MapSlot<pthread_t, isEmptyPthread, isEqualPthread> g_threadSlot;


