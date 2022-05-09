#pragma once
#include "winpthreads/include/pthread.h"
#include "winpthreads/include/sched.h"
#include "winpthreads/include/semaphore.h"

// pthread to sce pthread
// Note:
// PS4 system libraries (e.g. libc) will access the internal structure field
// of opaque types.
// So we need to provide enough dummy space to prevent access violations.
// This still have problems.
// If the system library read some fields of the returned result,
// because it's just dummy values, the behavior is undefined.

struct sce_pthread_attr_t
{
	uint32_t       dummy[256];
	pthread_attr_t handle;
};

struct sce_pthread_barrier_t
{
	uint32_t          dummy[256];
	pthread_barrier_t handle;
};

struct sce_pthread_barrierattr_t
{
	uint32_t              dummy[256];
	pthread_barrierattr_t handle;
};

struct sce_pthread_condattr_t
{
	uint32_t           dummy[256];
	pthread_condattr_t handle;
};

struct sce_pthread_cond_t
{
	uint32_t       dummy[256];
	pthread_cond_t handle;
};

struct sce_pthread_t
{
	uint32_t  dummy[256];
	pthread_t handle;
};

struct sce_pthread_key_t
{
	uint32_t      dummy[256];
	pthread_key_t handle;
};

struct sce_pthread_mutex_t
{
	uint32_t        dummy[256];
	pthread_mutex_t handle;
};

struct sce_pthread_mutexattr_t
{
	uint32_t            dummy[256];
	pthread_mutexattr_t handle;
};

struct sce_pthread_once_t
{
	uint32_t       dummy[256];
	pthread_once_t handle;
};

struct sce_pthread_rwlock_t
{
	uint32_t         dummy[256];
	pthread_rwlock_t handle;
};

struct sce_pthread_rwlockattr_t
{
	uint32_t             dummy[256];
	pthread_rwlockattr_t handle;
};


typedef struct sce_pthread_mutex_t* ScePthreadMutex;
typedef struct sce_pthread_attr_t*  ScePthreadAttr;

// Note:
// The following type mappings are unsafe.
// We currently use these types just because
// we assume the system library won't access
// private fields of those types.
// If we found problems as development going,
// we still need to use the above trick.

typedef pthread_t             ScePthread;
typedef pthread_condattr_t    ScePthreadCondattr;
typedef pthread_cond_t        ScePthreadCond;
typedef pthread_barrier_t     ScePthreadBarrier;
typedef pthread_barrierattr_t ScePthreadBarrierattr;
typedef pthread_mutexattr_t   ScePthreadMutexattr;
typedef pthread_once_t        ScePthreadOnce;
typedef pthread_rwlock_t      ScePthreadRwlock;
typedef pthread_rwlockattr_t  ScePthreadRwlockattr;


// The following types are simple integer types,
// it's save to map the original pthread type.
typedef pthread_key_t ScePthreadKey;


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


typedef enum ScePthreadRwlocktype 
{
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


struct sce_sched_param 
{
	int     sched_priority;
};

typedef struct sce_sched_param  SceKernelSchedParam;



#define SCE_KERNEL_PRIO_FIFO_DEFAULT    700
#define SCE_KERNEL_PRIO_FIFO_HIGHEST    256
#define SCE_KERNEL_PRIO_FIFO_LOWEST     767

