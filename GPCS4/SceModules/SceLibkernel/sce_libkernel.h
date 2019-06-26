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
#include "sce_kernel_scepthread.h"
#include "sce_kernel_file.h"

extern const SCE_EXPORT_MODULE g_ExpModuleSceLibkernel;


// Note:
// The codebase is generated using GenerateCode.py
// You may need to modify the code manually to fit development needs



//////////////////////////////////////////////////////////////////////////
// library: libkernel
//////////////////////////////////////////////////////////////////////////

int PS4API __error(void);


int PS4API __stack_chk_fail(void);


int PS4API __stack_chk_guard(void);


int PS4API __tls_get_addr(void);


int PS4API sceKernelAllocateDirectMemory(sce::off_t searchStart, sce::off_t searchEnd, size_t len, size_t alignment, int memoryType, sce::off_t *physAddrOut);


int PS4API sceKernelClockGettime(void);


int PS4API sceKernelClose(int d);


int PS4API sceKernelCreateEqueue(void);


int PS4API sceKernelCreateEventFlag(void);


int PS4API sceKernelCreateSema(void);


int PS4API sceKernelDeleteEqueue(void);


int PS4API sceKernelDeleteEventFlag(void);


int PS4API sceKernelDeleteSema(void);


int PS4API sceKernelFstat(void);


int PS4API sceKernelFsync(void);


int PS4API sceKernelFtruncate(void);


int PS4API sceKernelGetCpumode(void);


size_t PS4API sceKernelGetDirectMemorySize(void);


int PS4API sceKernelGetProcessTime(void);


int PS4API sceKernelGetProcessTimeCounter(void);


int PS4API sceKernelGetProcessTimeCounterFrequency(void);


uint64_t PS4API sceKernelGetTscFrequency(void);


int PS4API sceKernelGetdents(void);


int PS4API sceKernelIsNeoMode(void);


int PS4API sceKernelLseek(void);


int PS4API sceKernelMapDirectMemory(void **addr, size_t len, int prot, int flags, sce::off_t directMemoryStart, size_t maxPageSize);


int PS4API sceKernelMapFlexibleMemory(void **addrInOut, size_t len, int prot, int flags);


int PS4API sceKernelMkdir(void);


int PS4API sceKernelOpen(const char *path, int flags, SceKernelMode mode);


ssize_t PS4API sceKernelRead(int d, void *buf, size_t nbytes);


int PS4API sceKernelReadTsc(void);


int PS4API sceKernelReleaseDirectMemory(sce::off_t start, size_t len);


int PS4API sceKernelReleaseFlexibleMemory(void);


int PS4API sceKernelRename(void);


int PS4API sceKernelSetEventFlag(void);


int PS4API sceKernelSignalSema(void);


int PS4API sceKernelStat(void);


int PS4API sceKernelUnlink(void);


int PS4API sceKernelUsleep(void);


int PS4API sceKernelWaitEqueue(void);


int PS4API sceKernelWaitEventFlag(void);


int PS4API sceKernelWaitSema(void);


ssize_t PS4API sceKernelWrite(int d, const void *buf, size_t nbytes);


int PS4API scePthreadAttrDestroy(void);


int PS4API scePthreadAttrGetschedparam(void);


int PS4API scePthreadAttrInit(void);


int PS4API scePthreadAttrSetaffinity(void);


int PS4API scePthreadAttrSetdetachstate(void);


int PS4API scePthreadAttrSetinheritsched(void);


int PS4API scePthreadAttrSetschedparam(void);


int PS4API scePthreadAttrSetschedpolicy(void);


int PS4API scePthreadAttrSetstacksize(void);


int PS4API scePthreadCondBroadcast(void);


int PS4API scePthreadCondDestroy(void);


int PS4API scePthreadCondInit(void);


int PS4API scePthreadCondSignal(void);


int PS4API scePthreadCondWait(void);


int PS4API scePthreadCondattrDestroy(void);


int PS4API scePthreadCondattrInit(void);


int PS4API scePthreadCreate(void);


int PS4API scePthreadExit(void);


int PS4API scePthreadGetaffinity(void);


int PS4API scePthreadGetprio(void);


int PS4API scePthreadJoin(void);


int PS4API scePthreadMutexDestroy(ScePthreadMutex *mutex);


int PS4API scePthreadMutexInit(ScePthreadMutex *mutex, const ScePthreadMutexattr *attr, const char *name);


int PS4API scePthreadMutexLock(ScePthreadMutex *mutex);


int PS4API scePthreadMutexUnlock(ScePthreadMutex *mutex);


