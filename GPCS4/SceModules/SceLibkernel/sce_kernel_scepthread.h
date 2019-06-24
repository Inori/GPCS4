#pragma once

#include "pthreads4w/pthread.h"
#include "pthreads4w/sched.h"


// pthread to sce pthread
typedef pthread_attr_t            ScePthreadAttr;
typedef pthread_barrier_t         ScePthreadBarrier;
typedef pthread_barrierattr_t     ScePthreadBarrierattr;
typedef pthread_condattr_t        ScePthreadCondattr;
typedef pthread_cond_t            ScePthreadCond;
typedef pthread_t                 ScePthread;
typedef pthread_key_t             ScePthreadKey;
typedef pthread_mutex_t           ScePthreadMutex;
typedef pthread_mutexattr_t       ScePthreadMutexattr;
typedef pthread_once_t            ScePthreadOnce;
typedef pthread_rwlock_t          ScePthreadRwlock;
typedef pthread_rwlockattr_t      ScePthreadRwlockattr;


typedef enum ScePthreadMutextype
{
	SCE_PTHREAD_MUTEX_ERRORCHECK = 1,
	SCE_PTHREAD_MUTEX_RECURSIVE = 2,
	SCE_PTHREAD_MUTEX_NORMAL = 3,	
	SCE_PTHREAD_MUTEX_ADAPTIVE = 4,
	SCE_PTHREAD_MUTEX_TYPE_MAX
} ScePthreadMutextype;


// used for scePthreadMutexattrSetprotocol
#define SCE_PTHREAD_PRIO_NONE           0
#define SCE_PTHREAD_PRIO_INHERIT        1
#define SCE_PTHREAD_PRIO_PROTECT        2


typedef enum ScePthreadRwlocktype {
	SCE_PTHREAD_RWLOCK_NORMAL = 1,
	SCE_PTHREAD_RWLOCK_PREFER_READER = 2,
	SCE_PTHREAD_RWLOCK_TYPE_MAX
} ScePthreadRwlocktype;
