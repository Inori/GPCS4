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


extern const SCE_EXPORT_MODULE g_ExpModuleSceLibkernel;


// Note:
// The codebase is genarated using GenerateCode.py
// You may need to modify the code manually to fit development needs



//////////////////////////////////////////////////////////////////////////
// library: libkernel
//////////////////////////////////////////////////////////////////////////

int PS4API __error(void);


int PS4API __stack_chk_fail(void);


int PS4API __stack_chk_guard(void);


int PS4API __tls_get_addr(void);


int PS4API sceKernelAllocateDirectMemory(void);


int PS4API sceKernelClockGettime(void);


int PS4API sceKernelClose(void);


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


int PS4API sceKernelGetDirectMemorySize(void);


int PS4API sceKernelGetProcessTime(void);


int PS4API sceKernelGetProcessTimeCounter(void);


int PS4API sceKernelGetProcessTimeCounterFrequency(void);


int PS4API sceKernelGetTscFrequency(void);


int PS4API sceKernelGetdents(void);


int PS4API sceKernelIsNeoMode(void);


int PS4API sceKernelLseek(void);


int PS4API sceKernelMapDirectMemory(void);


int PS4API sceKernelMapFlexibleMemory(void);


int PS4API sceKernelMkdir(void);


int PS4API sceKernelOpen(void);


int PS4API sceKernelRead(void);


int PS4API sceKernelReadTsc(void);


int PS4API sceKernelReleaseDirectMemory(void);


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


int PS4API sceKernelWrite(void);


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


int PS4API scePthreadMutexDestroy(void);


int PS4API scePthreadMutexInit(void);


int PS4API scePthreadMutexLock(void);


int PS4API scePthreadMutexUnlock(void);


int PS4API scePthreadMutexattrDestroy(void);


int PS4API scePthreadMutexattrInit(void);


int PS4API scePthreadMutexattrSetprotocol(void);


int PS4API scePthreadMutexattrSettype(void);


int PS4API scePthreadRename(void);


int PS4API scePthreadRwlockDestroy(void);


int PS4API scePthreadRwlockInit(void);


int PS4API scePthreadRwlockRdlock(void);


int PS4API scePthreadRwlockUnlock(void);


int PS4API scePthreadRwlockWrlock(void);


int PS4API scePthreadRwlockattrDestroy(void);


int PS4API scePthreadRwlockattrInit(void);


int PS4API scePthreadSelf(void);


int PS4API scePthreadSetaffinity(void);


int PS4API scePthreadSetprio(void);


int PS4API scePthreadYield(void);




//////////////////////////////////////////////////////////////////////////
// library: libScePosix
//////////////////////////////////////////////////////////////////////////

int PS4API clock_gettime(void);


int PS4API gettimeofday(void);


int PS4API nanosleep(void);


int PS4API pthread_cond_destroy(void);


int PS4API pthread_cond_init(void);


int PS4API pthread_cond_signal(void);


int PS4API pthread_cond_timedwait(void);


int PS4API pthread_cond_wait(void);


int PS4API pthread_create(void);


int PS4API pthread_equal(void);


int PS4API pthread_join(void);


int PS4API pthread_mutex_destroy(void);


int PS4API pthread_mutex_init(void);


int PS4API pthread_mutex_lock(void);


int PS4API pthread_mutex_unlock(void);


int PS4API pthread_mutexattr_destroy(void);


int PS4API pthread_mutexattr_init(void);


int PS4API pthread_mutexattr_settype(void);


int PS4API pthread_self(void);


int PS4API sched_yield(void);


int PS4API usleep(void);




