/*
 *    GPCS4
 *    
 *    This file implements:
 *    module: libkernel
 *        library: libkernel
 *        library: libScePosix
 *    
 */

#pragma once

#include "sce_module_common.h"
#include "sce_kernel_types.h"
#include "sce_kernel_scepthread.h"
#include "sce_kernel_file.h"
#include "sce_kernel_memory.h"
#include "sce_kernel_eventqueue.h"
#include "sce_kernel_eventflag.h"
#include "sce_kernel_tls.h"


extern const SCE_EXPORT_MODULE g_ExpModuleSceLibkernel;


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs



//////////////////////////////////////////////////////////////////////////
// library: libkernel
//////////////////////////////////////////////////////////////////////////

int* PS4API __error(void);


int PS4API __stack_chk_fail(void);


int PS4API __stack_chk_guard(void);


void* PS4API __tls_get_addr(tls_index *ti);


int PS4API __pthread_cxa_finalize(void);


int PS4API scek__write(int fd, const void* buf, size_t size);


int PS4API _sceKernelSetThreadDtors(void);


int PS4API _sceKernelSetThreadAtexitCount(void);


int PS4API _sceKernelSetThreadAtexitReport(void);


void *PS4API sceKernelGetProcParam(uint64_t p1, uint64_t p2);


int PS4API sceKernelAllocateDirectMemory(sce_off_t searchStart, sce_off_t searchEnd, size_t len, size_t alignment, int memoryType, sce_off_t *physAddrOut);


int PS4API sceKernelClockGettime(void);


int PS4API sceKernelClose(int d);


int PS4API sceKernelCreateEqueue(SceKernelEqueue *eq, const char *name);


int PS4API sceKernelCreateEventFlag(SceKernelEventFlag *ef, const char *pName, uint32_t attr, uint64_t initPattern, const SceKernelEventFlagOptParam *pOptParam);


int PS4API sceKernelPollEventFlag(SceKernelEventFlag ef, uint64_t bitPattern, uint32_t waitMode, uint64_t* pResultPat);


int PS4API sceKernelCreateSema(SceKernelSema *sem, const char *name, uint32_t attr, int init, int max, const SceKernelSemaOptParam *opt);


int PS4API sceKernelDeleteEqueue(SceKernelEqueue eq);


int PS4API sceKernelDeleteEventFlag(SceKernelEventFlag ef);


int PS4API sceKernelDeleteSema(SceKernelSema sem);


int PS4API sceKernelFstat(int fd, SceKernelStat *sb);


int PS4API sceKernelFsync(void);


int PS4API sceKernelFtruncate(void);


int PS4API sceKernelGetCpumode(void);


size_t PS4API sceKernelGetDirectMemorySize(void);


uint64_t PS4API sceKernelGetProcessTime(void);


uint64_t PS4API sceKernelGetProcessTimeCounter(void);


uint64_t PS4API sceKernelGetProcessTimeCounterFrequency(void);


uint64_t PS4API sceKernelGetTscFrequency(void);


int PS4API sceKernelGetdents(int fd, char *buf, int nbytes);


int PS4API sceKernelIsNeoMode(void);


sce_off_t PS4API sceKernelLseek(int fildes, sce_off_t offset, int whence);


int PS4API sceKernelMapDirectMemory(void **addr, size_t len, int prot, int flags, sce_off_t directMemoryStart, size_t maxPageSize);


int PS4API sceKernelMapFlexibleMemory(void **addrInOut, size_t len, int prot, int flags);


int PS4API sceKernelCheckReachability(void);


int PS4API sceKernelChmod(void);


int PS4API sceKernelMkdir(void);


int PS4API sceKernelOpen(const char *path, int flags, SceKernelMode mode);


ssize_t PS4API sceKernelRead(int d, void *buf, size_t nbytes);


uint64_t PS4API sceKernelReadTsc(void);


int PS4API sceKernelReleaseDirectMemory(sce_off_t start, size_t len);


int PS4API sceKernelReleaseFlexibleMemory(void);


int PS4API sceKernelRename(void);


int PS4API sceKernelSetEventFlag(SceKernelEventFlag ef, uint64_t bitPattern);


int PS4API sceKernelSignalSema(SceKernelSema sem, int count);


int PS4API sceKernelStat(const char *path, SceKernelStat *sb);


int PS4API sceKernelUnlink(void);


int PS4API sceKernelUsleep(SceKernelUseconds microseconds);


int PS4API sceKernelWaitEqueue(SceKernelEqueue eq, SceKernelEvent *ev, int num, int *out, SceKernelUseconds *timo);


int PS4API sceKernelWaitEventFlag(SceKernelEventFlag ef, uint64_t bitPattern, uint32_t waitMode, uint64_t *pResultPat, SceKernelUseconds *pTimeout);


int PS4API sceKernelWaitSema(SceKernelSema sem, int need, SceKernelUseconds *timo);


ssize_t PS4API sceKernelWrite(int d, const void *buf, size_t nbytes);


int PS4API scePthreadAttrDestroy(ScePthreadAttr *attr);


