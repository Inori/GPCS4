#pragma once
#include "sce_libkernel.h"
#include "pthreads4w/pthread.h"
#include "MapSlot.h"

struct SCE_THREAD_PARAM
{
	void* entry;
	void* arg;
};

inline bool isEmptyPthreadT(pthread_t& pt)
{
	return pt.p == NULL && pt.x == 0;
}

inline bool isEqualPthreadT(const pthread_t& lhs, const pthread_t& rhs)
{
	return lhs.p == rhs.p && lhs.x == rhs.x;
}

void *newThreadWrapper(void *arg);

#define SCE_THREAD_COUNT_MAX 1024
extern MapSlot<pthread_t, decltype(isEmptyPthreadT)> g_threadSlot;

typedef void* (PS4API *PFUNC_PS4_THREAD_ENTRY)(void*);