int PS4API scePthreadMutexattrDestroy(ScePthreadMutexattr *attr);


int PS4API scePthreadMutexattrInit(ScePthreadMutexattr *attr);


int PS4API scePthreadMutexattrSetprotocol(ScePthreadMutexattr *attr, int protocol);


int PS4API scePthreadMutexattrSettype(ScePthreadMutexattr *attr, int type);


int PS4API scePthreadRename(void);


int PS4API scePthreadRwlockDestroy(void);


int PS4API scePthreadRwlockInit(void);


int PS4API scePthreadRwlockRdlock(void);


int PS4API scePthreadRwlockUnlock(void);


int PS4API scePthreadRwlockWrlock(void);


int PS4API scePthreadRwlockattrDestroy(void);


int PS4API scePthreadRwlockattrInit(void);


ScePthread PS4API scePthreadSelf(void);


int PS4API scePthreadSetaffinity(ScePthread thread, const SceKernelCpumask mask);


int PS4API scePthreadSetprio(void);


int PS4API scePthreadYield(void);


int PS4API sceKernelBatchMap(void);


int PS4API sceKernelCheckedReleaseDirectMemory(void);


int PS4API sceKernelDlsym(void);


int PS4API sceKernelGetDirectMemoryType(void);


int PS4API sceKernelGetGPI(void);


int PS4API sceKernelGetPageTableStats(void);


int PS4API sceKernelGettimeofday(void);


int PS4API sceKernelIsStack(void);


int PS4API sceKernelLoadStartModule(void);


int PS4API sceKernelMapNamedDirectMemory(void);


int PS4API sceKernelMapNamedFlexibleMemory(void);


int PS4API sceKernelMprotect(void);


int PS4API sceKernelMtypeprotect(void);


int PS4API sceKernelMunmap(void);


int PS4API sceKernelQueryMemoryProtection(void);


int PS4API sceKernelReserveVirtualRange(void);


int PS4API sceKernelSetPrtAperture(void);


int PS4API sceKernelSetVirtualRangeName(void);


int PS4API sceKernelUuidCreate(void);


int PS4API sceKernelVirtualQuery(void);


int PS4API scePthreadCancel(void);


int PS4API scePthreadDetach(void);


int PS4API scePthreadGetname(void);


int PS4API scePthreadGetschedparam(void);


int PS4API scePthreadGetspecific(void);


int PS4API scePthreadKeyCreate(void);


int PS4API scePthreadMutexTimedlock(void);


int PS4API scePthreadMutexTrylock(void);


int PS4API scePthreadSetschedparam(void);


int PS4API scePthreadSetspecific(void);


//////////////////////////////////////////////////////////////////////////
// library: libSceCoredump
//////////////////////////////////////////////////////////////////////////

int PS4API sceCoredumpAttachMemoryRegion(void);


int PS4API sceCoredumpRegisterCoredumpHandler(void);


int PS4API sceCoredumpWriteUserData(void);




//////////////////////////////////////////////////////////////////////////
// library: libScePosix
//////////////////////////////////////////////////////////////////////////

int PS4API scek_clock_gettime(void);


int PS4API scek_gettimeofday(void);


int PS4API scek_nanosleep(void);


int PS4API scek_pthread_cond_destroy(void);


int PS4API scek_pthread_cond_init(void);


int PS4API scek_pthread_cond_signal(void);


int PS4API scek_pthread_cond_timedwait(void);


int PS4API scek_pthread_cond_wait(void);


int PS4API scek_pthread_create(void);


int PS4API scek_pthread_equal(void);


int PS4API scek_pthread_join(void);


int PS4API scek_pthread_mutex_destroy(void);


int PS4API scek_pthread_mutex_init(void);


int PS4API scek_pthread_mutex_lock(void);


int PS4API scek_pthread_mutex_unlock(void);


int PS4API scek_pthread_mutexattr_destroy(void);


int PS4API scek_pthread_mutexattr_init(void);


int PS4API scek_pthread_mutexattr_settype(void);


int PS4API scek_pthread_self(void);


int PS4API scek_sched_yield(void);


int PS4API scek_usleep(void);


int PS4API scek_close(void);


int PS4API scek_connect(void);


int PS4API scek_pthread_setschedparam(void);


int PS4API scek_recv(void);


int PS4API scek_select(void);


int PS4API scek_sem_destroy(void);


int PS4API scek_sem_init(void);


int PS4API scek_sem_post(void);


int PS4API scek_sem_timedwait(void);


int PS4API scek_sem_wait(void);


int PS4API scek_send(void);


int PS4API scek_shutdown(void);