int PS4API scePthreadAttrGetschedparam(void);


int PS4API scePthreadAttrInit(ScePthreadAttr *attr);


int PS4API scePthreadAttrSetaffinity(void);


int PS4API scePthreadAttrSetdetachstate(ScePthreadAttr *attr, int state);


int PS4API scePthreadAttrSetinheritsched(ScePthreadAttr *attr, int inheritSched);


int PS4API scePthreadAttrSetschedparam(ScePthreadAttr *attr, const SceKernelSchedParam *param);


int PS4API scePthreadAttrSetschedpolicy(void);


int PS4API scePthreadAttrSetstacksize(ScePthreadAttr *attr, size_t stackSize);


int PS4API scePthreadCondBroadcast(ScePthreadCond *cond);


int PS4API scePthreadCondDestroy(ScePthreadCond *cond);


int PS4API scePthreadCondInit(ScePthreadCond *cond, const ScePthreadCondattr *attr, const char *name);


int PS4API scePthreadCondSignal(ScePthreadCond *cond);


int PS4API scePthreadCondWait(ScePthreadCond *cond, ScePthreadMutex *mutex);


int PS4API scePthreadCondattrDestroy(ScePthreadCondattr *attr);


int PS4API scePthreadCondattrInit(ScePthreadCondattr *attr);


int PS4API scePthreadCreate(ScePthread *thread, const ScePthreadAttr *attr, void *(PS4API *entry) (void *), void *arg, const char *name);


void PS4API scePthreadExit(void *value_ptr);


int PS4API scePthreadGetaffinity(void);


int PS4API scePthreadGetprio(ScePthread thread, int *prio);


int PS4API scePthreadJoin(ScePthread thread, void **value);


int PS4API scePthreadMutexDestroy(ScePthreadMutex *mutex);


int PS4API scePthreadMutexInit(ScePthreadMutex *mutex, const ScePthreadMutexattr *attr, const char *name);


int PS4API scePthreadMutexLock(ScePthreadMutex *mutex);


int PS4API scePthreadMutexUnlock(ScePthreadMutex *mutex);


int PS4API scePthreadMutexattrDestroy(ScePthreadMutexattr *attr);


int PS4API scePthreadMutexattrInit(ScePthreadMutexattr *attr);


int PS4API scePthreadMutexattrSetprotocol(ScePthreadMutexattr *attr, int protocol);


int PS4API scePthreadMutexattrSettype(ScePthreadMutexattr *attr, int type);


int PS4API scePthreadMutexGetprioceiling(void);


int PS4API scePthreadRename(void);


int PS4API scePthreadRwlockDestroy(ScePthreadRwlock *rwlock);


int PS4API scePthreadRwlockInit(ScePthreadRwlock *rwlock, const ScePthreadRwlockattr *attr, const char *name);


int PS4API scePthreadRwlockRdlock(ScePthreadRwlock *rwlock);


int PS4API scePthreadRwlockUnlock(ScePthreadRwlock *rwlock);


int PS4API scePthreadRwlockWrlock(ScePthreadRwlock *rwlock);


int PS4API scePthreadRwlockattrDestroy(ScePthreadRwlockattr *attr);


int PS4API scePthreadRwlockattrInit(ScePthreadRwlockattr *attr);


ScePthread PS4API scePthreadSelf(void);


int PS4API scePthreadSetaffinity(ScePthread thread, const SceKernelCpumask mask);


int PS4API scePthreadSetprio(ScePthread thread, int prio);


void PS4API scePthreadYield(void);


int PS4API sceKernelBatchMap(void);


int PS4API sceKernelCheckedReleaseDirectMemory(void);


int PS4API sceKernelDlsym(void);


int PS4API sceKernelGetDirectMemoryType(sce_off_t start, int *memoryType, sce_off_t *regionStartOut, sce_off_t *regionEndOut);


int PS4API sceKernelGetGPI(void);


int PS4API sceKernelGetPageTableStats(void);


int PS4API sceKernelGettimeofday(void);


int PS4API sceKernelIsStack(void);


int PS4API sceKernelLoadStartModule(void);


int PS4API sceKernelMapNamedDirectMemory(void **addr, size_t len, int prot, int flags, sce_off_t directMemoryStart, size_t alignment, const char *name);


int PS4API sceKernelMapNamedFlexibleMemory(void** addrInOut, size_t len, int prot, int flags, const char* name);


int PS4API sceKernelMprotect(void);


int PS4API sceKernelMtypeprotect(void);


int PS4API sceKernelMunmap(void);


int PS4API sceKernelQueryMemoryProtection(void* addr, void** start, void** end, uint32_t* prot);


int PS4API sceKernelReserveVirtualRange(void **addr, size_t len, int flags, size_t alignment);


int PS4API sceKernelGetPrtAperture(int apertureId, void **addr, size_t *len);


int PS4API sceKernelSetPrtAperture(int apertureId, void *addr, size_t len);


int PS4API sceKernelSetVirtualRangeName(void* start, size_t len, const char *name);


int PS4API sceKernelUuidCreate(void);


