#pragma once
#include "pthreads4w/pthread.h"
#include "pthreads4w/sched.h"
#include "pthreads4w/semaphore.h"

// pthread to sce pthread
// Note:
// we need to take care of the size of these structs if it's not a pointer type
// pthread4w's implementation and ps4's implementation has different size
typedef uint64_t				  ScePthread;
typedef pthread_once_t            ScePthreadOnce;

// the following types are pointers, which is safe
typedef pthread_attr_t            ScePthreadAttr;
typedef pthread_barrier_t         ScePthreadBarrier;
typedef pthread_barrierattr_t     ScePthreadBarrierattr;
typedef pthread_condattr_t        ScePthreadCondattr;
typedef pthread_cond_t            ScePthreadCond;
typedef pthread_key_t             ScePthreadKey;
typedef pthread_mutex_t           ScePthreadMutex;
typedef pthread_mutexattr_t       ScePthreadMutexattr;
typedef pthread_rwlock_t          ScePthreadRwlock;
typedef pthread_rwlockattr_t      ScePthreadRwlockattr;


typedef enum ScePthreadMutextype
{
	SCE_PTHREAD_MUTEX_ERRORCHECK = 1,
	SCE_PTHREAD_MUTEX_RECURSIVE = 2,
	SCE_PTHREAD_MUTEX_NORMAL = 3,	
	SCE_PTHREAD_MUTEX_ADAPTIVE_NP = 4,
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


typedef uint64_t	SceKernelCpumask;
// SceKernelCpumask 
#define SCE_KERNEL_CPUMASK_6CPU_ALL   0x3f
#define SCE_KERNEL_CPUMASK_7CPU_ALL   0x7f
#define SCE_KERNEL_CPUMASK_USER_ALL   0x3f

// ps4 have at most have 7 cpus
#define SCE_KERNEL_CPU_MAX	7

// detach state
#define SCE_PTHREAD_CREATE_DETACHED     1
#define SCE_PTHREAD_CREATE_JOINABLE     0

// inherit sched
#define SCE_PTHREAD_EXPLICIT_SCHED      0
#define SCE_PTHREAD_INHERIT_SCHED       0x4


struct sce_sched_param {
	int     sched_priority;
};

typedef struct sce_sched_param  SceKernelSchedParam;



#define SCE_KERNEL_PRIO_FIFO_DEFAULT    700
#define SCE_KERNEL_PRIO_FIFO_HIGHEST    256
#define SCE_KERNEL_PRIO_FIFO_LOWEST     767