int PS4API sceKernelVirtualQuery(const void *addr, int flags, SceKernelVirtualQueryInfo *info, size_t infoSize);


int PS4API scePthreadCancel(void);


int PS4API scePthreadDetach(void);


int PS4API scePthreadGetname(void);


int PS4API scePthreadGetschedparam(void);


int PS4API scePthreadGetspecific(void);


int PS4API scePthreadKeyCreate(void);


int PS4API scePthreadAttrGet(ScePthread thread, ScePthreadAttr* attr);


int PS4API scePthreadAttrGetaffinity(ScePthread thread, SceKernelCpumask* mask);


int PS4API scePthreadMutexTimedlock(void);


int PS4API scePthreadMutexTrylock(ScePthreadMutex *mutex);


int PS4API scePthreadSetschedparam(void);


int PS4API scePthreadSetspecific(void);


void PS4API sceKernelDebugRaiseExceptionOnReleaseMode(uint32_t error_code, uint32_t param);


void PS4NORETURN PS4API sceKernelDebugRaiseException(uint32_t error_code, uint32_t param);


void PS4API _sceKernelRtldSetApplicationHeapAPI(void* heap_api);


bool PS4API sceKernelGetSanitizerMallocReplaceExternal();


bool PS4API sceKernelGetSanitizerNewReplaceExternal();


int PS4API scek___sys_regmgr_call(void);


int PS4API scek__open(const char* path, int flags, SceKernelMode mode);


int PS4API sceKernelGetProcessType(int pid);


int PS4API sceKernelIsAddressSanitizerEnabled(void);
//////////////////////////////////////////////////////////////////////////
// library: libSceCoredump
//////////////////////////////////////////////////////////////////////////

int PS4API sceCoredumpAttachMemoryRegion(void);


int PS4API sceCoredumpRegisterCoredumpHandler(void);


int PS4API sceCoredumpWriteUserData(void);



//////////////////////////////////////////////////////////////////////////
// library: libSceCoredump_debug
//////////////////////////////////////////////////////////////////////////

int PS4API sceCoredumpDebugTriggerCoredump(void);



//////////////////////////////////////////////////////////////////////////
// library: libSceOpenPsId
//////////////////////////////////////////////////////////////////////////

int PS4API sceKernelGetOpenPsId(void);



//////////////////////////////////////////////////////////////////////////
// library: libScePosix
//////////////////////////////////////////////////////////////////////////

int PS4API scek_clock_gettime(sce_clockid_t clk_id, struct sce_timespec * tp);


int PS4API scek_gettimeofday(void);


int PS4API scek_nanosleep(void);


//int PS4API scek_pthread_cond_destroy(void);
int PS4API scek_pthread_cond_destroy(pthread_cond_t *cond);

int PS4API scek_pthread_cond_init(pthread_cond_t *cond,
								  const pthread_condattr_t *attr);

//int PS4API scek_pthread_cond_signal(void);
int PS4API scek_pthread_cond_signal(pthread_cond_t *cond);


int PS4API scek_pthread_cond_timedwait(void);


//int PS4API scek_pthread_cond_wait(void);
int PS4API scek_pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);


int PS4API scek_pthread_create(ScePthread *thread,
							   const pthread_attr_t *attr,
							   void *(PS4API *entry)(void *),
							   void *args);


int PS4API scek_pthread_equal(void);


int PS4API scek_pthread_join(void);


int PS4API scek_pthread_mutex_destroy(pthread_mutex_t *mutex);

int PS4API scek_pthread_mutex_init(pthread_mutex_t *mutex,
								   const pthread_mutexattr_t *attr);


int PS4API scek_pthread_mutex_lock(pthread_mutex_t* mtx);


int PS4API scek_pthread_mutex_trylock(pthread_mutex_t *mtx);


int PS4API scek_pthread_mutex_unlock(pthread_mutex_t* mtx);


int PS4API scek_pthread_mutexattr_destroy(void);


int PS4API scek_pthread_mutexattr_init(pthread_mutexattr_t * attr);


int PS4API scek_pthread_mutexattr_settype(pthread_mutexattr_t* attr, int type);


ScePthread PS4API scek_pthread_self(void);


int PS4API scek_sched_yield(void);


int PS4API scek_usleep(sce_useconds_t microsecond);


int PS4API scek_close(void);


int PS4API scek_connect(void);


int PS4API scek_pthread_setschedparam(void);


void * PS4API scek_pthread_getspecific(pthread_key_t key);


int PS4API scek_pthread_key_create(void);


int PS4API scek_recv(void);


int PS4API scek_select(void);


int PS4API scek_sem_destroy(void);


int PS4API scek_sem_init(void);


int PS4API scek_sem_post(void);


int PS4API scek_sem_timedwait(void);


int PS4API scek_sem_wait(void);


int PS4API scek_send(void);


int PS4API scek_shutdown(void);


int PS4API scek_getpid(void);


int PS4API scek_getppid(void);


int PS4API scek_shm_open(const char *name, int oflag, SceKernelMode mode);


int PS4API scek_shm_unlink(const char *name);


void PS4API scek_pthread_exit(void *value_ptr);

